using MauiIcons.Core;

namespace CalApp.Shared.UI;

public partial class DeviceInfoPageView : ContentView
{
	public DeviceInfoPageView()
	{
		BindingContext = JPower.Shared.Mvvm.ServiceProvider.GetService<DeviceInfoPageViewModel>(); ;
		InitializeComponent();
        _ = new MauiIcon(); // Workaround from: https://github.com/AathifMahir/MauiIcons
    }
}