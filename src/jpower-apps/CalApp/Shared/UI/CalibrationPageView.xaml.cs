namespace CalApp.Shared.UI;

public partial class CalibrationPageView : ContentView
{
	public CalibrationPageView()
	{
		BindingContext = Mvvm.ServiceProvider.GetService<CalibrationPageViewModel>(); ;
		InitializeComponent();
	}
}