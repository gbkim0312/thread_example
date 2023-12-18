#pragma once
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <spdlog/fmt/fmt.h>
#include <thread>
namespace cv_ex
{
class CvExample
{
private:
    std::condition_variable cv_;
    std::mutex mutex_;
    std::mutex io_mutex_;
    int data_ = 0;

public:
    void runCvExample()
    {
        std::thread producer{[this]()
                             {
                                 {
                                     using namespace std::chrono_literals;
                                     std::this_thread::sleep_for(2s);
                                 }
                                 {
                                     std::lock_guard<std::mutex> lock{mutex_};
                                     data_ = 42;
                                     cv_.notify_one(); // 데이터 생성 후 알림
                                 }
                                 {
                                     std::lock_guard<std::mutex> lock{io_mutex_};
                                     fmt::print("produced: {}\n", data_);
                                 }
                             }};

        std::thread consumer{[this]()
                             {
                                 std::unique_lock<std::mutex> lock(mutex_);
                                 cv_.wait(lock, [this]
                                          { return data_ != 0; }); // 조건을 명시하여 대기
                                 {
                                     std::lock_guard<std::mutex> lock{io_mutex_};
                                     fmt::print("consumed: {}\n", data_);
                                 }
                             }};

        // 스레드 조인
        producer.join();
        consumer.join();
    }
};
} // namespace cv_ex