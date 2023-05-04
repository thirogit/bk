#ifndef __MYSQL_FUNC_H__ 
#define __MYSQL_FUNC_H__

#define escape_string(s1,s2,ll) escape_string_for_FB(s1, 0, s2, ll);

size_t escape_string_for_FB(wchar_t *to, size_t to_length,const wchar_t *from, size_t length);


#endif