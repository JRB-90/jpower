namespace CalApp.Shared.Services
{
    public class AlertService : IAlertService
    {
        public Task DisplayAlert(
            string title, 
            string message, 
            string cancel)
        {
            if (Application.Current == null)
            {
                throw new InvalidOperationException("Failed to get Application Context");
            }

            if (Application.Current.MainPage == null)
            {
                throw new InvalidOperationException("Failed to get MainPage Context");
            }

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
            if (Application.Current == null)
            {
                throw new InvalidOperationException("Failed to get Application Context");
            }

            if (Application.Current.MainPage == null)
            {
                throw new InvalidOperationException("Failed to get MainPage Context");
            }

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
            if (Application.Current == null)
            {
                throw new InvalidOperationException("Failed to get Application Context");
            }

            if (Application.Current.MainPage == null)
            {
                throw new InvalidOperationException("Failed to get MainPage Context");
            }

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
            if (Application.Current == null)
            {
                throw new InvalidOperationException("Failed to get Application Context");
            }

            if (Application.Current.MainPage == null)
            {
                throw new InvalidOperationException("Failed to get MainPage Context");
            }

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
