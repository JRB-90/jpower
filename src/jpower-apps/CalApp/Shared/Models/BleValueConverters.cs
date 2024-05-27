namespace CalApp.Shared.Models
{
    public static class BleValueConverters
    {
        public static sbyte ToInt8(byte[] data)
        {
            return (sbyte)data[0];
        }

        public static short ToInt16(byte[] data)
        {
            return BitConverter.ToInt16(data, 0);
        }

        public static int ToInt32(byte[] data)
        {
            return BitConverter.ToInt32(data, 0);
        }

        public static long ToUint64(byte[] data)
        {
            return BitConverter.ToInt64(data, 0);
        }

        public static byte ToUint8(byte[] data)
        {
            return data[0];
        }

        public static ushort ToUint16(byte[] data)
        {
            return BitConverter.ToUInt16(data, 0);
        }

        public static uint ToUint32(byte[] data)
        {
            return BitConverter.ToUInt32(data, 0);
        }

        public static ulong ToInt64(byte[] data)
        {
            return BitConverter.ToUInt64(data, 0);
        }
    }
}
