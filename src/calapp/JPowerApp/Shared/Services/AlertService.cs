namespace JPowerApp.Shared.Services
{
    public class AlertService : IAlertService
    {
        public Task DisplayAlert(
            string title, 
            string message, 
            string cancel)
        {
            return 
                Application.Current.MainPage.DisplayAlert(
                    title, 
                    message, 
                    cancel
                );
        }

        public Task DisplayAlert(
            string title, 
            string message, 
            string cancel, 
            FlowDirection flowDirection)
        {
            return 
                Application.Current.MainPage.DisplayAlert(
                    title, 
                    message, 
                    cancel, 
                    flowDirection
                );
        }

        public Task DisplayAlert(
            string title, 
            string message, 
            string accept, 
            string cancel)
        {
            return 
                Application.Current.MainPage.DisplayAlert(
                    title, 
                    message, 
                    accept, 
                    cancel
                );
        }

        public Task DisplayAlert(
            string title, 
            string message, 
            string accept, 
            string cancel, 
            FlowDirection flowDirection)
        {
            return Application.Current.MainPage.DisplayAlert(
                title, 
                message, 
                accept, 
                cancel, 
                flowDirection
            );
        }
    }
}
