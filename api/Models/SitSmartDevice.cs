using System.ComponentModel.DataAnnotations;

namespace API.Models
{
    public class SitSmartDevice
    {
        [Key]
        public String Id { get; set; }
        public List<TempHumidity?> TempHumiditys { get; set; }
        public List<Movement?> Movements { get; set; }
        public List<DistanceObject?> DistanceObjects { get; set; }
        public List<UserSitSmart?> Users { get; set; }
    }
    public class sitSmartDeviceDTO
    {
    }
}
