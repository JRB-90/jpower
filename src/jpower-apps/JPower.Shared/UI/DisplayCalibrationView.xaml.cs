using CommunityToolkit.Maui.Views;

namespace JPower.Shared.UI;

public partial class DisplayCalibrationView : Popup
{
	public DisplayCalibrationView(DisplayCalibrationViewModel viewModel)
	{
		BindingContext = viewModel;
		InitializeComponent();
	}
}