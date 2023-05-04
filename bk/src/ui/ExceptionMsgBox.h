#pragma once

#include <string>
#include <functional>
#include <boost/utility.hpp>
#include <boost\shared_ptr.hpp>
#include <exception\TagException.h>


struct Sequence
{
	std::function<void ()> prepare_stage;
	std::function<void ()> do_stage;
	std::function<void ()> fallback_stage;
	std::function<void ()> teardown_stage;
	std::function<void (wexception&)> catch_stage;


	void run()
	{
		try
		{
			if(prepare_stage) prepare_stage();
			try
			{
				if(do_stage) do_stage();
				if(teardown_stage) teardown_stage();
			}
			catch(wexception& e)
			{
				if(fallback_stage) fallback_stage();
				if(teardown_stage) teardown_stage();
				if(catch_stage) catch_stage(e);
			}
		}
		catch(wexception& e)
		{
			if(catch_stage) catch_stage(e);
		}
	}
};

typedef boost::shared_ptr<Sequence> SequencePtr;

class CatchStage 
{
public:
	
	CatchStage(SequencePtr sequence) : m_sequence(sequence)
	{		 
	}

	void Run()
	{
		m_sequence->run();
	}

private:
	SequencePtr m_sequence;
};


class TearDownStage 
{
public:
	
	TearDownStage(SequencePtr sequence) : m_sequence(sequence)
	{		 
	}

	template<typename F>
	CatchStage Catch(F& f)
	{
		m_sequence->catch_stage = f;
		return CatchStage(m_sequence);
	}

	void Run()
	{
		m_sequence->run();
	}

private:
	SequencePtr m_sequence;
};


class FallbackStage
{
public:
	
	FallbackStage(SequencePtr sequence) : m_sequence(sequence)
	{		 
	}

	template<typename F>
	TearDownStage TearDown(F& f)
	{
		m_sequence->teardown_stage = f;
		return TearDownStage(m_sequence);
	}

	template<typename F>
	CatchStage Catch(F& f)
	{
		m_sequence->catch_stage = f;
		return CatchStage(m_sequence);
	}

	 void Run()
	 {
		 m_sequence->run();
	 }

private:
	SequencePtr m_sequence;
};

class DoStage 
{
public:
	
	DoStage(SequencePtr sequence) : m_sequence(sequence)
	{		 
	}

	template<typename F>
	FallbackStage Fallback(F& f)
	{
		m_sequence->fallback_stage = f;
		return FallbackStage(m_sequence);
	}

	template<typename F>
	TearDownStage TearDown(F& f)
	{
		m_sequence->teardown_stage = f;
		return TearDownStage(m_sequence);
	}

	 void Run()
	 {
		 m_sequence->run();
	 }

private:
	SequencePtr m_sequence;
};


class PrepareStage 
{
public:
	PrepareStage(SequencePtr sequence) : m_sequence(sequence)
	{		 
	}
	
	template<typename F>
	DoStage Do(F& f)
	{
		m_sequence->do_stage = f;
		return DoStage(m_sequence);
	}
private:
	SequencePtr m_sequence;
};


class Try
{
public:
	template<typename F>
	PrepareStage Prepare(F& f)
	{
		SequencePtr sequence(new Sequence());
		sequence->prepare_stage = f;
		return PrepareStage(sequence);
	}

	template<typename F>
	DoStage Do(F& f)
	{
		SequencePtr sequence(new Sequence());
		sequence->do_stage = f;
		return DoStage(sequence);
	}
};



class ExceptionMsgBox
{
public:
	
	template<typename F> void TryCatchAny(F& f)
	{
		std::function<void ()> functor = f;
		TryCatch(functor);		
	}
	
	void TryCatch(std::function<void ()>& f);

	void Show(std::exception& e);
	void Show(wexception& e);		
};