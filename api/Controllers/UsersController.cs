using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using API.Models;
using System.Text.RegularExpressions;

namespace API.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class UsersController : ControllerBase
    {
        private readonly AppDBContext _context;

        public UsersController(AppDBContext context)
        {
            _context = context;
        }

        // GET: api/Users
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
        public async Task<ActionResult<User>> GetUser(int id)
        {
            var user = await _context.Users.FindAsync(id);

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
        public async Task<ActionResult<User>> PostUser(User newUser)
        {
            if (await _context.Users.AnyAsync(item => item.Name == newUser.Name))
            {
                return Conflict(new { message = "Username is already in use." });
            }

            if (await _context.Users.AnyAsync(item => item.Email == newUser.Email))
            {
                return Conflict(new { message = "Email is already in use." });
            } else if (!isValidEmail(newUser.Email))
            {
                return Conflict(new { message = "Email is not valid." });
            }

            if (!IsPasswordSecure(newUser.Password))
            {
                return Conflict(new { message = "Password is not secure." });
            }

            _context.Users.Add(newUser);
            try
            {
                await _context.SaveChangesAsync();
            }
            catch (DbUpdateException)
            {
                if (UserExists(newUser.Id)) 
                {
                    return Conflict();
                } else
                {
                    throw;
                }
            }

            return Ok(new { newUser.Id, newUser.Name });
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

        private bool IsPasswordSecure(string Password)
        {
            // Minimum 8 chars, one or more upper case, one or more lower case, one number and one special character
            return new Regex(@"^(?=.*?[A-Z])(?=.*?[a-z])(?=.*?[0-9])(?=.*?[#?!@$ %^&*-]).{8,}$").IsMatch(Password);
        }

        private bool isValidEmail(string Email)
        {
            return new Regex(@"^[\\w\\-\\.]+@([\\w-]+\\.)+[\\w-]{2,}$").IsMatch(Email);
        }

        private User MapSignUpDTOToUser(SignUpDTO signUpDTO)
        {
            string hashedPassword = BCrypt.Net.BCrypt.HashPassword(signUpDTO.Password);
            string salt = hashedPassword.Substring(0, 29);

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
            };
        }

        private bool UserExists(String id)
        {
            return _context.Users.Any(e => e.Id == id);
        }
    }
}
