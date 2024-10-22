﻿using System;
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
    public class SitSmartDevicesController : ControllerBase
    {
        private readonly AppDBContext _context;

        public SitSmartDevicesController(AppDBContext context)
        {
            _context = context;
        }

        // GET: api/SitSmartDevices
        [HttpGet]
        public async Task<ActionResult<IEnumerable<SitSmartDevice>>> GetsitSmartDevices()
        {
            return await _context.sitSmartDevices.ToListAsync();
        }

        // GET: api/SitSmartDevices/5
        [HttpGet("{id}")]
        public async Task<ActionResult<SitSmartDevice>> GetSitSmartDevice(string id)
        {
            var sitSmartDevice = await _context.sitSmartDevices.FindAsync(id);

            if (sitSmartDevice == null)
            {
                return NotFound();
            }

            return sitSmartDevice;
        }

        // PUT: api/SitSmartDevices/5
        // To protect from overposting attacks, see https://go.microsoft.com/fwlink/?linkid=2123754
        [HttpPut("{id}")]
        public async Task<IActionResult> PutSitSmartDevice(string id, SitSmartDevice sitSmartDevice)
        {
            if (id != sitSmartDevice.Id)
            {
                return BadRequest();
            }

            _context.Entry(sitSmartDevice).State = EntityState.Modified;

            try
            {
                await _context.SaveChangesAsync();
            }
            catch (DbUpdateConcurrencyException)
            {
                if (!SitSmartDeviceExists(id))
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

        // POST: api/SitSmartDevices
        // To protect from overposting attacks, see https://go.microsoft.com/fwlink/?linkid=2123754
        [HttpPost]
        public async Task<ActionResult<SitSmartDevice>> PostSitSmartDevice(sitSmartDeviceDTO newSitSmart)
        {
            var sitSmartDevice = MapDTOToSitSmartDevice(newSitSmart);

            _context.sitSmartDevices.Add(sitSmartDevice);
            try
            {
                await _context.SaveChangesAsync();
            }
            catch (DbUpdateException)
            {
                if (SitSmartDeviceExists(sitSmartDevice.Id))
                {
                    return Conflict();
                }
                else
                {
                    throw;
                }
            }

            return CreatedAtAction("GetSitSmartDevice", new { id = sitSmartDevice.Id }, sitSmartDevice);
        }

        // DELETE: api/SitSmartDevices/5
        [HttpDelete("{id}")]
        public async Task<IActionResult> DeleteSitSmartDevice(string id)
        {
            var sitSmartDevice = await _context.sitSmartDevices.FindAsync(id);
            if (sitSmartDevice == null)
            {
                return NotFound();
            }

            _context.sitSmartDevices.Remove(sitSmartDevice);
            await _context.SaveChangesAsync();

            return NoContent();
        }

        private bool SitSmartDeviceExists(string id)
        {
            return _context.sitSmartDevices.Any(e => e.Id == id);
        }
        private SitSmartDevice MapDTOToSitSmartDevice(sitSmartDeviceDTO dto)
        {
            return new SitSmartDevice
            {
                Id = Guid.NewGuid().ToString("N"),
            };
        }
    }
}
