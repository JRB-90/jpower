using CalApp.Shared.Calibration;

namespace CalApp.Shared.UI;

public partial class MeasurementsView : ContentView
{
	public MeasurementsView()
	{
		InitializeComponent();
	}

    public static readonly BindableProperty MeasurementsProperty =
        BindableProperty.Create(
            "Measurements",
            typeof(IReadOnlyCollection<Measurement>),
            typeof(MeasurementsView),
            new List<Measurement>()
        );

    public IReadOnlyCollection<Measurement> Measurements
    {
        get => (IReadOnlyCollection<Measurement>)GetValue(MeasurementsProperty);
        set => SetValue(MeasurementsProperty, value);
    }
}