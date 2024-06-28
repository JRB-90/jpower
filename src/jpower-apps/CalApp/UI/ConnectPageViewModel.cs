using CalApp.Shared.Services;
using JPower.Shared.Ble;
using JPower.Shared.Mvvm;
using JPower.Shared.Services;
using System.Collections.ObjectModel;

namespace CalApp.Shared.UI
{
    public class ConnectPageViewModel : ViewModelBase
    {
        public ConnectPageViewModel(
            IAppContext appContext,
            IPermissionsService permissionsService,
            INavigationService navigation,
            IAlertService alertService,
            IBleService bleService)
        {
            this.appContext = appContext;
            this.permissionsService = permissionsService;
            this.navigation = navigation;
            this.alertService = alertService;
            this.bleService = bleService;
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

            appContext.BusyStateChanged += AppContext_BusyStateChanged;
            bleService.ScanningStateChanged += JpowerDiscovery_ScanStateChanged;
            bleService.BleDeviceDiscovered += JpowerDiscovery_BleDeviceDiscovered;
        }

        public bool IsBusy => appContext.IsBusy;

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
                appContext.IsBusy = true;

                await bleService.StopScan();
                appContext.BleDevice = await bleService.CreateBleDevice(SelectedDevice);
                var isConnected = await appContext.BleDevice.Connect();

                if (isConnected)
                {
                    if (!await bleService.IsJPowerDevice(appContext.BleDevice))
                    {
                        throw new InvalidOperationException("Not a JPower device");
                    }

                    appContext.JPowerDevice = await bleService.CreateJPowerDevice(appContext.BleDevice);
                    
                    if (appContext.JPowerDevice == null)
                    {
                        throw new InvalidOperationException("Failed to create JPower device");
                    }

                    await appContext.JPowerDevice.StartStreaming();

                    await navigation.NavigateToDeviceOverviewPage();
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
                if (appContext.BleDevice != null &&
                    appContext.BleDevice.DeviceState == BleDeviceState.Connected)
                {
                    await appContext.BleDevice.Disconnect();
                }

                appContext.JPowerDevice = null;
                appContext.BleDevice = null;

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

        private readonly IAppContext appContext;
        private readonly IPermissionsService permissionsService;
        private readonly INavigationService navigation;
        private readonly IAlertService alertService;
        private readonly IBleService bleService;
        private BleDeviceInfo? selectedDevice;
    }
}
