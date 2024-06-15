namespace CalApp.Shared.Ble
{
    public class BleDeviceInfo
    {
        public BleDeviceInfo(
            Guid uuid, 
            string name)
        {
            UUID = uuid;
            ID = uuid.ToString();
            Name = name;
        }

        public Guid UUID { get; }

        public string ID { get; }

        public string Name { get; }
    }
}
