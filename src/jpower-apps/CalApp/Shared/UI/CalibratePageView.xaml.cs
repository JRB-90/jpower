using UraniumUI.Pages;

namespace CalApp.Shared.UI;

public partial class CalibratePageView : UraniumContentPage
{
	public CalibratePageView(CalibratePageViewModel viewModel)
	{
        BindingContext = viewModel;
        InitializeComponent();
	}
}