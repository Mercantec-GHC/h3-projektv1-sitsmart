namespace API.Models
{
    public class Movement : Common
    {
        public required float XValue { get; set; }
        public required float YValue { get; set; }
        public required float ZValue { get; set; }
        public string sitSmartDeviceId { get; set; }
        public SitSmartDevice sitSmartDevice { get; set; }
    }
}
