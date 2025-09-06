licenses(["notice"])

package(default_visibility = ["//visibility:public"])

#bazel build --define MEDIAPIPE_DISABLE_GPU=1 //mediapipe/examples/text/service:text_service
cc_library(
    name = "text_service",
    srcs = ["text_service.cc"],
    hdrs = ["text_service.h"],
    deps = [
        "//mediapipe/examples/text/calculators:text_merge_calculator",
        "//mediapipe/examples/text/calculators:text_transform_calculator",
        "//mediapipe/framework:calculator_graph",
        "//mediapipe/framework/port:parse_text_proto",
        "//mediapipe/framework/port:status",
        "@com_google_absl//absl/log:absl_check",
        "@com_google_absl//absl/log:absl_log",
        "@com_google_absl//absl/strings",
    ],
)
