using System.ComponentModel.DataAnnotations;

namespace API.Models
{
    public class UserSitSmart
    {
        [Key]
        public int Id { get; set; }
        public required String UserId { get; set; }
        public required List<User> Users { get; set; }
        public required String SitSmartId { get; set; }
        public required List<SitSmartDevice?> Devices { get; set; }
    }
}
