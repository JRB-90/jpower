namespace CalApp.Shared.Calibration
{
    public class Measurement
    {
        public Measurement(
            double weight,
            uint adcValue,
            uint samplesTaken)
        {
            Weight = weight;
            AdcValue = adcValue;
            SamplesTaken = samplesTaken;
        }

        public double Weight { get; }

        public uint AdcValue { get; }

        public uint SamplesTaken { get; }
    }
}
