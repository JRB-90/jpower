using CalApp.Shared.JPower;
using System.Globalization;

namespace CalApp.Shared.Converters
{
    internal class JPStateToStringConverter : IValueConverter
    {
        public object? Convert(object? value, Type targetType, object? parameter, CultureInfo culture)
        {
            if (value is JPowerState state)
            {
                switch (state)
                {
                    case JPowerState.JP_STATE_STARTUP:
                        return "Startup";

                    case JPowerState.JP_STATE_INITIALISED:
                        return "Initialised";

                    case JPowerState.JP_STATE_RUNNING:
                        return "Running";

                    case JPowerState.JP_STATE_CALIBRATING:
                        return "Calibrating";
                }
            }

            return "";
        }

        public object? ConvertBack(object? value, Type targetType, object? parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}
