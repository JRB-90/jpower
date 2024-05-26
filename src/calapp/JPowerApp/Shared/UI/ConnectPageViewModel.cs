using JPowerApp.Shared.Models;
using JPowerApp.Shared.Mvvm;
using JPowerApp.Shared.Services;
using System.Collections.ObjectModel;

namespace JPowerApp.Shared.UI
{
    public class ConnectPageViewModel : ViewModelBase
    {
        public ConnectPageViewModel(
            IPermissionsService permissionsService,
            INavigationService navigation,
            IAlertService alertService,
            IJPowerDiscoveryService jpowerDiscovery)
        {
            this.permissionsService = permissionsService;
            this.navigation = navigation;
            this.alertService = alertService;
            this.jpowerDiscovery = jpowerDiscovery;
            selectedDevice = null;
            isBusy = false;
            DiscoveredDevices = new ObservableCollection<BleDeviceInfo>();
            
            ScanCommand = 
                new Command(
                    async () => await Scan(),
                    () => jpowerDiscovery.ScanState != BleScanState.Scanning
                );
            
            ConnectCommand = 
                new Command(
                    async () => await Connect(),
                    () => SelectedDevice != null
                );

            jpowerDiscovery.ScanStateChanged += JpowerDiscovery_ScanStateChanged;
            jpowerDiscovery.BleDeviceDiscovered += JpowerDiscovery_BleDeviceDiscovered;
        }

        public ObservableCollection<BleDeviceInfo> DiscoveredDevices { get; }

        public BleDeviceInfo SelectedDevice
        {
            get => selectedDevice;
            set
            {
                selectedDevice = value;
                ConnectCommand.ChangeCanExecute();
            }
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

        public Command ScanCommand { get; }

        public Command ConnectCommand { get; }

        private async Task Scan()
        {
            if (await permissionsService.HasBLEPermission() == false)
            {
                await alertService.DisplayAlert(
                    "Permissions Error",
                    "App cannot function without BLE permission",
                    "OK"
                );

                return;
            }

            SelectedDevice = null;
            DiscoveredDevices.Clear();
            await jpowerDiscovery.ScanForDevices();
        }

        private async Task Connect()
        {
            IsBusy = true;

            await Task.Delay(3000);

            //await jpowerDiscovery.StopScan();
            //await navigation.NavigateToCalibratePage(SelectedDevice);

            IsBusy = false;
        }

        private void JpowerDiscovery_ScanStateChanged(object sender, BleScanState e)
        {
            ScanCommand.ChangeCanExecute();
        }

        private void JpowerDiscovery_BleDeviceDiscovered(object sender, BleDeviceInfo e)
        {
            MainThread.BeginInvokeOnMainThread(() =>
            {
                DiscoveredDevices.Add(e);
            });
        }

        private readonly IPermissionsService permissionsService;
        private readonly INavigationService navigation;
        private readonly IAlertService alertService;
        private readonly IJPowerDiscoveryService jpowerDiscovery;
        private BleDeviceInfo selectedDevice;
        private bool isBusy;
    }
}
