using CalApp.Shared.Ble;
using CalApp.Shared.Calibration;

namespace CalApp.Shared.Services
{
    public interface IAppContext
    {
        bool IsBusy { get; set; }

        IBleDevice? BleDevice { get; set; }

        IJPowerDevice? JPowerDevice { get; set; }

        event EventHandler<bool>? BusyStateChanged;

        event EventHandler<IBleDevice?>? BleDeviceChanged;

        event EventHandler<IJPowerDevice?>? JPowerDeviceChanged;
    }
}
