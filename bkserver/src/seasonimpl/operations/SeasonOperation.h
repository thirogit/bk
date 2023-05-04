#ifndef __SEASONOPERATION_H__
#define __SEASONOPERATION_H__

class SeasonSession;

class SeasonOperation 
{
public:
	SeasonOperation(SeasonSession* session);
	virtual void execute() = 0;
protected:
	SeasonSession* m_session;	
};

#endif
