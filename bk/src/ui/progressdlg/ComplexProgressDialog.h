#ifndef __COMPLEXPROGRESSDIALOG_H__
#define __COMPLEXPROGRESSDIALOG_H__

#include <progress\ComplexProgress.h>
#include <progress\SimpleProgress.h>
#include <string>
#include <map>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/exception_ptr.hpp>


#define WM_PROGRESS_SET_ACTIVITY_STEPS (WM_APP+2201)
#define WM_PROGRESS_STEP_ACTIVITY (WM_APP+2202)
#define WM_PROGRESS_SET_TASK_STEPS (WM_APP+2203)
#define WM_PROGRESS_STEP_TASK (WM_APP+2204)

class CComplexProgressDialog : public CDialog, protected ComplexProgress
{
	class SimpleProgressProxy : public SimpleProgress
	{
	public:
		SimpleProgressProxy(CComplexProgressDialog* pOwner);
		virtual void SetSteps(int maxSteps);
		virtual bool Step(int stepsToMake);
	private:
		CComplexProgressDialog* m_pOwner;

	};
public:
    CComplexProgressDialog (const std::wstring& sActivityTitle,CWnd* pParent);
    virtual ~CComplexProgressDialog ();

	void WaitFor(boost::function<void(ComplexProgress*)> &f);

	template<typename F> void WaitForTask(F& task)
	{
		boost::function<void(ComplexProgress*)> f = task;
		WaitFor(f);
	}

	template<typename R,typename FUNC> R WaitForTaskR(const FUNC& task)
	{
		Show();

		boost::function<R (ComplexProgress*)> f = task;
		boost::packaged_task<R> waitTask(boost::bind(f, ((ComplexProgress*)this)));
		boost::unique_future<R> future = waitTask.get_future();

		boost::thread thread(boost::move(waitTask));
		while (!future.is_ready())
		{
			Pump();
		}
		Hide();
		return future.get();
	}

	CComplexProgressDialog& operator()(const std::wstring& fromtitle, const std::wstring& totitle);

protected:

	virtual SimpleProgress* BeginTask(const std::wstring& sTaskName);
	virtual void SetTaskCount(int taskCount);
	virtual void EndTask();

	BOOL Pump();
	void Show();
	void Hide();

	bool WasCanceled();
	
private:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	void PumpMessages();
	virtual void OnCancel();	
	void SetActivityTitle(const std::wstring& sActivityTitle);
	void SetTaskName(const std::wstring& sTaskName);
	void SetActivitySteps(int maxSteps);
	bool StepActivity();
	bool StepTask(int stepsToMake);
	void SetTaskSteps(int maxSteps);
    void ReEnableParent();	
	
private:
	bool m_bCancel;
	std::wstring m_sActivityTitle;
	std::wstring m_sTaskName;
	CWnd* m_pParentWnd;
	bool m_bParentDisabled;
	CProgressCtrl	m_ActivityProgress;
	CProgressCtrl	m_TaskProgress;
	SimpleProgressProxy m_TaskProgressProxy;
	std::map<std::wstring,std::wstring> m_titleTranslations;
private:
	     
	afx_msg	LRESULT OnMessageSetActivitySteps(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMessageStepActivity(WPARAM wParam,LPARAM lParam);
	afx_msg	LRESULT OnMessageSetTaskSteps(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMessageStepTask(WPARAM wParam,LPARAM lParam);

	afx_msg void OnShowWindow(BOOL bShow,UINT nStatus);

	DECLARE_MESSAGE_MAP()

	friend class SimpleProgressProxy;
};


#endif