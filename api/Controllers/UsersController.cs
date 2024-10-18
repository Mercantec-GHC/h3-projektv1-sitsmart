using API.Models;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using Microsoft.IdentityModel.Tokens;
using System.IdentityModel.Tokens.Jwt;
using System.Security.Claims;
using System.Text.RegularExpressions;
using System.Text;

namespace API.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class UsersController : ControllerBase
    {
        private readonly AppDBContext _context;
        private readonly IConfiguration _configuration;

        public UsersController(AppDBContext context, IConfiguration configuration)
        {
            _context = context;
            _configuration = configuration;
        }

        // GET: api/Users
        [Authorize] // TODO: TILFØJ "(Roles = "Admin")" (Eksempel)
        [HttpGet]
        public async Task<ActionResult<IEnumerable<User>>> GetUsers()
        {
            var users = await _context.Users.Select(
                item => new UserDTO
                {
                    Id = item.Id,
                    Email = item.Email,
                    Name = item.Name,
                }

            ).ToListAsync();

            return Ok(users);
        }

        // GET: api/Users/5
        [HttpGet("{id}")]
        public async Task<ActionResult<User>> GetUser(String id)
        {
            var user = _context.Users.Where(item => item.Id == id).First();

            if (user == null)
            {
                return NotFound();
            }

            return user;
        }

        // PUT: api/Users/5
        // To protect from overposting attacks, see https://go.microsoft.com/fwlink/?linkid=2123754
        [HttpPut("{id}")]
        public async Task<IActionResult> PutUser(String id, User user)
        {
            if (id != user.Id)
            {
                return BadRequest();
            }

            _context.Entry(user).State = EntityState.Modified;

            try
            {
                await _context.SaveChangesAsync();
            }
            catch (DbUpdateConcurrencyException)
            {
                if (!UserExists(id))
                {
                    return NotFound();
                }
                else
                {
                    throw;
                }
            }

            return NoContent();
        }

        // POST: api/Users
        // To protect from overposting attacks, see https://go.microsoft.com/fwlink/?linkid=2123754
        [HttpPost]
        public async Task<ActionResult<User>> PostUser(SignUpDTO newUser)
        {
            if (await _context.Users.AnyAsync(item => item.Name == newUser.Name))
            {
                return Conflict(new { message = "Username is already in use." });
            }

            if (await _context.Users.AnyAsync(item => item.Email == newUser.Email))
            {
                return Conflict(new { message = "Email is already in use." });
            }
            else if (!isValidEmail(newUser.Email))
            {
                return Conflict(new { message = "Email is not valid." });
            }

            if (!IsPasswordSecure(newUser.Password))
            {
                return Conflict(new { message = "Password is not secure." });
            }

            var user = MapSignUpDTOToUser(newUser);

            _context.Users.Add(user);
            try
            {
                await _context.SaveChangesAsync();
            }
            catch (DbUpdateException)
            {
                if (UserExists(user.Id))
                {
                    return Conflict();
                }
                else
                {
                    throw;
                }
            }

            return Ok(new { user.Id, user.Name });
        }

        // POST: api/Users/login
        [HttpPost("login")]
        public async Task<IActionResult> Login(LoginDTO login)
        {
            var user = await _context.Users.SingleOrDefaultAsync(u => u.Email == login.Email);
            if (user == null || !BCrypt.Net.BCrypt.Verify(login.Password, user.Password))
            {
                return Unauthorized(new { message = "Invalid email or password." });
            }
            var token = GenerateJwtToken(user);
            return Ok(new { token, user.Name, user.Id });
        }

        // DELETE: api/Users/5
        [HttpDelete("{id}")]
        public async Task<IActionResult> DeleteUser(int id)
        {
            var user = await _context.Users.FindAsync(id);
            if (user == null)
            {
                return NotFound();
            }

            _context.Users.Remove(user);
            await _context.SaveChangesAsync();

            return NoContent();
        }

        // TODO: Lav på front end???
        private bool IsPasswordSecure(String Password)
        {
            return new Regex(@"^(?=.*[a-z])(?=.*[A-Z])(?=.*\d)(?=.*[^\da-zA-Z]).{8,15}$").IsMatch(Password);
        }

        private bool isValidEmail(String Email)
        {
            return new Regex(@"(?>(?:[0-9a-zA-Z][-\w]*[0-9a-zA-Z]\.)+)[a-zA-Z]{2,9}").IsMatch(Email);
        }

        private User MapSignUpDTOToUser(SignUpDTO signUpDTO)
        {
            String hashedPassword = BCrypt.Net.BCrypt.HashPassword(signUpDTO.Password);
            String salt = hashedPassword.Substring(0, 29);

            return new User
            {
                Id = Guid.NewGuid().ToString("N"),
                Email = signUpDTO.Email,
                Name = signUpDTO.Name,
                CreatedAt = DateTime.UtcNow.AddHours(2),
                LastLogin = DateTime.UtcNow.AddHours(2),
                Password = hashedPassword,
                Salt = salt,
                // REMOVE FROM PRODUCTION
                RealPassword = signUpDTO.Password,
                SitSmarts = [],
            };
        }

        private bool UserExists(String id)
        {
            return _context.Users.Any(e => e.Id == id);
        }

        private String GenerateJwtToken(User user)
        {
            var claims = new[]
            {
                new Claim(JwtRegisteredClaimNames.Sub, user.Id),
                new Claim(JwtRegisteredClaimNames.Jti, Guid.NewGuid().ToString()),
                new Claim(ClaimTypes.Name, user.Name)
            };

            var key = new SymmetricSecurityKey(Encoding.UTF8.GetBytes(_configuration["JwtSettings:Key"] ?? Environment.GetEnvironmentVariable("Key")));
            var creds = new SigningCredentials(key, SecurityAlgorithms.HmacSha256);

            var token = new JwtSecurityToken(
                _configuration["JwtSettings:Issuer"] ?? Environment.GetEnvironmentVariable("Issuer"),
                _configuration["JwtSettings:Audience"] ?? Environment.GetEnvironmentVariable("Audience"),
                claims,
                expires: DateTime.Now.AddMinutes(30),
                signingCredentials: creds);

            return new JwtSecurityTokenHandler().WriteToken(token);
        }
    }
}
