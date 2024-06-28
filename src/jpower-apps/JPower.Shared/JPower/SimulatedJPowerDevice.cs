using JPower.Shared.Calibration;
using CommunityToolkit.Mvvm.ComponentModel;
using System.Reactive.Subjects;

namespace JPower.Shared.JPowDevice
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
            stateValues = new Subject<JPowerState>();
            StateValues = stateValues;

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
            await Task.Delay(100);
            timer.Start();
        }

        public async Task StopStreaming()
        {
            timer.Stop();
            await Task.Delay(100);
        }

        public async Task<bool> SwitchToCalMode()
        {
            await Task.Delay(random.Next(500, 1500));

            return true;
        }

        public async Task<bool> SwitchToRunMode()
        {
            await Task.Delay(random.Next(500, 1500));

            return true;
        }

        public async Task<bool> StateRequest(JPowerStateRequest stateRequest)
        {
            await Task.Delay(100);

            return true;
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

        private void Timer_Elapsed(object? sender, System.Timers.ElapsedEventArgs e)
        {
            lock (syncObj)
            {
                var noise = random.Next(-1000, 1000);
                RawAdcValue = (uint)(U24_ZERO_POINT + offset + noise);
            }
        }

        public async Task<Slope> PullSlope()
        {
            await Task.Delay(random.Next(1000, 2000));

            return new Slope(123.456, 4242);
        }

        private uint rawAdcValue;
        private Subject<uint> rawAdcValues;
        private JPowerState state;
        private Subject<JPowerState> stateValues;
        private System.Timers.Timer timer;
        private Random random;
        private int offset;

        private object syncObj = new object();
    }
}
