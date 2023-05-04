#include "stdafx.h"
#include "CRUDObject.h"


CRUDObject::CRUDObject(ItemId id) : m_flag(CRUDFlag_None), m_id(id) {
}

ItemId CRUDObject::GetId() const
{
	return m_id;
}

CRUDFlag CRUDObject::GetFlag() const {
	return m_flag;
}

void CRUDObject::ChangeFlag(CRUDFlag flag) {
	if (m_flag == CRUDFlag_None)
	{ 
		m_flag = flag;
	}			
}

