namespace API.Models
{
    public class DistanceObject : Common
    {
        public int Distance { get; set; }
        public String sitSmartDeviceId { get; set; }
        public SitSmartDevice sitSmartDevice { get; set; }
    }
    public class postDistanceDTO
    {
        public int Distance { get; set; }
        public String sitSmartDeviceId { get; set; }
    }
}
