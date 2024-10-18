using Microsoft.EntityFrameworkCore.Migrations;

#nullable disable

namespace API.Migrations
{
    /// <inheritdoc />
    public partial class updatedvariables : Migration
    {
        /// <inheritdoc />
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.RenameColumn(
                name: "Length",
                table: "DistanceObjects",
                newName: "Distance");

            migrationBuilder.AlterColumn<int>(
                name: "ZValue",
                table: "MovementObjects",
                type: "integer",
                nullable: false,
                oldClrType: typeof(float),
                oldType: "real");

            migrationBuilder.AlterColumn<int>(
                name: "YValue",
                table: "MovementObjects",
                type: "integer",
                nullable: false,
                oldClrType: typeof(float),
                oldType: "real");

            migrationBuilder.AlterColumn<int>(
                name: "XValue",
                table: "MovementObjects",
                type: "integer",
                nullable: false,
                oldClrType: typeof(float),
                oldType: "real");
        }

        /// <inheritdoc />
        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.RenameColumn(
                name: "Distance",
                table: "DistanceObjects",
                newName: "Length");

            migrationBuilder.AlterColumn<float>(
                name: "ZValue",
                table: "MovementObjects",
                type: "real",
                nullable: false,
                oldClrType: typeof(int),
                oldType: "integer");

            migrationBuilder.AlterColumn<float>(
                name: "YValue",
                table: "MovementObjects",
                type: "real",
                nullable: false,
                oldClrType: typeof(int),
                oldType: "integer");

            migrationBuilder.AlterColumn<float>(
                name: "XValue",
                table: "MovementObjects",
                type: "real",
                nullable: false,
                oldClrType: typeof(int),
                oldType: "integer");
        }
    }
}
