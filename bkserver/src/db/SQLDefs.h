#ifndef __SQLDEFS_H__
#define __SQLDEFS_H__

/// \brief Alias for 'true', to make code requesting exceptions more
/// readable.
//const bool use_exceptions = true;

/// \brief Appears to be unused!  Remove?
//enum sql_cmp_type {sql_use_compare};

#if defined(NO_LONG_LONGS)
typedef unsigned long ulonglong;
typedef long longlong;
#elif !defined(__GNUC__) && (defined(__WIN32__) || defined(_WIN32))
typedef unsigned __int64 ulonglong;
typedef __int64 longlong;
#else
/// \brief unsigned 64-bit integer type for GCC-based systems
typedef unsigned long long ulonglong;
/// \brief signed 64-bit integer type for GCC-based systems
typedef long long longlong;
#endif

/// \brief Alias for MYSQL_FIELD
//typedef MYSQL_FIELD Field;
/// \brief Contraction for 'const char*'
typedef const char cchar;
#ifndef uint
/// \brief Contraction for 'unsigned int'
typedef unsigned int uint;
#endif



#endif

