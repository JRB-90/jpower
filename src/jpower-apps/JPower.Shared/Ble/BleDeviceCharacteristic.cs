using CommunityToolkit.Mvvm.ComponentModel;
using Plugin.BLE.Abstractions;
using Plugin.BLE.Abstractions.Contracts;

namespace JPower.Shared.Ble
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

            CanRead = characteristic.CanRead;
            CanWrite = characteristic.CanWrite;
            IsNotifying = characteristic.CanUpdate;
            characteristic.WriteType = CanWrite ? CharacteristicWriteType.WithResponse : CharacteristicWriteType.Default;
            WriteWithResponse = characteristic.WriteType == CharacteristicWriteType.WithResponse;

            characteristic.ValueUpdated += Characteristic_ValueUpdated;
        }

        public Guid UUID { get; }

        public string Name { get; }

        public bool CanRead { get; }

        public bool CanWrite { get; }

        public bool WriteWithResponse { get; }

        public bool IsNotifying { get; }

        public byte[] CurrentValue
        {
            get => currentValue;
            private set => SetProperty(ref currentValue, value);
        }

        public ICharacteristic Characteristic => characteristic;

        public event EventHandler<byte[]>? ValueUpdated;

        public async Task StartListening()
        {
            await characteristic.StartUpdatesAsync();
        }

        public async Task StopListening()
        {
            await characteristic.StopUpdatesAsync();
        }

        public async Task<bool> WriteValue(byte[] value)
        {
            return await characteristic.WriteAsync(value) == 0;
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
