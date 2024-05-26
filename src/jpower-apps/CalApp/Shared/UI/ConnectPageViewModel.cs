using CalApp.Shared.Models;
using CalApp.Shared.Mvvm;
using CalApp.Shared.Services;
using System.Collections.ObjectModel;

namespace CalApp.Shared.UI
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

        public BleDeviceInfo? SelectedDevice
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
            if (SelectedDevice == null)
            {
                await alertService.DisplayAlert(
                    "Error",
                    "Failed to connect, no device selected",
                    "OK"
                );

                return;
            }

            try
            {
                IsBusy = true;

                await jpowerDiscovery.StopScan();
                var bleDevice = new BleDevice(SelectedDevice);
                var isConnected = await bleDevice.Connect();

                if (isConnected)
                {
                    await navigation.NavigateToCalibratePage(bleDevice);
                }
                else
                {
                    await alertService.DisplayAlert(
                        "Error",
                        "Failed to connect to device",
                        "OK"
                    );
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
                IsBusy = false;
            }
        }

        private void JpowerDiscovery_ScanStateChanged(object? sender, BleScanState e)
        {
            ScanCommand.ChangeCanExecute();
        }

        private void JpowerDiscovery_BleDeviceDiscovered(object? sender, BleDeviceInfo e)
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
        private BleDeviceInfo? selectedDevice;
        private bool isBusy;
    }
}
