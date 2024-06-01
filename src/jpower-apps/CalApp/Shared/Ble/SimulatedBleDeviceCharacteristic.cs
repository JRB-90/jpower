using CommunityToolkit.Mvvm.ComponentModel;

namespace CalApp.Shared.Ble
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
            bool isNotifying)
        {
            UUID = uuid;
            Name = name;
            CanRead = canRead;
            CanWrite = canWrite;
            IsNotifying = isNotifying;
            currentValue = new byte[0];
        }

        public Guid UUID { get; }

        public string Name { get; }

        public bool CanRead { get; }

        public bool CanWrite { get; }

        public bool IsNotifying { get; }

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

        private byte[] currentValue;
    }
}
