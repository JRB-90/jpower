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
            set => SetProperty(ref isBusy, value);
        }

        public BleDevice? DeviceManager
        {
            get => deviceManager;
            set
            {
                SetProperty(ref deviceManager, value);
                if (deviceManager != null )
                {
                    deviceManager.DeviceStateChanged += DeviceManager_DeviceStateChanged;
                }
            }
        }

        public JPowerDevice? JPower
        {
            get => jpower;
            set
            {
                SetProperty(ref jpower, value);
                if (jpower != null)
                {
                    JPowerVM = new JPowerViewModel(jpower);
                }
            }
        }

        public BleServicesViewModel? ServicesVM
        {
            get => servicesVM;
            private set => SetProperty(ref servicesVM, value);
        }

        public JPowerViewModel? JPowerVM
        {
            get => jpowerVM;
            set => SetProperty(ref jpowerVM, value);
        }

        public Command DisconnectCommand { get; }

        public override async Task OnNavigatingTo(object? parameter)
        {
            DeviceManager = parameter as BleDevice;

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

            ServicesVM = new BleServicesViewModel(DeviceManager.Services);
            //JPower = await JPowerDevice.CreateAsync(DeviceManager);
            //await JPower.StartStreaming();
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

        private async void DeviceManager_DeviceStateChanged(object? sender, BleDeviceState newState)
        {
            OnPropertyChanged(nameof(DeviceManager));
            DisconnectCommand.ChangeCanExecute();

            if (newState == BleDeviceState.Disconnected)
            {
                await alertService.DisplayAlert(
                    "Error",
                    "Device disconnected",
                    "OK"
                );

                await navigationService.NavigateToConnectPage();
            }
        }

        private readonly INavigationService navigationService;
        private readonly IAlertService alertService;
        private BleServicesViewModel? servicesVM;
        private JPowerViewModel? jpowerVM;
        private BleDevice? deviceManager;
        private JPowerDevice? jpower;
        private bool isBusy;
    }
}
