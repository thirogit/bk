#include "stdafx.h"
#include "WildcardMatch.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <cwchar>
#include <cwctype>

#define	EOS	L'\0'

static const wchar_t *rangematch(const wchar_t *, wchar_t, int);

int wildmatch(const wchar_t *pattern, const wchar_t *string, int flags)
{
	const wchar_t *stringstart;
	wchar_t c, test;

	for (stringstart = string;;)
		switch (c = *pattern++) {
		case EOS:
			if ((flags & FNM_LEADING_DIR) && *string == L'/')
				return (0);
			return (*string == EOS ? 0 : FNM_NOMATCH);
		case L'?':
			if (*string == EOS)
				return (FNM_NOMATCH);
			if (*string == L'/' && (flags & FNM_PATHNAME))
				return (FNM_NOMATCH);
			if (*string == L'.' && (flags & FNM_PERIOD) &&
			    (string == stringstart ||
			    ((flags & FNM_PATHNAME) && *(string - 1) == L'/')))
				return (FNM_NOMATCH);
			++string;
			break;
		case L'*':
			c = *pattern;
			/* Collapse multiple stars. */
			while (c == L'*')
				c = *++pattern;

			if (*string == L'.' && (flags & FNM_PERIOD) &&
			    (string == stringstart ||
			    ((flags & FNM_PATHNAME) && *(string - 1) == L'/')))
				return (FNM_NOMATCH);

			/* Optimize for pattern with * at end or before /. */
			if (c == EOS)
				if (flags & FNM_PATHNAME)
					return ((flags & FNM_LEADING_DIR) ||
					    std::wcschr(string, L'/') == NULL ?
					    0 : FNM_NOMATCH);
				else
					return (0);
			else if (c == L'/' && flags & FNM_PATHNAME) {
				if ((string = wcschr(string, L'/')) == NULL)
					return (FNM_NOMATCH);
				break;
			}

			/* General case, use recursion. */
			while ((test = *string) != EOS) {
				if (!wildmatch(pattern, string, flags & ~FNM_PERIOD))
					return (0);
				if (test == '/' && flags & FNM_PATHNAME)
					break;
				++string;
			}
			return (FNM_NOMATCH);
		case L'[':
			if (*string == EOS)
				return (FNM_NOMATCH);
			if (*string == '/' && flags & FNM_PATHNAME)
				return (FNM_NOMATCH);
			if ((pattern =
			    rangematch(pattern, *string, flags)) == NULL)
				return (FNM_NOMATCH);
			++string;
			break;
		case '\\':
			if (!(flags & FNM_NOESCAPE)) {
				if ((c = *pattern++) == EOS) {
					c = '\\';
					--pattern;
				}
			}
			/* FALLTHROUGH */
		default:
			if (c == *string)
				;
			else if ((flags & FNM_CASEFOLD) &&
				 (towlower(c) ==
				  towlower(*string)))
				;
			else if ((flags & FNM_PREFIX_DIRS) && *string == EOS &&
			     ((c == '/' && string != stringstart) ||
			     (string == stringstart+1 && *stringstart == '/')))
				return (0);
			else
				return (FNM_NOMATCH);
			string++;
			break;
		}
	/* NOTREACHED */
}

static const wchar_t *
rangematch(const wchar_t *pattern, wchar_t test, int flags)
{
	int negate, ok;
	wchar_t c, c2;

	/*
	 * A bracket expression starting with an unquoted circumflex
	 * character produces unspecified results (IEEE 1003.2-1992,
	 * 3.13.2).  This implementation treats it like '!', for
	 * consistency with the regular expression syntax.
	 * J.T. Conklin (conklin@ngai.kaleida.com)
	 */
	if ( (negate = (*pattern == '!' || *pattern == '^')) )
		++pattern;

	if (flags & FNM_CASEFOLD)
		test = towlower(test);

	for (ok = 0; (c = *pattern++) != ']';) {
		if (c == '\\' && !(flags & FNM_NOESCAPE))
			c = *pattern++;
		if (c == EOS)
			return (NULL);

		if (flags & FNM_CASEFOLD)
			c = towlower(c);

		if (*pattern == '-'
		    && (c2 = *(pattern+1)) != EOS && c2 != ']') {
			pattern += 2;
			if (c2 == '\\' && !(flags & FNM_NOESCAPE))
				c2 = *pattern++;
			if (c2 == EOS)
				return (NULL);

			if (flags & FNM_CASEFOLD)
				c2 = towlower(c2);

			if ((unsigned char)c <= (unsigned char)test &&
			    (unsigned char)test <= (unsigned char)c2)
				ok = 1;
		} else if (c == test)
			ok = 1;
	}
	return (ok == negate ? NULL : pattern);
}
