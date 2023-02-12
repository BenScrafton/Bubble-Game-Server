#pragma once
#include <condition_variable>
#include <mutex>
#include <queue>
#include <iostream>

template<typename Type>
class Queue
{
private:
	std::queue<Type> m_queue;
	std::mutex m_mutex;
	std::condition_variable m_emptyQueueCond;
public:
	Queue() = default;
	Queue(const Queue&) = delete;
	Queue& operator = (const Queue&) = delete;

	void Push(Type p_type)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_queue.push(p_type);
		m_emptyQueueCond.notify_one();
	}

	Type Pop()
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_emptyQueueCond.wait(lock, [this] {return !m_queue.empty(); });
		auto val = m_queue.front();
		m_queue.pop();
		return val;
	}

	void Pop(Type& item)
	{
		std::unique_lock<std::mutex> lock(m_mutex);;

		if(m_queue.empty())
		{
			return;
		}

		item = m_queue.front();
		m_queue.pop();
	}
};

