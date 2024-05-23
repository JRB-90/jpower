namespace JPowerApp.Shared.UI;

public partial class ConnectPageView : ContentPage
{
	public ConnectPageView(ConnectPageViewModel viewModel)
	{
		BindingContext = viewModel;
		InitializeComponent();
    }
}