#include "absl/log/absl_check.h"
#include "absl/log/absl_log.h"
#include "absl/strings/str_join.h"
#include "mediapipe/framework/calculator_graph.h"
#include "mediapipe/framework/port/parse_text_proto.h"
#include "mediapipe/framework/port/status.h"

#include <vector>
#include <string>

namespace mediapipe {

absl::Status RunTextTransformGraph() {
  CalculatorGraphConfig config =
      ParseTextProtoOrDie<CalculatorGraphConfig>(R"pb(
        input_stream: "in0"
        input_stream: "in1"
        output_stream: "out"
        node {
          calculator: "TextTransformCalculator"
          input_stream: "in0"
          input_stream: "in1"
          output_stream: "out0"
          output_stream: "out1"
          options {
            [mediapipe.TextTransformCalculatorOptions.ext] {
              lowercase: true
              suffix: "_ext"
            }
          }
        }
        node {
          calculator: "TextMergeCalculator"
          input_stream: "out0"
          input_stream: "out1"
          output_stream: "out"
        }
      )pb");

  CalculatorGraph graph;
  MP_RETURN_IF_ERROR(graph.Initialize(config));

  MP_ASSIGN_OR_RETURN(OutputStreamPoller poller,
                      graph.AddOutputStreamPoller("out"));

  MP_RETURN_IF_ERROR(graph.StartRun({}));

  // 发送两路输入
  MP_RETURN_IF_ERROR(graph.AddPacketToInputStream(
      "in0", MakePacket<std::string>("Hello").At(Timestamp(0))));
  MP_RETURN_IF_ERROR(graph.AddPacketToInputStream(
      "in1", MakePacket<std::string>("WORLD").At(Timestamp(0))));

  MP_RETURN_IF_ERROR(graph.CloseInputStream("in0"));
  MP_RETURN_IF_ERROR(graph.CloseInputStream("in1"));

  mediapipe::Packet packet;
  while (poller.Next(&packet)) {
    const std::vector<std::string>& out_vec = packet.Get<std::vector<std::string>>();
    ABSL_LOG(INFO) << absl::StrJoin(out_vec, ",");
  }

  return graph.WaitUntilDone();
}

}  // namespace mediapipe

int main(int argc, char** argv) {
  google::InitGoogleLogging(argv[0]);
  ABSL_CHECK(mediapipe::RunTextTransformGraph().ok());
  return 0;
}
