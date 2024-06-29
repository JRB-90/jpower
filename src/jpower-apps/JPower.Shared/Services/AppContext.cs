using JPower.Shared.Ble;
using JPower.Shared.Calibration;
using JPower.Shared.JPowDevice;
using JPower.Shared.JPower;

namespace JPower.Shared.Services
{
    public class AppContext : IAppContext
    {
        public AppContext()
        {
            IsBusy = false;
            BleDevice = null;
            LegacyJPowerDevice = null;
            CalibrationContext = new CalibrationContext();
        }

        public bool IsBusy
        {
            get => isBusy;
            set
            {
                isBusy = value;
                BusyStateChanged?.Invoke(this, isBusy);
            }
        }

        public IBleDevice? BleDevice
        {
            get => bleDevice;
            set
            {
                bleDevice = value;
                BleDeviceChanged?.Invoke(this, bleDevice);
            }
        }

        public IJPowerDevice? JPowerDevice
        {
            get => jPowerDevice;
            set
            {
                jPowerDevice = value;
                JPowerDeviceChanged?.Invoke(this, jPowerDevice);
            }
        }

        public ILegacyJPowerDevice? LegacyJPowerDevice
        {
            get => legacyJPowerDevice;
            set
            {
                legacyJPowerDevice = value;
                LegacyJPowerDeviceChanged?.Invoke(this, legacyJPowerDevice);
            }
        }

        public ICalibrationContext CalibrationContext { get; }

        public event EventHandler<bool>? BusyStateChanged;

        public event EventHandler<IBleDevice?>? BleDeviceChanged;

        public event EventHandler<IJPowerDevice?>? JPowerDeviceChanged;

        public event EventHandler<ILegacyJPowerDevice?>? LegacyJPowerDeviceChanged;

        private bool isBusy;
        private IBleDevice? bleDevice;
        private IJPowerDevice? jPowerDevice;
        private ILegacyJPowerDevice? legacyJPowerDevice;
    }
}
