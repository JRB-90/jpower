using CalApp.Shared.Ble;

namespace CalApp.Shared.JPower
{
    /// <summary>
    /// Here are stored all of the Nordic service UUIDs we use in JPower
    /// plus the custom BLE service UUIDs we have created so that they
    /// can all be identified.
    /// </summary>
    internal static class JPowerBleUUIDs
    {
        public static Guid JPOWER_CAL_SRV_UUID = new Guid("0000A000-1212-efde-1523-785fef13d123");
        public static Guid JPOWER_CAL_SRV_ADC_RAW_UUID = new Guid("0000AA01-1212-efde-1523-785fef13d123");

        public static Guid JPOWER_JP_STATE_SRV_UUID = new Guid("0000B000-1212-efde-1523-785fef13d123");
        public static Guid JPOWER_JP_STATE_SRV_GET_STATE_UUID = new Guid("0000BB01-1212-efde-1523-785fef13d123");
        public static Guid JPOWER_JP_STATE_SRV_ENTER_STATE_UUID = new Guid("0000BB02-1212-efde-1523-785fef13d123");

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
