const std = @import("std");
const this = @This();
const rl = @import("raylib");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const raylib = b.dependency("raylib", .{
        .target = target,
        .optimize = optimize,
        .linux_display_backend = .X11,
    });

    const raylib_artifact = raylib.artifact("raylib");

    const exe_mod = b.createModule(.{
        .root_source_file = b.path("src/main.zig"),
        .target = target,
        .optimize = optimize,
    });

    const exe = b.addExecutable(.{
        .name = "bauripixel",
        .root_module = exe_mod,
    });

    if (optimize == .Debug and target.result.os.tag != .windows) {
        exe.use_lld = false;
    }

    if (target.result.os.tag == .windows) {
        exe_mod.addCMacro("BPM_OS_WIN", "");
        exe_mod.linkSystemLibrary("comdlg32", .{});
        exe_mod.linkSystemLibrary("ole32", .{});
    } else if (target.result.os.tag == .linux) {
        exe_mod.addCMacro("BPM_OS_LINUX", "");
    } else if (target.result.os.tag == .macos) {
        exe_mod.addCMacro("BPM_OS_WIN", "");
    }

    exe.addIncludePath(b.path("src/include/"));
    exe.addIncludePath(b.path("src/external/"));
    exe.addIncludePath(b.path("src/external/stb"));
    //exe.addIncludePath(b.path("src/external/tinyfiledialogs/"));

    exe.addCSourceFiles(.{
        .files = &.{
            "src/widgets/statusbar.c",
            "src/widgets/colorbar.c",
            "src/widgets/canvas.c",
            "src/widgets/menubar.c",
            "src/widgets/widget.c",
            "src/widgets/drawtoolbar.c",
            "src/widgets/layerbar.c",
            "src/windows/layeropts.c",
            "src/windows/newsprite.c",
            "src/windows/window.c",
            "src/components/slider.c",
            "src/components/panels.c",
            "src/components/button.c",
            "src/components/winpanels.c",
            "src/canvas/draw.c",
            "src/options/opttheme.c",
            "src/layers.c",
            "src/tabs.c",
            "src/utils.c",
            "src/gui.c",
            "src/app.c",
        },
        .flags = &.{"-g3"},
    });

    exe_mod.linkLibrary(raylib_artifact);
    exe_mod.link_libc = true;
    exe.linkLibC();
    exe.linkLibrary(raylib_artifact);

    b.installArtifact(exe);

    const run_cmd = b.addRunArtifact(exe);

    run_cmd.step.dependOn(b.getInstallStep());

    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);
}
