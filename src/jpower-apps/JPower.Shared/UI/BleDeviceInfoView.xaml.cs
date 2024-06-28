using JPower.Shared.Ble;
using MauiIcons.Core;
using System.Collections.ObjectModel;

namespace JPower.Shared.UI;

public partial class BleDeviceInfoView : ContentView
{
	public BleDeviceInfoView()
	{
		InitializeComponent();
        _ = new MauiIcon(); // Workaround from: https://github.com/AathifMahir/MauiIcons
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