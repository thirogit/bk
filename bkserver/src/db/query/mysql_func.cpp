#include "stdafx.h"
#include "mysql_func.h"

size_t escape_string_for_FB(wchar_t *to, size_t to_length, const wchar_t *from, size_t length)
 {
	const wchar_t *to_start = to;
	const wchar_t *end, *to_end = to_start + (to_length ? to_length - 1 : 2 * length);
   bool overflow = false;
 
   for (end= from + length; from < end; from++)
   {
     
     if (*from == L'\'')
     {
       if (to + 2 > to_end)
       {
         overflow= true;
         break;
       }
       *to++= L'\'';
       *to++= L'\'';
     }
     else
     {
       if (to + 1 > to_end)
       {
         overflow= true;
         break;
       }
       *to++= *from;
     }
   }
   *to= 0;
   return overflow ? (size_t)~0 : (size_t)(to - to_start);
 
}
