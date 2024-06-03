using CalApp.Shared.Mvvm;
using CalApp.Shared.Services;
using OxyPlot;
using OxyPlot.Axes;
using OxyPlot.Series;

namespace CalApp.Shared.UI
{
    public class SlopeResultPageViewModel : ViewModelBase
    {
        public SlopeResultPageViewModel(IAppContext appContext)
        {
            this.appContext = appContext;
            calibrationContext = appContext.CalibrationContext;
            slopePlotModel = new PlotModel();

            appContext.BusyStateChanged += AppContext_BusyStateChanged;
            calibrationContext.Measurements.CollectionChanged += Measurements_CollectionChanged;

            BuildChartFromMeasurements();
        }

        public bool IsBusy => appContext.IsBusy;

        public PlotModel SlopePlotModel
        {
            get => slopePlotModel;
            set => SetProperty(ref slopePlotModel, value);
        }

        private void BuildChartFromMeasurements()
        {
            PlotModel model = new PlotModel();

            model.Title = "Slope Plot";
            model.TitleColor = OxyColor.FromRgb(255, 255, 255);
            model.TextColor = OxyColor.FromRgb(255, 255, 255);
            model.PlotAreaBorderColor = OxyColor.FromRgb(255, 255, 255);
            model.SubtitleColor = OxyColor.FromRgb(255, 255, 255);

            var weightAxis =
                new LinearAxis()
                {
                    Title = "Weight (kg)",
                    Position = AxisPosition.Bottom,
                    AxislineColor = OxyColor.FromRgb(255, 255, 255),
                    TitleColor = OxyColor.FromRgb(255, 255, 255),
                    TextColor = OxyColor.FromRgb(255, 255, 255),
                    MajorGridlineColor = OxyColor.FromRgb(255, 255, 255),
                    MinorGridlineColor = OxyColor.FromRgb(255, 255, 255),
                    ExtraGridlineColor = OxyColor.FromRgb(255, 255, 255),
                    TicklineColor = OxyColor.FromRgb(255, 255, 255),
                };

            var adcAxis =
                new LinearAxis()
                {
                    Title = "ADC Value",
                    Position = AxisPosition.Left,
                    AxislineColor = OxyColor.FromRgb(255, 255, 255),
                    TitleColor = OxyColor.FromRgb(255, 255, 255),
                    TextColor = OxyColor.FromRgb(255, 255, 255),
                    MajorGridlineColor = OxyColor.FromRgb(255, 255, 255),
                    MinorGridlineColor = OxyColor.FromRgb(255, 255, 255),
                    ExtraGridlineColor = OxyColor.FromRgb(255, 255, 255),
                    TicklineColor = OxyColor.FromRgb(255, 255, 255),
                };

            model.Axes.Add(weightAxis);
            model.Axes.Add(adcAxis);

            var dataSeries = new LineSeries();

            foreach (var measurement in calibrationContext.Measurements)
            {
                dataSeries.Points.Add(
                    new DataPoint(measurement.Weight, (double)measurement.AdcValue)
                );
            }

            model.Series.Add(dataSeries);

            SlopePlotModel = model;
        }

        private void AppContext_BusyStateChanged(object? sender, bool e)
        {
            OnPropertyChanged(nameof(IsBusy));
        }

        private void Measurements_CollectionChanged(object? sender, System.Collections.Specialized.NotifyCollectionChangedEventArgs e)
        {
            BuildChartFromMeasurements();
        }

        private readonly IAppContext appContext;
        private readonly ICalibrationContext calibrationContext;
        private PlotModel slopePlotModel;
    }
}
