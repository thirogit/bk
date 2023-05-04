#ifndef __SQLEXCEPTION_H__
#define __SQLEXCEPTION_H__

#include <stdexcept>
#include <exception/TagException.h>

struct tag_SQLException{};


typedef TagException<tag_SQLException> SQLException;


#endif
