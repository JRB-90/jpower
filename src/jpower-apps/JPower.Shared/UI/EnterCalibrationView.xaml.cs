using CommunityToolkit.Maui.Views;

namespace JPower.Shared.UI;

public partial class EnterCalibrationView : Popup
{
	public EnterCalibrationView(EnterCalibrationViewModel viewModel)
	{
        BindingContext = viewModel;
		InitializeComponent();
	}

    private async void OkButton_Clicked(object sender, EventArgs e)
    {
        var cts = new CancellationTokenSource(TimeSpan.FromSeconds(5));
        await CloseAsync(true, cts.Token);
    }

    private async void CancelButton_Clicked(object sender, EventArgs e)
    {
        var cts = new CancellationTokenSource(TimeSpan.FromSeconds(5));
        await CloseAsync(false, cts.Token);
    }
}