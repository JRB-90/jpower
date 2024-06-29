using JPower.Shared.JPower;

namespace JPower.Shared.UI;

public partial class JPowerDiagnosticsView : ContentView
{
	public JPowerDiagnosticsView()
	{
		InitializeComponent();
	}

    public static readonly BindableProperty JPowerDeviceProperty =
        BindableProperty.Create(
            "JPowerDevice",
            typeof(IJPowerDevice),
            typeof(JPowerDiagnosticsView),
            default(IJPowerDevice)
        );

    public IJPowerDevice JPowerDevice
    {
        get => (IJPowerDevice)GetValue(JPowerDeviceProperty);
        set => SetValue(JPowerDeviceProperty, value);
    }
}