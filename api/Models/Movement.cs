namespace API.Models
{
    public class Movement : Common
    {
        public int XValue { get; set; }
        public int YValue { get; set; }
        public int ZValue { get; set; }
        public String sitSmartDeviceId { get; set; }
        public SitSmartDevice sitSmartDevice { get; set; }
    }
    public class postMovementDTO
    {
        public int XValue { get; set; }
        public int YValue { get; set; }
        public int ZValue { get; set; }
        public String sitSmartDeviceId { get; set; }
    }
}
