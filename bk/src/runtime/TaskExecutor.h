#ifndef __TASKEXECUTOR_H__
#define __TASKEXECUTOR_H__

#include <string>
#include <functional>
#include <progress/SimpleProgress.h>
#include <progress/ComplexProgress.h>
#include <vector>

class _taskexe
{
public:
	_taskexe(const std::wstring& taskTitle, int steps, std::function<void(SimpleProgress*)>& f);
	_taskexe(const _taskexe& src);
	_taskexe& operator=(const _taskexe& right);
private:
	std::wstring m_taskTitle;
	int m_steps;
	std::function<void(SimpleProgress*)> m_f;

	friend class TaskExecutor;
};

_taskexe task(const std::wstring& taskTitle, int steps, std::function<void(SimpleProgress*)>& f);

template<typename F>
_taskexe task(const std::wstring& taskTitle, int steps, F& f)
{
	std::function<void(SimpleProgress*)> functor = f;
	return task(taskTitle, steps, functor);
}	

class TaskExecutor 
{
		
public:
    TaskExecutor (ComplexProgress* progress);
	~TaskExecutor();
	
	void Run();

	TaskExecutor& operator<<(_taskexe& t);

private:

    ComplexProgress* m_progress;
	std::vector<_taskexe*> m_tasks;

	

	
};


#endif