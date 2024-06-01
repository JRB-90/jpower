namespace CalApp.Shared.Calibration
{
    public interface IJPowerDevice
    {
        uint RawAdcValue
        {
            get;
        }

        IObservable<uint> RawAdcValues { get; }

        Task StartStreaming();

        Task StopStreaming();

        Task<bool> ZeroOffset();

        Task<bool> PushSlope(Slope slope);
    }
}
