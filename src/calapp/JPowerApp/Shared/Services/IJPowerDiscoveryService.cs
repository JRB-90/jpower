using JPowerApp.Shared.Models;

namespace JPowerApp.Shared.Services
{
    public interface IJPowerDiscoveryService
    {
        BleScanState ScanState { get; }

        event EventHandler<BleScanState> ScanStateChanged;

        event EventHandler<BleDeviceModel> BleDeviceDiscovered;

        Task ScanForDevices();

        Task StopScan();
    }
}
