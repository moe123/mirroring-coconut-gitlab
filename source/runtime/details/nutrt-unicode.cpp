//
// nutrt-unicode.cpp
//
// Copyright (C) 2015-2016 Cucurbita. All rights reserved.
//

#include <coconut/runtime/details/nutrt-unicode.hpp>

using namespace coconut::runtime;

#include <source/runtime/builtins/nutrt-unicode_wide.hxx>

std::wstring unicode::utf8_to_wide(const std::string & utf8_in)
{
	std::wstring wide_out;

#if defined(__MICROSOFT__) && \
	(defined(COCONUT_SIZEOF_WIDE) && COCONUT_SIZEOF_WIDE == 2)
	
	std::string utf8_out(utf8_in);
	utf8_del_bom(utf8_out);
	builtins::unicode_wide_utf8_to_wide(utf8_out, wide_out);
	
#else
	
#if defined(COCONUT_SIZEOF_WIDE) && COCONUT_SIZEOF_WIDE == 4

#pragma warning WRONG USAGE

	using Char8T = std::string::value_type;
	using Char32T = std::wstring::value_type;
	
	_conv_utf8_to_ucs4<Char8T, Char32T, unicode_conv_default>(utf8_in, wide_out);

#else
	
#pragma warning WRONG USAGE

	using Char8T = std::string::value_type;
	using Char16T = std::wstring::value_type;
	
	_conv_utf8_to_utf16<Char8T, Char16T, unicode_conv_default>(utf8_in, wide_out);

#endif
	
#endif
	return wide_out;
}

std::string unicode::wide_to_utf8(const std::wstring & wide_in)
{
	std::string utf8_out;

#if defined(__MICROSOFT__) && \
	(defined(COCONUT_SIZEOF_WIDE) && COCONUT_SIZEOF_WIDE == 2)
	
	builtins::unicode_wide_wide_to_utf8(wide_in, utf8_out);
	utf8_del_bom(utf8_out);
	
#else

#if defined(COCONUT_SIZEOF_WIDE) && COCONUT_SIZEOF_WIDE == 4

#pragma warning WRONG USAGE
	
	using Char32T = std::wstring::value_type;
	using Char8T = std::string::value_type;
	_conv_ucs4_to_utf8<Char32T, Char8T>(wide_in, utf8_out);
	
#else
	
#pragma warning WRONG USAGE
	
	using Char16T = std::wstring::value_type;
	using Char8T = std::string::value_type;
	_conv_utf16_to_utf8<Char16T, Char8T>(wide_in, utf8_out);
	
#endif

#endif
	return utf8_out;
}

COCONUT_PRIVATE COCONUT_VISIBLE
std::string utf8_to_ansi(const std::string & utf8_in)
{
	std::string ansi_out;
	
#if defined(__MICROSOFT__)
	
	std::string utf8_out(utf8_in);
	utf8_del_bom(utf8_out);
	builtins::unicode_utf8_to_ansi(utf8_out, ansi_out);
	
#else
	
#pragma warning WRONG USAGE
	
	ansi_out.assign(utf8_in);
	
#endif
	return ansi_out;
}

COCONUT_PRIVATE COCONUT_VISIBLE
std::string ansi_to_utf8(const std::string & ansi_in)
{
	std::string utf8_out;
	
#if defined(__MICROSOFT__)
	
	builtins::unicode_ansi_to_utf8(ansi_in, utf8_out);
	utf8_del_bom(utf8_out);
	
#else
	
#pragma warning WRONG USAGE
	
	utf8_out.assign(ansi_in);
	
#endif
	return utf8_out;
}

/* EOF */