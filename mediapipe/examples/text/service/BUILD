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
    alwayslink = 1,
)

##
#NOTES:
#1. https://bazel.build/docs/user-manual#dynamic-mode
#2. https://github.com/bazelbuild/rules_cc/blob/main/cc/private/rules_impl/cc_binary.bzl
#3. https://bazel.build/reference/be/c-cpp#cc_binary
# linkshared
# linkstatic
##

#bazel build --define MEDIAPIPE_DISABLE_GPU=1 //mediapipe/examples/text/service:libtext_service_api.so
cc_binary(
    name = "libtext_service_api.so",
    linkshared = 1,
    linkstatic = 1,
    deps = [
        ":text_service",
    ],
)

#bazel build --define MEDIAPIPE_DISABLE_GPU=1 //mediapipe/examples/text/service:text_service_lib
cc_library(
    name = "text_service_lib",
    srcs = [":libtext_service_api.so"],
    alwayslink = 1,
)