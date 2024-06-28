namespace CalApp.Services
{
    public interface INavigationService
    {
        Task NavigateToConnectPage();
        Task NavigateToDeviceOverviewPage();
        Task NavigateBack();
    }
}
