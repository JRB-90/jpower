namespace CalApp.Shared.UI;

public partial class CalibratePageView : ContentPage
{
	public CalibratePageView(CalibratePageViewModel viewModel)
	{
        BindingContext = viewModel;
        InitializeComponent();
	}
}