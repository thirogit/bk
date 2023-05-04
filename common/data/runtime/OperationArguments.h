#ifndef __OPERATIONARGUMENTS_H__
#define __OPERATIONARGUMENTS_H__

#include <boost/any.hpp>
#include <vector>

class SerializableOperationArguments;

class OperationArguments
{
public:
	
	template<typename ARG> 	void add(ARG& arg)
	{
		m_arguments.push_back(boost::any(arg));
	}

	template<typename ARG> ARG get(int argIndex) const
	{
		return boost::any_cast<ARG>(m_arguments[argIndex]);
	}	


	template<typename A1>
	static OperationArguments pack(A1& arg1)
	{
		OperationArguments args;
		args.add<A1>(arg1);
		return args;
	}

	template<typename A1,typename A2>
	static OperationArguments pack(A1& arg1,A2& arg2)
	{
		OperationArguments args;
		args.add<A1>(arg1);
		args.add<A2>(arg2);
		return args;
	}

	template<typename A1,typename A2,typename A3>
	static OperationArguments pack(A1& arg1,A2& arg2,A2& arg3)
	{
		OperationArguments args;
		args.add<A1>(arg1);
		args.add<A2>(arg2);
		args.add<A3>(arg3);
		return args;
	}


	


private:
	std::vector<boost::any> m_arguments;

	friend class SerializableOperationArguments;
};



#endif