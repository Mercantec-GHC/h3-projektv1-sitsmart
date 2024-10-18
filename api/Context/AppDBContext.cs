using API.Models;
using Microsoft.EntityFrameworkCore;
using System.Security.Cryptography.X509Certificates;

public class AppDBContext : DbContext
{
    public AppDBContext(DbContextOptions<AppDBContext> options)
        : base(options)
    {
    }

    public DbSet<User> Users { get; set; }
    public DbSet<TempHumidity> TempHumidityObjects { get; set; }
    public DbSet<Movement> MovementObjects {  get; set; }
    public DbSet<DistanceObject> DistanceObjects { get; set; }
    public DbSet<SitSmartDevice> sitSmartDevices { get; set; }

public DbSet<API.Models.SitSmartDevice> SitSmartDevice { get; set; } = default!;

public DbSet<API.Models.UserSitSmart> UserSitSmart { get; set; } = default!;
}