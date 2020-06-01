#include "messagebusthread.h"

static void thread_run(CThreadInfo* _thread_info)
{
	if (nullptr == _thread_info)
	{
		return;
	}

	while (_thread_info->is_run_)
	{
		if (_thread_info->thread_queue_data_.size() == 0)
		{
			std::unique_lock <std::mutex> lock(_thread_info->thread_mutex_);
			_thread_info->condition_.wait(lock);
		}
		else
		{
			vector<UserFunctor> _thread_curr_queue_data;
			_thread_info->thread_mutex_.lock();
			_thread_curr_queue_data.swap(_thread_info->thread_queue_data_);
			_thread_info->thread_mutex_.unlock();

			for (UserFunctor f : _thread_curr_queue_data)
			{
				f();
			}
		}
	}

	delete _thread_info;
	_thread_info = NULL;
}

CMessageThread::CMessageThread()
{

}

bool CMessageThread::Create(int _thread_logic_id)
{
	if (NULL == _thread_info)
	{
		_thread_info = new CThreadInfo();
	}

	_thread_info->thread_logic_id_ = _thread_logic_id;
	std::thread thread_ = std::thread(thread_run, _thread_info);
	thread_.detach();

	return true;
}
