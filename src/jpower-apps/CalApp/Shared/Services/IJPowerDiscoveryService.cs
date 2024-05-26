using CalApp.Shared.Models;

namespace CalApp.Shared.Services
{
    public interface IJPowerDiscoveryService
    {
        BleScanState ScanState { get; }

        event EventHandler<BleScanState> ScanStateChanged;

        event EventHandler<BleDeviceInfo> BleDeviceDiscovered;

        Task ScanForDevices();

        Task StopScan();
    }
}
