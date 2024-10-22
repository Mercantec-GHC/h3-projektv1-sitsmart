using System.ComponentModel.DataAnnotations;

namespace API.Models
{
    public class UserSitSmart
    {
        [Key]
        public String id { get; set; }
        public String userId { get; set; }
        public User? user { get; set; }
        public String? deviceId { get; set; }
        public SitSmartDevice? device { get; set; }
    }

    public class createRelationDTO
    {
        public String userId { get; set; }
        public String? deviceId { get; set; }
    }
}
