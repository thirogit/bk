#ifndef __DOCUMENTINPUT_H__
#define __DOCUMENTINPUT_H__

#include <stdint.h>

enum DocumentInput : uint32_t
{
	DocumentInput_Hent = 1,
	DocumentInput_InDoc = 2,
	DocumentInput_OutDoc = 4,
	DocumentInput_BuyDoc = 8,
	DocumentInput_SellDoc = 16,
	DocumentInput_Cow = 32,	
	DocumentInput_BuyInvoiceRegular = 64,
	DocumentInput_BuyInvoiceLump = 128,
	DocumentInput_BuyInvoice = 192,	
	DocumentInput_SellInvoiceRegular = 256,
	DocumentInput_SellInvoiceLump = 1024,
	DocumentInput_SellInvoice = 1280,
	DocumentInput_MoveDoc = 2048,
	DocumentInput_None = 0	
};

#endif
