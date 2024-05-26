using Plugin.BLE.Abstractions.Contracts;

namespace CalApp.Shared.Models
{
    public class BleCharacteristic
    {
        public BleCharacteristic(ICharacteristic characteristic)
        {
            UUID = characteristic.Id;
            Name =
                characteristic.Name != null
                ? characteristic.Name
                : "Unknown characteristic";
        }

        public Guid UUID { get; }

        public string Name { get; }
    }
}
