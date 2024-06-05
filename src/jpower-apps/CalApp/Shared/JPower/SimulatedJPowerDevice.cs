using CalApp.Shared.Calibration;
using CommunityToolkit.Mvvm.ComponentModel;
using System.Reactive.Subjects;

namespace CalApp.Shared.JPower
{
    public class SimulatedJPowerDevice : ObservableObject, IJPowerDevice
    {
        public const uint U24_ZERO_POINT = 8388608;

        public SimulatedJPowerDevice()
        {
            random = new Random(DateTime.Now.Nanosecond);
            offset = random.Next(-100000, 100000);

            rawAdcValues = new Subject<uint>();
            RawAdcValues = rawAdcValues;

            timer = new System.Timers.Timer();
            timer.Interval = 500;
            timer.Elapsed += Timer_Elapsed;
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

        public IObservable<uint> RawAdcValues { get; }

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

        public async Task<bool> PushSlope(Slope slope)
        {
            await Task.Delay(random.Next(1000, 2000));

            return true;
        }

        private void Timer_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
        {
            lock (syncObj)
            {
                var noise = random.Next(-1000, 1000);
                RawAdcValue = (uint)(U24_ZERO_POINT + offset + noise);
            }
        }

        private uint rawAdcValue;
        private Subject<uint> rawAdcValues;
        private System.Timers.Timer timer;
        private Random random;
        private int offset;

        private static object syncObj = new object();
    }
}
