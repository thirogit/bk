#ifndef __DOCUMENTWITHINPUTINTERFACE_H__
#define __DOCUMENTWITHINPUTINTERFACE_H__

template<class INPUTINTERFACE,class DOCUMENT>
class DocumentWithInputInterface : public DOCUMENT
{
public:
	DocumentWithInputInterface(bkprint::Template* templ) : DOCUMENT(templ, &m_input)
	{
	}
private:
	INPUTINTERFACE m_input;
};


#endif
