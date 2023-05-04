#ifndef __IDGENERATOR_H__
#define __IDGENERATOR_H__

#include <boost/uuid/uuid_generators.hpp>

class IdGenerator
{
public:
	std::wstring Generate() const;
private:
	mutable boost::uuids::basic_random_generator<boost::mt19937> m_Generator;
};


#endif