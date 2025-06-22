const app = @cImport({
    @cInclude("app.h");
});

pub fn main() !void {
    _ = app.RunApp();
}
