#include "mediapipe/framework/calculator_graph.h"
#include "mediapipe/framework/port/parse_text_proto.h"
#include "mediapipe/framework/port/status.h"
#include "mediapipe/framework/port/statusor.h"

#include "absl/log/absl_log.h"
#include "absl/strings/str_join.h"

#include <thread>
#include <mutex>
#include <atomic>
#include <string>
#include <vector>
#include <memory>
#include <condition_variable>

#include "mediapipe/examples/text/service/text_service.h"

namespace mediapipe {

class TextService {
public:
    static TextService& Instance() {
        static TextService instance;
        return instance;
    }

    absl::Status Start(void (*callback)(const char*)) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (running_) {
            return absl::FailedPreconditionError("Service already running");
        }
        callback_ = callback;

        // Graph 配置
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

        MP_RETURN_IF_ERROR(graph_.Initialize(config));
        MP_ASSIGN_OR_RETURN(OutputStreamPoller poller, graph_.AddOutputStreamPoller("out"));
        poller_ = std::make_unique<OutputStreamPoller>(std::move(poller));
        MP_RETURN_IF_ERROR(graph_.StartRun({}));

        running_ = true;
        worker_ = std::thread(&TextService::PollerLoop, this);

        return absl::OkStatus();
    }

    absl::Status Post(const std::string& s0, const std::string& s1) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (!running_) {
                return absl::FailedPreconditionError("Service not running");
            }

            Timestamp ts(ts_++);
            MP_RETURN_IF_ERROR(graph_.AddPacketToInputStream(
                "in0", MakePacket<std::string>(s0).At(ts)));
            MP_RETURN_IF_ERROR(graph_.AddPacketToInputStream(
                "in1", MakePacket<std::string>(s1).At(ts)));
        }

        cv_.notify_one();
        return absl::OkStatus();
    }

    absl::Status Stop() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (!running_) return absl::FailedPreconditionError("Service not running");
            running_ = false;
        }

        graph_.CloseAllInputStreams();
        cv_.notify_one();
        graph_.WaitUntilDone();

        if (worker_.joinable()) worker_.join();
        poller_.reset();
        callback_ = nullptr;

        return absl::OkStatus();
    }

private:
    TextService() : running_(false), ts_(0), callback_(nullptr) {}
    ~TextService() { if (running_) Stop(); }

    TextService(const TextService&) = delete;
    TextService& operator=(const TextService&) = delete;

    void PollerLoop() {
        mediapipe::Packet packet;

        while (true) {
            {
                std::unique_lock<std::mutex> lock(mutex_);
                cv_.wait(lock, [&]{ return !running_ || (poller_ && poller_->QueueSize() > 0); });
                if (!running_ && (poller_ || poller_->QueueSize() == 0)) break;
            }

            while (poller_ && poller_->Next(&packet)) {
                const auto& out_vec = packet.Get<std::vector<std::string>>();
                std::string joined = absl::StrJoin(out_vec, ",");
                if (callback_) callback_(joined.c_str());
            }
        }
    }

    CalculatorGraph graph_;
    std::unique_ptr<OutputStreamPoller> poller_;
    std::thread worker_;
    std::atomic<bool> running_;
    std::atomic<int64_t> ts_;
    void (*callback_)(const char*);
    std::mutex mutex_;
    std::condition_variable cv_;
};

}  // namespace mediapipe

extern "C" {

int mediapipe_text_join_start(void (*callback)(const char*)) {
    return mediapipe::TextService::Instance().Start(callback).ok() ? 0 : -1;
}

int mediapipe_text_join_post(const char* s0, const char* s1) {
    return mediapipe::TextService::Instance().Post(s0, s1).ok() ? 0 : -1;
}

int mediapipe_text_join_stop() {
    return mediapipe::TextService::Instance().Stop().ok() ? 0 : -1;
}

}  // extern "C"
