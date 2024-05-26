using JPowerApp.Shared.Models;
using Plugin.BLE;
using Plugin.BLE.Abstractions.Contracts;
using Plugin.BLE.Abstractions.EventArgs;

namespace JPowerApp.Shared.Services
{
    /// <summary>
    /// Tracks the device scanning state of a BLE adapter.
    /// </summary>
    public enum BleScanState
    {
        Idle,
        Scanning,
        Completed,
        Interupted
    }

    /// <summary>
    /// Service for discovering BLE devices.
    /// </summary>
    public class JPowerDiscoveryService : IJPowerDiscoveryService
    {
        public JPowerDiscoveryService()
        {
            scanState = BleScanState.Idle;
            adapter = CrossBluetoothLE.Current.Adapter;
            adapter.DeviceDiscovered += Adapter_DeviceDiscovered;
            adapter.ScanTimeoutElapsed += Adapter_ScanTimeoutElapsed;
        }

        public event EventHandler<BleScanState> ScanStateChanged;

        public event EventHandler<BleDeviceInfo> BleDeviceDiscovered;

        public BleScanState ScanState
        {
            get => scanState;
            private set
            {
                scanState = value;
                ScanStateChanged?.Invoke(this, scanState);
            }
        }

        public async Task ScanForDevices()
        {
            if (!adapter.IsScanning)
            {
                ScanState = BleScanState.Scanning;
                await adapter.StartScanningForDevicesAsync();
            }
        }
        
        public async Task StopScan()
        {
            if (adapter.IsScanning)
            {
                await adapter.StopScanningForDevicesAsync();
                ScanState = BleScanState.Interupted;
            }
        }

        private void Adapter_DeviceDiscovered(object sender, DeviceEventArgs e)
        {
            if (e.Device.IsConnectable)
            {
                BleDeviceDiscovered?.Invoke(this, new BleDeviceInfo(e.Device));
            }
        }

        private void Adapter_ScanTimeoutElapsed(object sender, EventArgs e)
        {
            ScanState = BleScanState.Completed;
        }

        private BleScanState scanState;
        private IAdapter adapter;
    }
}
