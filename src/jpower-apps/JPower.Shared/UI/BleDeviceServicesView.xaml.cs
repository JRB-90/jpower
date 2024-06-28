using JPower.Shared.Ble;

namespace JPower.Shared.UI;

public partial class BleDeviceServicesView : ContentView
{
	public BleDeviceServicesView()
	{
		InitializeComponent();
	}

    public static readonly BindableProperty BleDeviceServicesProperty =
		BindableProperty.Create(
			"BleDeviceServices", 
			typeof(IReadOnlyCollection<IBleDeviceService>), 
			typeof(BleDeviceServicesView), 
			new List<IBleDeviceService>()
		);
	
	public IReadOnlyCollection<IBleDeviceService> BleDeviceServices
	{
		get => (IReadOnlyCollection<IBleDeviceService>)GetValue(BleDeviceServicesProperty);
		set => SetValue(BleDeviceServicesProperty, value);
	}
}