using System.Numerics;
using System.Runtime.InteropServices;

namespace CalApp.Shared.Ble
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

        public static T CastToStruct<T>(this byte[] data) where T : struct
        {
            var pData = GCHandle.Alloc(data, GCHandleType.Pinned);
            var result = (T)Marshal.PtrToStructure(pData.AddrOfPinnedObject(), typeof(T));
            pData.Free();

            return result;
        }

        public static byte[] CastToArray<T>(this T data) where T : struct
        {
            var result = new byte[Marshal.SizeOf(typeof(T))];
            var pResult = GCHandle.Alloc(result, GCHandleType.Pinned);
            Marshal.StructureToPtr(data, pResult.AddrOfPinnedObject(), true);
            pResult.Free();

            return result;
        }

        public static UInt128 ToUInt128(this Guid guid)
        {
            var bi = new BigInteger(guid.ToByteArray());

            return (UInt128)bi;
        }

        public static Guid ToGuid(this UInt128 value)
        {
            BigInteger bi = (BigInteger)value;

            return new Guid(bi.ToByteArray());
        }
    }
}
