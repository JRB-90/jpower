using CalApp.Shared.Models;
using CalApp.Shared.Mvvm;

namespace CalApp.Shared.UI
{
    public class BleServicesViewModel : ViewModelBase
    {
        public BleServicesViewModel(IReadOnlyCollection<BleService> services)
        {
            Services = services;
        }

        public IReadOnlyCollection<BleService> Services { get; }
    }
}
