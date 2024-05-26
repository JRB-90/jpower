using Plugin.BLE.Abstractions.Contracts;

namespace CalApp.Shared.Models
{
    public class BleService
    {
        public BleService(IService service)
        {
            UUID = service.Id;
            Name =
                service.Name != null
                ? service.Name
                : "Unknown Service";

            var t = service.GetCharacteristicsAsync();
            t.Wait();

            var characteristics = new List<BleCharacteristic>();

            foreach (var characteristic in t.Result)
            {
                if (characteristic != null)
                {
                    characteristics.Add(new BleCharacteristic(characteristic));
                }
            }

            Characteristics = characteristics;
        }
        
        public Guid UUID { get; }

        public string Name { get; }

        public IReadOnlyCollection<BleCharacteristic> Characteristics { get; }
    }
}
