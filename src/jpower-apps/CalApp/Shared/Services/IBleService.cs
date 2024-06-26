﻿using CalApp.Shared.Ble;
using CalApp.Shared.JPower;

namespace CalApp.Shared.Services
{
    public interface IBleService
    {
        BleScanningState ScanningState { get; }

        event EventHandler<BleScanningState>? ScanningStateChanged;

        event EventHandler<BleDeviceInfo>? BleDeviceDiscovered;

        Task ScanForDevices();

        Task StopScan();

        Task<IBleDevice> CreateBleDevice(BleDeviceInfo deviceInfo);

        Task<IJPowerDevice> CreateJPowerDevice(IBleDevice bleDevice);

        Task<bool> IsJPowerDevice(IBleDevice device);
    }
}
