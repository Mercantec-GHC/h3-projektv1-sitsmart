using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using API.Models;

namespace API.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class MovementsController : ControllerBase
    {
        private readonly AppDBContext _context;

        public MovementsController(AppDBContext context)
        {
            _context = context;
        }

        // GET: api/Movements
        [HttpGet]
        public async Task<ActionResult<IEnumerable<Movement>>> GetMovementObjects()
        {
            return await _context.MovementObjects.ToListAsync();
        }

        // GET: api/Movements/5
        [HttpGet("{id}")]
        public async Task<ActionResult<Movement>> GetMovement(string id)
        {
            var movement = await _context.MovementObjects.FindAsync(id);

            if (movement == null)
            {
                return NotFound();
            }

            return movement;
        }

        // PUT: api/Movements/5
        // To protect from overposting attacks, see https://go.microsoft.com/fwlink/?linkid=2123754
        [HttpPut("{id}")]
        public async Task<IActionResult> PutMovement(string id, Movement movement)
        {
            if (id != movement.Id)
            {
                return BadRequest();
            }

            _context.Entry(movement).State = EntityState.Modified;

            try
            {
                await _context.SaveChangesAsync();
            }
            catch (DbUpdateConcurrencyException)
            {
                if (!MovementExists(id))
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

        // POST: api/Movements
        // To protect from overposting attacks, see https://go.microsoft.com/fwlink/?linkid=2123754
        [HttpPost]
        public async Task<ActionResult<Movement>> PostMovement(Movement movement)
        {
            _context.MovementObjects.Add(movement);
            try
            {
                await _context.SaveChangesAsync();
            }
            catch (DbUpdateException)
            {
                if (MovementExists(movement.Id))
                {
                    return Conflict();
                }
                else
                {
                    throw;
                }
            }

            return CreatedAtAction("GetMovement", new { id = movement.Id }, movement);
        }

        // DELETE: api/Movements/5
        [HttpDelete("{id}")]
        public async Task<IActionResult> DeleteMovement(string id)
        {
            var movement = await _context.MovementObjects.FindAsync(id);
            if (movement == null)
            {
                return NotFound();
            }

            _context.MovementObjects.Remove(movement);
            await _context.SaveChangesAsync();

            return NoContent();
        }

        private bool MovementExists(string id)
        {
            return _context.MovementObjects.Any(e => e.Id == id);
        }
    }
}
