#include "stdafx.h"
#include "ImageSettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


ImageSettings::ImageSettings()
{
	m_bDoPaletteEnhance = false;
	m_BlackLevel = DEFAULT_BLACKLEVEL;
	m_PaletteCenter = DEFAULT_PALETTECENTER;
	m_PaletteSigma = DEFAULT_PALETTESIGMA;
}

ImageSettings::ImageSettings(const ImageSettings& src)
{
	CopyFrom(src);
}

void ImageSettings::CopyFrom(const ImageSettings& src)
{
	m_bDoPaletteEnhance = src.m_bDoPaletteEnhance;
	m_BlackLevel = src.m_BlackLevel;
	m_PaletteCenter = src.m_PaletteCenter;
	m_PaletteSigma = src.m_PaletteSigma;
}

ImageSettings& ImageSettings::operator=(const ImageSettings& right)
{
	CopyFrom(right);
	return *this;
}

bool ImageSettings::GetDoPaletteEnhance() const
{
	return m_bDoPaletteEnhance;
}

int ImageSettings::GetBlackLevel() const
{
	return m_BlackLevel;
}

int ImageSettings::GetPaletteCenter() const
{
	return m_PaletteCenter;
}

int ImageSettings::GetPaletteSigma() const
{
	return m_PaletteSigma;
}

void ImageSettings::SetDoPaletteEnhance(bool bEnhance)
{
	m_bDoPaletteEnhance = bEnhance;
}

void ImageSettings::SetBlackLevel(int blackLevel)
{
	m_BlackLevel = blackLevel;
}

void ImageSettings::SetPaletteCenter(int center)
{
	m_PaletteCenter = center;
}

void ImageSettings::SetPaletteSigma(int sigma)
{
	m_PaletteSigma = sigma;
}

