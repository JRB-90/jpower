using CalApp.Shared.Ble;

namespace CalApp.Shared.Services
{
    public interface INavigationService
    {
        Task NavigateToConnectPage();
        Task NavigateToDeviceOverviewPage();
        Task NavigateBack();
    }
}
