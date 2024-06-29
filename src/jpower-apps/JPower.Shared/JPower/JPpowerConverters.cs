namespace JPower.Shared.JPower
{
    public static class JPpowerConverters
    {
        public static Vector3D ToVector3D(this JPowerAccelData accelData)
        {
            return
                new Vector3D(
                    accelData.x,
                    accelData.y,
                    accelData.z
                );
        }

        public static Vector3D ToVector3D(this JPowerGyroData gyroData)
        {
            return
                new Vector3D(
                    gyroData.rx,
                    gyroData.ry,
                    gyroData.rz
                );
        }

        public static Vector3D ToVector3D(this JPowerOrientData accelData)
        {
            return 
                QuaternionToEulerXYZ(
                    accelData.w,
                    accelData.x,
                    accelData.y,
                    accelData.z
                );
        }

        public static Vector3D QuaternionToEulerXYZ(
            double w,
            double x,
            double y,
            double z)
        {
            double sinr_cosp = 2 * (w * x + y * z);
            double cosr_cosp = 1 - 2 * (x * x + y * y);
            double roll = Math.Atan2(sinr_cosp, cosr_cosp);

            double sinp = Math.Sqrt(1 + 2 * (w * y - x * z));
            double cosp = Math.Sqrt(1 - 2 * (w * y - x * z));
            double pitch = 2 * Math.Atan2(sinp, cosp) - Math.PI / 2.0;

            double siny_cosp = 2 * (w * z + x * y);
            double cosy_cosp = 1 - 2 * (y * y + z * z);
            double yaw =  Math.Atan2(siny_cosp, cosy_cosp);

            return
                new Vector3D(
                    roll,
                    pitch,
                    yaw
                );
        }
    }
}
