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

            builder.Services.AddSingleton<IPermissionsService, PermissionsService>();
            builder.Services.AddSingleton<INavigationService, NavigationService>();
            builder.Services.AddSingleton<IAlertService, AlertService>();
            builder.Services.AddSingleton<IJPowerDiscoveryService, JPowerDiscoveryService>();

            builder.Services.AddTransient<ConnectPageView>();
            builder.Services.AddTransient<ConnectPageViewModel>();
            builder.Services.AddTransient<CalibratePageView>();
            builder.Services.AddTransient<CalibratePageViewModel>();
            builder.Services.AddTransient<BleServicesView>();
            builder.Services.AddTransient<BleServicesViewModel>();

            return builder.Build();
        }
    }
}
