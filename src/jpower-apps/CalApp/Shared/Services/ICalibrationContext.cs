using CalApp.Shared.Calibration;
using System.Collections.ObjectModel;

namespace CalApp.Shared.Services
{
    public interface ICalibrationContext
    {
        ObservableCollection<Measurement> Measurements { get; }
    }
}
