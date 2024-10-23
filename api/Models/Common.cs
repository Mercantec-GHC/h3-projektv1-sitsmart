using System.ComponentModel.DataAnnotations;

namespace API.Models
{
    public abstract class Common
    {
        [Key]
        public String Id { get; set; }
        public DateTime CreatedAt { get; set; } = DateTime.Now;
    }
}
