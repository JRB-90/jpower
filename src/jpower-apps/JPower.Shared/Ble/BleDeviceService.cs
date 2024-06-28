using Plugin.BLE.Abstractions.Contracts;

namespace JPower.Shared.Ble
{
    public class BleDeviceService : IBleDeviceService
    {
        public BleDeviceService(IService service)
        {
            UUID = service.Id;
            Name =
                service.Name != null
                ? service.Name
                : "Unknown Service";

            var t = service.GetCharacteristicsAsync();
            t.Wait();

            var characteristics = new List<BleDeviceCharacteristic>();

            foreach (var characteristic in t.Result)
            {
                if (characteristic != null)
                {
                    characteristics.Add(new BleDeviceCharacteristic(characteristic));
                }
            }

            Characteristics = characteristics;
            Service = service;
        }

        public Guid UUID { get; }

        public string Name { get; }

        public IReadOnlyCollection<IBleDeviceCharacteristic> Characteristics { get; }

        public IService Service { get; }
    }
}
