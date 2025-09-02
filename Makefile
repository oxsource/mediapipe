# -------------- Paths --------------
MP_ROOT := $(shell pwd)
SDK_DIR := $(MP_ROOT)/bazel-release
INCLUDE_DIR := $(SDK_DIR)/include/mediapipe/framework
FRAMEWORK_DIR := $(MP_ROOT)/mediapipe/framework
BAZEL_BIN := $(MP_ROOT)/bazel-bin

# -------------- Targets --------------

.PHONY: all clean sdk framework_lib

all: sdk framework_lib
	@echo "=== Mediapipe SDK build complete ==="

# -----------------------------
# 1. Export headers
# -----------------------------
sdk/framework:
	@echo "=== Exporting framework headers to $(INCLUDE_DIR) ==="
	mkdir -p $(INCLUDE_DIR)
	cd $(FRAMEWORK_DIR) && \
	find . \( -name "*.h" -o -name "*.proto" \) -type f | cpio -pdm $(INCLUDE_DIR)

sdk: sdk/framework

# -----------------------------
# 2. Build calculator_runner library
# -----------------------------
framework_lib:
	@echo "=== Building calculator_runner libraries via Bazel ==="
	bazel build -c opt //mediapipe/framework:calculator_runner
	@echo "Copying output libraries to SDK folder"
	mkdir -p $(SDK_DIR)/lib
	cp $(BAZEL_BIN)/mediapipe/framework/libcalculator_runner.a $(SDK_DIR)/lib/
	cp $(BAZEL_BIN)/mediapipe/framework/libcalculator_runner.so $(SDK_DIR)/lib/

# -----------------------------
# 3. Clean
# -----------------------------
clean:
	@echo "=== Cleaning SDK folder and Bazel outputs ==="
	rm -rf $(SDK_DIR)
	bazel clean
