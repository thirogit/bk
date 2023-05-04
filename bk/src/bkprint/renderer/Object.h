#ifndef __OBJECT_H__
#define __OBJECT_H__

namespace bkprint
{

class Object 
{
public:	
	virtual void Draw(CDC* pDC) = 0;	
	virtual ~Object() = 0
	{

	}
};

}

#endif