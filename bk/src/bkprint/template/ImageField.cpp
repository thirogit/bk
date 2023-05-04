#include "stdafx.h"
#include "ImageField.h"
#include "util\ColorUtil.h"
#include "unmarshaler\TypesUnmarshaler.h"
#include <b64\decode.h>
namespace bkprint
{

	ImageField::ImageField()
	{
	}

	ImageField::~ImageField()
	{

	}

	Field* ImageField::Clone()
	{
		ImageField* clone = new ImageField();
		clone->CopyFrom(*this);
		return clone;
	}

	void ImageField::CopyFrom(const ImageField& src)
	{
		RectField::CopyFrom(src);
		m_image = src.m_image;
	}


	void ImageField::Unmarshal(pugi::xml_node & node)
	{
		RectField::Unmarshal(node);

		pugi::xml_node datanode = node.child(L"content");
		if (datanode)
		{
			const pugi::char_t* b64data = datanode.text().as_string();		
			size_t maxImgDataDecodedSize = ((wcslen(b64data)*3)/4)+1;
			size_t imgDataSize = 0;
			base64::base64_decodestate state;
			base64::base64_init_decodestate(&state);
			

			BYTE* imgData = new BYTE[maxImgDataDecodedSize];
			const int N = 1024;
			char* code = new char[N];
			char* plaintext = new char[N];
			int codelength;
			int plainlength;

			while (*b64data != L'\0')
			{
				codelength = 0;
				while (codelength < 1024 && *b64data != L'\0')
				{
					code[codelength] = (char)*b64data;
					b64data++;
					codelength++;
				}
			
				plainlength = base64::base64_decode_block(code, codelength, plaintext,&state);
				memcpy(imgData + imgDataSize, plaintext, plainlength);
				imgDataSize += plainlength;
			} 
			
			delete[] code;
			delete[] plaintext;

			fipMemoryIO imgDataIO(imgData, imgDataSize);

			m_image.loadFromMemory(imgDataIO);
			
			delete[] imgData;
		}
		
	}

	void ImageField::Visit(FieldVisitor* visitor)
	{
		visitor->Accept(this);
	}

	const fipWinImage& ImageField::GetImage() const
	{
		return m_image;
	}
}
