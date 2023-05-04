#ifndef __SOLUTIONPARAMETERS_H__
#define __SOLUTIONPARAMETERS_H__

#include <boost/any.hpp>
#include "SolutionType.h"

class SolutionParametersSerializer;

class SolutionParameters
{
public:
	
	SolutionParameters();	
	SolutionParameters(const SolutionParameters& src);
	SolutionParameters(SolutionParameters&& src);
	SolutionParameters& operator=(const SolutionParameters& right);
	void type(SolutionType typee);
	SolutionType type() const;

	template<typename ARG, typename std::enable_if<std::is_pod<ARG>::value, int>::type = 0> ARG parameter_default(ARG defaultValue) const
	{
		ARG* arg = boost::any_cast<ARG>(&m_parameter);
		if (arg == NULL)
			return defaultValue;

		return *arg;
	}

	template<typename ARG> ARG parameter_default(const ARG& defaultValue) const
	{
		const ARG* arg = boost::any_cast<ARG>(&m_parameter);
		if (arg == NULL)
			return defaultValue;

		return *arg;
	}
	
	template<typename ARG> ARG parameter() const
	{
		return boost::any_cast<ARG>(m_parameter);
	}	
	
	template<typename ARG> 	void parameter(ARG& arg)
	{
		m_parameter = boost::any(arg);
	}

	static SolutionParameters make(SolutionType typee);

	template<typename ARG, typename std::enable_if<!std::is_pod<ARG>::value, int>::type = 0>
	static SolutionParameters make(SolutionType typee, ARG& arg)
	{
		SolutionParameters result;
		result.type(typee);
		result.parameter<ARG>(arg);
		return result;		
	}	

	template<typename ARG, typename std::enable_if<std::is_pod<ARG>::value, int>::type = 0>
	static SolutionParameters make(SolutionType typee, ARG arg)
	{
		SolutionParameters result;
		result.type(typee);
		result.parameter<ARG>(arg);
		return result;
	}
	
	

private:
	SolutionType m_type;
	boost::any m_parameter;

	friend class SolutionParametersSerializer;
};



#endif