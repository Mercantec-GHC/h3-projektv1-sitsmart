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
    public class DistanceObjectsController : ControllerBase
    {
        private readonly AppDBContext _context;

        public DistanceObjectsController(AppDBContext context)
        {
            _context = context;
        }

        // GET: api/DistanceObjects
        [HttpGet]
        public async Task<ActionResult<IEnumerable<DistanceObject>>> GetDistanceObjects()
        {
            return await _context.DistanceObjects.ToListAsync();
        }

        // GET: api/DistanceObjects/5
        [HttpGet("{id}")]
        public async Task<ActionResult<DistanceObject>> GetDistanceObject(string id)
        {
            var distanceObject = await _context.DistanceObjects.FindAsync(id);

            if (distanceObject == null)
            {
                return NotFound();
            }

            return distanceObject;
        }

        // PUT: api/DistanceObjects/5
        // To protect from overposting attacks, see https://go.microsoft.com/fwlink/?linkid=2123754
        [HttpPut("{id}")]
        public async Task<IActionResult> PutDistanceObject(string id, DistanceObject distanceObject)
        {
            if (id != distanceObject.Id)
            {
                return BadRequest();
            }

            _context.Entry(distanceObject).State = EntityState.Modified;

            try
            {
                await _context.SaveChangesAsync();
            }
            catch (DbUpdateConcurrencyException)
            {
                if (!DistanceObjectExists(id))
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

        // POST: api/DistanceObjects
        // To protect from overposting attacks, see https://go.microsoft.com/fwlink/?linkid=2123754
        [HttpPost]
        public async Task<ActionResult<DistanceObject>> PostDistanceObject(DistanceObject distanceObject)
        {
            _context.DistanceObjects.Add(distanceObject);
            try
            {
                await _context.SaveChangesAsync();
            }
            catch (DbUpdateException)
            {
                if (DistanceObjectExists(distanceObject.Id))
                {
                    return Conflict();
                }
                else
                {
                    throw;
                }
            }

            return CreatedAtAction("GetDistanceObject", new { id = distanceObject.Id }, distanceObject);
        }

        // DELETE: api/DistanceObjects/5
        [HttpDelete("{id}")]
        public async Task<IActionResult> DeleteDistanceObject(string id)
        {
            var distanceObject = await _context.DistanceObjects.FindAsync(id);
            if (distanceObject == null)
            {
                return NotFound();
            }

            _context.DistanceObjects.Remove(distanceObject);
            await _context.SaveChangesAsync();

            return NoContent();
        }

        private bool DistanceObjectExists(string id)
        {
            return _context.DistanceObjects.Any(e => e.Id == id);
        }
    }
}
