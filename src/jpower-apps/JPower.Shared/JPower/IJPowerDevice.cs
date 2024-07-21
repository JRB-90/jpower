using System.Runtime.InteropServices;

namespace JPower.Shared.JPower
{
    public enum JPowerCalSrvRequest
    {
        CALIBRATE_SRV_PULL_CAL = 0x01,
        CALIBRATE_SRV_ZERO_OFFSET = 0x02,
        CALIBRATE_SRV_MEASURE = 0x03,
    }

    public enum JPowerCalSrvResponse
    {
        CALIBRATE_SRV_OK = 0x01,
        CALIBRATE_SRV_NOK = 0x02,
    }

    [StructLayout(LayoutKind.Explicit, Size = 12, CharSet = CharSet.Ansi)]
    public struct JPowerAccelData
    {
        [FieldOffset(0)] public float x;
        [FieldOffset(4)] public float y;
        [FieldOffset(8)] public float z;
    }

    [StructLayout(LayoutKind.Explicit, Size = 12, CharSet = CharSet.Ansi)]
    public struct JPowerGyroData
    {
        [FieldOffset(0)] public float rx;
        [FieldOffset(4)] public float ry;
        [FieldOffset(8)] public float rz;
    }

    [StructLayout(LayoutKind.Explicit, Size = 24, CharSet = CharSet.Ansi)]
    public struct JPowerImuData
    {
        [FieldOffset(0)] public JPowerAccelData accel;
        [FieldOffset(12)] public JPowerGyroData gyro;
    }

    [StructLayout(LayoutKind.Explicit, Size = 16, CharSet = CharSet.Ansi)]
    public struct JPowerOrientData
    {
        [FieldOffset(0)] public float w;
        [FieldOffset(4)] public float x;
        [FieldOffset(8)] public float y;
        [FieldOffset(12)] public float z;
    }

    [StructLayout(LayoutKind.Explicit, Size = 56, CharSet = CharSet.Ansi)]
    public struct JPowerSensorDiagData
    {
        [FieldOffset(0)] public float temp;
        [FieldOffset(4)] public uint adcValue;
        [FieldOffset(8)] public float force;
        [FieldOffset(12)] public float torque;
        [FieldOffset(16)] public JPowerImuData imuData;
        [FieldOffset(40)] public JPowerOrientData orientation;
        [FieldOffset(56)] public ushort cadence;
        [FieldOffset(58)] public ushort power;
    }

    [StructLayout(LayoutKind.Explicit, Size = 32, CharSet = CharSet.Ansi)]
    public struct JPowerCalibrationData
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        [FieldOffset(0)] public byte[] guid;
        [FieldOffset(16)] public float crankLength;
        [FieldOffset(20)] public ushort offset;
        [FieldOffset(24)] public float slope;
        [FieldOffset(28)] public float intercept;
    }

    public interface IJPowerDevice
    {
        uint AdcValue { get; }

        float ForceValue { get; }

        float TorqueValue { get; }

        ushort PowerValue { get; }

        Vector3D AccelValue { get; }

        Vector3D GyroValue { get; }

        Vector3D OrientValue { get; }

        ushort CadenceValue { get; }

        float TempValue { get; }

        ushort BatteryLevel { get; }

        IObservable<uint> AdcValues { get; }

        IObservable<float> ForceValues { get; }

        IObservable<float> TorqueValues { get; }

        IObservable<ushort> PowerValues { get; }

        IObservable<Vector3D> AccelValues { get; }

        IObservable<Vector3D> GyroValues { get; }

        IObservable<Vector3D> OrientValues { get; }

        IObservable<ushort> CadenceValues { get; }

        IObservable<float> TempValues { get; }

        IObservable<ushort> BatteryLevels { get; }

        Task StartStreaming();

        Task StopStreaming();

        Task<JPowerCalibrationData> PullCalibration();

        Task PushCalibration(JPowerCalibrationData calibration);

        Task<bool> ZeroOffset();

        Task<uint> Measure(byte numberSamples);
    }
}
