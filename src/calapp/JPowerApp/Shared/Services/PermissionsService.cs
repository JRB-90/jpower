namespace JPowerApp.Shared.Services
{
    public class PermissionsService : IPermissionsService
    {
        public async Task<bool> HasBLEPermission()
        {
            if (await Permissions.CheckStatusAsync<BluetoothPermissions>() == PermissionStatus.Granted)
            {
                return true;
            }

            if (await Permissions.RequestAsync<BluetoothPermissions>() == PermissionStatus.Granted)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}
