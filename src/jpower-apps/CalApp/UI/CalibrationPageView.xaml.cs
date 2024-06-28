namespace CalApp.Shared.UI;

public partial class CalibrationPageView : ContentView
{
	public CalibrationPageView()
	{
		BindingContext = JPower.Shared.Mvvm.ServiceProvider.GetService<CalibrationPageViewModel>(); ;
		InitializeComponent();
	}
}