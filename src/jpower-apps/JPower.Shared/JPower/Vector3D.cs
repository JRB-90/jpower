namespace JPower.Shared.JPower
{
    public class Vector3D
    {
        public Vector3D(
            double x, 
            double y, 
            double z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        public double X { get; }

        public double Y { get; }

        public double Z { get; }
    }
}
