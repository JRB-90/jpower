using System.ComponentModel;

namespace CalApp.Shared.Mvvm
{
    public abstract class ViewModelBase : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler? PropertyChanged;

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

        protected void NotifyOfChange(string propertyName)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}
