using Microsoft.EntityFrameworkCore;
using MySQL.Data.EntityFrameworkCore.Extensions;


public class ModelContext : DbContext
{
    public ModelContext(DbContextOptions<ModelContext> options)
    : base(options)
    { }

    public DbSet<Model> Models { get; set; }
}

public static class ModelContextFactory
{
    public static ModelContext Create(string connectionString)
    {
        var optionsBuilder = new DbContextOptionsBuilder<ModelContext>();
        optionsBuilder.UseMySQL(connectionString);
        
        var context = new ModelContext(optionsBuilder.Options);
        context.Database.EnsureCreated();

        return context;
    }
}

public class Model
{
    public Model()
    {
    }

    public int Id { get; set; }

    public int UserId { get; set; }

    public string Data { get; set; }

    public string CreationDate { get; set; }
}