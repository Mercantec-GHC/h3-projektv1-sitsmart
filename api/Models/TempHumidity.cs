namespace API.Models
{
    public class TempHumidity : Common
    {
        public required int Temp { get; set; }
        public required int Humidity { get; set; }
        public required String sitSmartDeviceId { get; set; }
        public required SitSmartDevice sitSmartDevice { get; set; }
    }
}
