load("@rules_foreign_cc//foreign_cc:defs.bzl", "cmake")

filegroup(
    name = "vulkan_headers_srcs",
    srcs = glob(["**"]),
)

cmake(
    name = "vulkan_headers",
    lib_source = ":vulkan_headers_srcs",
    out_headers_only = True,
    visibility = ["//visibility:public"],
)
