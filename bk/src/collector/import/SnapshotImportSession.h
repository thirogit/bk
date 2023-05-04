#pragma once

#include "items/ImportItem.h"
#include "../data/DownloadSnapshot.h"
#include "../data/DownloadSnapshotHeader.h"
#include "problems/ImportProblem.h"
#include "items/CowImportItem.h"
#include "items/InDocImportItem.h"
#include "items/OutDocImportItem.h"
#include "../../context/Observerable.h"
#include "../../context/SeasonSession.h"
#include <boost/uuid/uuid_generators.hpp>
#include "items/ImportItemVisitor.h"

class SnapshotImportSessionObserver
{
	virtual void OnHeaderUpdated() = 0;
	virtual void OnLonlyCowUpdated(ItemId id) = 0;
	virtual void OnInDocUpdated(ItemId id) = 0;
	virtual void OnInDocCowUpdated(ItemId indocid,ItemId cowid) = 0;
	virtual void OnInDocCowRemoved(ItemId indocid, ItemId cowid) = 0;
	virtual void OnOutDocUpdated(ItemId id) = 0;
	virtual void OnOutDocCowRemoved(ItemId outdocid, ItemId cowid) = 0;
};

class NullImportItemVisitor : public ImportItemVisitor
{
public:
	virtual void Visit(InDocImportItemVisitorFacade* item) {};
	virtual void Visit(CowImportItemVisitorFacade* item) {};
	virtual void Visit(OutDocImportItemVisitorFacade* item) {};
};


class SnapshotImportSession : public Observerable<SnapshotImportSessionObserver>
{
	class SessionImportItemVisitor : public ImportItemVisitor
	{
	public:
		SessionImportItemVisitor(SnapshotImportSession* parent,SeasonSession* session);

		virtual void Visit(InDocImportItemVisitorFacade* item);
		virtual void Visit(CowImportItemVisitorFacade* item);
		virtual void Visit(OutDocImportItemVisitorFacade* item);

		SnapshotImportSession* m_parent;
		SeasonSession* m_session;
	};

	class InDocCowItemVisitor : NullImportItemVisitor
	{
	public:
		InDocCowItemVisitor(InDocImportItemVisitorFacade* indocItem);
		virtual void Visit(CowImportItemVisitorFacade* item);
	private:
		InDocImportItemVisitorFacade* m_indocItem;
	};

	class OutDocCowItemVisitor : NullImportItemVisitor
	{
	public:
		OutDocCowItemVisitor(OutDocImportItemVisitorFacade* outdocItem);
		virtual void Visit(CowImportItemVisitorFacade* item);
	private:
		OutDocImportItemVisitorFacade* m_outdocItem;
	};


public:
	SnapshotImportSession(const DownloadSnapshot* snapshot);
	SnapshotImportSession(SnapshotImportSession&& src);
	SnapshotImportSession(const SnapshotImportSession& src);
	~SnapshotImportSession();
		
	DownloadSnapshotHeader GetSnapshotHeader();
	CowImportItemRange LonelyCowItems();
	InDocImportItemRange InDocItems();
	OutDocImportItemRange OutDocItems();

	void IdentifyProblems(SeasonSession* session);
protected:
		

private:
	DownloadSnapshotHeader m_snapshotHeader;
	std::vector<ImportProblem*> m_problems;
	std::vector<CowImportItem*> m_lonelycows;
	std::vector<InDocImportItem*> m_indocs;
	std::vector<OutDocImportItem*> m_outdocs;

	boost::uuids::basic_random_generator<boost::mt19937> m_IdGenerator;
};
