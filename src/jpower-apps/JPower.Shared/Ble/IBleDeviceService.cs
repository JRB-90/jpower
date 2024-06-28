namespace JPower.Shared.Ble
{
    public interface IBleDeviceService
    {
        Guid UUID { get; }

        string Name { get; }

        IReadOnlyCollection<IBleDeviceCharacteristic> Characteristics { get; }
    }
}
