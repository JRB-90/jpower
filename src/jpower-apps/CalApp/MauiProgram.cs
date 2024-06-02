using CalApp.Shared.Ble;
using CalApp.Shared.Services;
using CalApp.Shared.UI;
using CommunityToolkit.Maui;
using MauiIcons.Fluent;
using MauiIcons.Material;
using Microsoft.Extensions.Logging;
using UraniumUI;

namespace CalApp
{
    public static class MauiProgram
    {
        public static MauiApp CreateMauiApp()
        {
            bool isSimMode = true;

            var builder = MauiApp.CreateBuilder();
            builder
                .UseMauiApp<App>()
                .UseMauiCommunityToolkit()
                .UseMaterialMauiIcons()
                .UseFluentMauiIcons()
                .UseUraniumUI()
                .UseUraniumUIMaterial()
                .ConfigureFonts(fonts =>
                {
                    fonts.AddFont("OpenSans-Regular.ttf", "OpenSansRegular");
                    fonts.AddFont("OpenSans-Semibold.ttf", "OpenSansSemibold");
                });

            builder.UseMauiCommunityToolkit(options =>
            {
                options.SetShouldSuppressExceptionsInConverters(false);
                options.SetShouldSuppressExceptionsInBehaviors(false);
                options.SetShouldSuppressExceptionsInAnimations(false);
            });

#if DEBUG
            builder.Logging.AddDebug();
#endif

            builder.Services.AddSingleton<IAppContext, Shared.Services.AppContext>();
            builder.Services.AddSingleton<IPermissionsService, PermissionsService>();
            builder.Services.AddSingleton<INavigationService, NavigationService>();
            builder.Services.AddSingleton<IAlertService, AlertService>();

            if (isSimMode)
            {
                builder.Services.AddSingleton<IBleService, SimulatedBleService>();
            }
            else
            {
                builder.Services.AddSingleton<IBleService, BleService>();
            }

            builder.Services.AddTransient<ConnectPageView>();
            builder.Services.AddTransient<ConnectPageViewModel>();
            builder.Services.AddTransient<DeviceOverviewView>();
            builder.Services.AddTransient<DeviceOverviewViewModel>();
            builder.Services.AddTransient<CalibrationPageView>();
            builder.Services.AddTransient<CalibrationPageViewModel>();
            builder.Services.AddTransient<SlopeResultPageView>();
            builder.Services.AddTransient<SlopeResultPageViewModel>();
            builder.Services.AddTransient<DeviceInfoPageView>();
            builder.Services.AddTransient<DeviceInfoPageViewModel>();

            return builder.Build();
        }
    }
}
