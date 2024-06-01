using CalApp.Shared.Calibration;
using CalApp.Shared.Services;
using Plugin.BLE;
using Plugin.BLE.Abstractions.Contracts;
using Plugin.BLE.Abstractions.EventArgs;

namespace CalApp.Shared.Ble
{
    public class BleService : IBleService
    {
        public BleService()
        {
            scanningState = BleScanningState.Idle;
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

        public async Task<IJPowerDevice> CreateJPowerDevice(IBleDevice bleDevice)
        {
            // TODO
            throw new NotImplementedException();
        }

        public async Task<Slope> CalculateSlope(IReadOnlyCollection<Measurement> measurements)
        {
            // TODO
            throw new NotImplementedException();
        }

        private void Adapter_DeviceDiscovered(object? sender, DeviceEventArgs e)
        {
            if (e.Device.IsConnectable)
            {
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
            IBleDevice bleDevice = new BleDevice(deviceInfo);

            return Task.FromResult(bleDevice);
        }

        private BleScanningState scanningState;
        private IAdapter adapter;
    }
}
