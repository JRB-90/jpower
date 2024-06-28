
using JPower.Shared.Ble;
using JPower.Shared.Mvvm;
using JPower.Shared.Services;

namespace CalApp.Shared.UI
{
    public class DeviceInfoPageViewModel : ViewModelBase
    {
        public DeviceInfoPageViewModel(IAppContext appContext)
        {
            this.appContext = appContext;
            
            if (appContext.BleDevice != null)
            {
                services = appContext.BleDevice.Services;
                appContext.BleDevice.DeviceStateChanged += BleDevice_DeviceStateChanged;
            }
            else
            {
                services = new List<IBleDeviceService>();
            }

            appContext.BusyStateChanged += AppContext_BusyStateChanged;
        }

        public bool IsBusy => appContext.IsBusy;

        public IReadOnlyCollection<IBleDeviceService> Services
        {
            get => services;
            private set => SetProperty(ref services, value);
        }

        private void AppContext_BusyStateChanged(object? sender, bool e)
        {
            OnPropertyChanged(nameof(IsBusy));
        }

        private void BleDevice_DeviceStateChanged(object? sender, BleDeviceState e)
        {
            if (e == BleDeviceState.Connected &&
                appContext.BleDevice != null)
            {
                Services = appContext.BleDevice.Services;
            }
        }

        private readonly IAppContext appContext;
        private IReadOnlyCollection<IBleDeviceService> services;
    }
}
