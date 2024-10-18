using System.ComponentModel.DataAnnotations;

namespace API.Models
{
    public class UserSitSmart
    {
        [Key]
        public int Id { get; set; }
        public required String UserId;
        public required List<User> Users;
        public required String SitSmartId;
        public required List<SitSmartDevice?> Devices;
    }
}
