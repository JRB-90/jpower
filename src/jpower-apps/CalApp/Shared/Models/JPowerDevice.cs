using CommunityToolkit.Mvvm.ComponentModel;

namespace CalApp.Shared.Models
{
    public class JPowerDevice : ObservableObject
    {
        private JPowerDevice(
            BleDevice bleDevice,
            BleCharacteristic rawAdcChar)
        {
            this.bleDevice = bleDevice;
            this.rawAdcChar = rawAdcChar;

            rawAdcChar.ValueUpdated += RawAdcChar_ValueUpdated;
        }

        public static async Task<JPowerDevice> CreateAsync(BleDevice bleDevice)
        {
            if (bleDevice.DeviceState != BleDeviceState.Connected)
            {
                throw new ArgumentException("Device is not connected");
            }

            if (!bleDevice.IsJPowerDevice())
            {
                throw new ArgumentException("Device is not a JPower device");
            }

            var rawAdcChar =
                await bleDevice.GetCharacteristic(
                    BleServiceUUIDs.JPOWER_CAL_SRV_UUID,
                    BleServiceUUIDs.JPOWER_CAL_SRV_ADC_RAW_UUID
                );

            return
                new JPowerDevice(
                    bleDevice,
                    rawAdcChar
                );
        }

        public uint RawAdcValue
        {
            get => rawAdcValue;
            set => SetProperty(ref rawAdcValue, value );
        }

        public async Task StartStreaming()
        {
            await rawAdcChar.StartListing();
        }

        public async Task StopStreaming()
        {
            await rawAdcChar.StopListening();
        }

        private void RawAdcChar_ValueUpdated(object? sender, byte[] data)
        {
            RawAdcValue = BleValueConverters.ToUint32(data);
        }

        private readonly BleDevice bleDevice;
        private BleCharacteristic rawAdcChar;
        private uint rawAdcValue;
    }
}
