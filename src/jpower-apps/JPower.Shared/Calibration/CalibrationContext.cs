using JPower.Shared.Services;
using MathNet.Numerics;
using System.Collections.ObjectModel;

namespace JPower.Shared.Calibration
{
    public class CalibrationContext : ICalibrationContext
    {
        public CalibrationContext()
        {
            Measurements = new ObservableCollection<Measurement>();
        }

        public ObservableCollection<Measurement> Measurements { get; }

        public Slope CalculateSlope()
        {
            if (Measurements.Count < 2)
            {
                throw new InvalidOperationException(
                    "Need at least two measurements to calculate calibration"
                );
            }

            (double intercept, double slope) result = 
                Fit.Line(
                    Measurements.Select(x => x.Weight).ToArray(),
                    Measurements.Select(x => (double)x.AdcValue).ToArray()
                );

            return new Slope(
                (float)result.slope,
                (float)result.intercept
            );
        }
    }
}
