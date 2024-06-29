using JPower.Shared.Ble;
using JPower.Shared.JPowDevice;
using JPower.Shared.JPower;

namespace JPower.Shared.Services
{
    public interface IAppContext
    {
        bool IsBusy { get; set; }

        IBleDevice? BleDevice { get; set; }

        IJPowerDevice? JPowerDevice { get; set; }

        ILegacyJPowerDevice? LegacyJPowerDevice { get; set; }

        ICalibrationContext CalibrationContext { get; }

        event EventHandler<bool>? BusyStateChanged;

        event EventHandler<IBleDevice?>? BleDeviceChanged;

        event EventHandler<IJPowerDevice?>? JPowerDeviceChanged;

        event EventHandler<ILegacyJPowerDevice?>? LegacyJPowerDeviceChanged;
    }
}
