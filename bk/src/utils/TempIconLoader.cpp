#include "stdafx.h"
#include "TempIconLoader.h"




TempIconLoader::TempIconLoader(UINT uResourceID)
{	
	m_hIcon = AfxGetApp()->LoadIcon(uResourceID);
}

TempIconLoader::~TempIconLoader()
{
	if (m_hIcon)
		VERIFY( DestroyIcon(m_hIcon) );
}

TempIconLoader::operator HICON()  const
{
	return this == NULL ? NULL : m_hIcon;
}


