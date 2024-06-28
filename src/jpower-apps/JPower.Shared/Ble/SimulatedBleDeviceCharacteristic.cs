using CommunityToolkit.Mvvm.ComponentModel;

namespace JPower.Shared.Ble
{
    internal class SimulatedBleDeviceCharacteristic 
      :
        ObservableObject, 
        IBleDeviceCharacteristic
    {
        public SimulatedBleDeviceCharacteristic(
            Guid uuid,
            string name,
            bool canRead,
            bool canWrite,
            bool isNotifying,
            bool writeWithResponse)
        {
            UUID = uuid;
            Name = name;
            CanRead = canRead;
            CanWrite = canWrite;
            IsNotifying = isNotifying;
            WriteWithResponse = writeWithResponse;
            currentValue = new byte[0];
        }

        public Guid UUID { get; }

        public string Name { get; }

        public bool CanRead { get; }

        public bool CanWrite { get; }

        public bool IsNotifying { get; }

        public bool WriteWithResponse { get; }

        public byte[] CurrentValue
        {
            get => currentValue;
            private set
            {
                SetProperty(ref currentValue, value);
                ValueUpdated?.Invoke(this, value);
            }
        }

        public event EventHandler<byte[]>? ValueUpdated;

        public async Task StartListening()
        {
            await Task.Delay(100);
        }

        public async Task StopListening()
        {
            await Task.Delay(100);
        }

        public async Task<bool> WriteValue(byte[] value)
        {
            await Task.Delay(100);

            return true;
        }

        private byte[] currentValue;
    }
}
