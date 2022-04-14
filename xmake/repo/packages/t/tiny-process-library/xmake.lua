package("tiny-process-library")
    set_homepage("https://gitlab.com/eidheim/tiny-process-library")
    set_description("A small platform independent library making it simple to create and stop new processes in C++, as well as writing to stdin and reading from stdout and stderr of a new process.")
    set_license("MIT License")

    add_urls("https://gitlab.com/eidheim/tiny-process-library/-/archive/$(version)/tiny-process-library-$(version).tar.gz",
             "https://gitlab.com/eidheim/tiny-process-library.git")

    add_versions("v2.0.4", "b99dcb51461323b8284a7762ad105c159b88cdcce0c2cc183e4f474f80ef1f1a")

    on_install("windows|x64", "mingw|x86_64", "macosx|x86_64", "linux|x86_64", "linux|arm64", "iphoneos|arm64", function(package)
        local configs = {}
        if package:config("shared") then
            configs.kind = "shared"
        end

        os.cp(path.join(package:scriptdir(), "port", "xmake.lua"), "xmake.lua")
        import("package.tools.xmake").install(package, configs)
    end)
