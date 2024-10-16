using System;
using Microsoft.EntityFrameworkCore.Migrations;
using Npgsql.EntityFrameworkCore.PostgreSQL.Metadata;

#nullable disable

namespace API.Migrations
{
    /// <inheritdoc />
    public partial class init : Migration
    {
        /// <inheritdoc />
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.CreateTable(
                name: "SitSmartDevice",
                columns: table => new
                {
                    id = table.Column<int>(type: "integer", nullable: false)
                        .Annotation("Npgsql:ValueGenerationStrategy", NpgsqlValueGenerationStrategy.IdentityByDefaultColumn),
                    CreatedAt = table.Column<DateTime>(type: "timestamp with time zone", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_SitSmartDevice", x => x.id);
                });

            migrationBuilder.CreateTable(
                name: "DistanceObjects",
                columns: table => new
                {
                    id = table.Column<int>(type: "integer", nullable: false)
                        .Annotation("Npgsql:ValueGenerationStrategy", NpgsqlValueGenerationStrategy.IdentityByDefaultColumn),
                    length = table.Column<int>(type: "integer", nullable: false),
                    SitSmartDeviceid = table.Column<int>(type: "integer", nullable: true),
                    CreatedAt = table.Column<DateTime>(type: "timestamp with time zone", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_DistanceObjects", x => x.id);
                    table.ForeignKey(
                        name: "FK_DistanceObjects_SitSmartDevice_SitSmartDeviceid",
                        column: x => x.SitSmartDeviceid,
                        principalTable: "SitSmartDevice",
                        principalColumn: "id");
                });

            migrationBuilder.CreateTable(
                name: "MovementObjects",
                columns: table => new
                {
                    id = table.Column<int>(type: "integer", nullable: false)
                        .Annotation("Npgsql:ValueGenerationStrategy", NpgsqlValueGenerationStrategy.IdentityByDefaultColumn),
                    xValue = table.Column<float>(type: "real", nullable: false),
                    yValue = table.Column<float>(type: "real", nullable: false),
                    zValue = table.Column<float>(type: "real", nullable: false),
                    SitSmartDeviceid = table.Column<int>(type: "integer", nullable: true),
                    CreatedAt = table.Column<DateTime>(type: "timestamp with time zone", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_MovementObjects", x => x.id);
                    table.ForeignKey(
                        name: "FK_MovementObjects_SitSmartDevice_SitSmartDeviceid",
                        column: x => x.SitSmartDeviceid,
                        principalTable: "SitSmartDevice",
                        principalColumn: "id");
                });

            migrationBuilder.CreateTable(
                name: "TempHumidityObjects",
                columns: table => new
                {
                    id = table.Column<int>(type: "integer", nullable: false)
                        .Annotation("Npgsql:ValueGenerationStrategy", NpgsqlValueGenerationStrategy.IdentityByDefaultColumn),
                    temp = table.Column<int>(type: "integer", nullable: false),
                    humidity = table.Column<int>(type: "integer", nullable: false),
                    SitSmartDeviceid = table.Column<int>(type: "integer", nullable: true),
                    CreatedAt = table.Column<DateTime>(type: "timestamp with time zone", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_TempHumidityObjects", x => x.id);
                    table.ForeignKey(
                        name: "FK_TempHumidityObjects_SitSmartDevice_SitSmartDeviceid",
                        column: x => x.SitSmartDeviceid,
                        principalTable: "SitSmartDevice",
                        principalColumn: "id");
                });

            migrationBuilder.CreateTable(
                name: "Users",
                columns: table => new
                {
                    id = table.Column<int>(type: "integer", nullable: false)
                        .Annotation("Npgsql:ValueGenerationStrategy", NpgsqlValueGenerationStrategy.IdentityByDefaultColumn),
                    email = table.Column<string>(type: "text", nullable: false),
                    password = table.Column<string>(type: "text", nullable: false),
                    realPassword = table.Column<string>(type: "text", nullable: false),
                    salt = table.Column<string>(type: "text", nullable: false),
                    SitSmartDeviceid = table.Column<int>(type: "integer", nullable: true),
                    CreatedAt = table.Column<DateTime>(type: "timestamp with time zone", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Users", x => x.id);
                    table.ForeignKey(
                        name: "FK_Users_SitSmartDevice_SitSmartDeviceid",
                        column: x => x.SitSmartDeviceid,
                        principalTable: "SitSmartDevice",
                        principalColumn: "id");
                });

            migrationBuilder.CreateIndex(
                name: "IX_DistanceObjects_SitSmartDeviceid",
                table: "DistanceObjects",
                column: "SitSmartDeviceid");

            migrationBuilder.CreateIndex(
                name: "IX_MovementObjects_SitSmartDeviceid",
                table: "MovementObjects",
                column: "SitSmartDeviceid");

            migrationBuilder.CreateIndex(
                name: "IX_TempHumidityObjects_SitSmartDeviceid",
                table: "TempHumidityObjects",
                column: "SitSmartDeviceid");

            migrationBuilder.CreateIndex(
                name: "IX_Users_SitSmartDeviceid",
                table: "Users",
                column: "SitSmartDeviceid");
        }

        /// <inheritdoc />
        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropTable(
                name: "DistanceObjects");

            migrationBuilder.DropTable(
                name: "MovementObjects");

            migrationBuilder.DropTable(
                name: "TempHumidityObjects");

            migrationBuilder.DropTable(
                name: "Users");

            migrationBuilder.DropTable(
                name: "SitSmartDevice");
        }
    }
}
