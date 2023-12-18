#pragma once
#include <chrono>
#include <future>
#include <mutex>
#include <spdlog/fmt/fmt.h>
#include <thread>

namespace async_ex
{

std::mutex io_mutex;

void doSomething()
{
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(2s);
    }
    std::lock_guard<std::mutex> lock(io_mutex);
    fmt::print("operation 1 done...\n");
}

void doSomethingElse()
{
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
    }
    std::lock_guard<std::mutex> lock(io_mutex);
    fmt::print("operation 2 done...\n");
}

int computeSomething()
{
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(2s);
    }
    return 42;
}

int computeSomethingElse()
{
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
    }
    return 24;
}

namespace do_something
{
void runExample()
{
    auto f = std::async(std::launch::async, doSomething);
    doSomethingElse();
    f.wait(); // future가 어떠한 값을 반환하지 않을 때는 wait을 호출하여 작업이 완료될 떄까지 대기
}
} // namespace do_something

namespace compute_something
{
void runExample()
{
    auto f = std::async(std::launch::async, computeSomething);
    int value = computeSomethingElse();
    value += f.get(); // future이 어떠한 결과를 반환할 때는 wait 대신 get을 사용 가능
    fmt::print("result: {}\n", value);
}
} // namespace compute_something

namespace polling_using_wait_for
{
void runExample()
{
    using namespace std::chrono_literals;
    auto f = std::async(std::launch::async, doSomething);

    std::future_status status;
    while ((status = f.wait_for(500ms)) != std::future_status::ready)
    {
        fmt::print("waitting...\n");
    }
    fmt::print("Done.\n");
}
} // namespace polling_using_wait_for

} // namespace async_ex