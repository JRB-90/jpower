using JPower.Shared.Ble;

namespace JPower.Shared.JPower
{
    /// <summary>
    /// Here are stored all of the Nordic service UUIDs we use in JPower
    /// plus the custom BLE service UUIDs we have created so that they
    /// can all be identified.
    /// </summary>
    internal static class JPowerBleUUIDs
    {
        public static Guid JPOWER_SENSOR_SRV_UUID               = new Guid("0000a000-1212-efde-1523-785fef13d123");
        public static Guid JPOWER_SENSOR_SRV_DIAG_DATA_UUID     = new Guid("0000aa01-0000-1000-8000-00805f9b34fb");

        public static Guid JPOWER_BATTERY_SRV_UUID              = new Guid("0000180f-0000-1000-8000-00805f9b34fb");
        public static Guid JPOWER_BATTERY_LEVEL_SRV_UUID        = new Guid("00002a19-0000-1000-8000-00805f9b34fb");

        public static Guid JPOWER_CAL_SRV_UUID                  = new Guid("0000c000-1212-efde-1523-785fef13d123");
        public static Guid JPOWER_CAL_SRV_SEND_REQ_UUID         = new Guid("0000c001-0000-1000-8000-00805f9b34fb");
        public static Guid JPOWER_CAL_SRV_PUSH_CAL_UUID         = new Guid("0000cc02-0000-1000-8000-00805f9b34fb");
        public static Guid JPOWER_CAL_SRV_PULL_CAL_RES_UUID     = new Guid("0000cc03-0000-1000-8000-00805f9b34fb");
        public static Guid JPOWER_CAL_SRV_MEASURE_RES_UUID      = new Guid("0000cc04-0000-1000-8000-00805f9b34fb");
        public static Guid JPOWER_CAL_SRV_ZERO_RES_UUID         = new Guid("0000cc05-0000-1000-8000-00805f9b34fb");

        public static Guid[] JPOWER_SERVICES =
        {
            JPOWER_SENSOR_SRV_UUID,
            JPOWER_BATTERY_SRV_UUID,
            JPOWER_CAL_SRV_UUID,
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
