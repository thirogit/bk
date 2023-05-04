#ifndef __REPOEXCEPTION_H__
#define __REPOEXCEPTION_H__

#include <exception/TagException.h>

struct tag_RepoException{};

typedef TagException<tag_RepoException> RepoException;


#endif