#pragma once

//#include "ImportProblem.h"
//#include <vector>
//#include <arrays/Enumerator.h>
//#include <boost/uuid/uuid_generators.hpp>
//#include <data/runtime/ItemId.h>
//#include "../../context/SeasonSession.h"
//#include "ImportProblem.h"
//#include "ImportItem.h"
//#include "ImportItemVisitor.h"
//
//class ImportProblemIdentifier : private ImportItemVisitor
//{
//public:
//	ImportProblemIdentifier(SeasonSession* session);
//	~ImportProblemIdentifier();
//	void IdentifyProblems(ImportItem* item);
//
//private:
//	virtual void Visit(InDocImportItem* item);
//	virtual void Visit(CowImportItem* item);
//	virtual void Visit(OutDocImportItem* item);
//	virtual void Visit(SnapshotImportItem* item);
//private:
//	SeasonSession* m_session;
//
//	boost::uuids::basic_random_generator<boost::mt19937> m_IdGenerator;
//};
