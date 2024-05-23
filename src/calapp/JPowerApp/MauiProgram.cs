using CommunityToolkit.Maui;
using JPowerApp.Shared.Services;
using JPowerApp.Shared.UI;
using Microsoft.Extensions.Logging;

namespace JPowerApp;

public static class MauiProgram
{
	public static MauiApp CreateMauiApp()
	{
		var builder = MauiApp.CreateBuilder();
		builder
			.UseMauiApp<App>()
            .UseMauiCommunityToolkit()
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

        builder.Services.AddSingleton<IPermissionsService, PermissionsService>();
        builder.Services.AddSingleton<INavigationService, NavigationService>();
        builder.Services.AddSingleton<IAlertService, AlertService>();
		builder.Services.AddSingleton<IJPowerDiscoveryService, JPowerDiscoveryService>();

		builder.Services.AddTransient<ConnectPageView>();
        builder.Services.AddTransient<ConnectPageViewModel>();
        builder.Services.AddTransient<CalibratePageView>();
        builder.Services.AddTransient<CalibratePageViewModel>();

#if DEBUG
        builder.Logging.AddDebug();
#endif

		return builder.Build();
	}
}
