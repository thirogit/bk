#ifndef __IMAGESETTINGS_H__
#define __IMAGESETTINGS_H__

#define DEFAULT_BLACKLEVEL 190
#define DEFAULT_PALETTECENTER 255;
#define DEFAULT_PALETTESIGMA 128;

class ImageSettings 
{
public:
	ImageSettings();
	ImageSettings(const ImageSettings& src);
	void CopyFrom(const ImageSettings& src);
	ImageSettings& operator=(const ImageSettings& right);

	bool GetDoPaletteEnhance() const;	
	int GetBlackLevel() const;
	int GetPaletteCenter() const;
	int GetPaletteSigma() const;
	
	void SetDoPaletteEnhance(bool bEnhance);
	void SetBlackLevel(int blackLevel);
	void SetPaletteCenter(int center);
	void SetPaletteSigma(int sigma);

private:
	bool	m_bDoPaletteEnhance;
	int 	m_BlackLevel;
	int 	m_PaletteCenter;
	int 	m_PaletteSigma;

};


#endif