#ifndef __TEMPICONLOADER_H__
#define __TEMPICONLOADER_H__

//////////////////////////////////////////////////////////////////////////////
// CTempIconLoader

class TempIconLoader
{
public:
	TempIconLoader(UINT uResourceID);
	~TempIconLoader();

	operator HICON() const;

protected:
	HICON m_hIcon;
};

#endif