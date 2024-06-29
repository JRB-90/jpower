using JPower.Shared.JPower;

namespace JPower.Shared.UI;

public partial class Vector3DView : ContentView
{
	public Vector3DView()
	{
		InitializeComponent();
	}

    public static readonly BindableProperty Vector3DProperty =
        BindableProperty.Create(
            "Vector3D",
            typeof(Vector3D),
            typeof(Vector3DView),
            default(Vector3D)
        );

    public Vector3D Vector3D
    {
        get => (Vector3D)GetValue(Vector3DProperty);
        set => SetValue(Vector3DProperty, value);
    }
}