using JPowerApp.Shared.Services;

namespace JPowerApp;

public partial class App : Application
{
	public App(INavigationService navigationService)
	{
		InitializeComponent();
		MainPage = new NavigationPage();
        navigationService.NavigateToConnectPage();
	}

    protected override Window CreateWindow(IActivationState activationState)
    {
        var window = base.CreateWindow(activationState);

        const int newWidth = 400;
        const int newHeight = 600;

        window.Width = newWidth;
        window.Height = newHeight;

        return window;
    }
}
