using MauiIcons.Core;
using UraniumUI.Pages;

namespace JPower.DiagnosticsApp.UI;

public partial class DiagnosticsPageView : UraniumContentPage
{
	public DiagnosticsPageView(DiagnosticsPageViewModel viewModel)
	{
        BindingContext = viewModel;
        InitializeComponent();
        _ = new MauiIcon(); // Workaround from: https://github.com/AathifMahir/MauiIcons
    }
}