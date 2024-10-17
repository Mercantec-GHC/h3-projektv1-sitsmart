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
                    Id = table.Column<string>(type: "text", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_SitSmartDevice", x => x.Id);
                });

            migrationBuilder.CreateTable(
                name: "Users",
                columns: table => new
                {
                    Id = table.Column<string>(type: "text", nullable: false),
                    Email = table.Column<string>(type: "text", nullable: false),
                    Name = table.Column<string>(type: "text", nullable: false),
                    Password = table.Column<string>(type: "text", nullable: false),
                    RealPassword = table.Column<string>(type: "text", nullable: false),
                    Salt = table.Column<string>(type: "text", nullable: false),
                    LastLogin = table.Column<DateTime>(type: "timestamp with time zone", nullable: false),
                    CreatedAt = table.Column<DateTime>(type: "timestamp with time zone", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Users", x => x.Id);
                });

            migrationBuilder.CreateTable(
                name: "DistanceObjects",
                columns: table => new
                {
                    Id = table.Column<string>(type: "text", nullable: false),
                    Length = table.Column<int>(type: "integer", nullable: false),
                    sitSmartDeviceId = table.Column<string>(type: "text", nullable: false),
                    CreatedAt = table.Column<DateTime>(type: "timestamp with time zone", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_DistanceObjects", x => x.Id);
                    table.ForeignKey(
                        name: "FK_DistanceObjects_SitSmartDevice_sitSmartDeviceId",
                        column: x => x.sitSmartDeviceId,
                        principalTable: "SitSmartDevice",
                        principalColumn: "Id",
                        onDelete: ReferentialAction.Cascade);
                });

            migrationBuilder.CreateTable(
                name: "MovementObjects",
                columns: table => new
                {
                    Id = table.Column<string>(type: "text", nullable: false),
                    XValue = table.Column<float>(type: "real", nullable: false),
                    YValue = table.Column<float>(type: "real", nullable: false),
                    ZValue = table.Column<float>(type: "real", nullable: false),
                    sitSmartDeviceId = table.Column<string>(type: "text", nullable: false),
                    CreatedAt = table.Column<DateTime>(type: "timestamp with time zone", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_MovementObjects", x => x.Id);
                    table.ForeignKey(
                        name: "FK_MovementObjects_SitSmartDevice_sitSmartDeviceId",
                        column: x => x.sitSmartDeviceId,
                        principalTable: "SitSmartDevice",
                        principalColumn: "Id",
                        onDelete: ReferentialAction.Cascade);
                });

            migrationBuilder.CreateTable(
                name: "TempHumidityObjects",
                columns: table => new
                {
                    Id = table.Column<string>(type: "text", nullable: false),
                    Temp = table.Column<int>(type: "integer", nullable: false),
                    Humidity = table.Column<int>(type: "integer", nullable: false),
                    sitSmartDeviceId = table.Column<string>(type: "text", nullable: false),
                    CreatedAt = table.Column<DateTime>(type: "timestamp with time zone", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_TempHumidityObjects", x => x.Id);
                    table.ForeignKey(
                        name: "FK_TempHumidityObjects_SitSmartDevice_sitSmartDeviceId",
                        column: x => x.sitSmartDeviceId,
                        principalTable: "SitSmartDevice",
                        principalColumn: "Id",
                        onDelete: ReferentialAction.Cascade);
                });

            migrationBuilder.CreateTable(
                name: "UserSitSmart",
                columns: table => new
                {
                    Id = table.Column<int>(type: "integer", nullable: false)
                        .Annotation("Npgsql:ValueGenerationStrategy", NpgsqlValueGenerationStrategy.IdentityByDefaultColumn),
                    SitSmartDeviceId = table.Column<string>(type: "text", nullable: true),
                    UserId1 = table.Column<string>(type: "text", nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_UserSitSmart", x => x.Id);
                    table.ForeignKey(
                        name: "FK_UserSitSmart_SitSmartDevice_SitSmartDeviceId",
                        column: x => x.SitSmartDeviceId,
                        principalTable: "SitSmartDevice",
                        principalColumn: "Id");
                    table.ForeignKey(
                        name: "FK_UserSitSmart_Users_UserId1",
                        column: x => x.UserId1,
                        principalTable: "Users",
                        principalColumn: "Id");
                });

            migrationBuilder.CreateIndex(
                name: "IX_DistanceObjects_sitSmartDeviceId",
                table: "DistanceObjects",
                column: "sitSmartDeviceId");

            migrationBuilder.CreateIndex(
                name: "IX_MovementObjects_sitSmartDeviceId",
                table: "MovementObjects",
                column: "sitSmartDeviceId");

            migrationBuilder.CreateIndex(
                name: "IX_TempHumidityObjects_sitSmartDeviceId",
                table: "TempHumidityObjects",
                column: "sitSmartDeviceId");

            migrationBuilder.CreateIndex(
                name: "IX_UserSitSmart_SitSmartDeviceId",
                table: "UserSitSmart",
                column: "SitSmartDeviceId");

            migrationBuilder.CreateIndex(
                name: "IX_UserSitSmart_UserId1",
                table: "UserSitSmart",
                column: "UserId1");
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
                name: "UserSitSmart");

            migrationBuilder.DropTable(
                name: "SitSmartDevice");

            migrationBuilder.DropTable(
                name: "Users");
        }
    }
}
