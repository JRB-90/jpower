using JPowerApp.Shared.Models;
using JPowerApp.Shared.Mvvm;
using JPowerApp.Shared.Services;

namespace JPowerApp.Shared.UI
{
    public class CalibratePageViewModel : ViewModelBase
    {
        public CalibratePageViewModel(INavigationService navigationService)
        {
            this.navigationService = navigationService;

            DisconnectCommand =
                new Command(
                    async () => await Disconnect(),
                    () => DeviceManager != null && 
                          DeviceManager.DeviceState == BleDeviceState.Connected
                );
        }

        public BleDeviceManager DeviceManager
        {
            get => deviceManager;
            set
            {
                deviceManager = value;
                deviceManager.DeviceStateChanged += DeviceManager_DeviceStateChanged;
                NotifyOfChange(nameof(DeviceManager));
            }
        }

        public Command DisconnectCommand { get; }

        public override async Task OnNavigatingTo(object parameter)
        {
            BleDeviceInfo deviceInfo = parameter as BleDeviceInfo;
            DeviceManager = new BleDeviceManager(deviceInfo);

            var isConnected = await DeviceManager.Connect();
            
            if (!isConnected)
            {
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
            var isDisconnected = await deviceManager.Disconnect();
            await navigationService.NavigateToConnectPage();
        }

        private void DeviceManager_DeviceStateChanged(object sender, BleDeviceState e)
        {
            NotifyOfChange(nameof(DeviceManager));
            DisconnectCommand.ChangeCanExecute();
        }

        private readonly INavigationService navigationService;
        private BleDeviceManager deviceManager;
    }
}
