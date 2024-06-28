namespace JPower.Shared.Mvvm
{
    public static class ServiceProvider
    {
        public static IServiceProvider Current
        {
            get
            {
#if WINDOWS10_0_17763_0_OR_GREATER
                return MauiWinUIApplication.Current.Services;
#elif ANDROID
                return MauiApplication.Current.Services;
#elif IOS || MACCATALYST
			    return MauiUIApplicationDelegate.Current.Services;
#else
			    return null;
#endif
            }
        }

        public static TService? GetService<TService>()
        {
            return Current.GetService<TService>();
        }
    }
}
