using CalApp.Shared.Models;
using CalApp.Shared.Mvvm;
using CalApp.Shared.Services;

namespace CalApp.Shared.UI
{
    public class CalibratePageViewModel : ViewModelBase
    {
        public CalibratePageViewModel(
            INavigationService navigationService,
            IAlertService alertService)
        {
            this.navigationService = navigationService;
            this.alertService = alertService;

            DisconnectCommand =
                new Command(
                    async () => await Disconnect(),
                    () => DeviceManager != null &&
                          DeviceManager.DeviceState == BleDeviceState.Connected
                );
        }

        public bool IsBusy
        {
            get => isBusy;
            set
            {
                isBusy = value;
                NotifyOfChange(nameof(IsBusy));
            }
        }

        public BleDeviceManager? DeviceManager
        {
            get => deviceManager;
            set
            {
                deviceManager = value;
                if ( deviceManager != null )
                {
                    deviceManager.DeviceStateChanged += DeviceManager_DeviceStateChanged;
                }
                NotifyOfChange(nameof(DeviceManager));
            }
        }

        public Command DisconnectCommand { get; }

        public override async Task OnNavigatingTo(object? parameter)
        {
            DeviceManager = parameter as BleDeviceManager;

            if (DeviceManager == null)
            {
                await alertService.DisplayAlert(
                    "Error",
                    "No device selected, returning to connect page",
                    "OK"
                );

                await navigationService.NavigateToConnectPage();

                return;
            }

            if (DeviceManager.DeviceState != BleDeviceState.Connected)
            {
                await alertService.DisplayAlert(
                    "Error",
                    "Device disconnected, returning to connect page",
                    "OK"
                );

                await navigationService.NavigateToConnectPage();
            }
        }

        public override async Task OnNavigatedFrom(bool isForwardNavigation)
        {
            if (!isForwardNavigation)
            {
                await Disconnect();
            }
        }

        private async Task Disconnect()
        {
            if (DeviceManager != null)
            {
                try
                {
                    IsBusy = true;
                    await DeviceManager.Disconnect();
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
                    IsBusy = false;
                }
            }

            await navigationService.NavigateToConnectPage();
        }

        private void DeviceManager_DeviceStateChanged(object? sender, BleDeviceState e)
        {
            NotifyOfChange(nameof(DeviceManager));
            DisconnectCommand.ChangeCanExecute();
        }

        private readonly INavigationService navigationService;
        private readonly IAlertService alertService;
        private BleDeviceManager? deviceManager;
        private bool isBusy;
    }
}
