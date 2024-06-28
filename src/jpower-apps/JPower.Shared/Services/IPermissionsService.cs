namespace JPower.Shared.Services
{
    /// <summary>
    /// Used to check permission status and prompt the user to allow
    /// in the case that permission is not found.
    /// </summary>
    public interface IPermissionsService
    {
        Task<bool> HasBLEPermission();
    }
}
