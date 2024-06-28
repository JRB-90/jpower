using JPower.Shared.Calibration;
using System.Runtime.InteropServices;

namespace JPower.Shared.JPowDevice
{
    public enum JPowerState
    {
        JP_STATE_STARTUP                        = 0,
        JP_STATE_INITIALISED                    = 1,
        JP_STATE_RUNNING                        = 2,
        JP_STATE_CALIBRATING                    = 3,
    }

    public enum JPowerStateRequest
    {
        JP_STATE_REQUEST_NONE                   = 0,
        JP_STATE_REQUEST_PUBLISH_STATE          = 1,
        JP_STATE_REQUEST_SWITCH_TO_RUNNING      = 2,
        JP_STATE_REQUEST_SWITCH_TO_CALIBRATE    = 3,
    }

    [StructLayout(LayoutKind.Explicit, Size = 16, CharSet = CharSet.Ansi)]
    public struct JPowerCalibrationData
    {
        [FieldOffset(0)]public UInt128 guid;
        [FieldOffset(16)] public float slope;
        [FieldOffset(20)] public float intercept;
    }

    public interface IJPowerDevice
    {
        uint RawAdcValue { get; }

        JPowerState State { get; }

        IObservable<uint> RawAdcValues { get; }

        IObservable<JPowerState> StateValues { get; }

        Task StartStreaming();

        Task StopStreaming();

        Task<bool> SwitchToCalMode();

        Task<bool> SwitchToRunMode();

        Task<bool> StateRequest(JPowerStateRequest stateRequest);

        Task<bool> ZeroOffset();

        Task<bool> PushSlope(Slope slope);

        Task<Slope> PullSlope();
    }
}
