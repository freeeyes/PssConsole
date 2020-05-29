#pragma once
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <unordered_map>
#include <functional>

using namespace std;

using UserFunctor = std::function<void()>;

class CThreadInfo
{
public:
	CThreadInfo() : thread_logic_id_(0), is_run_(true)
	{

	};

	int thread_logic_id_;
	bool is_run_;
	mutable std::mutex thread_mutex_;
	std::condition_variable condition_;
	vector<UserFunctor> thread_queue_data_;
};

class CMessageThread
{
public:
	CMessageThread();

	bool Create(int _thread_logic_id);

	bool Add_do_function(UserFunctor&& functor)
	{
		if (NULL == _thread_info)
		{
			return false;
		}

		std::lock_guard <std::mutex> lock(_thread_info->thread_mutex_);
		_thread_info->thread_queue_data_.emplace_back(functor);
		_thread_info->condition_.notify_one();

		return true;
	};

private:
	CThreadInfo* _thread_info = NULL;
};
