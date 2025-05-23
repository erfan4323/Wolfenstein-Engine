add_rules("mode.debug", "mode.release")

add_requires("libsdl2", "stb")

function add_asset_folders(folders)
    after_build(function (target)
        for _, foldername in ipairs(folders) do
            local src_dir = path.join(os.projectdir(), foldername)
            local dst_dir = path.join(target:targetdir(), foldername)
            os.cp(src_dir, dst_dir)
        end
    end)
end

target("raycast")
    set_kind("binary")
    set_languages("c99")
    add_files("src/*.c")
    add_packages("libsdl2", "stb")
    add_asset_folders({"images"})
