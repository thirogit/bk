#include "stdafx.h"
#include "TaskExecutor.h"
#include <arrays/DeepDelete.h>
#include <boost/foreach.hpp>

_taskexe::_taskexe(const std::wstring& taskTitle, int steps, std::function<void(SimpleProgress*)>& f) : m_taskTitle(taskTitle), m_steps(steps), m_f(f)
{
}

_taskexe::_taskexe(const _taskexe& src) : m_taskTitle(src.m_taskTitle), m_steps(src.m_steps), m_f(src.m_f)
{

}

_taskexe& _taskexe::operator=(const _taskexe& right)
{
	m_taskTitle = right.m_taskTitle;
	m_steps = right.m_steps;
	m_f = right.m_f;
	return *this;
}

//#################################################################################################

_taskexe task(const std::wstring& taskTitle, int steps, std::function<void(SimpleProgress*)>& f)
{
	return _taskexe(taskTitle, steps, f);
}

//#################################################################################################

TaskExecutor::TaskExecutor(ComplexProgress* progress) : m_progress(progress)
{
}

TaskExecutor::~TaskExecutor()
{
	DeepDelete(m_tasks);
}

void TaskExecutor::Run()
{
	m_progress->SetTaskCount(m_tasks.size());
	BOOST_FOREACH(_taskexe* atask, m_tasks)
	{
		SimpleProgress* taskProgress = m_progress->BeginTask(atask->m_taskTitle);
		taskProgress->SetSteps(atask->m_steps);
		atask->m_f(taskProgress);
		m_progress->EndTask();
	}
}

TaskExecutor& TaskExecutor::operator<<(_taskexe& t)
{
	m_tasks.push_back(new _taskexe(t));
	return *this;
}

