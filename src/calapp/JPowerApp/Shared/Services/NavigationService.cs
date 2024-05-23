using JPowerApp.Shared.Mvvm;
using JPowerApp.Shared.UI;
using System.Diagnostics;
using System.Diagnostics.Contracts;

namespace JPowerApp.Shared.Services
{
    public class NavigationService : INavigationService
    {
        public NavigationService(IServiceProvider serviceProvider)
        {
            this.serviceProvider = serviceProvider;
        }

        protected INavigation Navigation
        {
            get
            {
                INavigation? navigation = Application.Current?.MainPage?.Navigation;

                if (navigation is not null)
                {
                    return navigation;
                }
                else
                {
                    if (Debugger.IsAttached)
                    {
                        Debugger.Break();
                    }

                    throw new Exception();
                }
            }
        }

        public Task NavigateToConnectPage()
        {
            return NavigateToPage<ConnectPageView>();
        }

        public Task NavigateToCalibratePage()
        {
            return NavigateToPage<CalibratePageView>();
        }

        public Task NavigateBack()
        {
            if (Navigation.NavigationStack.Count > 1)
            {
                return Navigation.PopAsync();
            }
            else
            {
                throw new InvalidOperationException("No pages to navigate back to!");
            }
        }

        private async Task NavigateToPage<T>(object? parameter = null) where T : Page
        {
            var toPage = ResolvePage<T>();

            if (toPage is not null)
            {
                toPage.NavigatedTo += Page_NavigatedTo; ;
                var toViewModel = GetPageViewModelBase(toPage);

                if (toViewModel is not null)
                {
                    await toViewModel.OnNavigatingTo(parameter);
                }

                await Navigation.PushAsync(toPage, true);
                toPage.NavigatedFrom += Page_NavigatedFrom;
            }
            else
            {
                throw new InvalidOperationException($"Unable to resolve type {typeof(T).FullName}");
            }
        }

        private async void Page_NavigatedTo(object sender, NavigatedToEventArgs e)
        {
            await CallNavigatedTo(sender as Page);
        }

        private async void Page_NavigatedFrom(object sender, NavigatedFromEventArgs e)
        {
            bool isForwardNavigation = 
                Navigation.NavigationStack.Count > 1 &&
                Navigation.NavigationStack[^2] == sender;

            if (sender is Page thisPage)
            {
                if (!isForwardNavigation)
                {
                    thisPage.NavigatedTo -= Page_NavigatedTo;
                    thisPage.NavigatedFrom -= Page_NavigatedFrom;
                }

                await CallNavigatedFrom(thisPage, isForwardNavigation);
            }
        }

        private T? ResolvePage<T>() where T : Page
        {
            return serviceProvider.GetService<T>();
        }

        private ViewModelBase? GetPageViewModelBase(Page? p)
        {
            return p?.BindingContext as ViewModelBase;
        }

        private Task CallNavigatedTo(Page? p)
        {
            var fromViewModel = GetPageViewModelBase(p);

            if (fromViewModel is not null)
            {
                return fromViewModel.OnNavigatedTo();
            }
            else
            {
                return Task.CompletedTask;
            }
        }

        private Task CallNavigatedFrom(Page p, bool isForward)
        {
            var fromViewModel = GetPageViewModelBase(p);

            if (fromViewModel is not null)
            {
                return fromViewModel.OnNavigatedFrom(isForward);
            }
            else
            {
                return Task.CompletedTask;
            }
        }

        readonly IServiceProvider serviceProvider;
    }
}
