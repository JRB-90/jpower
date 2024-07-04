using CommunityToolkit.Mvvm.ComponentModel;
using JPower.Shared.Ble;
using JPower.Shared.JPowDevice;
using System.Reactive.Subjects;

namespace JPower.Shared.JPower
{
    public class JPowerDevice : ObservableObject, IJPowerDevice
    {
        public const int REPLY_TIMEOUT_MS = 5000;

        public JPowerDevice(BleDevice bleDevice)
        {
            this.bleDevice = bleDevice;
            mre = new ManualResetEvent(false);

            adcValues = new Subject<uint>();
            torqueValues = new Subject<float>();
            powerValues = new Subject<ushort>();
            accelValues = new Subject<Vector3D>();
            gyroValues = new Subject<Vector3D>();
            orientValues = new Subject<Vector3D>();
            cadenceValues = new Subject<ushort>();
            tempValues = new Subject<float>();
            batteryLevels = new Subject<ushort>();

            adcValue = 0;
            torqueValue = 0.0f;
            powerValue = 0;
            accelValue = new Vector3D(0.0, 0.0, 0.0);
            gyroValue = new Vector3D(0.0, 0.0, 0.0);
            orientValue = new Vector3D(0.0, 0.0, 0.0);
            cadenceValue = 0;
            tempValue = 0.0f;
            batteryLevel = 0;

            sensorService       = GetService(JPowerBleUUIDs.JPOWER_SENSOR_SRV_UUID);
            diagDataChar        = GetCharacteristic(sensorService, JPowerBleUUIDs.JPOWER_SENSOR_SRV_DIAG_DATA_UUID);

            batteryService      = GetService(JPowerBleUUIDs.JPOWER_BATTERY_SRV_UUID);
            batteryLevelChar    = GetCharacteristic(batteryService, JPowerBleUUIDs.JPOWER_BATTERY_LEVEL_SRV_UUID);

            calibrateService    = GetService(JPowerBleUUIDs.JPOWER_CAL_SRV_UUID);
            sendReqChar         = GetCharacteristic(calibrateService, JPowerBleUUIDs.JPOWER_CAL_SRV_SEND_REQ_UUID);
            pushCalChar         = GetCharacteristic(calibrateService, JPowerBleUUIDs.JPOWER_CAL_SRV_PUSH_CAL_UUID);
            pullCalResChar      = GetCharacteristic(calibrateService, JPowerBleUUIDs.JPOWER_CAL_SRV_PULL_CAL_RES_UUID);
            measureResChar      = GetCharacteristic(calibrateService, JPowerBleUUIDs.JPOWER_CAL_SRV_MEASURE_RES_UUID);
            zeroOffsetResChar   = GetCharacteristic(calibrateService, JPowerBleUUIDs.JPOWER_CAL_SRV_ZERO_RES_UUID);

            diagDataChar.ValueUpdated += DiagDataChar_ValueUpdated;

            batteryTimer = new System.Timers.Timer(TimeSpan.FromSeconds(3));
            batteryTimer.Elapsed += BatteryTimer_Elapsed;
        }

        public uint AdcValue
        {
            get => adcValue;
            set
            {
                SetProperty(ref adcValue, value);
                adcValues.OnNext(value);
            }
        }

        public float TorqueValue
        {
            get => torqueValue;
            set
            {
                SetProperty(ref torqueValue, value);
                torqueValues.OnNext(value);
            }
        }

        public ushort PowerValue
        {
            get => powerValue;
            set
            {
                SetProperty(ref powerValue, value);
                powerValues.OnNext(value);
            }
        }

        public Vector3D AccelValue
        {
            get => accelValue;
            set
            {
                SetProperty(ref accelValue, value);
                accelValues.OnNext(value);
            }
        }

        public Vector3D GyroValue
        {
            get => gyroValue;
            set
            {
                SetProperty(ref gyroValue, value);
                gyroValues.OnNext(value);
            }
        }

        public Vector3D OrientValue
        {
            get => orientValue;
            set
            {
                SetProperty(ref orientValue, value);
                orientValues.OnNext(value);
            }
        }

        public ushort CadenceValue
        {
            get => cadenceValue;
            set
            {
                SetProperty(ref cadenceValue, value);
                cadenceValues.OnNext(value);
            }
        }

        public float TempValue
        {
            get => tempValue;
            set
            {
                SetProperty(ref tempValue, value);
                tempValues.OnNext(value);
            }
        }

        public ushort BatteryLevel
        {
            get => batteryLevel;
            set
            {
                SetProperty(ref batteryLevel, value);
                batteryLevels.OnNext(value);
            }
        }

        public IObservable<uint> AdcValues => adcValues;

        public IObservable<float> TorqueValues => torqueValues;

        public IObservable<ushort> PowerValues => powerValues;

        public IObservable<Vector3D> AccelValues => accelValues;

        public IObservable<Vector3D> GyroValues => gyroValues;

        public IObservable<Vector3D> OrientValues => orientValues;

