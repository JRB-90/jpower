using JPowerApp.Shared.Models;
using Plugin.BLE;
using Plugin.BLE.Abstractions.Contracts;
using Plugin.BLE.Abstractions.Exceptions;
using System.Collections.ObjectModel;
using System.ComponentModel;

namespace JPowerApp;

public partial class MainPage : ContentPage, INotifyPropertyChanged
{
	public MainPage()
	{
		InitializeComponent();

        ble = CrossBluetoothLE.Current;
        adapter = CrossBluetoothLE.Current.Adapter;
        DiscoveredDevices = new ObservableCollection<IDevice>();

        BindingContext = this;
    }

    public ObservableCollection<IDevice> DiscoveredDevices { get; }

    public IDevice SelectedDevice
    {
        get => selectedDevice;
        set
        {
            selectedDevice = value;
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(CanConnect)));
        }
    }

    public bool CanConnect => SelectedDevice != null;

    public event PropertyChangedEventHandler PropertyChanged;

    private async void ScanButton_Clicked(object sender, EventArgs e)
    {
        ScanButton.IsEnabled = false;
        adapter.DeviceDiscovered += Adapter_DeviceDiscovered;
        adapter.ScanTimeoutElapsed += Adapter_ScanTimeoutElapsed;
        adapter.ScanTimeout = 10000;
        adapter.ScanMode = ScanMode.Balanced;

        adapter.DeviceConnected += Adapter_DeviceConnected;

        DiscoveredDevices.Clear();
        await adapter.StartScanningForDevicesAsync();
    }

    private async void ConnectButton_Clicked(object sender, EventArgs e)
    {
        try
        {
            await adapter.ConnectToDeviceAsync(SelectedDevice);
        }
        catch (DeviceConnectionException ex)
        {
            // TODO - Display popup to user?
        }
    }

    private void Adapter_ScanTimeoutElapsed(object sender, EventArgs e)
    {
        ScanButton.IsEnabled = true;
    }

    private void Adapter_DeviceDiscovered(object sender, Plugin.BLE.Abstractions.EventArgs.DeviceEventArgs e)
    {
        MainThread.BeginInvokeOnMainThread(() =>
        {
            DiscoveredDevices.Add(e.Device);
        });
    }

    private async void Adapter_DeviceConnected(object sender, Plugin.BLE.Abstractions.EventArgs.DeviceEventArgs e)
    {
        connectedDevice = e.Device;

        var services = await connectedDevice.GetServicesAsync();

        foreach (var service in services)
        {
            if (service.Id == BleServiceUUIDs.JPOWER_CAL_SRV_UUID)
            {
                var characteristics = await service.GetCharacteristicsAsync();

                foreach (var characteristic in characteristics)
                {
                    if (characteristic.Id == BleServiceUUIDs.JPOWER_CAL_SRV_ADC_RAW_UUID)
                    {
                        characteristic.ValueUpdated += Characteristic_ValueUpdated;
                        await characteristic.StartUpdatesAsync();
                    }
                }
            }
        }
    }

    private void Characteristic_ValueUpdated(object sender, Plugin.BLE.Abstractions.EventArgs.CharacteristicUpdatedEventArgs e)
    {
        var bytes = e.Characteristic.Value;
    }

    private IBluetoothLE ble;
    private IAdapter adapter;
    private IDevice selectedDevice;
    private IDevice connectedDevice;
}

