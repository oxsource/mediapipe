load("//mediapipe/framework/port:build_config.bzl", "mediapipe_cc_proto_library", "mediapipe_proto_library")

licenses(["notice"])

package(default_visibility = ["//visibility:private"])

proto_library(
    name = "text_transform_calculator_proto",
    srcs = ["text_transform_calculator.proto"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework:calculator_proto",
    ],
)

mediapipe_cc_proto_library(
    name = "text_transform_calculator_cc_proto",
    srcs = ["text_transform_calculator.proto"],
    cc_deps = ["//mediapipe/framework:calculator_cc_proto"],
    visibility = ["//visibility:public"],
    deps = [":text_transform_calculator_proto"],
)

cc_library(
    name = "text_transform_calculator",
    srcs = ["text_transform_calculator.cc"],
    deps = [
        ":text_transform_calculator_cc_proto",
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework/port:status",
    ],
    visibility = ["//visibility:public"],
    alwayslink = 1,
)

cc_library(
    name = "text_merge_calculator",
    srcs = ["text_merge_calculator.cc"],
    deps = [
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework/port:status",
    ],
    visibility = ["//visibility:public"],
    alwayslink = 1,
)
