#pragma once

#include "SeasonSession.h"

class SeasonSessionBuilder
{
	class LoggingSimpleProgress : public SimpleProgress
	{
	public:
		LoggingSimpleProgress(const std::wstring& message,BkLogger* log);
		virtual void SetSteps(int maxSteps);
		virtual bool Step(int stepsToMake = 1);
	protected:
		void Log();

	private:
		BkLogger* m_log;
		int m_maxSteps;
		int m_currentStep;
		int m_currentPercent;
		std::wstring m_message;
	};


	class LoggingComplexProgress : public ComplexProgress
	{
	public:
		LoggingComplexProgress(const std::wstring& message, BkLogger* log);
		~LoggingComplexProgress();
		virtual void SetTaskCount(int taskCount);
		virtual SimpleProgress* BeginTask(const std::wstring& sTaskName);
		virtual void EndTask();
	protected:
		void Log();

	private:
		LoggingSimpleProgress* m_currentTaskProgress;
		BkLogger* m_log;
		std::wstring m_message;
	};

public:
	SeasonSessionBuilder(BkLogger* log);
	void SetSeason(Season* pSeason);
	void SetParentSession(FarmSession* pFarmSession);
	void SetTokenGenerator(SessionTokenGenerator* generator);
	
	SeasonSession* Build();
	

private:
	Season* m_pSeason;
	FarmSession* m_pFarmSession;
	SessionTokenGenerator* m_pTokenGenerator;
	BkLogger* m_log;


};