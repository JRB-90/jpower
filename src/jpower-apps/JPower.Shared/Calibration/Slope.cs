namespace JPower.Shared.Calibration
{
    public class Slope
    {
        public Slope(double slopeValue, double intercept)
        {
            SlopeValue = slopeValue;
            Intercept = intercept;
        }

        public double SlopeValue { get; }

        public double Intercept { get; }
    }
}
