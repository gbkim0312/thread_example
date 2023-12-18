#include "async_example.hpp"
#include "cv_example.hpp"
#include "mutex_example.hpp"
#include "promise_future_ex.hpp"
#include "thread_ex.hpp"

void runBasicThreadEx()
{
    basic_thread_example::ThreadExample ex{};
    ex.runThreadEx();
}

void exceptionHandlingInThreadEx()
{
    exception_in_thread_ex::ThreadExmaple ex{};
    ex.runThreadEx();
}
void mutexExample()
{
    mutex_ex::MutexExample mutex_ex{};
    mutex_ex.runMutexExample();
}

void cvExample()
{
    cv_ex::CvExample ex{};
    ex.runCvExample();
}

void promiseFutureEx()
{
    promise_future_ex::PromiseFuture pf{};
    pf.runPromiseFutureExample();
}

void promiseWithExceptionEx()
{
    promise_with_exception::PromiseExceptionExample pe{};
    pe.runPromiseExceptionExample();
}

int main()
{
    // exceptionHandlingInThreadEx();
    // mutexExample();
    // cvExample();
    // promiseFutureEx();
    // promiseWithExceptionEx();
    // async_ex::do_something::runExample();
    // async_ex::compute_something::runExample();
    async_ex::polling_using_wait_for::runExample();

    return 0;
}
