using CommunityToolkit.Maui;
using JPower.Shared.Services;
using JPower.DiagnosticsApp.Services;
using MauiIcons.Fluent;
using MauiIcons.Material;
using Microsoft.Extensions.Logging;
using OxyPlot.Maui.Skia;
using SkiaSharp.Views.Maui.Controls.Hosting;
using UraniumUI;
using JPower.DiagnosticsApp.UI;

namespace JPower.DiagnosticsApp
{
    public static class MauiProgram
    {
        public static MauiApp CreateMauiApp()
        {
            bool isSimMode = false;

            var builder = MauiApp.CreateBuilder();
            builder
                .UseMauiApp<App>()
                .UseSkiaSharp()
                .UseOxyPlotSkia()
                .UseMauiCommunityToolkit()
                .UseMaterialMauiIcons()
                .UseFluentMauiIcons()
                .UseUraniumUI()
                .UseUraniumUIMaterial()
                .ConfigureFonts(fonts =>
                {
                    fonts.AddFont("OpenSans-Regular.ttf", "OpenSansRegular");
                    fonts.AddFont("OpenSans-Semibold.ttf", "OpenSansSemibold");
                    fonts.AddMaterialIconFonts();
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

            builder.Services.AddSingleton<IAppContext, JPower.Shared.Services.AppContext>();
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

            builder.Services.AddTransient<ScanPageView>();
            builder.Services.AddTransient<ScanPageViewModel>();
            builder.Services.AddTransient<DiagnosticsPageView>();
            builder.Services.AddTransient<DiagnosticsPageViewModel>();

            return builder.Build();
        }
    }
}
