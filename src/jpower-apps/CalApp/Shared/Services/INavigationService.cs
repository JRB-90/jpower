using CalApp.Shared.Models;

namespace CalApp.Shared.Services
{
    public interface INavigationService
    {
        Task NavigateToConnectPage();
        Task NavigateToCalibratePage(BleDeviceManager deviceManager);
        Task NavigateBack();
    }
}
