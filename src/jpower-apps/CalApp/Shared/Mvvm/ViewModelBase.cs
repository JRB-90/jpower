using CommunityToolkit.Mvvm.ComponentModel;

namespace CalApp.Shared.Mvvm
{
    public abstract class ViewModelBase : ObservableObject
    {
        public virtual Task OnNavigatingTo(object? parameter)
        {
            return Task.CompletedTask;
        }

        public virtual Task OnNavigatedFrom(bool isForwardNavigation)
        {
            return Task.CompletedTask;
        }

        public virtual Task OnNavigatedTo()
        {
            return Task.CompletedTask;
        }
    }
}
