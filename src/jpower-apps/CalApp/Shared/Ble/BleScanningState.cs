namespace CalApp.Shared.Ble
{
    /// <summary>
    /// Tracks the device scanning state of a BLE adapter.
    /// </summary>
    public enum BleScanningState
    {
        Idle,
        Scanning,
        Completed,
        Interupted
    }
}
