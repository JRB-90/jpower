using DotNext.Threading;
using Plugin.BLE;
using Plugin.BLE.Abstractions;
using Plugin.BLE.Abstractions.Contracts;

namespace CalApp.Shared.Models
{
    /// <summary>
    /// Currect state of a BLE device.
    /// </summary>
    public enum BleDeviceState
    {
        Connecting,
        Connected,
        Disconnecting,
        Disconnected,
    }

    /// <summary>
    /// Mamages the connection to and data exchange with a BLE device.
    /// </summary>
    public class BleDevice
    {
        public const int ConnectTimeoutMs = 5000;
        public const int DisconnectTimeoutMs = 5000;

        public BleDevice(BleDeviceInfo deviceInfo)
        {
            deviceState = BleDeviceState.Disconnected;
            services = new List<BleService>();
            device = deviceInfo.Device;
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
            set
            {
                deviceState = value;
                DeviceStateChanged?.Invoke(this, deviceState);
            }
        }

        public IReadOnlyCollection<BleService> Services
        {
            get => services;
            private set => services = value;
        }

        public event EventHandler<BleDeviceState>? DeviceStateChanged;

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

        public async Task Disconnect()
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
        }

        public bool IsJPowerDevice()
        {
            if (Services.Count == 0)
            {
                return false;
            }

            foreach (var serviceUUID in BleServiceUUIDs.JPOWER_SERVICES)
            {
                if (!Services.Select(x => x.UUID).Contains(serviceUUID))
                {
                    return false;
                }
            }

            return true;
        }

        public async Task<BleService> GetService(Guid serviceUUID)
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

            return new BleService(service);
        }

        public async Task<BleCharacteristic> GetCharacteristic(Guid serviceUUID, Guid charUUID)
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

            return new BleCharacteristic(characteristic);
        }

        private async Task PopulateServices()
        {
            var bleServices = new List<BleService>();
            var deviceServices = await device.GetServicesAsync();

            foreach (var service in deviceServices)
            {
                if (service != null)
                {
                    bleServices.Add(new BleService(service));
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
        private IReadOnlyCollection<BleService> services;
        private AsyncManualResetEvent connectResetEvent;
        private AsyncManualResetEvent disconnectResetEvent;
    }
}
