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