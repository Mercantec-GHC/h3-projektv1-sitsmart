using Microsoft.EntityFrameworkCore.Migrations;

#nullable disable

namespace API.Migrations
{
    /// <inheritdoc />
    public partial class updatedUserSitSmart : Migration
    {
        /// <inheritdoc />
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropTable(
                name: "SitSmartDeviceUserSitSmart");

            migrationBuilder.DropTable(
                name: "UserUserSitSmart");

            migrationBuilder.AddColumn<string>(
                name: "DeviceId",
                table: "UserSitSmart",
                type: "text",
                nullable: true);

            migrationBuilder.CreateIndex(
                name: "IX_UserSitSmart_DeviceId",
                table: "UserSitSmart",
                column: "DeviceId");

            migrationBuilder.CreateIndex(
                name: "IX_UserSitSmart_UserId",
                table: "UserSitSmart",
                column: "UserId");

            migrationBuilder.AddForeignKey(
                name: "FK_UserSitSmart_SitSmartDevice_DeviceId",
                table: "UserSitSmart",
                column: "DeviceId",
                principalTable: "SitSmartDevice",
                principalColumn: "Id");

            migrationBuilder.AddForeignKey(
                name: "FK_UserSitSmart_Users_UserId",
                table: "UserSitSmart",
                column: "UserId",
                principalTable: "Users",
                principalColumn: "Id",
                onDelete: ReferentialAction.Cascade);
        }

        /// <inheritdoc />
        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropForeignKey(
                name: "FK_UserSitSmart_SitSmartDevice_DeviceId",
                table: "UserSitSmart");

            migrationBuilder.DropForeignKey(
                name: "FK_UserSitSmart_Users_UserId",
                table: "UserSitSmart");

            migrationBuilder.DropIndex(
                name: "IX_UserSitSmart_DeviceId",
                table: "UserSitSmart");

            migrationBuilder.DropIndex(
                name: "IX_UserSitSmart_UserId",
                table: "UserSitSmart");

            migrationBuilder.DropColumn(
                name: "DeviceId",
                table: "UserSitSmart");

            migrationBuilder.CreateTable(
                name: "SitSmartDeviceUserSitSmart",
                columns: table => new
                {
                    DevicesId = table.Column<string>(type: "text", nullable: false),
                    UsersId = table.Column<int>(type: "integer", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_SitSmartDeviceUserSitSmart", x => new { x.DevicesId, x.UsersId });
                    table.ForeignKey(
                        name: "FK_SitSmartDeviceUserSitSmart_SitSmartDevice_DevicesId",
                        column: x => x.DevicesId,
                        principalTable: "SitSmartDevice",
                        principalColumn: "Id",
                        onDelete: ReferentialAction.Cascade);
                    table.ForeignKey(
                        name: "FK_SitSmartDeviceUserSitSmart_UserSitSmart_UsersId",
                        column: x => x.UsersId,
                        principalTable: "UserSitSmart",
                        principalColumn: "Id",
                        onDelete: ReferentialAction.Cascade);
                });

            migrationBuilder.CreateTable(
                name: "UserUserSitSmart",
                columns: table => new
                {
                    SitSmartsId = table.Column<int>(type: "integer", nullable: false),
                    UsersId = table.Column<string>(type: "text", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_UserUserSitSmart", x => new { x.SitSmartsId, x.UsersId });
                    table.ForeignKey(
                        name: "FK_UserUserSitSmart_UserSitSmart_SitSmartsId",
                        column: x => x.SitSmartsId,
                        principalTable: "UserSitSmart",
                        principalColumn: "Id",
                        onDelete: ReferentialAction.Cascade);
                    table.ForeignKey(
                        name: "FK_UserUserSitSmart_Users_UsersId",
                        column: x => x.UsersId,
                        principalTable: "Users",
                        principalColumn: "Id",
                        onDelete: ReferentialAction.Cascade);
                });

            migrationBuilder.CreateIndex(
                name: "IX_SitSmartDeviceUserSitSmart_UsersId",
                table: "SitSmartDeviceUserSitSmart",
                column: "UsersId");

            migrationBuilder.CreateIndex(
                name: "IX_UserUserSitSmart_UsersId",
                table: "UserUserSitSmart",
                column: "UsersId");
        }
    }
}
