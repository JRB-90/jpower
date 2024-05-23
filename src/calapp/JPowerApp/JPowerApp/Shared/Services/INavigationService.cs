namespace JPowerApp.Shared.Services
{
    public interface INavigationService
    {
        Task NavigateToConnectPage();
        Task NavigateToCalibratePage();
        Task NavigateBack();
    }
}
