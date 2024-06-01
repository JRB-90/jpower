using CalApp.Shared.Ble;
using CalApp.Shared.Calibration;
using CalApp.Shared.Mvvm;
using CalApp.Shared.Services;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.Reactive.Linq;

namespace CalApp.Shared.UI
{
    public class CalibratePageViewModel : ViewModelBase
    {
        public const int NumSamples             = 10;
        public const int MinMeasurementCount    = 5;

        public CalibratePageViewModel(
            INavigationService navigationService,
            IAlertService alertService,
            IBleService bleService)
        {
            this.navigationService = navigationService;
            this.alertService = alertService;
            this.bleService = bleService;

            Measurements = new ObservableCollection<Measurement>();
            Measurements.CollectionChanged += Measurements_CollectionChanged;

            weightInput = "0.0";

            ZeroCommand =
                new Command(
                    async () => await Zero(),
                    () => JPowerDevice != null &&
                          BleDevice != null &&
                          BleDevice.DeviceState == BleDeviceState.Connected
                );

            PushCalibrationCommand =
                new Command(
                    async () => await Calibrate(),
                    () => JPowerDevice != null &&
                          BleDevice != null &&
                          BleDevice.DeviceState == BleDeviceState.Connected &&
                          Measurements.Count >= MinMeasurementCount
                );

            TakeMeasurementCommand =
                new Command(
                    async () => await TakeMeasurement(),
                    () => JPowerDevice != null &&
                          BleDevice != null &&
                          BleDevice.DeviceState == BleDeviceState.Connected &&
                          WeightInput != null &&
                          double.TryParse(WeightInput, out double result)
                );

            ResetMeasurementsCommand =
                new Command(
                    () => Measurements.Clear(),
                    () => Measurements.Count > 0
                );

            DisconnectCommand =
                new Command(
                    async () => await Disconnect(),
                    () => BleDevice != null &&
                          BleDevice.DeviceState == BleDeviceState.Connected
                );
        }

        public bool IsBusy
        {
            get => isBusy;
            set => SetProperty(ref isBusy, value);
        }

        public IBleDevice? BleDevice
        {
            get => bleDevice;
            set
            {
                SetProperty(ref bleDevice, value);
                if (bleDevice != null )
                {
                    bleDevice.DeviceStateChanged += DeviceManager_DeviceStateChanged;
                }
                RefreshCommandEnables();
            }
        }

        public IJPowerDevice? JPowerDevice
        {
            get => jpower;
            set
            {
                SetProperty(ref jpower, value);
                RefreshCommandEnables();
            }
        }

        public string WeightInput
        {
            get => weightInput;
            set
            {
                SetProperty(ref weightInput, value);
                RefreshCommandEnables();
            }
        }

        public ObservableCollection<Measurement> Measurements { get; }

        public Command ZeroCommand { get; }

        public Command PushCalibrationCommand { get; }

        public Command TakeMeasurementCommand { get; }

        public Command ResetMeasurementsCommand { get; }

        public Command DisconnectCommand { get; }

        public override async Task OnNavigatingTo(object? parameter)
        {
            BleDevice = parameter as IBleDevice;

            if (BleDevice == null)
            {
                await alertService.DisplayAlert(
                    "Error",
                    "No device selected, returning to connect page",
                    "OK"
                );

                await navigationService.NavigateToConnectPage();

                return;
            }

            if (BleDevice.DeviceState != BleDeviceState.Connected)
            {
                await alertService.DisplayAlert(
                    "Error",
                    "Device disconnected, returning to connect page",
                    "OK"
                );

                await navigationService.NavigateToConnectPage();
            }

            Measurements.Clear();
            JPowerDevice = await bleService.CreateJPowerDevice(BleDevice);
            await JPowerDevice.StartStreaming();
        }

        public override async Task OnNavigatedFrom(bool isForwardNavigation)
        {
            if (!isForwardNavigation)
            {
                await Disconnect();
                Measurements.Clear();
            }
        }

