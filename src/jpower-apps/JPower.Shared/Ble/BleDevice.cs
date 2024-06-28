using DotNext.Threading;
using Plugin.BLE;
using Plugin.BLE.Abstractions;
using Plugin.BLE.Abstractions.Contracts;

namespace JPower.Shared.Ble
{
    /// <summary>
    /// Manages the connection to and data exchange with a BLE device.
    /// </summary>
    public class BleDevice : IBleDevice
    {
        public const int ConnectTimeoutMs = 5000;
        public const int DisconnectTimeoutMs = 5000;

        public BleDevice(IDevice device)
        {
            this.device = device;
            deviceState = BleDeviceState.Disconnected;
            services = new List<BleDeviceService>();
            
            connectResetEvent = new AsyncManualResetEvent(false);
            disconnectResetEvent = new AsyncManualResetEvent(false);

            adapter = CrossBluetoothLE.Current.Adapter;
            adapter.DeviceConnected += Adapter_DeviceConnected;
            adapter.DeviceConnectionError += Adapter_DeviceConnectionError;
            adapter.DeviceConnectionLost += Adapter_DeviceConnectionLost;
            adapter.DeviceDisconnected += Adapter_DeviceDisconnected;
        }

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

        public IDevice Device => device;

        public BleDeviceInfo BleDeviceInfo => throw new NotImplementedException();

        public event EventHandler<BleDeviceState>? DeviceStateChanged;

        public event EventHandler? DevicePropertiesUpdated;

        public async Task<bool> Connect()
        {
            if (DeviceState != BleDeviceState.Disconnected)
            {
                throw new InvalidOperationException("Device is not in disconnected state");
            }

            DeviceState = BleDeviceState.Connecting;

            connectResetEvent = new AsyncManualResetEvent(false);
            var cancellationTokenSource = new CancellationTokenSource();
            var connectParameters =
                new ConnectParameters(
                    false,
                    false,
                    ConnectionParameterSet.Balanced
                );

            await adapter.ConnectToDeviceAsync(
                device,
                connectParameters,
                cancellationTokenSource.Token
            );

            var hasConnected =
                await connectResetEvent.WaitAsync(
                    TimeSpan.FromMilliseconds(ConnectTimeoutMs)
                );

            cancellationTokenSource.Cancel();

            if (!hasConnected)
            {
                throw new InvalidOperationException("Timed out waiting to connect");
            }

            if (deviceState != BleDeviceState.Connected)
            {
                throw new InvalidOperationException("Failed to connect");
            }

            await PopulateServices();

            return true;
        }

        public async Task<bool> Disconnect()
        {
            if (DeviceState != BleDeviceState.Connected)
            {
                throw new InvalidOperationException("Device is not in connected state");
            }

            DeviceState = BleDeviceState.Disconnecting;
            disconnectResetEvent = new AsyncManualResetEvent(false);

            await adapter.DisconnectDeviceAsync(device);

            var hasDisconnected =
                await disconnectResetEvent.WaitAsync(
                    TimeSpan.FromMilliseconds(DisconnectTimeoutMs)
                );

            if (!hasDisconnected)
            {
                DeviceState = BleDeviceState.Disconnected;
                throw new InvalidOperationException("Device is not in connected state");
            }

            return true;
        }

        public async Task<BleDeviceService> GetService(Guid serviceUUID)
        {
            if (DeviceState != BleDeviceState.Connected)
            {
                throw new InvalidOperationException("Not connected to device");
            }

            IService service = await device.GetServiceAsync(serviceUUID);
            if (service == null)
            {
                throw new InvalidOperationException("Failed to find service from UUID");
            }

            return new BleDeviceService(service);
        }

        public async Task<BleDeviceCharacteristic> GetCharacteristic(Guid serviceUUID, Guid charUUID)
        {
            if (DeviceState != BleDeviceState.Connected)
            {
                throw new InvalidOperationException("Not connected to device");
            }

            IService service = await device.GetServiceAsync(serviceUUID);
            if (service == null)
            {
                throw new InvalidOperationException("Failed to find service from UUID");
            }

            ICharacteristic characteristic = await service.GetCharacteristicAsync(charUUID);
            if (characteristic == null)
            {
                throw new InvalidOperationException("Failed to find characteristic from UUID");
            }

            return new BleDeviceCharacteristic(characteristic);
        }

        private async Task PopulateServices()
        {
            var bleServices = new List<BleDeviceService>();
            var deviceServices = await device.GetServicesAsync();

            foreach (var service in deviceServices)
            {
                if (service != null)
                {
                    bleServices.Add(new BleDeviceService(service));
                }
            }

            Services = bleServices;
        }

        private void Adapter_DeviceConnected(
            object? sender, 
            Plugin.BLE.Abstractions.EventArgs.DeviceEventArgs e)
        {
            DeviceState = BleDeviceState.Connected;
            connectResetEvent.Set();
        }

        private void Adapter_DeviceConnectionError(
            object? sender, 
            Plugin.BLE.Abstractions.EventArgs.DeviceErrorEventArgs e)
        {
            DeviceState = BleDeviceState.Disconnected;
            connectResetEvent.Set();
        }

        private void Adapter_DeviceConnectionLost(
            object? sender, 
            Plugin.BLE.Abstractions.EventArgs.DeviceErrorEventArgs e)
        {
            DeviceState = BleDeviceState.Disconnected;
        }

        private void Adapter_DeviceDisconnected(
            object? sender, 
            Plugin.BLE.Abstractions.EventArgs.DeviceEventArgs e)
        {
            disconnectResetEvent.Set();
            DeviceState = BleDeviceState.Disconnected;
        }

        private readonly IAdapter adapter;
        private readonly IDevice device;
        private BleDeviceState deviceState;
        private IReadOnlyCollection<IBleDeviceService> services;
        private AsyncManualResetEvent connectResetEvent;
        private AsyncManualResetEvent disconnectResetEvent;
    }
}
