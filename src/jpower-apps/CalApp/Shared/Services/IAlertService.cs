namespace CalApp.Shared.Services
{
    public interface IAlertService
    {
        Task DisplayAlert(
            string title, 
            string message, 
            string cancel
        );

        Task DisplayAlert(
            string title, 
            string message, 
            string cancel, 
            FlowDirection flowDirection
        );

        Task DisplayAlert(
            string title, 
            string message, 
            string accept, 
            string cancel
        );

        Task DisplayAlert(
            string title, 
            string message,
            string accept, 
            string cancel, 
            FlowDirection flowDirection
        );
    }
}
