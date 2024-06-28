namespace JPower.Shared.Ble
{
    /// <summary>
    /// Currect state of a BLE device.
    /// </summary>
    public enum BleDeviceState
    {
        Connecting,
        Connected,
        Disconnecting,
        Disconnected,
    }
}
