load("@rules_cc//cc:defs.bzl", "cc_library")

filegroup(
    name = "glm_srcs",
    srcs = glob(["**"]),
)

cc_library(
    name = "glm",
    hdrs = [":glm_srcs"],
    visibility = ["//visibility:public"],
)
