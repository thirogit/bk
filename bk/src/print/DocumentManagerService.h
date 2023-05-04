#ifndef __DOCUMENTMANAGERSERVICE_H__
#define __DOCUMENTMANAGERSERVICE_H__

#include "DocumentManager.h"
#include "../bkprint/template/Template.h"
#include "../logo/LogoService.h"
#include "../services/AppService.h"

class DocumentManagerService : public DocumentManager,public AppService
{
public:
	~DocumentManagerService();
	
	virtual void Start();
	virtual void Stop();
	virtual void Init(ServiceRegistry* registry);
	
	DocumentSelection FindDocuments(uint32_t inpuTypeMask);
protected:
	void DestroyArchetypes();
	bkprint::Template* LoadTemplate(const std::wstring& templateName);
	bkprint::Template* ObtainTemplate(const std::wstring& templateName);

private:
	LogoService* m_logoService;
	std::vector<DocumentArchetype*> m_archetypes;
	std::map<std::wstring, bkprint::Template*> m_templates;
};


#endif
