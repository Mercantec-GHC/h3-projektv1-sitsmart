namespace API.Models
{
    public class User : Common
    {
        public String Email { get; set; }
        public String Name { get; set; }
        public String Password { get; set; }

        // NOT FOR PRODUCTION
        public String RealPassword { get; set; }
        public String Salt { get; set; }
        public DateTime LastLogin { get; set; }
        public List<UserSitSmart?> SitSmarts { get; set; }
    }

    public class UserDTO
    {
        public String Id { get; set; }
        public String Email { get; set; }
        public String Name { get; set; }
    }

    public class LoginDTO
    {
        public String Email { get; set; }
        public String Password { get; set; }
    }

    public class SignUpDTO
    {
        public String Email { get; set; }
        public String Name { get; set; }
        public String Password { get; set; }
    }
}
