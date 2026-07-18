load("@rules_foreign_cc//foreign_cc:defs.bzl", "cmake")

filegroup(
    name = "vulkan_loader_srcs",
    srcs = glob(["**"]),
)

cmake(
    name = "vulkan_loader",
    lib_source = ":vulkan_loader_srcs",
    cache_entries = {
        "BUILD_SHARED_LIBS": "ON",
        "BUILD_TESTS": "OFF",
        "UPDATE_DEPS": "OFF",
    },
    env = select({
        "@platforms//os:windows": {},
        "@platforms//os:macos": {},
        "//conditions:default": {
            "PKG_CONFIG_PATH": "/usr/lib/pkgconfig:/usr/share/pkgconfig"
        },
    }),
    deps = ["@vulkan_headers"],
    out_static_libs = select({
        "@platforms//os:windows": ["vulkan-1.lib"],
        "//conditions:default": [],
    }),
    out_shared_libs = select({
        "@platforms//os:windows": [],
        "@platforms//os:macos": ["libvulkan.1.dylib"],
        "//conditions:default": ["libvulkan.so.1"],
    }),
    visibility = ["//visibility:public"],
)
