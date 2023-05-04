#if !defined(__FILEFORMATTARGET_H__)
#define __FILEFORMATTARGET_H__

#include <xercesc/framework/XMLFormatter.hpp>

class FileFormatTarget : public xercesc::XMLFormatTarget 
{
public:

    FileFormatTarget(CFile* pFile);
    ~FileFormatTarget();
    
	virtual void writeChars(const XMLByte* const toWrite
                          , const XMLSize_t      count
                          , xercesc::XMLFormatter* const  formatter);

    virtual void flush();

private:
    
	FileFormatTarget(const FileFormatTarget&);
    FileFormatTarget& operator=(const FileFormatTarget&);

	CFile* m_pTargetFile;

    
};

#endif
