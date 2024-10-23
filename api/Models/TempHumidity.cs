namespace API.Models
{
    public class TempHumidity : Common
    {
        public int Temp { get; set; }
        public int Humidity { get; set; }
        public String sitSmartDeviceId { get; set; }
        public SitSmartDevice sitSmartDevice { get; set; }
    }
    public class PostTempHumidDTO
    {
        public int Temp { get; set; }
        public int Humidity { get; set; }
        public String sitSmartDeviceId { get; set; }
    }
}
