using JPower.Shared.JPowDevice;
using Plugin.BLE;
using Plugin.BLE.Abstractions.Contracts;
using Plugin.BLE.Abstractions.EventArgs;
using JPower.Shared.Ble;
using JPower.Shared.JPower;

namespace JPower.Shared.Services
{
    public class BleService : IBleService
    {
        public BleService()
        {
            scanningState = BleScanningState.Idle;
            knownDevices = new Dictionary<Guid, IDevice>();
            adapter = CrossBluetoothLE.Current.Adapter;
            adapter.DeviceDiscovered += Adapter_DeviceDiscovered;
            adapter.ScanTimeoutElapsed += Adapter_ScanTimeoutElapsed;
        }

        public BleScanningState ScanningState
        {
            get => scanningState;
            set
            {
                scanningState = value;
                ScanningStateChanged?.Invoke(this, scanningState);
            }
        }

        public event EventHandler<BleScanningState>? ScanningStateChanged;

        public event EventHandler<BleDeviceInfo>? BleDeviceDiscovered;

        public async Task ScanForDevices()
        {
            if (!adapter.IsScanning)
            {
                ScanningState = BleScanningState.Scanning;
                await adapter.StartScanningForDevicesAsync();
            }
        }

        public async Task StopScan()
        {
            if (adapter.IsScanning)
            {
                await adapter.StopScanningForDevicesAsync();
                ScanningState = BleScanningState.Interupted;
            }
        }

        public Task<IJPowerDevice> CreateJPowerDevice(IBleDevice bleDevice)
        {
            throw new NotImplementedException();

            // TODO

            //if (bleDevice is BleDevice device)
            //{
            //    IJPowerDevice jPowerDevice = new JPowerDevice(device);

            //    return Task.FromResult(jPowerDevice);
            //}
            //else
            //{
            //    throw new ArgumentException("Cannot create JPower device");
            //}
        }

        public Task<ILegacyJPowerDevice> CreateLegacyJPowerDevice(IBleDevice bleDevice)
        {
            if (bleDevice is BleDevice device)
            {
                ILegacyJPowerDevice jPowerDevice = new LegacyJPowerDevice(device);

                return Task.FromResult(jPowerDevice);
            }
            else
            {
                throw new ArgumentException("Cannot create JPower device");
            }
        }

        private void Adapter_DeviceDiscovered(object? sender, DeviceEventArgs e)
        {
            if (e.Device.IsConnectable)
            {
                if (!knownDevices.ContainsKey(e.Device.Id))
                {
                    knownDevices.Add(e.Device.Id, e.Device);
                }

                var device = new BleDeviceInfo(e.Device.Id, e.Device.Name);
                BleDeviceDiscovered?.Invoke(this, device);
            }
        }

        private void Adapter_ScanTimeoutElapsed(object? sender, EventArgs e)
        {
            ScanningState = BleScanningState.Completed;
        }

        public Task<IBleDevice> CreateBleDevice(BleDeviceInfo deviceInfo)
        {
            if (knownDevices.TryGetValue(deviceInfo.UUID, out var device))
            {
                IBleDevice bleDevice = new BleDevice(device);

                return Task.FromResult(bleDevice);
            }
            else
            {
                throw new ArgumentException(
                    $"BLE Device not found with UUID: {deviceInfo.UUID}"
                );
            }
        }

        public Task<bool> IsJPowerDevice(IBleDevice device)
        {
            return Task.FromResult(device.IsLegacyJPowerDevice());
        }

        private BleScanningState scanningState;
        private IAdapter adapter;
        private Dictionary<Guid, IDevice> knownDevices;
    }
}
