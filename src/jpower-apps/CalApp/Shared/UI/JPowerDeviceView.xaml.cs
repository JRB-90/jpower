using CalApp.Shared.JPower;

namespace CalApp.Shared.UI;

public partial class JPowerDeviceView : ContentView
{
    public JPowerDeviceView()
	{
		InitializeComponent();
    }

    public static readonly BindableProperty JPowerDeviceProperty =
        BindableProperty.Create(
            "JPowerDevice",
            typeof(IJPowerDevice),
            typeof(JPowerDeviceView),
            default(IJPowerDevice)
        );

    public IJPowerDevice JPowerDevice
    {
        get => (IJPowerDevice)GetValue(JPowerDeviceProperty);
        set => SetValue(JPowerDeviceProperty, value);
    }
}