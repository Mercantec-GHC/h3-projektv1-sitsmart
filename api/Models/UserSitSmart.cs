using System.ComponentModel.DataAnnotations;

namespace API.Models
{
    public class UserSitSmart
    {
        [Key]
        public String id { get; set; }
        public required String userId { get; set; }
        public required User? user { get; set; }
        public required String? deviceId { get; set; }
        public required SitSmartDevice? device { get; set; }
    }

    public class createRelationDTO
    {
        public required String Id { get; set; }
        public required String userId { get; set; }
        public required String? deviceId { get; set; }
    }
}
