using JPower.Shared.JPowDevice;
using JPower.Shared.Services;

namespace JPower.Shared.Ble
{
    public class SimulatedBleService : IBleService
    {
        public SimulatedBleService()
        {
            scanningState = BleScanningState.Idle;
            random = new Random((int)DateTime.UtcNow.Ticks);
        }

        public BleScanningState ScanningState
        {
            get => scanningState;
            set
            {
                scanningState = value;
                ScanningStateChanged?.Invoke(this, scanningState);
            }
        }

        public event EventHandler<BleScanningState>? ScanningStateChanged;

        public event EventHandler<BleDeviceInfo>? BleDeviceDiscovered;

        public async Task ScanForDevices()
        {
            ScanningState = BleScanningState.Scanning;
            var numDevices = random.Next(8, 15);

            for (int i = 1; i < numDevices + 1; i++)
            {
                await Task.Delay(random.Next(100, 1000));

                var deviceInfo = 
                    new BleDeviceInfo(
                        Guid.NewGuid(),
                        $"BLE Device {i}"
                    );

                BleDeviceDiscovered?.Invoke(this, deviceInfo);
            }

            ScanningState = BleScanningState.Completed;
        }

        public async Task StopScan()
        {
            await Task.Delay(random.Next(100, 500));

            if (ScanningState == BleScanningState.Scanning)
            {
                ScanningState = BleScanningState.Interupted;
            }
            else
            {
                ScanningState = BleScanningState.Idle;
            }
        }

        public async Task<IBleDevice> CreateBleDevice(BleDeviceInfo deviceInfo)
        {
            await Task.Delay(random.Next(100, 500));

            return new SimulatedBleDevice(deviceInfo);
        }

        public async Task<IJPowerDevice> CreateJPowerDevice(IBleDevice bleDevice)
        {
            await Task.Delay(random.Next(100, 500));

            return new SimulatedJPowerDevice();
        }

        public async Task<bool> IsJPowerDevice(IBleDevice device)
        {
            await Task.Delay(100);

            return true;
        }

        private BleScanningState scanningState;
        private Random random;
    }
}
