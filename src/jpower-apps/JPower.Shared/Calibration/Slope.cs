namespace JPower.Shared.Calibration
{
    public class Slope
    {
        public Slope(float slopeValue, float intercept)
        {
            SlopeValue = slopeValue;
            Intercept = intercept;
        }

        public float SlopeValue { get; }

        public float Intercept { get; }
    }
}
