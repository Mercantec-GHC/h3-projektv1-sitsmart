using System.ComponentModel.DataAnnotations;

namespace API.Models
{
    public abstract class Common
    {
        [Key]
        public required String Id { get; set; }
        public required DateTime CreatedAt { get; set; } = DateTime.Now;
    }
}
