
using Microsoft.EntityFrameworkCore;
using Scalar.AspNetCore;
using System;

namespace API
{
    public class Program
    {
        public static void Main(string[] args)
        {
            var builder = WebApplication.CreateBuilder(args);

            // Add 

            builder.Services.AddControllers();
            // Learn more about configuring Swagger/OpenAPI at https://aka.ms/aspnetcore/swashbuckle
            builder.Services.AddEndpointsApiExplorer();
            builder.Services.AddSwaggerGen();

            IConfiguration Configuration = builder.Configuration;

            string connectionString = Configuration.GetConnectionString("DefaultConnection")
                                        ?? Environment.GetEnvironmentVariable("DefaultConnection");

            builder.Services.AddDbContext<AppDBContext>(options =>
            options.UseNpgsql(connectionString));

            var app = builder.Build();

            // Configure the HTTP request pipeline.
            if (app.Environment.IsDevelopment())
            {
                app.UseSwagger();
                app.UseSwaggerUI(c =>
                {
                    c.SwaggerEndpoint("/swagger/v1/swagger.json", "My API V1");
                });

                app.MapScalarApiReference(options =>
                {   
                    options
                        .WithDefaultHttpClient(ScalarTarget.C, ScalarClient.Libcurl)
                        .OpenApiRoutePattern = "/swagger/v1/swagger.json";
                });
            }

            app.UseHttpsRedirection();

            app.UseAuthorization();


            app.MapControllers();

            app.Run();
        }
    }
}
