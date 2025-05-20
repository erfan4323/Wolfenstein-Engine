add_rules("mode.debug", "mode.release")

add_requires("libsdl2", "stb")

target("raycast")
    set_kind("binary")
    set_languages("c99")
    add_files("src/*.c")
    add_packages("libsdl2", "stb")
    after_build(function (target)
        local src_dir = path.join(os.projectdir(), "images")
        local dst_dir = path.join(target:targetdir(), "images")
        os.cp(src_dir, dst_dir)
    end)
