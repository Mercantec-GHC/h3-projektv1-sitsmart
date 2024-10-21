using System.ComponentModel.DataAnnotations;

namespace API.Models
{
    public class UserSitSmart
    {
        [Key]
        public int Id { get; set; }
        public required String UserId { get; set; }
        public required User User { get; set; }
        public required String? SitSmartId { get; set; }
        public required SitSmartDevice? Device { get; set; }
    }
}
