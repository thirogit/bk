#include "stdafx.h"
#include "PurchaseHeaderView.h"


PurchaseHeaderView::PurchaseHeaderView(const std::wstring& purchaseId) : m_purchaseId(purchaseId),m_Herd(0),m_CowAmount(0),m_Downloaded(false)
{
}
	
const std::wstring& PurchaseHeaderView::GetPurchaseId() const
{
	return m_purchaseId;	
}

int PurchaseHeaderView::GetHerd() const
{
	return m_Herd;
}

const std::wstring& PurchaseHeaderView::GetAgent() const
{
	return m_Agent;
}

const DateTime& PurchaseHeaderView::GetStartDt() const
{
	return m_StartDt;
}

int PurchaseHeaderView::GetCowAmount() const
{
	return m_CowAmount;
}

bool PurchaseHeaderView::GetDownloaded() const
{
	return m_Downloaded;
}

const std::wstring& PurchaseHeaderView::GetPlateNo() const
{
	return m_PlateNo;
}

void PurchaseHeaderView::SetHerd(int herdIndex)
{
	m_Herd = herdIndex;
}

void PurchaseHeaderView::SetAgent(const std::wstring& agentCd)
{
	m_Agent = agentCd;
}

void PurchaseHeaderView::SetStartDt(const DateTime& startDt)
{
	m_StartDt = startDt;
}

void PurchaseHeaderView::SetCowAmount(int amount)
{
	m_CowAmount = amount;
}

void PurchaseHeaderView::SetDownloaded(bool downloaded)
{
	m_Downloaded = downloaded;
}

void PurchaseHeaderView::SetPlateNo(const std::wstring& plateNo)
{
	m_PlateNo = plateNo;
}
	