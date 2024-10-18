using System.ComponentModel.DataAnnotations;

namespace API.Models
{
    public class SitSmartDevice
    {
        [Key]
        public required String Id { get; set; }
        public required List<TempHumidity?> TempHumiditys { get; set; }
        public required List<Movement?> Movements { get; set; }
        public required List<DistanceObject?> DistanceObjects { get; set; }
        public required List<UserSitSmart> Users { get; set; }
    }
}
