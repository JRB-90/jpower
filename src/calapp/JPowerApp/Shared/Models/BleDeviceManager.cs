using DotNext.Threading;
using Plugin.BLE;
using Plugin.BLE.Abstractions;
using Plugin.BLE.Abstractions.Contracts;

namespace JPowerApp.Shared.Models
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
    public class BleDeviceManager
    {
        public const int ConnectTimeoutMs = 5000;
        public const int DisconnectTimeoutMs = 2000;

        public BleDeviceManager(BleDeviceInfo deviceInfo)
        {
            deviceState = BleDeviceState.Disconnected;
            device = deviceInfo.Device;
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

        public event EventHandler<BleDeviceState> DeviceStateChanged;

        public async Task<bool> Connect()
        {
            try
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

                return true;
            }
            catch (Exception ex)
            {
                // TODO - Display to user?

                return false;
            }
        }

        public async Task<bool> Disconnect()
        {
            try
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
            catch (Exception ex)
            {
                // TODO - Display to user?

                return false;
            }
        }

        private void Adapter_DeviceConnected(object sender, Plugin.BLE.Abstractions.EventArgs.DeviceEventArgs e)
        {
            DeviceState = BleDeviceState.Connected;
            connectResetEvent.Set();
        }

        private void Adapter_DeviceConnectionError(object sender, Plugin.BLE.Abstractions.EventArgs.DeviceErrorEventArgs e)
        {
            DeviceState = BleDeviceState.Disconnected;
            connectResetEvent.Set();
        }

        private void Adapter_DeviceConnectionLost(object sender, Plugin.BLE.Abstractions.EventArgs.DeviceErrorEventArgs e)
        {
            DeviceState = BleDeviceState.Disconnected;
        }

        private void Adapter_DeviceDisconnected(object sender, Plugin.BLE.Abstractions.EventArgs.DeviceEventArgs e)
        {
            disconnectResetEvent.Set();
            DeviceState = BleDeviceState.Disconnected;
        }

        private readonly IAdapter adapter;
        private readonly IDevice device;
        private BleDeviceState deviceState;
        private AsyncManualResetEvent connectResetEvent;
        private AsyncManualResetEvent disconnectResetEvent;
    }
}
