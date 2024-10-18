namespace API.Models
{
    public class DistanceObject : Common
    {
        public required int Distance { get; set; }
        public required String sitSmartDeviceId { get; set; }
        public required SitSmartDevice sitSmartDevice { get; set; }
    }
}
