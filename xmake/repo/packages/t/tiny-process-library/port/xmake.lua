add_rules("mode.debug", "mode.release")

target("tiny-process-library")
    set_languages("c++11")
    set_kind("$(kind)")

    add_files("process.cpp")
    add_headerfiles("process.hpp")

    if is_plat("windows") then
        add_files("process_win.cpp")
    else
        add_files("process_unix.cpp")
    end
