#ifndef __LOGOSERVICE_H__
#define __LOGOSERVICE_H__

#include "Logo.h"
#include "../services/AppService.h"

class LogoService : public AppService
{
public:

	LogoService();

	virtual void Start();
	virtual void Stop();
	virtual void Init(ServiceRegistry* registry);

	const Logo& GetLogo() const;
protected:
	void LoadDefaultLogo();
	void LoadInvalidLogo();
	void LoadLogoFromResources(int resourceId);
private:
	Logo m_logo;

};






#endif