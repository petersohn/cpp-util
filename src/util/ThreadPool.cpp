#include "ThreadPool.hpp"
#include <functional>
#include <boost/exception/all.hpp>
#include <iostream>
#include <boost/thread/tss.hpp>

namespace util {

void ThreadPool::runInThread()
{
    while (true) {
        try {
            ioService.run();
            break;
        } catch (std::exception &e) {
            std::cerr << boost::diagnostic_information(e) << std::endl;
        }
        // retry
    }
}

void ThreadPool::setNumThreads(std::size_t value)
{
    if (numThreads == value) {
        return;
    }
    bool wasRunning = running;
    if (wasRunning) {
        wait();
    }
    numThreads = value;
    if (wasRunning) {
        start();
    }

}

static boost::thread_specific_ptr<std::size_t> localThreadId;

void ThreadPool::start()
{
    if (!running && numThreads > 0) {
        work.reset(new boost::asio::io_service::work(ioService));
        threads.reserve(numThreads);
        while (threads.size() < numThreads) {
            std::size_t id = threads.size();
            threads.push_back(std::make_shared<std::thread>([this, id]() {
                    localThreadId.reset(new std::size_t(id));
                    runInThread();
                    localThreadId.reset();
                })
            );
        }
        running = true;
    }
}

void ThreadPool::wait()
{
    if (running) {
        work.reset();
        for(auto t: threads) {
            t->join();
        }
        threads.clear();
        ioService.reset();
        running = false;
    }
}

const std::size_t* ThreadPool::getCurrentThreadId()
{
    return localThreadId.get();
}

} // namespace util
