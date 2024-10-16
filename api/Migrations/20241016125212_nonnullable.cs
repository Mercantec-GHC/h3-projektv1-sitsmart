using Microsoft.EntityFrameworkCore.Migrations;

#nullable disable

namespace API.Migrations
{
    /// <inheritdoc />
    public partial class nonnullable : Migration
    {
        /// <inheritdoc />
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropForeignKey(
                name: "FK_DistanceObjects_SitSmartDevice_SitSmartDeviceid",
                table: "DistanceObjects");

            migrationBuilder.DropForeignKey(
                name: "FK_MovementObjects_SitSmartDevice_SitSmartDeviceid",
                table: "MovementObjects");

            migrationBuilder.DropForeignKey(
                name: "FK_TempHumidityObjects_SitSmartDevice_SitSmartDeviceid",
                table: "TempHumidityObjects");

            migrationBuilder.DropForeignKey(
                name: "FK_Users_SitSmartDevice_SitSmartDeviceid",
                table: "Users");

            migrationBuilder.DropIndex(
                name: "IX_Users_SitSmartDeviceid",
                table: "Users");

            migrationBuilder.DropColumn(
                name: "SitSmartDeviceid",
                table: "Users");

            migrationBuilder.RenameColumn(
                name: "salt",
                table: "Users",
                newName: "Salt");

            migrationBuilder.RenameColumn(
                name: "realPassword",
                table: "Users",
                newName: "RealPassword");

            migrationBuilder.RenameColumn(
                name: "password",
                table: "Users",
                newName: "Password");

            migrationBuilder.RenameColumn(
                name: "email",
                table: "Users",
                newName: "Email");

            migrationBuilder.RenameColumn(
                name: "id",
                table: "Users",
                newName: "Id");

            migrationBuilder.RenameColumn(
                name: "temp",
                table: "TempHumidityObjects",
                newName: "Temp");

            migrationBuilder.RenameColumn(
                name: "humidity",
                table: "TempHumidityObjects",
                newName: "Humidity");

            migrationBuilder.RenameColumn(
                name: "SitSmartDeviceid",
                table: "TempHumidityObjects",
                newName: "SitSmartDeviceId");

            migrationBuilder.RenameColumn(
                name: "id",
                table: "TempHumidityObjects",
                newName: "Id");

            migrationBuilder.RenameIndex(
                name: "IX_TempHumidityObjects_SitSmartDeviceid",
                table: "TempHumidityObjects",
                newName: "IX_TempHumidityObjects_SitSmartDeviceId");

            migrationBuilder.RenameColumn(
                name: "id",
                table: "SitSmartDevice",
                newName: "Id");

            migrationBuilder.RenameColumn(
                name: "zValue",
                table: "MovementObjects",
                newName: "ZValue");

            migrationBuilder.RenameColumn(
                name: "yValue",
                table: "MovementObjects",
                newName: "YValue");

            migrationBuilder.RenameColumn(
                name: "xValue",
                table: "MovementObjects",
                newName: "XValue");

            migrationBuilder.RenameColumn(
                name: "SitSmartDeviceid",
                table: "MovementObjects",
                newName: "SitSmartDeviceId");

            migrationBuilder.RenameColumn(
                name: "id",
                table: "MovementObjects",
                newName: "Id");

            migrationBuilder.RenameIndex(
                name: "IX_MovementObjects_SitSmartDeviceid",
                table: "MovementObjects",
                newName: "IX_MovementObjects_SitSmartDeviceId");

            migrationBuilder.RenameColumn(
                name: "length",
                table: "DistanceObjects",
                newName: "Length");

            migrationBuilder.RenameColumn(
                name: "SitSmartDeviceid",
                table: "DistanceObjects",
                newName: "SitSmartDeviceId");

            migrationBuilder.RenameColumn(
                name: "id",
                table: "DistanceObjects",
                newName: "Id");

            migrationBuilder.RenameIndex(
                name: "IX_DistanceObjects_SitSmartDeviceid",
                table: "DistanceObjects",
                newName: "IX_DistanceObjects_SitSmartDeviceId");

            migrationBuilder.AddColumn<string>(
                name: "Name",
                table: "Users",
                type: "text",
                nullable: false,
                defaultValue: "");

            migrationBuilder.AddForeignKey(
                name: "FK_DistanceObjects_SitSmartDevice_SitSmartDeviceId",
                table: "DistanceObjects",
                column: "SitSmartDeviceId",
                principalTable: "SitSmartDevice",
                principalColumn: "Id");

            migrationBuilder.AddForeignKey(
                name: "FK_MovementObjects_SitSmartDevice_SitSmartDeviceId",
                table: "MovementObjects",
                column: "SitSmartDeviceId",
                principalTable: "SitSmartDevice",
                principalColumn: "Id");

            migrationBuilder.AddForeignKey(
                name: "FK_TempHumidityObjects_SitSmartDevice_SitSmartDeviceId",
                table: "TempHumidityObjects",
                column: "SitSmartDeviceId",
                principalTable: "SitSmartDevice",
                principalColumn: "Id");
        }

