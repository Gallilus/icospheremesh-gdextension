#!/usr/bin/env python

libname = "libicospheremesh"

env = SConscript("godot-cpp/SConstruct")

env.Append(CPPPATH=["src/"])
sources = Glob("src/*.cpp")

if env["platform"] == "macos":
    platlibname = "{}.{}.{}".format(libname, env["platform"], env["target"])
    library = env.SharedLibrary(
        "demo/bin/{}.framework/{}".format(platlibname, platlibname),
        source=sources,
    )
else:
    library = env.SharedLibrary(
        "demo/bin/{}{}{}".format(libname, env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)