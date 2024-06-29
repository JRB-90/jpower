using CalApp.Services;
using JPower.Shared.Ble;
using JPower.Shared.Calibration;
using JPower.Shared.JPowDevice;
using JPower.Shared.Mvvm;
using JPower.Shared.Services;
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

            SwitchCalCommand =
                new Command(
                    async () => await SwitchToCalMode(),
                    () => appContext.LegacyJPowerDevice != null &&
                          appContext.LegacyJPowerDevice.State != JPowerState.JP_STATE_CALIBRATING
                );

            SwitchRunCommand =
                new Command(
                    async () => await SwitchToRunMode(),
                    () => appContext.LegacyJPowerDevice != null &&
                          appContext.LegacyJPowerDevice.State != JPowerState.JP_STATE_RUNNING
                );

            ZeroCommand =
                new Command(
                    async () => await Zero(),
                    () => appContext.LegacyJPowerDevice != null &&
                          appContext.BleDevice != null &&
                          appContext.BleDevice.DeviceState == BleDeviceState.Connected
                );

            PushCalibrationCommand =
                new Command(
                    async () => await Calibrate(),
                    () => appContext.LegacyJPowerDevice != null &&
                          appContext.BleDevice != null &&
                          appContext.BleDevice.DeviceState == BleDeviceState.Connected &&
                          calibrationContext.Measurements.Count >= MinMeasurementCount
                );

            TakeMeasurementCommand =
                new Command(
                    async () => await TakeMeasurement(),
                    () => appContext.LegacyJPowerDevice != null &&
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
            appContext.LegacyJPowerDeviceChanged += AppContext_JPowerDeviceChanged;

            if (appContext.BleDevice != null )
            {
                appContext.BleDevice.DeviceStateChanged += BleDevice_DeviceStateChanged;
            }

            if (appContext.LegacyJPowerDevice != null)
            {
                appContext.LegacyJPowerDevice.StateValues.Subscribe(_ => OnJPowerStateChanged());
            }

            calibrationContext.Measurements.Add(new Measurement(1.0,   8000000, 10));
            calibrationContext.Measurements.Add(new Measurement(10.0,  8800000, 10));
            calibrationContext.Measurements.Add(new Measurement(25.0,  9500000, 10));
            calibrationContext.Measurements.Add(new Measurement(50.0, 12400000, 10));
            calibrationContext.Measurements.Add(new Measurement(75.0, 15100000, 10));
        }

        public bool IsBusy => appContext.IsBusy;

        public bool CanCalibrate
        {
            get
            {
                if (appContext.LegacyJPowerDevice != null &&
                    appContext.LegacyJPowerDevice.State == JPowerState.JP_STATE_CALIBRATING)
                {
                    return true;
                }
                else
                {
                    return false;
                }
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

        public IBleDevice? CurrentBleDevice => appContext.BleDevice;

        public ILegacyJPowerDevice? CurrentJPowerDevice => appContext.LegacyJPowerDevice;

        public ObservableCollection<Measurement> Measurements => calibrationContext.Measurements;

        public Command SwitchCalCommand { get; }

        public Command SwitchRunCommand { get; }

        public Command ZeroCommand { get; }

        public Command PushCalibrationCommand { get; }

        public Command TakeMeasurementCommand { get; }

        public Command ResetMeasurementsCommand { get; }

        public Command DisconnectCommand { get; }

        private async Task SwitchToCalMode()
        {
            appContext.IsBusy = true;

            try
            {
                if (appContext.LegacyJPowerDevice == null)
                {
                    throw new InvalidOperationException("JPower device not connected");
                }

                var wasSuccessful = await appContext.LegacyJPowerDevice.SwitchToCalMode();

                if (!wasSuccessful)
                {
                    throw new InvalidOperationException("Failed to switch to calibration mode");
                }

                await alertService.DisplayAlert(
                    "JPower",
                    "Deviced switched to calibration mode",
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

        private async Task SwitchToRunMode()
        {
            appContext.IsBusy = true;

            try
            {
                if (appContext.LegacyJPowerDevice == null)
                {
                    throw new InvalidOperationException("JPower device not connected");
                }

                var wasSuccessful = await appContext.LegacyJPowerDevice.SwitchToRunMode();

                if (!wasSuccessful)
                {
                    throw new InvalidOperationException("Failed to switch to running mode");
                }

                await alertService.DisplayAlert(
                    "JPower",
                    "Deviced switched to running mode",
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

        private async Task Zero()
        {
            appContext.IsBusy = true;

            try
            {
                if (appContext.LegacyJPowerDevice == null)
                {
                    throw new InvalidOperationException("JPower device not connected");
                }

                var wasSuccessful = await appContext.LegacyJPowerDevice.ZeroOffset();

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
                if (appContext.LegacyJPowerDevice == null)
                {
                    throw new InvalidOperationException("JPower device not connected");
                }

                var slope = appContext.CalibrationContext.CalculateSlope();
                var wasSuccessful = await appContext.LegacyJPowerDevice.PushSlope(slope);

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
                if (appContext.LegacyJPowerDevice == null)
                {
                    throw new InvalidOperationException("JPower device not connected");
                }

                double weight = Convert.ToDouble(WeightInput);

                var measurements =
                    await
                    appContext
                    .LegacyJPowerDevice
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
            SwitchCalCommand.ChangeCanExecute();
            SwitchRunCommand.ChangeCanExecute();
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

        private void AppContext_JPowerDeviceChanged(object? sender, ILegacyJPowerDevice? e)
        {
            OnPropertyChanged(nameof(CurrentJPowerDevice));

            if (appContext.LegacyJPowerDevice != null)
            {
                appContext.LegacyJPowerDevice.StateValues.Subscribe(_ => OnJPowerStateChanged());
            }
        }

        private void Measurements_CollectionChanged(object? sender, NotifyCollectionChangedEventArgs e)
        {
            RefreshCommandEnables();
        }

        private async void BleDevice_DeviceStateChanged(object? sender, BleDeviceState newState)
        {
            OnPropertyChanged(nameof(BleDevice));

            if (newState == BleDeviceState.Disconnected)
            {
                //await alertService.DisplayAlert(
                //    "Error",
                //    "Device disconnected",
                //    "OK"
                //);

                await navigationService.NavigateToConnectPage();
            }

            RefreshCommandEnables();
        }

        private void OnJPowerStateChanged()
        {
            OnPropertyChanged(nameof(CanCalibrate));
        }

        private readonly IAppContext appContext;
        private readonly INavigationService navigationService;
        private readonly IAlertService alertService;
        private readonly IBleService bleService;
        private readonly ICalibrationContext calibrationContext;
        private string weightInput;
    }
}
