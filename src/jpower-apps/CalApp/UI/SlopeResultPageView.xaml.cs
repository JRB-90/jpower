using MauiIcons.Core;

namespace CalApp.Shared.UI;

public partial class SlopeResultPageView : ContentView
{
	public SlopeResultPageView()
	{
		BindingContext = JPower.Shared.Mvvm.ServiceProvider.GetService<SlopeResultPageViewModel>();
		InitializeComponent();
        _ = new MauiIcon(); // Workaround from: https://github.com/AathifMahir/MauiIcons
    }
}