using CalApp.Shared.Ble;
using CalApp.Shared.Calibration;
using CalApp.Shared.JPower;
using CalApp.Shared.Mvvm;
using CalApp.Shared.Services;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.Reactive.Linq;

namespace CalApp.Shared.UI
{
    public class CalibrationPageViewModel : ViewModelBase
    {
        public const int NumSamples             = 10;
        public const int MinMeasurementCount    = 5;

        public CalibrationPageViewModel(
            IAppContext appContext,
            INavigationService navigationService,
            IAlertService alertService,
            IBleService bleService)
        {
            this.appContext = appContext;
            this.navigationService = navigationService;
            this.alertService = alertService;
            this.bleService = bleService;

            weightInput = "0.0";
            calibrationContext = appContext.CalibrationContext;
            calibrationContext.Measurements.CollectionChanged += Measurements_CollectionChanged;

            ZeroCommand =
                new Command(
                    async () => await Zero(),
                    () => appContext.JPowerDevice != null &&
                          appContext.BleDevice != null &&
                          appContext.BleDevice.DeviceState == BleDeviceState.Connected
                );

            PushCalibrationCommand =
                new Command(
                    async () => await Calibrate(),
                    () => appContext.JPowerDevice != null &&
                          appContext.BleDevice != null &&
                          appContext.BleDevice.DeviceState == BleDeviceState.Connected &&
                          calibrationContext.Measurements.Count >= MinMeasurementCount
                );

            TakeMeasurementCommand =
                new Command(
                    async () => await TakeMeasurement(),
                    () => appContext.JPowerDevice != null &&
                          appContext.BleDevice != null &&
                          appContext.BleDevice.DeviceState == BleDeviceState.Connected &&
                          WeightInput != null &&
                          double.TryParse(WeightInput, out double result)
                );

            ResetMeasurementsCommand =
                new Command(
                    () => calibrationContext.Measurements.Clear(),
                    () => calibrationContext.Measurements.Count > 0
                );

            DisconnectCommand =
                new Command(
                    async () => await Disconnect(),
                    () => appContext.BleDevice != null &&
                          appContext.BleDevice.DeviceState == BleDeviceState.Connected
                );

            appContext.BusyStateChanged += AppContext_BusyStateChanged;
            appContext.BleDeviceChanged += AppContext_BleDeviceChanged;
            appContext.JPowerDeviceChanged += AppContext_JPowerDeviceChanged;

            if (appContext.BleDevice != null )
            {
                appContext.BleDevice.DeviceStateChanged += BleDevice_DeviceStateChanged;
            }

            calibrationContext.Measurements.Add(new Measurement(1.0,   8000000, 10));
            calibrationContext.Measurements.Add(new Measurement(10.0,  8800000, 10));
            calibrationContext.Measurements.Add(new Measurement(25.0,  9500000, 10));
            calibrationContext.Measurements.Add(new Measurement(50.0, 12400000, 10));
            calibrationContext.Measurements.Add(new Measurement(75.0, 15100000, 10));
        }

        public bool IsBusy => appContext.IsBusy;

        public string WeightInput
        {
            get => weightInput;
            set
            {
                SetProperty(ref weightInput, value);
                RefreshCommandEnables();
            }
        }

        public IBleDevice? CurrentBleDevice => appContext.BleDevice;

        public IJPowerDevice? CurrentJPowerDevice => appContext.JPowerDevice;

        public ObservableCollection<Measurement> Measurements => calibrationContext.Measurements;

        public Command ZeroCommand { get; }

        public Command PushCalibrationCommand { get; }

        public Command TakeMeasurementCommand { get; }

        public Command ResetMeasurementsCommand { get; }

        public Command DisconnectCommand { get; }

        private async Task Zero()
        {
            appContext.IsBusy = true;

            try
            {
                if (appContext.JPowerDevice == null)
                {
                    throw new InvalidOperationException("JPower device not connected");
                }

                var wasSuccessful = await appContext.JPowerDevice.ZeroOffset();

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
                appContext.IsBusy = false;
            }
        }

        private async Task Calibrate()
        {
            appContext.IsBusy = true;

            try
            {
                if (appContext.JPowerDevice == null)
                {
                    throw new InvalidOperationException("JPower device not connected");
                }

                var slope = appContext.CalibrationContext.CalculateSlope();
                var wasSuccessful = await appContext.JPowerDevice.PushSlope(slope);

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
                appContext.IsBusy = false;
            }
        }

        private async Task TakeMeasurement()
        {
            appContext.IsBusy = true;

            try
            {
                if (appContext.JPowerDevice == null)
                {
                    throw new InvalidOperationException("JPower device not connected");
                }

                double weight = Convert.ToDouble(WeightInput);

                var measurements =
                    await
                    appContext
                    .JPowerDevice
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
                appContext.IsBusy = false;
            }
        }

        private async Task Disconnect()
        {
            if (appContext.BleDevice != null)
            {
                try
                {
                    appContext.IsBusy = true;

                    if (appContext.BleDevice != null)
                    {
                        appContext.BleDevice.DeviceStateChanged -= BleDevice_DeviceStateChanged;
                        await appContext.BleDevice.Disconnect();
                    }
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
                    appContext.IsBusy = false;
                }
            }

            await navigationService.NavigateToConnectPage();
        }

        private void RefreshCommandEnables()
        {
            ZeroCommand.ChangeCanExecute();
            PushCalibrationCommand.ChangeCanExecute();
            TakeMeasurementCommand.ChangeCanExecute();
            ResetMeasurementsCommand.ChangeCanExecute();
            DisconnectCommand.ChangeCanExecute();
        }

        private void AppContext_BusyStateChanged(object? sender, bool e)
        {
            OnPropertyChanged(nameof(IsBusy));
        }

        private void AppContext_BleDeviceChanged(object? sender, IBleDevice? e)
        {
            if (appContext.BleDevice != null)
            {
                appContext.BleDevice.DeviceStateChanged += BleDevice_DeviceStateChanged;
            }

            OnPropertyChanged(nameof(CurrentBleDevice));
        }

        private void AppContext_JPowerDeviceChanged(object? sender, IJPowerDevice? e)
        {
            OnPropertyChanged(nameof(CurrentJPowerDevice));
        }

        private void Measurements_CollectionChanged(object? sender, NotifyCollectionChangedEventArgs e)
        {
            RefreshCommandEnables();
        }

        private async void BleDevice_DeviceStateChanged(object? sender, BleDeviceState newState)
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

        private readonly IAppContext appContext;
        private readonly INavigationService navigationService;
        private readonly IAlertService alertService;
        private readonly IBleService bleService;
        private readonly ICalibrationContext calibrationContext;
        private string weightInput;
    }
}
