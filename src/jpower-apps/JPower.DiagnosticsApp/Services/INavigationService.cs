namespace JPower.DiagnosticsApp.Services
{
    public interface INavigationService
    {
        Task NavigateToScanPage();
        Task NavigateToDiagnosticsPage();
        Task NavigateBack();
    }
}
