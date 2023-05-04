#pragma once

#include "BKService.h"

class BKServiceDestructor
{
public:
	BKServiceDestructor(BKService* pService);
	void Destroy();
private:
	BKService* m_pService;
};