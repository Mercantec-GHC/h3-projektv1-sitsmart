namespace API.Models
{
    public class DistanceObject : Common
    {
        public required int Length { get; set; }
        public string sitSmartDeviceId { get; set; }
        public SitSmartDevice sitSmartDevice { get; set; }
    }
}
