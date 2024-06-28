using JPower.Shared.Calibration;
using MauiIcons.Core;

namespace JPower.Shared.UI;

public partial class MeasurementsView : ContentView
{
	public MeasurementsView()
	{
		InitializeComponent();
        _ = new MauiIcon(); // Workaround from: https://github.com/AathifMahir/MauiIcons
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