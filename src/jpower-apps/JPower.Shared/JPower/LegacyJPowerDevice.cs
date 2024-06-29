using JPower.Shared.Ble;
using JPower.Shared.Calibration;
using CommunityToolkit.Mvvm.ComponentModel;
using System.Reactive.Subjects;

namespace JPower.Shared.JPowDevice
{
    public class LegacyJPowerDevice : ObservableObject, ILegacyJPowerDevice
    {
        public LegacyJPowerDevice(BleDevice bleDevice)
        {
            this.bleDevice = bleDevice;
            mre = new ManualResetEvent(false);
            rawAdcValue = 0;
            state = JPowerState.JP_STATE_STARTUP;
            
            rawAdcValues = new Subject<uint>();
            RawAdcValues = rawAdcValues;
            stateValues = new Subject<JPowerState>();
            StateValues = stateValues;

            calService          = GetService(LegacyJPowerBleUUIDs.JPOWER_CAL_SRV_UUID);
            adcRawChar          = GetCharacteristic(calService, LegacyJPowerBleUUIDs.JPOWER_CAL_SRV_ADC_RAW_UUID);
            zeroChar            = GetCharacteristic(calService, LegacyJPowerBleUUIDs.JPOWER_CAL_SRV_ZERO_UUID);
            pullCalReqChar      = GetCharacteristic(calService, LegacyJPowerBleUUIDs.JPOWER_CAL_SRV_PULL_CAL_REQ_UUID);
            pullCalChar         = GetCharacteristic(calService, LegacyJPowerBleUUIDs.JPOWER_CAL_SRV_PULL_CAL_UUID);
            pushCalChar         = GetCharacteristic(calService, LegacyJPowerBleUUIDs.JPOWER_CAL_SRV_PUSH_CAL_UUID);

            stateService        = GetService(LegacyJPowerBleUUIDs.JPOWER_JP_STATE_SRV_UUID);
            stateRequestChar    = GetCharacteristic(stateService, LegacyJPowerBleUUIDs.JPOWER_JP_STATE_SRV_STATE_REQUEST_UUID);
            currentStateChar    = GetCharacteristic(stateService, LegacyJPowerBleUUIDs.JPOWER_JP_STATE_SRV_CURRENT_STATE_UUID);

            adcRawChar.ValueUpdated += AdcRawChar_ValueUpdated;
            currentStateChar.ValueUpdated += GetStateChar_ValueUpdated;
            pullCalChar.ValueUpdated += PullCalChar_ValueUpdated;
        }

        public uint RawAdcValue
        {
            get => rawAdcValue;
            set
            {
                SetProperty(ref rawAdcValue, value);
                rawAdcValues.OnNext(value);
            }
        }

        public JPowerState State
        {
            get => state;
            set
            {
                SetProperty(ref state, value);
                stateValues.OnNext(value);
            }
        }

        public IObservable<uint> RawAdcValues { get; }

        public IObservable<JPowerState> StateValues { get; }

        public async Task StartStreaming()
        {
            await adcRawChar.StartListening();
            await currentStateChar.StartListening();
            await pullCalChar.StartListening();
            await StateRequest(JPowerStateRequest.JP_STATE_REQUEST_PUBLISH_STATE);
            
            var slope = await PullSlope();
        }

        public async Task StopStreaming()
        {
            await adcRawChar.StopListening();
            await currentStateChar.StopListening();
            await pullCalChar.StopListening();
        }

        public Task<bool> SwitchToCalMode()
        {
            return StateRequest(JPowerStateRequest.JP_STATE_REQUEST_SWITCH_TO_CALIBRATE);
        }

        public Task<bool> SwitchToRunMode()
        {
            return StateRequest(JPowerStateRequest.JP_STATE_REQUEST_SWITCH_TO_RUNNING);
        }

        public Task<bool> StateRequest(JPowerStateRequest stateRequest)
        {
            return stateRequestChar.WriteValue(new byte[1] { (byte)stateRequest });
        }

        public Task<bool> ZeroOffset()
        {
            return zeroChar.WriteValue(new byte[1] { 0x01 });
        }

        public Task<bool> PushSlope(Slope slope)
        {
            var calData = new JPowerCalibrationData();
            calData.guid = 0xA5A5A5A5A5A5A5A5;// Guid.NewGuid().ToUInt128();
            calData.slope = (float)slope.SlopeValue;
            calData.intercept = (float)slope.Intercept;
            var calDataBytes = calData.CastToArray<JPowerCalibrationData>();

            return pushCalChar.WriteValue(calDataBytes);
        }

        public async Task<Slope> PullSlope()
        {
            mre.Reset();

            if (!await pullCalReqChar.WriteValue(new byte[1] { 0x01 }))
            {
                throw new InvalidOperationException("Failed to request a calibration pull");
            }

            if (!mre.WaitOne(TimeSpan.FromMilliseconds(3000)))
            {
                throw new InvalidOperationException("Failed to receive calibration after request");
            }

            try
            {
                var calibrationData = pullCalChar.CurrentValue.CastToStruct<JPowerCalibrationData>();

                return
                    new Slope(
                        calibrationData.slope,
                        calibrationData.intercept
                    );
            }
            catch
            {
                throw new ArgumentException("Failed to parse claibration data");
            }
        }

        private BleDeviceService GetService(Guid uuid)
        {
            return
                bleDevice
                .Services
                .OfType<BleDeviceService>()
                .Where(s => s.UUID == uuid)
                .First();
        }

        private BleDeviceCharacteristic GetCharacteristic(BleDeviceService service, Guid uuid)
        {
           return
                service
                .Characteristics
                .OfType<BleDeviceCharacteristic>()
                .Where(c => c.UUID == uuid)
                .First();
        }

        private void AdcRawChar_ValueUpdated(object? sender, byte[] value)
        {
            RawAdcValue = BleValueConverters.ToUint32(value);
        }

        private void GetStateChar_ValueUpdated(object? sender, byte[] value)
        {
            State = (JPowerState)BleValueConverters.ToUint8(value);
        }

        private void PullCalChar_ValueUpdated(object? sender, byte[] value)
        {
            mre.Set();
        }

        private BleDevice bleDevice;

        private uint rawAdcValue;
        private Subject<uint> rawAdcValues;

        private JPowerState state;
        private Subject<JPowerState> stateValues;

        private BleDeviceService calService;
        private BleDeviceCharacteristic adcRawChar;
        private BleDeviceCharacteristic zeroChar;
        private BleDeviceCharacteristic pullCalReqChar;
        private BleDeviceCharacteristic pullCalChar;
        private BleDeviceCharacteristic pushCalChar;

        private BleDeviceService stateService;
        private BleDeviceCharacteristic currentStateChar;
        private BleDeviceCharacteristic stateRequestChar;

        private ManualResetEvent mre;
    }
}
