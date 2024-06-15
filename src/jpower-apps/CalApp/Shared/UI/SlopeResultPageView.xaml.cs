namespace CalApp.Shared.UI;

public partial class SlopeResultPageView : ContentView
{
	public SlopeResultPageView()
	{
		BindingContext = Mvvm.ServiceProvider.GetService<SlopeResultPageViewModel>();
		InitializeComponent();
	}
}