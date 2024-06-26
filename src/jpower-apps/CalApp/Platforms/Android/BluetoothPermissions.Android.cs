﻿// From: https://gist.github.com/salarcode/da8ad2b993e67c602db88a62259d0456

// IMPORTANT: Put this file in `Platforms/Android/` directory.
using Microsoft.Maui.ApplicationModel;
using System.Collections.Generic;

/// <summary>
///		MAUI Bluetooth Permission Decleration (for Android).
/// </summary>
/// <remarks>
///		IMPORTANT: Put this file in `Platforms/Android/` directory.
/// </remarks>
public partial class BluetoothPermissions : Permissions.BasePlatformPermission
{
    private readonly bool _scan;
    private readonly bool _advertise;
    private readonly bool _connect;
    private readonly bool _bluetoothLocation;

    public BluetoothPermissions()
        : this(true, false, true, false)
    {
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="scan">Needed only if your app looks for Bluetooth devices.
    /// If your app doesn't use Bluetooth scan results to derive physical location, you can make a strong assertion that your app never uses the Bluetooth permissions to derive physical location. 
    /// Add the `android:usesPermissionFlags` attribute to your BLUETOOTH_SCAN permission declaration, and set this attribute's value to `neverForLocation`.
    /// </param>
    /// <param name="advertise">Needed only if your app makes the device discoverable to Bluetooth devices.</param>
    /// <param name="connect">Needed only if your app communicates with already-paired Bluetooth devices.</param>
    /// <param name="bluetoothLocation">Needed only if your app uses Bluetooth scan results to derive physical location.</param>
    /// <remarks>
    ///  https://developer.android.com/guide/topics/connectivity/bluetooth/permissions
    /// </remarks>
    public BluetoothPermissions(bool scan = true, bool advertise = true, bool connect = true, bool bluetoothLocation = true)
    {
        _scan = scan;
        _advertise = advertise;
        _connect = connect;
        _bluetoothLocation = bluetoothLocation;
    }

    private (string androidPermission, bool isRuntime)[]? _requiredPermissions;

    public override (string androidPermission, bool isRuntime)[] RequiredPermissions
    {
        get
        {
            if (_requiredPermissions != null)
                return _requiredPermissions;

            var result = new List<(string androidPermission, bool isRuntime)>();

            var sdk = (int)Android.OS.Build.VERSION.SdkInt;
            if (sdk >= 31)
            {
                // If your app targets Android 12 (API level 31) or higher, declare the following permissions in your app's manifest file:

                if (_scan)
#pragma warning disable CA1416 // Validate platform compatibility
                    result.Add((global::Android.Manifest.Permission.BluetoothScan, true));
#pragma warning restore CA1416 // Validate platform compatibility

                if (_advertise)
#pragma warning disable CA1416 // Validate platform compatibility
                    result.Add((global::Android.Manifest.Permission.BluetoothAdvertise, true));
#pragma warning restore CA1416 // Validate platform compatibility

                if (_connect)
#pragma warning disable CA1416 // Validate platform compatibility
                    result.Add((global::Android.Manifest.Permission.BluetoothConnect, true));
#pragma warning restore CA1416 // Validate platform compatibility

                if (_bluetoothLocation)
                    result.Add((global::Android.Manifest.Permission.AccessFineLocation, true));
            }
            else
            {
                // If your app targets Android 11 (API level 30) or lower, declare the following permissions in your app's manifest file:

                result.Add((global::Android.Manifest.Permission.Bluetooth, true));

                if (sdk >= 29)
                {
                    result.Add((global::Android.Manifest.Permission.AccessFineLocation, true));
                }
                else
                {
                    // If your app targets Android 9 (API level 28) or lower, you can declare the ACCESS_COARSE_LOCATION permission instead of the ACCESS_FINE_LOCATION permission.

                    result.Add((global::Android.Manifest.Permission.AccessCoarseLocation, true));
                }

                if (_scan || _connect || _advertise)
                {
                    result.Add((global::Android.Manifest.Permission.BluetoothAdmin, true));

                    if (sdk >= 29)
                    {
                        // If your app supports a service and can run on Android 10 (API level 29) or Android 11, you must also declare the ACCESS_BACKGROUND_LOCATION permission to discover Bluetooth devices. 

#pragma warning disable CA1416 // Validate platform compatibility
                        result.Add((global::Android.Manifest.Permission.AccessBackgroundLocation, true));
#pragma warning restore CA1416 // Validate platform compatibility
                    }
                }
            }
            _requiredPermissions = [.. result];

            return _requiredPermissions;
        }
    }
}