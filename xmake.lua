add_rules("mode.debug", "mode.release")

set_languages("cxx20")
set_warnings("allextra")

add_repositories("local-repo xmake/repo")

add_requires("simdjson", { system = false })
add_requires("tiny-process-library", { system = false })

option("test")
    set_default(false)
    set_showmenu(true)
    set_description("Compile test program")

target("LaunchMyMC")
    set_kind("shared")

    add_files("src/**.cpp")
    add_includedirs("include", { public = true })

    add_packages("simdjson", { public = true })
    add_packages("tiny-process-library", { public = true })

    add_cxflags("-fPIC", { force = true })

if has_config("test") then
    target("test")
        set_kind("binary")
        add_files("test/**.cpp")
        add_deps("LaunchMyMC")
        add_packages("simdjson", { public = true })
end
