namespace CalApp.Shared.UI;

public partial class DeviceInfoPageView : ContentView
{
	public DeviceInfoPageView()
	{
		BindingContext = JPower.Shared.Mvvm.ServiceProvider.GetService<DeviceInfoPageViewModel>(); ;
		InitializeComponent();
	}
}