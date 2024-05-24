using MauiIcons.Core;

namespace JPowerApp.Shared.UI;

public partial class ConnectPageView : ContentPage
{
	public ConnectPageView(ConnectPageViewModel viewModel)
	{
		BindingContext = viewModel;
		InitializeComponent();
        _ = new MauiIcon(); // Workaround from: https://github.com/AathifMahir/MauiIcons
    }
}