load("@rules_foreign_cc//foreign_cc:defs.bzl", "cmake")

filegroup(
    name = "sdl_srcs",
    srcs = glob(["**"]),
)

cmake(
    name = "sdl",
    lib_source = ":sdl_srcs",
    cache_entries = {
        "-DSDL_SHARED": "ON",
        "-DSDL_STATIC": "OFF",
        "-DSDL_TESTS": "OFF",
    },
    out_shared_libs = select({
        "@platforms//os:windows": ["SDL3.dll"],
        "@platforms//os:macos": ["libSDL3.0.dylib"],
        "//conditions:default": ["libSDL3.so.0"],
    }),
    visibility = ["//visibility:public"],
)