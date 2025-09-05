// Copyright 2022 The MediaPipe Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <algorithm>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "mediapipe/examples/text/calculators/text_transform_calculator.pb.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/port/status.h"

namespace mediapipe {
namespace api2 {

using mediapipe::TextTransformCalculatorOptions;

class TextTransformCalculator : public CalculatorBase {
 public:

  static absl::Status GetContract(CalculatorContract* cc) {
      RET_CHECK_EQ(cc->Inputs().NumEntries(), cc->Outputs().NumEntries());
      for (int i = 0; i < cc->Inputs().NumEntries(); ++i) {
        cc->Inputs().Index(i).Set<std::string>();
        cc->Outputs().Index(i).Set<std::string>();
      }
      return absl::OkStatus();
  }

  absl::Status Open(CalculatorContext* cc) override {
    return absl::OkStatus();
  }

  // Copies packets between input and output streams.
  // Updates timestamp bounds on all output streams.
  absl::Status Process(CalculatorContext* cc) override {
    auto& options = cc->Options<TextTransformCalculatorOptions>();
    auto transform_fn = options.lowercase() 
                    ? [](unsigned char c){ return std::tolower(c); }
                    : [](unsigned char c){ return std::toupper(c); };
    for (int i = 0; i < cc->Inputs().NumEntries(); ++i) {
        std::string text = cc->Inputs().Index(i).Get<std::string>();
        std::transform(text.begin(), text.end(), text.begin(), transform_fn);
        if (!options.suffix().empty()) {
            text += options.suffix();
        }
        cc->Outputs().Index(i).AddPacket(MakePacket<std::string>(text).At(cc->InputTimestamp()));
    }
    return absl::OkStatus();
  }

  // Close all output streams.
  absl::Status Close(CalculatorContext* cc) override {
    for (auto id = cc->Outputs().BeginId(); id != cc->Outputs().EndId(); ++id) {
      cc->Outputs().Get(id).Close();
    }
    return absl::OkStatus();
  }

};

REGISTER_CALCULATOR(TextTransformCalculator);

}  // namespace api2
}  // namespace mediapipe
