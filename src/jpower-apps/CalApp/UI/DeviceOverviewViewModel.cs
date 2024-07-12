using CalApp.Services;
using JPower.Shared.Mvvm;
using JPower.Shared.Services;

namespace CalApp.Shared.UI
{
    public class DeviceOverviewViewModel : ViewModelBase
    {
        public DeviceOverviewViewModel(
            IAppContext appContext,
            INavigationService navigationService,
            IAlertService alertService,
            IBleService bleService)
        {
            this.appContext = appContext;
            this.navigationService = navigationService;
            this.alertService = alertService;
            this.bleService = bleService;

            appContext.BusyStateChanged += AppContext_BusyStateChanged;
        }

        public bool IsBusy => appContext.IsBusy;

        private void AppContext_BusyStateChanged(object? sender, bool e)
        {
            OnPropertyChanged(nameof(IsBusy));
        }

        private readonly IAppContext appContext;
        private readonly INavigationService navigationService;
        private readonly IAlertService alertService;
        private readonly IBleService bleService;
    }
}
