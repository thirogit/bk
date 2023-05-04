#include "stdafx.h"
#include "DocumentArchetype.h"


DocumentArchetype::DocumentInstanceImpl::DocumentInstanceImpl() : m_document(NULL)
{
}
 
bkprint::PrintOut DocumentArchetype::DocumentInstanceImpl::Print(IPrintProfile* profile)
{
	bkprint::PrintOut printOut;

	if(m_document)
	{
		printOut = std::move(m_document->Print(profile));
	}

	return std::move(printOut);
}

const std::wstring& DocumentArchetype::DocumentInstanceImpl::GetName() const
{
	return m_name;
}

const std::wstring& DocumentArchetype::DocumentInstanceImpl::GetCode() const
{
	return m_code;
}

const DocumentCfgDescription* DocumentArchetype::DocumentInstanceImpl::DescribeConfiguration()
{
	return &m_cfgDescription;
}

DocumentArchetype::DocumentInstanceImpl::~DocumentInstanceImpl()
{
	if(m_document)
		delete m_document;
}



//---------------------------------------------------------------------------------------
DocumentArchetype::DocumentArchetype()
{
}

DocumentArchetype::DocumentArchetype(const DocumentArchetype& src) : m_name(src.m_name),
																	 m_code(src.m_code),
																	 m_implFactory(src.m_implFactory),
																	 m_template(src.m_template),
																	 m_predicate(src.m_predicate),
																	 m_cfgDescPtr(src.m_cfgDescPtr),
																	 m_inputType(src.m_inputType)
{	


}

DocumentArchetype::DocumentArchetype(DocumentArchetype&& src) : m_name(src.m_name),
																m_code(src.m_code),
																m_implFactory(src.m_implFactory),
																m_template(std::move(src.m_template)),
																m_predicate(src.m_predicate),
																m_cfgDescPtr(src.m_cfgDescPtr)
{
	src.m_name.clear();
	src.m_code.clear();
	src.m_implFactory.clear();	
	src.m_predicate.reset();
	src.m_cfgDescPtr.reset();
	src.m_template = NULL;
}

const std::wstring& DocumentArchetype::GetName() const
{
	return m_name;
}

const std::wstring& DocumentArchetype::GetCode() const
{
	return m_code;
}


DocumentArchetype::~DocumentArchetype()
{
}

DocumentInstance* DocumentArchetype::CreateDocument(SeasonSession* pSession,
													DocumentContext* context,
													const UniquePresenceKeys& keys) const
{
	DocumentInterface* pInterface = m_implFactory(m_template);
	std::auto_ptr<DocumentInterface> guard(pInterface);
	pInterface->SetContext(context);

	KeysInputInterface* pInput = NULL;
	if(pInterface->QueryInputInterface(typeid(KeysInputInterface),(void**)&pInput))
	{
		pInput->Feed(pSession,keys);

		DocumentInstanceImpl* pInstance = new DocumentInstanceImpl();
		pInstance->m_document = pInterface;
		pInstance->m_code = m_code;
		pInstance->m_name = m_name;
		if (m_cfgDescPtr)
		{
			pInstance->m_cfgDescription.CopyOptionDefinitionsFrom(*m_cfgDescPtr);
			pInstance->m_cfgDescription.CopyComparableFieldsFrom(*m_cfgDescPtr);
		}
		guard.release();
		return pInstance;
	}	
	return NULL;
}
	
DocumentInstance* DocumentArchetype::CreateDocument(SeasonSession* pSession,
												     DocumentContext* context,
													const UniqueIds& ids) const
{
	DocumentInterface* pInterface = m_implFactory(m_template);	
	std::auto_ptr<DocumentInterface> guard(pInterface);
	pInterface->SetContext(context);

	IdsInputInterface* pInput = NULL;
	if(pInterface->QueryInputInterface(typeid(IdsInputInterface),(void**)&pInput))
	{
		pInput->Feed(pSession,ids);

		DocumentInstanceImpl* pInstance = new DocumentInstanceImpl();
		pInstance->m_document = pInterface;
		pInstance->m_code = m_code;
		pInstance->m_name = m_name;
		if (m_cfgDescPtr)
		{
			pInstance->m_cfgDescription.CopyOptionDefinitionsFrom(*m_cfgDescPtr);
			pInstance->m_cfgDescription.CopyComparableFieldsFrom(*m_cfgDescPtr);
		}
		guard.release();
		return pInstance;
	}

	delete pInterface;
	return NULL;
}
	
DocumentPredicate* DocumentArchetype::GetPredicate() const
{
	return m_predicate.get();
}
