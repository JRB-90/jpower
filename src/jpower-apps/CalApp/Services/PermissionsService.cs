namespace CalApp.Services
{
    public class PermissionsService : IPermissionsService
    {
        public async Task<bool> HasBLEPermission()
        {
            if (await Permissions.CheckStatusAsync<BluetoothPermissions>() != PermissionStatus.Granted)
            {
                if (await Permissions.RequestAsync<BluetoothPermissions>() != PermissionStatus.Granted)
                {
                    return false;
                }
            }

#if ANDROID
            if (await Permissions.CheckStatusAsync<Permissions.LocationWhenInUse>() != PermissionStatus.Granted)
            {
                if (await Permissions.RequestAsync<Permissions.LocationWhenInUse>() != PermissionStatus.Granted)
                {
                    return false;
                }
            }
#endif

            return true;
        }
    }
}
