#ifndef __FIREBASEEXECPTION_H__
#define __FIREBASEEXECPTION_H__

#include <exception/TagException.h>

namespace gcp
{

	struct tag_FirebaseException {};

	typedef TagException<tag_FirebaseException> FirebaseException;
}

#endif