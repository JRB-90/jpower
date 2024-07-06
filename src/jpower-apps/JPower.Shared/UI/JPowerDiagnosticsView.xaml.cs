using JPower.Shared.JPower;

namespace JPower.Shared.UI;

public partial class JPowerDiagnosticsView : ContentView
{
	public JPowerDiagnosticsView()
	{
		InitializeComponent();

        PullCalCommand =
            new Command(
                () => { },
                () => false
            );

        PushCalCommand =
            new Command(
                () => { },
                () => false
            );

        ZeroOffsetCommand =
            new Command(
                () => { },
                () => false
            );

        MeasureCommand =
            new Command(
                () => { },
                () => false
            );
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

    public static readonly BindableProperty PullCalCommandProperty =
        BindableProperty.Create(
            "PullCalCommand",
            typeof(Command),
            typeof(JPowerDiagnosticsView),
            default(Command)
        );

    public Command PullCalCommand
    {
        get => (Command)GetValue(PullCalCommandProperty);
        set => SetValue(PullCalCommandProperty, value);
    }

    public static readonly BindableProperty PushCalCommandProperty =
        BindableProperty.Create(
            "PushCalCommand",
            typeof(Command),
            typeof(JPowerDiagnosticsView),
            default(Command)
        );

    public Command PushCalCommand
    {
        get => (Command)GetValue(PushCalCommandProperty);
        set => SetValue(PushCalCommandProperty, value);
    }

    public static readonly BindableProperty ZeroOffsetCommandProperty =
        BindableProperty.Create(
            "ZeroOffsetCommand",
            typeof(Command),
            typeof(JPowerDiagnosticsView),
            default(Command)
        );

    public Command ZeroOffsetCommand
    {
        get => (Command)GetValue(ZeroOffsetCommandProperty);
        set => SetValue(ZeroOffsetCommandProperty, value);
    }

    public static readonly BindableProperty MeasureCommandProperty =
        BindableProperty.Create(
            "MeasureCommand",
            typeof(Command),
            typeof(JPowerDiagnosticsView),
            default(Command)
        );

    public Command MeasureCommand
    {
        get => (Command)GetValue(MeasureCommandProperty);
        set => SetValue(MeasureCommandProperty, value);
    }
}