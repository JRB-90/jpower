namespace JPower.Shared.Ble
{
    public class SimulatedBleDevice : IBleDevice
    {
        public SimulatedBleDevice(BleDeviceInfo bleDeviceInfo)
        {
            BleDeviceInfo = bleDeviceInfo;
            random = new Random(DateTime.UtcNow.Nanosecond);
            services = CreateFakeServices();
        }

        public BleDeviceInfo BleDeviceInfo { get; }

        public BleDeviceState DeviceState
        {
            get => deviceState;
            private set
            {
                deviceState = value;
                DeviceStateChanged?.Invoke(this, deviceState);
            }
        }

        public IReadOnlyCollection<IBleDeviceService> Services
        {
            get => services;
            private set
            {
                services = value;
                DevicePropertiesUpdated?.Invoke(this, new EventArgs());
            }
        }

        public event EventHandler<BleDeviceState>? DeviceStateChanged;

        public event EventHandler? DevicePropertiesUpdated;

        public async Task<bool> Connect()
        {
            DeviceState = BleDeviceState.Connecting;
            await Task.Delay(random.Next(500, 1500));
            DeviceState = BleDeviceState.Connected;

            return true;
        }

        public async Task<bool> Disconnect()
        {
            DeviceState = BleDeviceState.Disconnecting;
            await Task.Delay(random.Next(100, 500));
            DeviceState = BleDeviceState.Disconnected;

            return true;
        }

        private IReadOnlyCollection<IBleDeviceService> CreateFakeServices()
        {
            var services = new List<IBleDeviceService>();
            var numServices = random.Next(5, 8);

            for (int i = 1; i < numServices + 1; i++)
            {
                services.Add(
                    new SimulatedBleDeviceService(
                        Guid.NewGuid(),
                        $"Service {i}"
                    )
                );
            }

            return services;
        }

        private BleDeviceState deviceState;
        private IReadOnlyCollection<IBleDeviceService> services;

        private Random random;
    }
}
