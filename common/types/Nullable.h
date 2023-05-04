#ifndef __NULLABLE_H__
#define __NULLABLE_H__

class Nullable
{
public:
	virtual void MakeNull() = 0;
	virtual bool IsNull() const = 0;
};

#endif