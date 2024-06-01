using CalApp.Shared.Ble;
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
            IBleService bleService)
        {
            this.permissionsService = permissionsService;
            this.navigation = navigation;
            this.alertService = alertService;
            this.bleService = bleService;
            isBusy = false;
            DiscoveredDevices = new ObservableCollection<BleDeviceInfo>();
            
            ScanCommand = 
                new Command(
                    async () => await Scan(),
                    () => bleService.ScanningState != BleScanningState.Scanning
                );
            
            ConnectCommand = 
                new Command(
                    async () => await Connect(),
                    () => SelectedDevice != null
                );

            bleService.ScanningStateChanged += JpowerDiscovery_ScanStateChanged;
            bleService.BleDeviceDiscovered += JpowerDiscovery_BleDeviceDiscovered;
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
            set => SetProperty(ref isBusy, value);
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
            await bleService.ScanForDevices();
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

                await bleService.StopScan();
                var bleDevice = await bleService.CreateBleDevice(SelectedDevice);
                var isConnected = await bleDevice.Connect();

                if (isConnected)
                {
                    //if (!bleDevice.IsJPowerDevice())
                    //{
                    //    throw new InvalidOperationException("Not a JPower device");
                    //}

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

        private void JpowerDiscovery_ScanStateChanged(object? sender, BleScanningState e)
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
        private readonly IBleService bleService;
        private BleDeviceInfo? selectedDevice;
        private bool isBusy;
    }
}
