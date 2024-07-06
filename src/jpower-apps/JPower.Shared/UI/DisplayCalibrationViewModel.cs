using JPower.Shared.JPower;
using JPower.Shared.Mvvm;

namespace JPower.Shared.UI
{
    public class DisplayCalibrationViewModel : ViewModelBase
    {
        public DisplayCalibrationViewModel()
        {
            id = "";
            slope = "";
            intercept = "";
            crank = "";
        }

        public JPowerCalibrationData Calibration
        {
            get => calibration;
            set
            {
                SetProperty(ref calibration, value);
                ID = calibration.guid.ToString();
                Slope = Convert.ToString(calibration.slope);
                Intercept = Convert.ToString(calibration.intercept);
                Crank = Convert.ToString(calibration.crankLength);
            }
        }

        public string ID
        {
            get => id;
            set => SetProperty(ref id, value);
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

        public string Crank
        {
            get => crank; 
            set => SetProperty(ref crank, value);
        }

        private JPowerCalibrationData calibration;
        private string id;
        private string slope;
        private string intercept;
        private string crank;
    }
}
