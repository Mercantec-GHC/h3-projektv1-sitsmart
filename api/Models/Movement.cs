namespace API.Models
{
    public class Movement : Common
    {
        public required int XValue { get; set; }
        public required int YValue { get; set; }
        public required int ZValue { get; set; }
        public string sitSmartDeviceId { get; set; }
        public SitSmartDevice sitSmartDevice { get; set; }
    }
}
