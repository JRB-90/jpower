using JPower.Shared.Ble;

namespace JPower.Shared.JPowDevice
{
    /// <summary>
    /// Here are stored all of the Nordic service UUIDs we use in JPower
    /// plus the custom BLE service UUIDs we have created so that they
    /// can all be identified.
    /// </summary>
    internal static class JPowerBleUUIDs
    {
        public static Guid JPOWER_CAL_SRV_UUID                      = new Guid("0000a000-1212-efde-1523-785fef13d123");
        public static Guid JPOWER_CAL_SRV_ADC_RAW_UUID              = new Guid("0000aa01-0000-1000-8000-00805f9b34fb");
        public static Guid JPOWER_CAL_SRV_ZERO_UUID                 = new Guid("0000aa02-0000-1000-8000-00805f9b34fb");
        public static Guid JPOWER_CAL_SRV_PULL_CAL_REQ_UUID         = new Guid("0000aa03-0000-1000-8000-00805f9b34fb");
        public static Guid JPOWER_CAL_SRV_PULL_CAL_UUID             = new Guid("0000aa04-0000-1000-8000-00805f9b34fb");
        public static Guid JPOWER_CAL_SRV_PUSH_CAL_UUID             = new Guid("0000aa05-0000-1000-8000-00805f9b34fb");

        public static Guid JPOWER_JP_STATE_SRV_UUID                 = new Guid("0000b000-1212-efde-1523-785fef13d123");
        public static Guid JPOWER_JP_STATE_SRV_STATE_REQUEST_UUID   = new Guid("0000bb01-0000-1000-8000-00805f9b34fb");
        public static Guid JPOWER_JP_STATE_SRV_CURRENT_STATE_UUID   = new Guid("0000bb02-0000-1000-8000-00805f9b34fb");

        public static Guid[] JPOWER_SERVICES =
        {
            JPOWER_CAL_SRV_UUID,
            JPOWER_JP_STATE_SRV_UUID
        };

        public static bool IsJPowerDevice(this IBleDevice bleDevice)
        {
            if (bleDevice.Services.Count == 0)
            {
                return false;
            }

            foreach (var serviceUUID in JPOWER_SERVICES)
            {
                if (!bleDevice.Services.Select(x => x.UUID).Contains(serviceUUID))
                {
                    return false;
                }
            }

            return true;
        }
    }
}
