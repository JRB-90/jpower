using JPower.Shared.Ble;
using System.Collections.ObjectModel;

namespace CalApp.Shared.UI;

public partial class BleDeviceInfoView : ContentView
{
	public BleDeviceInfoView()
	{
		InitializeComponent();
	}

    public static readonly BindableProperty BleDeviceInfoProperty =
        BindableProperty.Create(
            "BleDeviceInfo",
            typeof(ObservableCollection<BleDeviceInfo>),
            typeof(BleDeviceInfoView),
            new ObservableCollection<BleDeviceInfo>()
        );

    public ObservableCollection<BleDeviceInfo> BleDeviceInfo
    {
        get => (ObservableCollection<BleDeviceInfo>)GetValue(BleDeviceInfoProperty);
        set => SetValue(BleDeviceInfoProperty, value);
    }

    public static readonly BindableProperty SelectedDeviceProperty =
        BindableProperty.Create(
            "SelectedDevice",
            typeof(BleDeviceInfo),
            typeof(BleDeviceInfoView),
            default(BleDeviceInfo)
        );

    public BleDeviceInfo SelectedDevice
    {
        get => (BleDeviceInfo)GetValue(SelectedDeviceProperty);
        set => SetValue(SelectedDeviceProperty, value);
    }
}