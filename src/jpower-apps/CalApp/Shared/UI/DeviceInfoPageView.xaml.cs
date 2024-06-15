namespace CalApp.Shared.UI;

public partial class DeviceInfoPageView : ContentView
{
	public DeviceInfoPageView()
	{
		BindingContext = Mvvm.ServiceProvider.GetService<DeviceInfoPageViewModel>(); ;
		InitializeComponent();
	}
}