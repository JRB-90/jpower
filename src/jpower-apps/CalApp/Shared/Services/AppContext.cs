using CalApp.Shared.Ble;
using CalApp.Shared.Calibration;
using CalApp.Shared.JPower;

namespace CalApp.Shared.Services
{
    public class AppContext : IAppContext
    {
        public AppContext()
        {
            IsBusy = false;
            BleDevice = null;
            JPowerDevice = null;
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

        public ICalibrationContext CalibrationContext { get; }

        public event EventHandler<bool>? BusyStateChanged;

        public event EventHandler<IBleDevice?>? BleDeviceChanged;

        public event EventHandler<IJPowerDevice?>? JPowerDeviceChanged;

        private bool isBusy;
        private IBleDevice? bleDevice;
        private IJPowerDevice? jPowerDevice;
    }
}
