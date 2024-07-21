using JPower.Shared.JPower;
using JPower.Shared.Mvvm;

namespace JPower.Shared.UI
{
    public class DisplayCalibrationViewModel : ViewModelBase
    {
        public DisplayCalibrationViewModel()
        {
            id = "";
            crank = "";
            offset = "";
            slope = "";
            intercept = "";
        }

        public JPowerCalibrationData Calibration
        {
            get => calibration;
            set
            {
                SetProperty(ref calibration, value);
                var guid = new Guid(calibration.guid);
                ID = guid.ToString();
                Crank = Convert.ToString(calibration.crankLength);
                Offset = Convert.ToString(calibration.offset);
                Slope = Convert.ToString(calibration.slope);
                Intercept = Convert.ToString(calibration.intercept);
            }
        }

        public string ID
        {
            get => id;
            set => SetProperty(ref id, value);
        }

        public string Crank
        {
            get => crank;
            set => SetProperty(ref crank, value);
        }

        public string Offset
        {
            get => offset;
            set => SetProperty(ref offset, value);
        }

        public string Slope
        {
            get => slope;
            set => SetProperty(ref slope, value);
        }

        public string Intercept
        {
            get => intercept; 
            set => SetProperty(ref intercept, value);
        }

        private JPowerCalibrationData calibration;
        private string id;
        private string crank;
        private string offset;
        private string slope;
        private string intercept;
    }
}
