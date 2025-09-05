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

#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/port/status.h"

namespace mediapipe {
namespace api2 {

class TextMergeCalculator : public CalculatorBase {
 public:
  static absl::Status GetContract(CalculatorContract* cc) {
      for (int i = 0; i < cc->Inputs().NumEntries(); ++i) {
        cc->Inputs().Index(i).Set<std::string>();
      }
      cc->Outputs().Index(0).Set<std::vector<std::string>>();
      return absl::OkStatus();
  }

  absl::Status Open(CalculatorContext* cc) override {
    return absl::OkStatus();
  }

  absl::Status Process(CalculatorContext* cc) override {
    std::vector<std::string> values;

    for (int i = 0; i < cc->Inputs().NumEntries(); ++i) {
      if (cc->Inputs().Index(i).IsEmpty()) continue;
      const auto& text = cc->Inputs().Index(i).Get<std::string>();
      values.push_back(text);
    }
    cc->Outputs().Index(0).AddPacket(mediapipe::MakePacket<std::vector<std::string>>(values).At(cc->InputTimestamp()));

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

REGISTER_CALCULATOR(TextMergeCalculator);

}  // namespace api2
}  // namespace mediapipe
