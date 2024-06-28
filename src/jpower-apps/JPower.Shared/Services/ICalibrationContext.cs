using JPower.Shared.Calibration;
using System.Collections.ObjectModel;

namespace JPower.Shared.Services
{
    public interface ICalibrationContext
    {
        ObservableCollection<Measurement> Measurements { get; }

        Slope CalculateSlope();
    }
}
