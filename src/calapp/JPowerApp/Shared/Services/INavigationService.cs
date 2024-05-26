using JPowerApp.Shared.Models;

namespace JPowerApp.Shared.Services
{
    public interface INavigationService
    {
        Task NavigateToConnectPage();
        Task NavigateToCalibratePage(BleDeviceInfo deviceInfo);
        Task NavigateBack();
    }
}
