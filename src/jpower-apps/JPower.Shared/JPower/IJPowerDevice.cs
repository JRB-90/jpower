using System.Runtime.InteropServices;

namespace JPower.Shared.JPower
{
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

    [StructLayout(LayoutKind.Explicit, Size = 16, CharSet = CharSet.Ansi)]
    public struct JPowerOrientData
    {
        [FieldOffset(0)] public float w;
        [FieldOffset(4)] public float x;
        [FieldOffset(8)] public float y;
        [FieldOffset(12)] public float z;
    }

    public interface IJPowerDevice
    {
        uint AdcValue { get; }

        ushort PowerValue { get; }

        Vector3D AccelValue { get; }

        Vector3D GyroValue { get; }

        Vector3D OrientValue { get; }

        ushort CadenceValue { get; }

        float TempValue { get; }

        ushort BatteryLevel { get; }

        IObservable<uint> AdcValues { get; }

        IObservable<ushort> PowerValues { get; }

        IObservable<Vector3D> AccelValues { get; }

        IObservable<Vector3D> GyroValues { get; }

        IObservable<Vector3D> OrientValues { get; }

        IObservable<ushort> CadenceValues { get; }

        IObservable<float> TempValues { get; }

        IObservable<ushort> BatteryLevels { get; }

        Task<bool> ZeroOffset();

        Task StartStreaming();

        Task StopStreaming();
    }
}
