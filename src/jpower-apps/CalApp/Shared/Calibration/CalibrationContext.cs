using CalApp.Shared.Services;
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
    }
}
