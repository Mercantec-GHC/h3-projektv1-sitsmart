namespace API.Models
{
    public class User : Common
    {
        public required String Email { get; set; }
        public required String Name { get; set; }
        public required String Password { get; set; }

        // NOT FOR PRODUCTION
        public required String RealPassword { get; set; }
        public required String Salt { get; set; }
        public required DateTime LastLogin { get; set; }
        public required List<UserSitSmart?> SitSmarts { get; set; }
    }

    public class UserDTO
    {
        public required String Id { get; set; }
        public required String Email { get; set; }
        public required String Name { get; set; }
    }

    public class LoginDTO
    {
        public required String Email { get; set; }
        public required String Password { get; set; }
    }

    public class SignUpDTO
    {
        public required String Email { get; set; }
        public required String Name { get; set; }
        public required String Password { get; set; }
    }
}
