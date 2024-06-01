using CommunityToolkit.Mvvm.ComponentModel;
using Plugin.BLE.Abstractions.Contracts;

namespace CalApp.Shared.Ble
{
    public class BleDeviceCharacteristic
      :
        ObservableObject,
        IBleDeviceCharacteristic
    {
        public BleDeviceCharacteristic(ICharacteristic characteristic)
        {
            this.characteristic = characteristic;
            currentValue = new byte[0];

            UUID = characteristic.Id;
            Name =
                characteristic.Name != null
                ? characteristic.Name
                : "Unknown characteristic";

            characteristic.ValueUpdated += Characteristic_ValueUpdated;
        }

        public Guid UUID { get; }

        public string Name { get; }

        public bool CanRead { get; }

        public bool CanWrite { get; }

        public bool IsNotifying { get; }

        public byte[] CurrentValue
        {
            get => currentValue;
            private set => SetProperty(ref currentValue, value);
        }

        public event EventHandler<byte[]>? ValueUpdated;

        public async Task StartListening()
        {
            await characteristic.StartUpdatesAsync();
        }

        public async Task StopListening()
        {
            await characteristic.StopUpdatesAsync();
        }

        private void Characteristic_ValueUpdated(
            object? sender,
            Plugin.BLE.Abstractions.EventArgs.CharacteristicUpdatedEventArgs e)
        {
            CurrentValue = e.Characteristic.Value;
            ValueUpdated?.Invoke(this, currentValue);
        }

        private readonly ICharacteristic characteristic;
        private byte[] currentValue;
    }
}
