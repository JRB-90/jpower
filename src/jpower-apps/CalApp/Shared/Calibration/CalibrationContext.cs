using CalApp.Shared.Services;
using MathNet.Numerics;
using System.Collections.ObjectModel;

namespace CalApp.Shared.Calibration
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
            if (Measurements.Count == 0)
            {
                return new Slope(0.0, 0.0);
            }

            (double intercept, double slope) result = 
                Fit.Line(
                    Measurements.Select(x => x.Weight).ToArray(),
                    Measurements.Select(x => (double)x.AdcValue).ToArray()
                );

            return new Slope(result.slope, result.intercept);
        }
    }
}
