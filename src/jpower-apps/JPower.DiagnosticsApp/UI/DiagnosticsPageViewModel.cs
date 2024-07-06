using JPower.Shared.Services;
using JPower.DiagnosticsApp.Services;
using JPower.Shared.Mvvm;
using JPower.Shared.Ble;
using JPower.Shared.JPower;
using CommunityToolkit.Maui.Core;
using JPower.Shared.UI;

namespace JPower.DiagnosticsApp.UI
{
    public class DiagnosticsPageViewModel : ViewModelBase
    {
        public DiagnosticsPageViewModel(
            IAppContext appContext,
            INavigationService navigationService,
            IAlertService alertService,
            IPopupService popupService,
            IBleService bleService)
        {
            this.appContext = appContext;
            this.navigationService = navigationService;
            this.popupService = popupService;
            this.alertService = alertService;
            this.bleService = bleService;

            PullCalCommand =
                new Command(
                    async () => await PullCal(),
                    () => appContext.JPowerDevice != null
                );

            PushCalCommand =
                new Command(
                    async () => await PushCal(),
                    () => appContext.JPowerDevice != null
                );

            ZeroOffsetCommand =
                new Command(
                    async () => await ZeroOffset(),
                    () => appContext.JPowerDevice != null
                );

            MeasureCommand =
                new Command(
                    async () => await Measure(),
                    () => appContext.JPowerDevice != null
                );

            appContext.BusyStateChanged += AppContext_BusyStateChanged;
        }

        public bool IsBusy => appContext.IsBusy;

        public IJPowerDevice? CurrentJPowerDevice => appContext.JPowerDevice;

        public Command PullCalCommand { get; }

        public Command PushCalCommand { get; }

        public Command ZeroOffsetCommand { get; }

        public Command MeasureCommand { get; }

        public override async Task OnNavigatingTo(object? parameter)
        {
            if (appContext.BleDevice == null)
            {
                await alertService.DisplayAlert(
                    "Error",
                    "No device selected, returning to scan page",
                    "OK"
                );

                await navigationService.NavigateToScanPage();

                return;
            }

            if (appContext.BleDevice.DeviceState != BleDeviceState.Connected)
            {
                await alertService.DisplayAlert(
                    "Error",
                    "Device disconnected, returning to scan page",
                    "OK"
                );

                await navigationService.NavigateToScanPage();
            }

            if (appContext.LegacyJPowerDevice == null)
            {
                // TODO
            }
        }

        public override async Task OnNavigatedFrom(bool isForwardNavigation)
        {
            try
            {
                appContext.IsBusy = true;

                if (!isForwardNavigation)
                {
                    if (appContext.LegacyJPowerDevice != null)
                    {
                        await appContext.LegacyJPowerDevice.StopStreaming();
                    }

                    if (appContext.BleDevice != null)
                    {
                        await appContext.BleDevice.Disconnect();
                    }
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

        private async Task PullCal()
        {
            try
            {
                appContext.IsBusy = true;

                // Minimum wait time for busy overlay
                await Task.Delay(100);

                if (appContext.JPowerDevice == null)
                {
                    throw new InvalidOperationException("JPower Device Invalid");
                }

                var cal = await appContext.JPowerDevice.PullCalibration();

                await popupService.ShowPopupAsync<DisplayCalibrationViewModel>(
                    onPresenting: x => x.Calibration = cal
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

        private async Task PushCal()
        {
            try
            {
                appContext.IsBusy = true;

                var cal = new JPowerCalibrationData();
                cal.guid = Guid.NewGuid().ToByteArray();
                EnterCalibrationViewModel? diag = null;

                var result =
                    await popupService.ShowPopupAsync<EnterCalibrationViewModel>(
                        onPresenting: x =>
                        {
                            diag = x;
                            x.Calibration = cal;
                        }
                    );

                if (diag == null)
                {
                    return;
                }

                if (result is bool boolResult)
                {
                    if (boolResult == false)
                    {
                        return;
                    }
                }

                // Minimum wait time for busy overlay
                await Task.Delay(100);

                if (appContext.JPowerDevice == null)
                {
                    throw new InvalidOperationException("JPower Device Invalid");
                }

                await appContext.JPowerDevice.PushCalibration(diag.Calibration);
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

        private async Task ZeroOffset()
        {
            try
            {
                appContext.IsBusy = true;

                // Minimum wait time for busy overlay
                await Task.Delay(100);

                if (appContext.JPowerDevice == null)
                {
                    throw new InvalidOperationException("JPower Device Invalid");
                }

                await appContext.JPowerDevice.ZeroOffset();
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

        private async Task Measure()
        {
            try
            {
                appContext.IsBusy = true;

                // Minimum wait time for busy overlay
                await Task.Delay(100);

                if (appContext.JPowerDevice == null)
                {
                    throw new InvalidOperationException("JPower Device Invalid");
                }

                var value = await appContext.JPowerDevice.Measure(10);

                await alertService.DisplayAlert(
                    "Measurement",
                    Convert.ToString(value),
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

        private void AppContext_BusyStateChanged(object? sender, bool e)
        {
            OnPropertyChanged(nameof(IsBusy));
        }

        private readonly IAppContext appContext;
        private readonly INavigationService navigationService;
        private readonly IPopupService popupService;
        private readonly IAlertService alertService;
        private readonly IBleService bleService;
    }
}
