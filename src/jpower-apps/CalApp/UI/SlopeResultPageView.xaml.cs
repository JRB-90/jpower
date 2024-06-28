namespace CalApp.Shared.UI;

public partial class SlopeResultPageView : ContentView
{
	public SlopeResultPageView()
	{
		BindingContext = JPower.Shared.Mvvm.ServiceProvider.GetService<SlopeResultPageViewModel>();
		InitializeComponent();
	}
}