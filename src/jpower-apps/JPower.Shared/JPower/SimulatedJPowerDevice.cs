using CommunityToolkit.Mvvm.ComponentModel;
using System.Reactive.Subjects;

namespace JPower.Shared.JPower
{
    internal class SimulatedJPowerDevice : ObservableObject, IJPowerDevice
    {
        public SimulatedJPowerDevice()
        {
            random = new Random(DateTime.Now.Nanosecond);
            offset = random.Next(-100000, 100000);

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

            timer = new System.Timers.Timer();
            timer.Interval = 500;
            timer.Elapsed += Timer_Elapsed;
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
            await Task.Delay(100);
            timer.Start();
        }

        public async Task StopStreaming()
        {
            timer.Stop();
            await Task.Delay(100);
        }

        public async Task<bool> ZeroOffset()
        {
            await Task.Delay(random.Next(1000, 3000));

            lock (syncObj)
            {
                offset = 0;
            }

            return true;
        }

        private void Timer_Elapsed(object? sender, System.Timers.ElapsedEventArgs e)
        {
            lock (syncObj)
            {
                AdcValue = (uint)(U24_ZERO_POINT + offset + random.Next(-1000, 1000));
                PowerValue = (ushort)(175 + random.Next(-100, 200));

                AccelValue =
                    new JPowerAccelData()
                    {
                        x = (float)random.NextDouble() * 20.0f,
                        y = (float)random.NextDouble() * 20.0f,
                        z = (float)random.NextDouble() * 20.0f,
                    }.ToVector3D();

                GyroValue =
                    new JPowerGyroData()
                    {
                        rx = (float)random.NextDouble() * 40.0f,
                        ry = (float)random.NextDouble() * 40.0f,
                        rz = (float)random.NextDouble() * 40.0f,
                    }.ToVector3D();

                OrientValue =
                    new JPowerOrientData()
                    {
                        w = 0.092296f,
                        x = 0.7010574f,
                        y = -0.092296f,
                        z = 0.7010574f,
                    }.ToVector3D();

                CadenceValue = (ushort)(90 + random.Next(-30, 30));
                TempValue = (float)random.NextDouble() * 24.0f;
                BatteryLevel = (ushort)(random.Next(0, 100));
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

        private Random random;
        private int offset;
        private System.Timers.Timer timer;

        private const uint U24_ZERO_POINT = 8388608;
        private object syncObj = new object();
    }
}
