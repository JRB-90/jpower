using JPower.Shared.JPowDevice;

namespace JPower.Shared.UI;

public partial class JPowerDeviceView : ContentView
{
    public JPowerDeviceView()
	{
		InitializeComponent();
    }

    public static readonly BindableProperty JPowerDeviceProperty =
        BindableProperty.Create(
            "JPowerDevice",
            typeof(ILegacyJPowerDevice),
            typeof(JPowerDeviceView),
            default(ILegacyJPowerDevice)
        );

    public ILegacyJPowerDevice JPowerDevice
    {
        get => (ILegacyJPowerDevice)GetValue(JPowerDeviceProperty);
        set => SetValue(JPowerDeviceProperty, value);
    }

    public static readonly BindableProperty SwitchCalCommandProperty =
        BindableProperty.Create(
            "SwitchCalCommand",
            typeof(Command),
            typeof(JPowerDeviceView),
            default(Command)
        );

    public Command SwitchCalCommand
    {
        get => (Command)GetValue(SwitchCalCommandProperty);
        set => SetValue(SwitchCalCommandProperty, value);
    }

    public static readonly BindableProperty SwitchRunCommandProperty =
        BindableProperty.Create(
            "SwitchRunCommand",
            typeof(Command),
            typeof(JPowerDeviceView),
            default(Command)
        );

    public Command SwitchRunCommand
    {
        get => (Command)GetValue(SwitchRunCommandProperty);
        set => SetValue(SwitchRunCommandProperty, value);
    }
}