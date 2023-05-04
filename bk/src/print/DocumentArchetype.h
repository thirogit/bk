#ifndef __DOCUMENTARCHETYPE_H__
#define __DOCUMENTARCHETYPE_H__

#include <boost\function.hpp>
#include <boost/range/any_range.hpp>
#include "../bkprint/template/Template.h"
#include "../bkprint/renderer/PrintOut.h"
#include "DocumentTerritory.h"
#include "../context/SeasonSession.h"
#include "DocumentPredicate.h"
#include "DocumentInput.h"
#include "DocumentInstance.h"
#include "DocumentInterface.h"
#include "IdsInputInterface.h"
#include "KeysInputInterface.h"

class DocumentManagerService;

class DocumentArchetype
{		
	class DocumentInstanceImpl : public DocumentInstance
	{
		friend class DocumentArchetype;
	public:
		DocumentInstanceImpl();
		virtual bkprint::PrintOut Print(IPrintProfile* profile);
		virtual const std::wstring& GetName() const;
		virtual const std::wstring& GetCode() const;
		virtual const DocumentCfgDescription* DescribeConfiguration();
		virtual ~DocumentInstanceImpl();
	private:
		DocumentInterface* m_document;
		DocumentCfgDescription m_cfgDescription;
		std::wstring m_name;
		std::wstring m_code;
	};

protected:
	DocumentArchetype();	
public:
	DocumentArchetype(const DocumentArchetype& src);
	DocumentArchetype(DocumentArchetype&& src);
	~DocumentArchetype();

	const std::wstring& GetName() const;
	const std::wstring& GetCode() const;

	DocumentArchetype& operator=(const DocumentArchetype& right);

	DocumentInstance* CreateDocument(SeasonSession* pSession,
									 DocumentContext* context,
									 const UniqueIds& ids) const;
	DocumentInstance* CreateDocument(SeasonSession* pSession,
									 DocumentContext* context,
									 const UniquePresenceKeys& keys) const;	

	DocumentPredicate* GetPredicate() const;
	
protected:
	std::wstring m_name;
	std::wstring m_code;
	DocumentInput m_inputType;
	boost::function<DocumentInterface* (bkprint::Template* const &)> m_implFactory;
	bkprint::Template* m_template;
	boost::shared_ptr<DocumentPredicate> m_predicate;
	boost::shared_ptr<DocumentCfgDescription> m_cfgDescPtr;

	friend class DocumentManagerService;
};



#endif
