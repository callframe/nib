load("@rules_cc//cc:defs.bzl", "cc_library")

filegroup(
    name = "entt_srcs",
    srcs = glob(["**"]),
)

cc_library(
    name = "entt",
    hdrs = [":entt_srcs"],
    includes = ["src"],
    visibility = ["//visibility:public"],
)
