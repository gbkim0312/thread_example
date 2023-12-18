#pragma once
#include <future>
#include <spdlog/fmt/fmt.h>

namespace promise_future_ex
{
class PromiseFuture
{
public:
    void runPromiseFutureExample()
    {
        std::promise<int> p;
        std::thread t1{&PromiseFuture::produce, this, std::ref(p)};

        std::future<int> f = p.get_future();
        std::thread t2(&PromiseFuture::consume, this, std::ref(f));

        t1.join();
        t2.join();
    }

private:
    void produce(std::promise<int> &p)
    {
        {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(2s);
        }
        fmt::print("produce value: {}\n", 42);
        p.set_value(42);
    }

    void consume(std::future<int> &f)
    {
        using namespace std::chrono_literals;
        int value = f.get(); // 결과가 나올 떄까지 대기
        f.wait_for(3s);
        fmt::print("consume value: {}\n", value);
    }
};
} // namespace promise_future_ex

namespace promise_with_exception
{
class PromiseExceptionExample
{
public:
    void runPromiseExceptionExample()
    {
        std::promise<int> p;
        std::thread t1(&PromiseExceptionExample::produce, this, std::ref(p));

        std::future<int> f = p.get_future();
        std::thread t2(&PromiseExceptionExample::consume, this, std::ref(f));

        t1.join();
        t2.join();
    }

private:
    void produce(std::promise<int> &p)
    {
        using namespace std::chrono_literals;
        fmt::print("producing value: {}...\n", 42);
        std::this_thread::sleep_for(2s);
        try
        {
            // p.set_value(42); // 만족했을때 exception을 던지면 promise already satisfied exception 던짐
            throw std::runtime_error("Error occured!");
        }
        catch (const std::exception &e)
        {
            fmt::print("error: {}\n", e.what());
            p.set_exception(std::current_exception());
        }
    }

    void consume(std::future<int> &f)
    {
        try
        {
            fmt::print("{}\n", f.get());
        }
        catch (const std::exception &e)
        {
            fmt::print("error: {}\n", e.what());
        }
    }
};
} // namespace promise_with_exception