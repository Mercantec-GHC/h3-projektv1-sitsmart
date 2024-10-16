namespace API.Models
{
    public class TempHumidity : Common
    {
        public required int Temp { get; set; }
        public required int Humidity { get; set; }
        public string sitSmartDeviceId { get; set; }
        public SitSmartDevice sitSmartDevice { get; set; }
    }
}
