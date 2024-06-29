namespace JPower.Shared.Ble
{
    public interface IBleDevice
    {
        BleDeviceState DeviceState { get; }

        IReadOnlyCollection<IBleDeviceService> Services { get; }

        event EventHandler<BleDeviceState>? DeviceStateChanged;

        event EventHandler? DevicePropertiesUpdated;

        Task<bool> Connect();

        Task<bool> Disconnect();
    }
}
