using JPowerApp.Shared.Models;
using JPowerApp.Shared.Mvvm;
using JPowerApp.Shared.Services;
using System.Collections.ObjectModel;

namespace JPowerApp.Shared.UI
{
    public class ConnectPageViewModel : ViewModelBase
    {
        public ConnectPageViewModel(
            INavigationService navigation,
            IJPowerDiscoveryService jpowerDiscovery)
        {
            this.navigation = navigation;
            this.jpowerDiscovery = jpowerDiscovery;
            DiscoveredDevices = new ObservableCollection<BleDeviceModel>();
            selectedDevice = null;
            ScanCommand = new Command(async () => await Scan());
            ConnectCommand = 
                new Command(
                    async () => await Connect(),
                    () => SelectedDevice != null
                );
        }

        public ObservableCollection<BleDeviceModel> DiscoveredDevices { get; }

        public BleDeviceModel SelectedDevice
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
            DiscoveredDevices.Clear();
            await Task.Delay(1000);
            DiscoveredDevices.Add(new BleDeviceModel("Test Device"));
        }

        private async Task Connect()
        {
            await navigation.NavigateToCalibratePage();
        }

        private readonly INavigationService navigation;
        private readonly IJPowerDiscoveryService jpowerDiscovery;
        private BleDeviceModel selectedDevice;
    }
}
