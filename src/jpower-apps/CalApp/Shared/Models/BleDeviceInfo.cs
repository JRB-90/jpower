using Plugin.BLE.Abstractions.Contracts;

namespace CalApp.Shared.Models
{
    public class BleDeviceInfo
    {
        public BleDeviceInfo(IDevice device)
        {
            Device = device;
            ID = Device.Id.ToString();
            Name =
                Device.Name != null
                ? Device.Name
                : "Unknown Device";
        }

        public string ID { get; }

        public string Name { get; }

        internal IDevice Device { get; }
    }
}
