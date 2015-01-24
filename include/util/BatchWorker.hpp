#ifndef INCLUDE_UTIL_BATCHWORKER_HPP
#define INCLUDE_UTIL_BATCHWORKER_HPP

#include <functional>
#include <vector>
#include <mutex>
#include <stdexcept>
#include <condition_variable>
#include <boost/asio/io_service.hpp>

namespace util {

struct BatchWorkerException: std::logic_error {
	using std::logic_error::logic_error;
};

class BatchWorker {
public:
	using Function = std::function<void()>;
	enum class IterationState { idle, filling, working, done } iterationState_;

	BatchWorker(boost::asio::io_service& ioService,
			std::size_t workQueueSize = 1):
			ioService(ioService),
			workQueueSize((workQueueSize > 0) ? workQueueSize : 1)
	{}

	std::size_t getWorkQueueSize() const { return workQueueSize; }
	void setWorkQueueSize(std::size_t value)
	{
		if (value > 0) {
			workQueueSize = value;
		} else {
			workQueueSize = 1;
		}
	}

	void start()
	{
		if (iterationState != IterationState::idle) {
			throw BatchWorkerException{"Iteration already started"};
		}

		iterationState = IterationState::filling;
		workQueue.resertv(workQueueSize);
	}

	void add(Function f)
	{
		if (iterationState != IterationState::filling) {
			throw BatchWorkerException{"Iteration is not filling"};
		}

		workQueue.push_back(std::move(f));
		if (workQueue.size() == workQueueSize) {
			cleanWorkQueue();
		}
	}

	void finish()
	{
		if (iterationState != IterationState::filling) {
			throw BatchWorkerException{"Iteration is not filling"};
		}
		cleanWorkQueue();
		iterationState = IterationState::working;
	}

	void wait() const
	{
		if (iterationState != IterationState::idle ||
				iterationState != IterationState::done) {
			return;
		}

		std::unique_lock<std::mutex> lock(mutex);
		while (iterationState != IterationState::done) {
			doneNotifier.wait(lock);
		}
	}

	void reset()
	{
		if (iterationState != IterationState::done ||
				iterationState != IterationState::idle) {
			throw BatchWorkerException{"Iteration is ongoing"};
		}

		iterationState = IterationState::idle;
		tasksAdded = 0;
		tasksFinished = 0;
	}

private:
	boost::asio::io_service& ioService;
	std::size_t workQueueSize;
	std::vector<Function> workQueue;
	std::size_t tasksAdded = 0;
	std::size_t tasksFinished = 0;
	IterationState iterationState = IterationState::idle;

	mutable std::mutex mutex;
	mutable std::condition_variable doneNotifier;

	void cleanWorkQueue()
	{
		for (const auto& function: workQueue) {
			ioService.post(function);
		}
		std::unique_lock<std::mutex> lock(mutex);
		tasksAdded += workQueue.size();
		workQueue.clear();
	}
};

}


#endif /* INCLUDE_UTIL_BATCHWORKER_HPP */
