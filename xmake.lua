add_rules("mode.debug", "mode.release")

add_requires("libsdl2")

target("raycast")
    set_kind("binary")
    set_languages("c99")
    add_files("src/*.c")
    add_packages("libsdl2")
