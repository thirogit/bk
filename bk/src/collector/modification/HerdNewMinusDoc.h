#ifndef __HERDMODIFICATION_H__
#define __HERDMODIFICATION_H__


class HerdModification
{
public:
	virtual void Accept(HerdModificationVisitor* visitor) = 0;
};

#endif
