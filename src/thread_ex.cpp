#include "thread_ex.hpp"
#include <functional>
#include <iostream>
#include <spdlog/fmt/fmt.h>

namespace basic_thread_example
{
void ThreadExample::printTime()
{
    auto now = std::chrono::system_clock::now();
    auto stime = std::chrono::system_clock::to_time_t(now);
    auto ltime = std::localtime(&stime);

    std::ostringstream oss;
    oss << std::put_time(ltime, "%c"); // 시간을 문자열 스트림에 쓴다

    fmt::print("{}\n", oss.str()); // 문자열 스트림에서 문자열을 얻어 fmt::print에 전달
}

void ThreadExample::func1()
{
    fmt::print("Thread func without params\n");
}

void ThreadExample::func2(const int i, const double d, const std::string &s)
{
    fmt::print("i: {} | d: {} | s: {}\n", i, d, s);
}

void ThreadExample::func3(const std::string &s)
{
    fmt::print("s: {}\n", s);
}

void ThreadExample::func4()
{
    using namespace std::literals::chrono_literals;
    this->printTime();
    std::this_thread::sleep_for(2s);
}

void ThreadExample::func5()
{
    using namespace std::literals::chrono_literals;
    this->printTime();
    std::this_thread::sleep_until(std::chrono::system_clock::now() + 2s);
    this->printTime();
}

void ThreadExample::func6(std::chrono::seconds timeout)
{
    auto now = std::chrono::system_clock::now();
    auto then = now + timeout;
    do
    {
        std::this_thread::yield();
    } while (std::chrono::system_clock::now() < then);
}

void ThreadExample::runThreadEx()
{
    // 아무런 인자를 갖지 않는 void함수
    std::thread t{std::bind(&ThreadExample::func1, this)};
    // 람다 함수를 스레드로 생성
    std::thread lambd_t{[]()
                        { fmt::print("lambda thread\n"); }};
    // 여러 인자를 갖는 스레드
    std::thread t2{std::bind(&ThreadExample::func2, this, 42, 42.0, "42")};
    // 함수의 참조를 인자로 절달하는 방식
    std::string s = "MyString";
    std::thread t3{std::bind(&ThreadExample::func3, this, std::ref(s))};
    std::thread t4{std::bind(&ThreadExample::func4, this)};
    std::thread t5{std::bind(&ThreadExample::func5, this)};
    std::thread t6(std::bind(&ThreadExample::func6, this, std::chrono::seconds(2)));

    std::array<std::thread, 7> threads{std::move(t), std::move(lambd_t), std::move(t2), std::move(t3), std::move(t4), std::move(t5), std::move(t6)};

    for (auto &t : threads)
    {
        t.join();
    }
}

void runExample()
{
    ThreadExample ex{};
    ex.runThreadEx();
}

} // namespace basic_thread_example

namespace exception_in_thread_ex
{
std::vector<std::exception_ptr> g_exceptions;
std::mutex g_mutex;
}