using UraniumUI.Pages;

namespace CalApp.Shared.UI;

public partial class DeviceOverviewView : UraniumContentPage
{
	public DeviceOverviewView(DeviceOverviewViewModel viewModel)
	{
        BindingContext = viewModel;
        InitializeComponent();
	}
}