        private async Task Zero()
        {
            IsBusy = true;

            try
            {
                if (JPowerDevice == null)
                {
                    throw new InvalidOperationException("JPower device not connected");
                }

                var wasSuccessful = await JPowerDevice.ZeroOffset();

                if (!wasSuccessful)
                {
                    throw new InvalidOperationException("Failed to zero device");
                }

                await alertService.DisplayAlert(
                    "JPower",
                    "Deviced zeroed",
                    "OK"
                );
            }
            catch (Exception ex)
            {
                await alertService.DisplayAlert(
                    "Error",
                    ex.Message,
                    "OK"
                );
            }
            finally
            {
                IsBusy = false;
            }
        }

        private async Task Calibrate()
        {
            IsBusy = true;

            try
            {
                if (JPowerDevice == null)
                {
                    throw new InvalidOperationException("JPower device not connected");
                }

                var slope = await bleService.CalculateSlope(Measurements);
                var wasSuccessful = await JPowerDevice.PushSlope(slope);

                if (!wasSuccessful)
                {
                    throw new InvalidOperationException("Failed to push calibration");
                }

                await alertService.DisplayAlert(
                    "JPower",
                    "Pushed calibration successfuly",
                    "OK"
                );
            }
            catch (Exception ex)
            {
                await alertService.DisplayAlert(
                    "Error",
                    ex.Message,
                    "OK"
                );
            }
            finally
            {
                IsBusy = false;
            }
        }

        private async Task TakeMeasurement()
        {
            IsBusy = true;

            try
            {
                if (JPowerDevice == null)
                {
                    throw new InvalidOperationException("JPower device not connected");
                }

                double weight = Convert.ToDouble(WeightInput);

                var measurements =
                    await
                    JPowerDevice
                    .RawAdcValues
                    .Take(NumSamples)
                    .Timeout(DateTime.Now.AddSeconds(10))
                    .Select(x => (double)x)
                    .ToList();

                var averagedValue = measurements.Average();

                var measurement = 
                    new Measurement(
                        weight,
                        (uint)averagedValue, 
                        NumSamples
                    );

                Measurements.Add(measurement);
            }
            catch (Exception ex)
            {
                await alertService.DisplayAlert(
                    "Error",
                    ex.Message,
                    "OK"
                );
            }
            finally
            {
                IsBusy = false;
            }
        }

        private async Task Disconnect()
        {
            if (BleDevice != null)
            {
                try
                {
                    IsBusy = true;

                    if (bleDevice != null)
                    {
                        bleDevice.DeviceStateChanged -= DeviceManager_DeviceStateChanged;
                    }
                    
                    await BleDevice.Disconnect();
                }
                catch (Exception ex)
                {
                    await alertService.DisplayAlert(
                        "Error",
                        ex.Message,
                        "OK"
                    );
                }
                finally
                {
                    IsBusy = false;
                }
            }

            await navigationService.NavigateToConnectPage();
        }

        private void Measurements_CollectionChanged(object? sender, NotifyCollectionChangedEventArgs e)
        {
            RefreshCommandEnables();
        }

        private async void DeviceManager_DeviceStateChanged(object? sender, BleDeviceState newState)
        {
            OnPropertyChanged(nameof(BleDevice));
            RefreshCommandEnables();

            if (newState == BleDeviceState.Disconnected)
            {
                await alertService.DisplayAlert(
                    "Error",
                    "Device disconnected",
                    "OK"
                );

                await navigationService.NavigateToConnectPage();
            }
        }

        private void RefreshCommandEnables()
        {
            ZeroCommand.ChangeCanExecute();
            PushCalibrationCommand.ChangeCanExecute();
            TakeMeasurementCommand.ChangeCanExecute();
            ResetMeasurementsCommand.ChangeCanExecute();
            DisconnectCommand.ChangeCanExecute();
        }

        private readonly INavigationService navigationService;
        private readonly IAlertService alertService;
        private readonly IBleService bleService;
        private IBleDevice? bleDevice;
        private IJPowerDevice? jpower;
        private bool isBusy;
        private string weightInput;
    }
}
