using CommunityToolkit.Mvvm.ComponentModel;
using JPower.Shared.Ble;
using System.Reactive.Subjects;

namespace JPower.Shared.JPower
{
    public class JPowerDevice : ObservableObject, IJPowerDevice
    {
        public JPowerDevice(BleDevice bleDevice)
        {
            this.bleDevice = bleDevice;

            adcValues = new Subject<uint>();
            powerValues = new Subject<ushort>();
            accelValues = new Subject<Vector3D>();
            gyroValues = new Subject<Vector3D>();
            orientValues = new Subject<Vector3D>();
            cadenceValues = new Subject<ushort>();
            tempValues = new Subject<float>();
            batteryLevels = new Subject<ushort>();

            adcValue = 0;
            powerValue = 0;
            accelValue = new Vector3D(0.0, 0.0, 0.0);
            gyroValue = new Vector3D(0.0, 0.0, 0.0);
            orientValue = new Vector3D(0.0, 0.0, 0.0);
            cadenceValue = 0;
            tempValue = 0.0f;
            batteryLevel = 0;

            sensorService       = GetService(JPowerBleUUIDs.JPOWER_SENSOR_SRV_UUID);
            zeroOffsetChar      = GetCharacteristic(sensorService, JPowerBleUUIDs.JPOWER_SENSOR_SRV_ZERO_OFFSEt_UUID);
            adcValueChar        = GetCharacteristic(sensorService, JPowerBleUUIDs.JPOWER_SENSOR_SRV_ADC_UUID);
            powerValueChar      = GetCharacteristic(sensorService, JPowerBleUUIDs.JPOWER_SENSOR_SRV_POWER_UUID);
            accelValueChar      = GetCharacteristic(sensorService, JPowerBleUUIDs.JPOWER_SENSOR_SRV_ACCEL_UUID);
            gyroValueChar       = GetCharacteristic(sensorService, JPowerBleUUIDs.JPOWER_SENSOR_SRV_GYRO_UUID);
            orientValueChar     = GetCharacteristic(sensorService, JPowerBleUUIDs.JPOWER_SENSOR_SRV_ORIENT_UUID);
            cadenceValueChar    = GetCharacteristic(sensorService, JPowerBleUUIDs.JPOWER_SENSOR_SRV_CADENCE_UUID);
            tempValueChar       = GetCharacteristic(sensorService, JPowerBleUUIDs.JPOWER_SENSOR_SRV_TEMP_UUID);

            batteryService      = GetService(JPowerBleUUIDs.JPOWER_BATTERY_SRV_UUID);
            batteryLevelChar    = GetCharacteristic(batteryService, JPowerBleUUIDs.JPOWER_BATTERY_LEVEL_SRV_UUID);

            adcValueChar.ValueUpdated       += AdcValueChar_ValueUpdated;
            powerValueChar.ValueUpdated     += PowerValueChar_ValueUpdated;
            accelValueChar.ValueUpdated     += AccelValueChar_ValueUpdated;
            gyroValueChar.ValueUpdated      += GyroValueChar_ValueUpdated;
            orientValueChar.ValueUpdated    += OrientValueChar_ValueUpdated;
            cadenceValueChar.ValueUpdated   += CadenceValueChar_ValueUpdated;
            tempValueChar.ValueUpdated      += TempValueChar_ValueUpdated;
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

        public IObservable<ushort> PowerValues => powerValues;

        public IObservable<Vector3D> AccelValues => accelValues;

        public IObservable<Vector3D> GyroValues => gyroValues;

        public IObservable<Vector3D> OrientValues => orientValues;

        public IObservable<ushort> CadenceValues => cadenceValues;

        public IObservable<float> TempValues => tempValues;

        public IObservable<ushort> BatteryLevels => batteryLevels;

        public async Task StartStreaming()
        {
            await adcValueChar.StartListening();
            await powerValueChar.StartListening();
            await accelValueChar.StartListening();
            await gyroValueChar.StartListening();
            await orientValueChar.StartListening();
            await cadenceValueChar.StartListening();
            await tempValueChar.StartListening();
        }

        public async Task StopStreaming()
        {
            await adcValueChar.StopListening();
            await powerValueChar.StopListening();
            await accelValueChar.StopListening();
            await gyroValueChar.StopListening();
            await orientValueChar.StopListening();
            await cadenceValueChar.StopListening();
            await tempValueChar.StopListening();
        }

        public Task<bool> ZeroOffset()
        {
            return zeroOffsetChar.WriteValue(new byte[1] { 0x01 });
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

        private void AdcValueChar_ValueUpdated(object? sender, byte[] value)
        {
            AdcValue = BleValueConverters.ToUint32(value);
        }

        private void PowerValueChar_ValueUpdated(object? sender, byte[] value)
        {
            PowerValue = BleValueConverters.ToUint16(value);
        }

        private void AccelValueChar_ValueUpdated(object? sender, byte[] value)
        {
            AccelValue = value.CastToStruct<JPowerAccelData>().ToVector3D();
        }

        private void GyroValueChar_ValueUpdated(object? sender, byte[] value)
        {
            GyroValue = value.CastToStruct<JPowerGyroData>().ToVector3D();
        }

        private void OrientValueChar_ValueUpdated(object? sender, byte[] value)
        {
            OrientValue = value.CastToStruct<JPowerOrientData>().ToVector3D();
        }

        private void CadenceValueChar_ValueUpdated(object? sender, byte[] value)
        {
            CadenceValue = BleValueConverters.ToUint16(value);
        }

        private async void TempValueChar_ValueUpdated(object? sender, byte[] value)
        {
            TempValue = BleValueConverters.ToFloat32(value);

            var battery = await batteryLevelChar.ReadValue();
            if (battery.Length == 1)
            {
                BatteryLevel = BleValueConverters.ToUint8(battery);
            }
        }

        private uint adcValue;
        private ushort powerValue;
        private Vector3D accelValue;
        private Vector3D gyroValue;
        private Vector3D orientValue;
        private ushort cadenceValue;
        private float tempValue;
        private ushort batteryLevel;

        private Subject<uint> adcValues;
        private Subject<ushort> powerValues;
        private Subject<Vector3D> accelValues;
        private Subject<Vector3D> gyroValues;
        private Subject<Vector3D> orientValues;
        private Subject<ushort> cadenceValues;
        private Subject<float> tempValues;
        private Subject<ushort> batteryLevels;

        private BleDevice bleDevice;

        private BleDeviceService sensorService;
        private BleDeviceCharacteristic zeroOffsetChar;
        private BleDeviceCharacteristic adcValueChar;
        private BleDeviceCharacteristic powerValueChar;
        private BleDeviceCharacteristic accelValueChar;
        private BleDeviceCharacteristic gyroValueChar;
        private BleDeviceCharacteristic orientValueChar;
        private BleDeviceCharacteristic cadenceValueChar;
        private BleDeviceCharacteristic tempValueChar;

        private BleDeviceService batteryService;
        private BleDeviceCharacteristic batteryLevelChar;
    }
}
