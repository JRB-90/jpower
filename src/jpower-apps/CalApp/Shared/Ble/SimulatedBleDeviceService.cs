namespace CalApp.Shared.Ble
{
    public class SimulatedBleDeviceService : IBleDeviceService
    {
        public SimulatedBleDeviceService(
            Guid uuid,
            string name)
        {
            UUID = uuid;
            Name = name;
            random = new Random(DateTime.UtcNow.Nanosecond);
            Characteristics = GenerateSimulatedChars();
        }

        public SimulatedBleDeviceService(
            Guid uuid,
            string name,
            IReadOnlyCollection<IBleDeviceCharacteristic> characteristics)
        {
            UUID = uuid;
            Name = name;
            Characteristics = characteristics;
            random = new Random(DateTime.UtcNow.Nanosecond);
        }

        public Guid UUID { get; }

        public string Name { get; }

        public IReadOnlyCollection<IBleDeviceCharacteristic> Characteristics { get; }

        private IReadOnlyCollection<IBleDeviceCharacteristic> GenerateSimulatedChars()
        {
            var chars = new List<IBleDeviceCharacteristic>();
            var numChars = random.Next(1, 5);

            for (int i = 1; i < numChars + 1; i++)
            {
                chars.Add(
                    new SimulatedBleDeviceCharacteristic(
                        Guid.NewGuid(),
                        $"Characteristic {i}",
                        true,
                        false,
                        true
                    )
                );
            }

            return chars;
        }

        private Random random;
    }
}
