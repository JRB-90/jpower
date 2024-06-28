using CalApp.Shared.Services;
using JPower.Shared.Ble;
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

        public override async Task OnNavigatingTo(object? parameter)
        {
            if (appContext.BleDevice == null)
            {
                await alertService.DisplayAlert(
                    "Error",
                    "No device selected, returning to connect page",
                    "OK"
                );

                await navigationService.NavigateToConnectPage();

                return;
            }

            if (appContext.BleDevice.DeviceState != BleDeviceState.Connected)
            {
                await alertService.DisplayAlert(
                    "Error",
                    "Device disconnected, returning to connect page",
                    "OK"
                );

                await navigationService.NavigateToConnectPage();
            }

            if (appContext.JPowerDevice == null)
            {

            }
        }

        public override async Task OnNavigatedFrom(bool isForwardNavigation)
        {
            try
            {
                appContext.IsBusy = true;

                if (!isForwardNavigation)
                {
                    if (appContext.JPowerDevice != null)
                    {
                        await appContext.JPowerDevice.StopStreaming();
                    }

                    if (appContext.BleDevice != null)
                    {
                        await appContext.BleDevice.Disconnect();
                    }
                }
            }
            catch (Exception ex)
            {
                await alertService.DisplayAlert(
                    "Error",
                    ex.Message,
                    "OK"
                );
            }
            finally
            {
                appContext.IsBusy = false;
            }
        }

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
