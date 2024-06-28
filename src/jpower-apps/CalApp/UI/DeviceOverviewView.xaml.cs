using MauiIcons.Core;
using UraniumUI.Pages;

namespace CalApp.Shared.UI;

public partial class DeviceOverviewView : UraniumContentPage
{
	public DeviceOverviewView(DeviceOverviewViewModel viewModel)
	{
        BindingContext = viewModel;
        InitializeComponent();
        _ = new MauiIcon(); // Workaround from: https://github.com/AathifMahir/MauiIcons
    }
}