using CalApp.Shared.Mvvm;
using CalApp.Shared.Services;

namespace CalApp.Shared.UI
{
    public class SlopeResultPageViewModel : ViewModelBase
    {
        public SlopeResultPageViewModel(IAppContext appContext)
        {
            this.appContext = appContext;

            appContext.BusyStateChanged += AppContext_BusyStateChanged;
        }

        public bool IsBusy => appContext.IsBusy;

        private void AppContext_BusyStateChanged(object? sender, bool e)
        {
            OnPropertyChanged(nameof(IsBusy));
        }

        private readonly IAppContext appContext;
    }
}
