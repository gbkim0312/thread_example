#pragma once
#include <chrono>
#include <exception>
#include <functional>
#include <iomanip>
#include <mutex>
#include <spdlog/fmt/fmt.h>
#include <stdexcept>
#include <thread>
#include <vector>

namespace basic_thread_example
{
class ThreadExample
{
public:
    void runThreadEx();

private:
    void printTime();
    void func1();
    void func2(const int i, const double b, const std::string &s);
    void func3(const std::string &s);
    void func4();
    void func5();
    void func6(std::chrono::seconds timeout);
};
} // namespace basic_thread_example

namespace exception_in_thread_ex
{
extern std::vector<std::exception_ptr> g_exceptions;
extern std::mutex g_mutex;
class ThreadExmaple
{
public:
    void runThreadEx()
    {
        g_exceptions.clear();
        std::thread t1{std::bind(&ThreadExmaple::thread_func1, this)};
        std::thread t2{std::bind(&ThreadExmaple::thread_func2, this)};

        t1.join();
        t2.join();

        for (const auto &e : g_exceptions)
        {
            try
            {
                if (e != nullptr)
                {
                    std::rethrow_exception(e); // 각 스레드에서 발생한 exceptiopn을 다시 이 스레드로 던져줌
                }
            }
            catch (std::exception &e)
            {
                fmt::print("exception: {}\n", e.what());
            }
        }
    }

private:
    void func1()
    {
        throw std::runtime_error("Exception 1");
    }

    void func2()
    {
        throw std::runtime_error("Exception 2");
    }

    void thread_func1()
    {
        try
        {
            this->func1();
        }
        catch (const std::exception &e)
        {
            std::lock_guard<std::mutex> lock(g_mutex);
            g_exceptions.push_back(std::current_exception()); // 현재 exceptiopn을 캡쳐한 뒤 std::exception_ptr 인스턴스 생성
            fmt::print("Exception in func1: {} ", e.what());
        }
    }

    void thread_func2()
    {
        try
        {
            {
                this->func2();
            }
        }
        catch (const std::exception &e)
        {
            std::lock_guard<std::mutex> lock(g_mutex);
            g_exceptions.push_back(std::current_exception());
            fmt::print("Exception in func2: {} ", e.what());
        }
    }
};

} // namespace exception_in_thread_ex