#include "mediapipe/examples/text/service/text_service.h"
#include <cstdio>
#include <thread>
#include <vector>
#include <string>
#include <mutex>
#include <condition_variable>
#include <atomic>

struct JoinState {
    std::mutex mtx;
    std::condition_variable cv;
    std::atomic<int> count{0};
    int expected_count = 0;
};

static JoinState g_state;

static void join_callback(const char* result) {
    printf("Joined result: %s\n", result);
    g_state.count++;
    g_state.cv.notify_one();
}

int main() {
    std::vector<std::pair<std::string, std::string>> inputs = {
        {"Hello", "WORLD"},
        {"Foo", "Bar"},
        {"A", "B"},
        {"C", "D"}
    };
    g_state.expected_count = static_cast<int>(inputs.size());
    mediapipe_text_join_start(join_callback);
    for (auto& p : inputs) {
        mediapipe_text_join_post(p.first.c_str(), p.second.c_str());
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    {
        std::unique_lock<std::mutex> lock(g_state.mtx);
        if (!g_state.cv.wait_for(lock, std::chrono::seconds(5), []{ return g_state.count.load() >= g_state.expected_count; })) {
            printf("Timeout waiting for all callbacks\n");
        }
    }
    mediapipe_text_join_stop();
    return 0;
}
