using CalApp.Shared.Models;
using CalApp.Shared.Mvvm;

namespace CalApp.Shared.UI
{
    public class JPowerViewModel : ViewModelBase
    {
        public JPowerViewModel(JPowerDevice jpower)
        {
            JPower = jpower;
        }

        public JPowerDevice JPower { get; }
    }
}
