namespace CalApp.Shared.Models
{
    /// <summary>
    /// Here are stored all of the Nordic service UUIDs we use in JPower
    /// plus the custom BLE service UUIDs we have created so that they
    /// can all be identified.
    /// </summary>
    internal static class BleServiceUUIDs
    {
        public static Guid JPOWER_CAL_SRV_UUID = new Guid("0000abcd-1212-efde-1523-785fef13d123");
        public static Guid JPOWER_CAL_SRV_ADC_RAW_UUID = new Guid("0000beef-0000-1000-8000-00805f9b34fb");
    }
}