        /// <inheritdoc />
        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropForeignKey(
                name: "FK_DistanceObjects_SitSmartDevice_SitSmartDeviceId",
                table: "DistanceObjects");

            migrationBuilder.DropForeignKey(
                name: "FK_MovementObjects_SitSmartDevice_SitSmartDeviceId",
                table: "MovementObjects");

            migrationBuilder.DropForeignKey(
                name: "FK_TempHumidityObjects_SitSmartDevice_SitSmartDeviceId",
                table: "TempHumidityObjects");

            migrationBuilder.DropColumn(
                name: "Name",
                table: "Users");

            migrationBuilder.RenameColumn(
                name: "Salt",
                table: "Users",
                newName: "salt");

            migrationBuilder.RenameColumn(
                name: "RealPassword",
                table: "Users",
                newName: "realPassword");

            migrationBuilder.RenameColumn(
                name: "Password",
                table: "Users",
                newName: "password");

            migrationBuilder.RenameColumn(
                name: "Email",
                table: "Users",
                newName: "email");

            migrationBuilder.RenameColumn(
                name: "Id",
                table: "Users",
                newName: "id");

            migrationBuilder.RenameColumn(
                name: "Temp",
                table: "TempHumidityObjects",
                newName: "temp");

            migrationBuilder.RenameColumn(
                name: "SitSmartDeviceId",
                table: "TempHumidityObjects",
                newName: "SitSmartDeviceid");

            migrationBuilder.RenameColumn(
                name: "Humidity",
                table: "TempHumidityObjects",
                newName: "humidity");

            migrationBuilder.RenameColumn(
                name: "Id",
                table: "TempHumidityObjects",
                newName: "id");

            migrationBuilder.RenameIndex(
                name: "IX_TempHumidityObjects_SitSmartDeviceId",
                table: "TempHumidityObjects",
                newName: "IX_TempHumidityObjects_SitSmartDeviceid");

            migrationBuilder.RenameColumn(
                name: "Id",
                table: "SitSmartDevice",
                newName: "id");

            migrationBuilder.RenameColumn(
                name: "ZValue",
                table: "MovementObjects",
                newName: "zValue");

            migrationBuilder.RenameColumn(
                name: "YValue",
                table: "MovementObjects",
                newName: "yValue");

            migrationBuilder.RenameColumn(
                name: "XValue",
                table: "MovementObjects",
                newName: "xValue");

            migrationBuilder.RenameColumn(
                name: "SitSmartDeviceId",
                table: "MovementObjects",
                newName: "SitSmartDeviceid");

            migrationBuilder.RenameColumn(
                name: "Id",
                table: "MovementObjects",
                newName: "id");

            migrationBuilder.RenameIndex(
                name: "IX_MovementObjects_SitSmartDeviceId",
                table: "MovementObjects",
                newName: "IX_MovementObjects_SitSmartDeviceid");

            migrationBuilder.RenameColumn(
                name: "SitSmartDeviceId",
                table: "DistanceObjects",
                newName: "SitSmartDeviceid");

            migrationBuilder.RenameColumn(
                name: "Length",
                table: "DistanceObjects",
                newName: "length");

            migrationBuilder.RenameColumn(
                name: "Id",
                table: "DistanceObjects",
                newName: "id");

            migrationBuilder.RenameIndex(
                name: "IX_DistanceObjects_SitSmartDeviceId",
                table: "DistanceObjects",
                newName: "IX_DistanceObjects_SitSmartDeviceid");

            migrationBuilder.AddColumn<int>(
                name: "SitSmartDeviceid",
                table: "Users",
                type: "integer",
                nullable: true);

            migrationBuilder.CreateIndex(
                name: "IX_Users_SitSmartDeviceid",
                table: "Users",
                column: "SitSmartDeviceid");

            migrationBuilder.AddForeignKey(
                name: "FK_DistanceObjects_SitSmartDevice_SitSmartDeviceid",
                table: "DistanceObjects",
                column: "SitSmartDeviceid",
                principalTable: "SitSmartDevice",
                principalColumn: "id");

            migrationBuilder.AddForeignKey(
                name: "FK_MovementObjects_SitSmartDevice_SitSmartDeviceid",
                table: "MovementObjects",
                column: "SitSmartDeviceid",
                principalTable: "SitSmartDevice",
                principalColumn: "id");

            migrationBuilder.AddForeignKey(
                name: "FK_TempHumidityObjects_SitSmartDevice_SitSmartDeviceid",
                table: "TempHumidityObjects",
                column: "SitSmartDeviceid",
                principalTable: "SitSmartDevice",
                principalColumn: "id");

            migrationBuilder.AddForeignKey(
                name: "FK_Users_SitSmartDevice_SitSmartDeviceid",
                table: "Users",
                column: "SitSmartDeviceid",
                principalTable: "SitSmartDevice",
                principalColumn: "id");
        }
    }
}
