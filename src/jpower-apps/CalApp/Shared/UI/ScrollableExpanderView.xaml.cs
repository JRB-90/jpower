namespace CalApp.Shared.UI;

public partial class ScrollableExpanderView : ContentView
{
	public ScrollableExpanderView()
	{
		InitializeComponent();
    }

    public static readonly BindableProperty IsExpandedProperty =
        BindableProperty.Create(
            "IsExpanded",
            typeof(bool),
            typeof(ScrollableExpanderView),
            default(bool)
        );

    public bool IsExpanded
    {
        get => (bool)GetValue(IsExpandedProperty);
        set => SetValue(IsExpandedProperty, value);
    }

    public static readonly BindableProperty HeaderLabelProperty =
        BindableProperty.Create(
            "HeaderLabel",
            typeof(string),
            typeof(ScrollableExpanderView),
            default(string)
        );

    public string HeaderLabel
    {
        get => (string)GetValue(HeaderLabelProperty);
        set => SetValue(HeaderLabelProperty, value);
    }

    public static readonly BindableProperty BodyContentProperty =
        BindableProperty.Create(
            "BodyContent",
            typeof(View),
            typeof(ScrollableExpanderView),
            default(View)
        );

    public View BodyContent
    {
        get => (View)GetValue(BodyContentProperty);
        set => SetValue(BodyContentProperty, value);
    }
}