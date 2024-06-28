using MauiIcons.Core;

namespace CalApp.Shared.UI;

public partial class CalibrationPageView : ContentView
{
	public CalibrationPageView()
	{
		BindingContext = JPower.Shared.Mvvm.ServiceProvider.GetService<CalibrationPageViewModel>(); ;
		InitializeComponent();
        _ = new MauiIcon(); // Workaround from: https://github.com/AathifMahir/MauiIcons
    }
}