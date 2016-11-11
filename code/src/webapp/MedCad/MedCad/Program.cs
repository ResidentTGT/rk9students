using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Hosting;
using Microsoft.Extensions.Configuration;

namespace MedCad
{
    public class Program
    {
        public static void Main(string[] args)
        {
            var config = new ConfigurationBuilder()
                .AddCommandLine(args)
                .AddEnvironmentVariables(prefix: "ASPNETCORE_")
                .Build();

            var host = new WebHostBuilder()
                .UseConfiguration(config)
                .UseKestrel()
                .UseContentRoot(Directory.GetCurrentDirectory())
                .UseIISIntegration()
                .UseStartup<Startup>()
                .Build();


            var builder = new ConfigurationBuilder();

            var configuration = builder.Build();

            string connectionString = "server=localhost;userid=root;pwd=20101995;port=3306;database=Medbox;sslmode=none;";

            // Create an employee instance and save the entity to the database
            var entry = new Model() { Id = 1, Data = "sdfdfs", CreationDate = "2015", UserId = 24 };

            using (var context = ModelContextFactory.Create(connectionString))
            {
                context.Add(entry);
                context.SaveChanges();
            }

            Console.WriteLine($"Employee was saved in the database with id: {entry.id}");

            host.Run();
        }
    }
}
