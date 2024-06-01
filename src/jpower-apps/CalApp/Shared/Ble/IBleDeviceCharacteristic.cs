namespace CalApp.Shared.Ble
{
    public interface IBleDeviceCharacteristic
    {
        Guid UUID { get; }

        string Name { get; }

        bool CanRead { get; }

        bool CanWrite { get; }

        bool IsNotifying { get; }

        byte[] CurrentValue { get; }

        event EventHandler<byte[]>? ValueUpdated;

        Task StartListening();

        Task StopListening();
    }
}
