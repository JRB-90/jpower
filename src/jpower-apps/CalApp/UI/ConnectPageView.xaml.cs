using MauiIcons.Core;
using UraniumUI.Pages;

namespace CalApp.Shared.UI;

public partial class ConnectPageView : UraniumContentPage
{
	public ConnectPageView(ConnectPageViewModel viewModel)
	{
        BindingContext = viewModel;
        InitializeComponent();
        _ = new MauiIcon(); // Workaround from: https://github.com/AathifMahir/MauiIcons
    }
}