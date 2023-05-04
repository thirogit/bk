// © ABBYY. 2010.
// SAMPLES code is property of ABBYY, exclusive rights are reserved. 
//
// DEVELOPER is allowed to incorporate SAMPLES into his own APPLICATION and modify it under 
// the  terms of  License Agreement between  ABBYY and DEVELOPER.


// ABBYY FineReader Engine 10 Sample

// Helper functions for BSTR handling
#include "stdafx.h"
#include "BstrWrap.h"

void CBstr::freeBuffer()
{
	if( string != 0 ) {
		::SysFreeString( string );
	}
}
