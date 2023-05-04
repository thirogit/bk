#include "stdafx.h"
#include "ItemId.h"

#include <iostream>
#include <sstream>

namespace ItemIdText
{
	std::wstring ToString(const ItemId& id)
	{	
		std::wstringbuf buffer;
		std::wostream os (&buffer);
		std::size_t i=0;

		os << std::hex;
        os.fill(os.widen(L'0'));
        for (ItemId::const_iterator i_data = id.begin(); i_data!=id.end(); ++i_data, ++i) 
		{
            os.width(2);
            os << static_cast<unsigned int>(*i_data);

            if (i == 3 || i == 5 || i == 7 || i == 9) 
			{
                os << os.widen(L'-');
            }
        }
		os.flush();
		return buffer.str();
	}
}
