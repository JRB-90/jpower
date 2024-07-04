namespace JPower.Shared
{
    public static class Extensions
    {
        public static byte[] InitTo(this byte[] buffer, byte value)
        {
            for (var i = 0; i < buffer.Length; i++)
            {
                buffer[i] = value;
            }

            return buffer;
        }
    }
}
