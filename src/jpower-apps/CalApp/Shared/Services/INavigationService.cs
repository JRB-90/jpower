using CalApp.Shared.Ble;

namespace CalApp.Shared.Services
{
    public interface INavigationService
    {
        Task NavigateToConnectPage();
        Task NavigateToCalibratePage(IBleDevice bleDevice);
        Task NavigateBack();
    }
}