        public IObservable<ushort> CadenceValues => cadenceValues;

        public IObservable<float> TempValues => tempValues;

        public IObservable<ushort> BatteryLevels => batteryLevels;

        public async Task StartStreaming()
        {
            await diagDataChar.StartListening();

            JPowerCalibrationData dummyCal = new()
            {
                guid = 42,
                slope = 0.42f,
                intercept = 1234.5f,
                crankLength = 0.1725f,
            };

            await PushCalibration(dummyCal);
            var cal = await PullCalibration();
            var meas = await Measure(10);
            var zero = await ZeroOffset();
        }

        public async Task StopStreaming()
        {
            await diagDataChar.StopListening();
        }

        public async Task PushCalibration(JPowerCalibrationData calibration)
        {
            var writeResult = await pushCalChar.WriteValue(calibration.CastToArray());

            if (writeResult == false)
            {
                throw new InvalidOperationException("Failed to push cal to device");
            }
        }

        public async Task<JPowerCalibrationData> PullCalibration()
        {
            await SendAndAwaitReturn(JPowerCalSrvRequest.CALIBRATE_SRV_PULL_CAL, pullCalResChar);
            var calData = pullCalResChar.CurrentValue.CastToStruct<JPowerCalibrationData>();

            return calData;
        }

        public async Task<uint> Measure(byte numberSamples)
        {
            await SendAndAwaitReturn(JPowerCalSrvRequest.CALIBRATE_SRV_MEASURE, measureResChar);
            uint measurement = BleValueConverters.ToUint32(measureResChar.CurrentValue);

            return measurement;
        }

        public async Task<bool> ZeroOffset()
        {
            await SendAndAwaitReturn(JPowerCalSrvRequest.CALIBRATE_SRV_ZERO_OFFSET, zeroOffsetResChar);
            var result = (JPowerCalSrvResponse)zeroOffsetResChar.CurrentValue[0];

            return 
                result == JPowerCalSrvResponse.CALIBRATE_SRV_OK
                ? true 
                : false;
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

        private void DiagDataChar_ValueUpdated(object? sender, byte[] value)
        {
            var diagData = value.CastToStruct<JPowerSensorDiagData>();

            TempValue = diagData.temp;
            AdcValue = diagData.adcValue;
            AccelValue = diagData.imuData.accel.ToVector3D();
            GyroValue = diagData.imuData.gyro.ToVector3D();
            OrientValue = diagData.orientation.ToVector3D();
            CadenceValue = diagData.cadence;
            PowerValue = diagData.power;
        }

        private async void BatteryTimer_Elapsed(object? sender, System.Timers.ElapsedEventArgs e)
        {
            try
            {
                var battery = await batteryLevelChar.ReadValue();
                if (battery.Length == 1)
                {
                    BatteryLevel = BleValueConverters.ToUint8(battery);
                }
            }
            catch
            {
            }
        }

        private async Task SendAndAwaitReturn(
            JPowerCalSrvRequest request,
            BleDeviceCharacteristic characteristic)
        {
            try
            {
                await characteristic.StartListening();

                mre.Reset();
                characteristic.ValueUpdated += (s, v) => mre.Set();
                var reqData = new byte[1] { (byte)request };
                var writeRes = await sendReqChar.WriteValue(reqData);

                if (writeRes == false)
                {
                    throw new InvalidOperationException("Failed to send request to JPower");
                }

                if (!mre.WaitOne(REPLY_TIMEOUT_MS))
                {
                    throw new InvalidOperationException("Did not receive return message inside the timeout");
                }
            }
            catch (Exception ex)
            {
                throw new InvalidOperationException(ex.Message);
            }
            finally
            {
                await characteristic.StopListening();
            }
        }

        private uint adcValue;
        private float torqueValue;
        private ushort powerValue;
        private Vector3D accelValue;
        private Vector3D gyroValue;
        private Vector3D orientValue;
        private ushort cadenceValue;
        private float tempValue;
        private ushort batteryLevel;

        private Subject<uint> adcValues;
        private Subject<float> torqueValues;
        private Subject<ushort> powerValues;
        private Subject<Vector3D> accelValues;
        private Subject<Vector3D> gyroValues;
        private Subject<Vector3D> orientValues;
        private Subject<ushort> cadenceValues;
        private Subject<float> tempValues;
        private Subject<ushort> batteryLevels;

        private BleDevice bleDevice;

        private BleDeviceService sensorService;
        private BleDeviceCharacteristic diagDataChar;

        private BleDeviceService batteryService;
        private BleDeviceCharacteristic batteryLevelChar;

        private BleDeviceService calibrateService;
        private BleDeviceCharacteristic sendReqChar;
        private BleDeviceCharacteristic pushCalChar;
        private BleDeviceCharacteristic pullCalResChar;
        private BleDeviceCharacteristic measureResChar;
        private BleDeviceCharacteristic zeroOffsetResChar;

        private System.Timers.Timer batteryTimer;
        private ManualResetEvent mre;
    }
}
