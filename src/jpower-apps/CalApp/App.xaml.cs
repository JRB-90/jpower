using CalApp.Shared.Services;

namespace CalApp
{
    public partial class App : Application
    {
        public App(INavigationService navigationService)
        {
            InitializeComponent();
            MainPage = new NavigationPage();
            navigationService.NavigateToConnectPage();
        }

        protected override Window CreateWindow(IActivationState? activationState)
        {
            var window = base.CreateWindow(activationState);

            const int newWidth = 600;
            const int newHeight = 800;

            window.Width = newWidth;
            window.Height = newHeight;

            return window;
        }
    }
}
