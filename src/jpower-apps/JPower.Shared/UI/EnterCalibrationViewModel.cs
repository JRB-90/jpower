using JPower.Shared.JPower;
using JPower.Shared.Mvvm;

namespace JPower.Shared.UI
{
    public class EnterCalibrationViewModel : ViewModelBase
    {
        public EnterCalibrationViewModel()
        {
            var id = Guid.NewGuid();
            ID = id.ToString();
            calibration = new JPowerCalibrationData();
        }

        public JPowerCalibrationData Calibration
        {
            get => calibration;
            set => SetProperty(ref calibration, value);
        }

        public string ID { get; }

        public float Slope
        {
            get => slope;
            set
            {
                SetProperty(ref slope, value);
                calibration.slope = value;
            }
        }

        public float Intercept
        {
            get => intercept;
            set
            {
                SetProperty(ref intercept, value);
                calibration.intercept = value;
            }
        }

        public float Crank
        {
            get => crank;
            set
            {
                SetProperty(ref crank, value);
                calibration.crankLength = value;
            }
        }

        private JPowerCalibrationData calibration;
        private float slope;
        private float intercept;
        private float crank;
    }
}
