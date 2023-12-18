#pragma once
#include <functional>
#include <mutex>
#include <spdlog/fmt/fmt.h>
#include <thread>
#include <vector>

namespace mutex_ex
{
class MutexExample
{
private:
    std::mutex mutex_;
    int shared_data_ = 0;
    std::vector<std::thread> thread_pool_;

    void increaseSharedData()
    {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            ++shared_data_;
        }
    }
    void addThreadToThreadPool()
    {
        for (int i = 0; i < 1000; i++)
        {
            thread_pool_.emplace_back(std::bind(&MutexExample::increaseSharedData, this));
        }
    }

public:
    void runMutexExample()
    {
        this->addThreadToThreadPool();
        for (auto &thread : thread_pool_)
        {
            thread.join();
        }

        fmt::print("result: {}\n", shared_data_);
    }
};
} // namespace mutex_ex

/**
 * Recursive Mutex: 동일한 스레드 내에서 다중 잠금을 사용할 수 있게 해줌
 * 아래 예제에서 일반 mutex사용 시, updateWithReturn함수 시작할 때 한 번 잠그고,
 * 그 뒤 update에서 한번 더 잠그니 데드락 현상 발생할 수 있음
 * 이렇듯 동일한 스레드 내에서 다중 잠금을 사용해아할 때 recursive_mutex 사용 가능.
 * 다만, recursive mutex는 non-recursive mutex보다 오버헤드가 크기 때문에 가능한 한 피해야 함
 */
namespace recursive_mutex_ex
{
class RecMutexEx
{
private:
    std::recursive_mutex m_;
    int data_;

public:
    RecMutexEx(int data = 0) : data_(data) {}
    void update(const int d)
    {
        std::lock_guard<std::recursive_mutex> lock(m_);
        data_ = d;
    }
    int updateWithReturn(const int d)
    {
        std::lock_guard<std::recursive_mutex> lock(m_);
        int temp = data_;
        this->update(d);
        return temp;
    }
};
/**
 * 위 recursive mutex를 일반 뮤텍스를 사용해 설계 변경
 */
class RecMutexToNormalMutex
{
public:
    RecMutexToNormalMutex(int data = 0) : data_(data) {}

private:
    std::mutex mutex_;
    int data_ = 0;

    void internalUpdate(const int data)
    {
        data_ = data;
    }

public:
    void update(const int data)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        internalUpdate(data);
    }

    int updateWithReturn(const int data)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        int temp = data_;
        data_ = data;
        return temp;
    }
};

/**
 * 아래의 경우, recursive mutex를 일반 뮤텍스로 대체하는것이 불가능함
 * 설계 자체를 다른 방식으로 하는 것을 추천
 */

// std::recursive_mutex rec_mtx;
// int shared_data = 0;

// void recursiveIncrement(int level)
// {
//     if (level <= 0)
//         return;

//     rec_mtx.lock(); // 잠금 획득
//     ++shared_data;
//     fmt::print("Level {} : shared data: {}\n", level, shared_data);
//     recursiveIncrement(level - 1); // 재귀 호출

//     rec_mtx.unlock(); // 잠금 해제
// }

} // namespace recursive_mutex_ex