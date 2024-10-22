using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using API.Models;
using Microsoft.AspNetCore.Http.HttpResults;

namespace API.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class TempHumiditiesController : ControllerBase
    {
        private readonly AppDBContext _context;

        public TempHumiditiesController(AppDBContext context)
        {
            _context = context;
        }

        // GET: api/TempHumidities
        [HttpGet]
        public async Task<ActionResult<IEnumerable<TempHumidity>>> GetTempHumidityObjects()
        {
            return await _context.TempHumidityObjects.ToListAsync();
        }

        // GET: api/TempHumidities/5
        [HttpGet("{id}")]
        public async Task<ActionResult<TempHumidity>> GetTempHumidity(string id)
        {
            var tempHumidity = await _context.TempHumidityObjects.FindAsync(id);

            if (tempHumidity == null)
            {
                return NotFound();
            }

            return tempHumidity;
        }

        // PUT: api/TempHumidities/5
        // To protect from overposting attacks, see https://go.microsoft.com/fwlink/?linkid=2123754
        [HttpPut("{id}")]
        public async Task<IActionResult> PutTempHumidity(string id, TempHumidity tempHumidity)
        {
            if (id != tempHumidity.Id)
            {
                return BadRequest();
            }

            _context.Entry(tempHumidity).State = EntityState.Modified;

            try
            {
                await _context.SaveChangesAsync();
            }
            catch (DbUpdateConcurrencyException)
            {
                if (!TempHumidityExists(id))
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

        // POST: api/TempHumidities
        // To protect from overposting attacks, see https://go.microsoft.com/fwlink/?linkid=2123754
        [HttpPost]
        public async Task<ActionResult<TempHumidity>> PostTempHumidity(PostTempHumidDTO newTempHumidity)
        {
            var tempHumidity = MapDTOToTempHumidity(newTempHumidity);

            _context.TempHumidityObjects.Add(tempHumidity);
            try
            {
                await _context.SaveChangesAsync();
            }
            catch (DbUpdateException)
            {
                if (TempHumidityExists(tempHumidity.Id))
                {
                    return Conflict();
                }
                else
                {
                    throw;
                }
            }

            return CreatedAtAction("GetTempHumidity", new { id = tempHumidity.Id }, tempHumidity);
        }

        // DELETE: api/TempHumidities/5
        [HttpDelete("{id}")]
        public async Task<IActionResult> DeleteTempHumidity(string id)
        {
            var tempHumidity = await _context.TempHumidityObjects.FindAsync(id);
            if (tempHumidity == null)
            {
                return NotFound();
            }

            _context.TempHumidityObjects.Remove(tempHumidity);
            await _context.SaveChangesAsync();

            return NoContent();
        }

        private bool TempHumidityExists(string id)
        {
            return _context.TempHumidityObjects.Any(e => e.Id == id);
        }
        private TempHumidity MapDTOToTempHumidity(PostTempHumidDTO dto)
        {
            return new TempHumidity
            {
                Id = Guid.NewGuid().ToString("N"),
                Temp = dto.Temp, 
                Humidity = dto.Humidity,
                CreatedAt = DateTime.UtcNow.AddHours(2),
                sitSmartDeviceId = dto.sitSmartDeviceId
            };
        } 
    }
}
