//
// nutrt-unicode.hpp
//
// Copyright (C) 2015-2016 Cucurbita. All rights reserved.
//

#include <coconut/runtime/details/nutrt-types.hpp>

#ifndef COCONUT_RUNTIME_UNICODE_HPP
#define COCONUT_RUNTIME_UNICODE_HPP

namespace coconut
{ namespace runtime
{ namespace unicode
{
	
template<typename CharT1, typename CharT2, typename CodecvtT>
COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
void __conv_from_bytes(
	const std::basic_string<CharT1, std::char_traits<CharT1>, std::allocator<CharT1> > & src,
	std::basic_string<CharT2, std::char_traits<CharT2>, std::allocator<CharT2> > & dest
) {
	std::wstring_convert<CodecvtT, CharT2> conv;
	dest = conv.from_bytes(src);
}

template<typename CharT1, typename CharT2, typename CodecvtT>
COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
void __conv_to_bytes(
	const std::basic_string<CharT1, std::char_traits<CharT1>, std::allocator<CharT1> > & src,
	std::basic_string<CharT2, std::char_traits<CharT2>, std::allocator<CharT2> > & dest
) {
	std::wstring_convert<CodecvtT, CharT1> conv;
	dest = conv.to_bytes(src);
}

template<typename Char8T,
	typename std::enable_if<
		sizeof(Char8T) == sizeof(char), void
	>::type* = nullptr
>
bool __utf8_have_bom(
	const std::basic_string<Char8T, std::char_traits<Char8T>, std::allocator<Char8T> > & in_utf8
) {
	bool have_bom = false;
	if (in_utf8.size() >= 3) {
		if (in_utf8[0] == 0xEF && in_utf8[1] == 0xBB &&  in_utf8[2] == 0xBF) {
			have_bom = true;
		}
	}
	return have_bom;
}
	
template<typename Char8T,
	typename std::enable_if<
		sizeof(Char8T) == sizeof(char), void
	>::type* = nullptr
>
void __utf8_add_bom(
	std::basic_string<Char8T, std::char_traits<Char8T>, std::allocator<Char8T> > & in_utf8
) { if (!__utf8_have_bom(in_utf8)) { in_utf8.insert(0, u8"\xEF\xBB\xBF"); } }
	
template<typename Char8T,
	typename std::enable_if<
		sizeof(Char8T) == sizeof(char), void
	>::type* = nullptr
>
void __utf8_del_bom(
	std::basic_string<Char8T, std::char_traits<Char8T>, std::allocator<Char8T> > & in_utf8
) { if (__utf8_have_bom(in_utf8)) { in_utf8.erase(0, 3); } }
	
template<typename Char8T,
	typename std::enable_if<
		sizeof(Char8T) == sizeof(char), void
	>::type* = nullptr
>
void __utf8_bom(
	std::basic_string<Char8T, std::char_traits<Char8T>, std::allocator<Char8T> > & in_utf8,
	unicode_option option
) {
	switch (option)
	{
		case unicode_conv_del_gen_bom:
		case unicode_conv_gen_bom:
			__utf8_add_bom(in_utf8);
		break;
		default:
			__utf8_del_bom(in_utf8);
		break;
	}
}

template<typename Char16T, typename Char8T,
	typename std::enable_if<
		sizeof(Char16T) == sizeof(char16_t) &&
		sizeof(Char8T) == sizeof(char), void
	>::type* = nullptr
>
COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
void _conv_utf16_to_utf8(
	const std::basic_string<Char16T, std::char_traits<Char16T>, std::allocator<Char16T> > & src,
	std::basic_string<Char8T, std::char_traits<Char8T>, std::allocator<Char8T> > & dest,
	unicode_option option
) {
	using CodecvtT = std::codecvt_utf8_utf16<Char16T>;
	__conv_to_bytes<Char16T, Char8T, CodecvtT>(src, dest);
	__utf8_bom<Char8T>(dest, option);
}

template<typename Char16T, typename Char8T,
	typename std::enable_if<
		sizeof(Char16T) == sizeof(char16_t) &&
		sizeof(Char8T) == sizeof(char), void
	>::type* = nullptr
>
COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
void _conv_utf16_to_utf8(
	const std::basic_string<Char16T, std::char_traits<Char16T>, std::allocator<Char16T> > & src,
	std::basic_string<Char8T, std::char_traits<Char8T>, std::allocator<Char8T> > & dest
) {
	using CodecvtT = std::codecvt_utf8_utf16<Char16T>;
	__conv_to_bytes<Char16T, Char8T, CodecvtT>(src, dest);
}

template<typename Char8T, typename Char16T, unicode_option O>
COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
typename std::enable_if<
	sizeof(Char8T) == sizeof(char) &&
	sizeof(Char16T) == sizeof(char16_t) &&
	O == unicode_conv_default, void
>::type _conv_utf8_to_utf16(
	const std::basic_string<Char8T, std::char_traits<Char8T>, std::allocator<Char8T> > & src,
	std::basic_string<Char16T, std::char_traits<Char16T>, std::allocator<Char16T> > & dest
) {
	using CodecvtT = std::codecvt_utf8_utf16<Char16T>;
	__conv_from_bytes<Char8T, Char16T, CodecvtT>(src, dest);
}
	
template<typename Char8T, typename Char16T, unicode_option O>
COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
typename std::enable_if<
	sizeof(Char8T) == sizeof(char) &&
	sizeof(Char16T) == sizeof(char16_t) &&
	O == unicode_conv_del_gen_bom, void
>::type _conv_utf8_to_utf16(
	const std::basic_string<Char8T, std::char_traits<Char8T>, std::allocator<Char8T> > & src,
	std::basic_string<Char16T, std::char_traits<Char16T>, std::allocator<Char16T> > & dest
) {
	using CodecvtT = std::codecvt_utf8_utf16<
		Char16T, 0x10FFFF, std::codecvt_mode(std::consume_header|std::generate_header)
	>;
	__conv_from_bytes<Char8T, Char16T, CodecvtT>(src, dest);
}
	
template<typename Char8T, typename Char16T, unicode_option O>
COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
typename std::enable_if<
	sizeof(Char8T) == sizeof(char) &&
	sizeof(Char16T) == sizeof(char16_t) &&
	O == unicode_conv_del_bom, void
>::type _conv_utf8_to_utf16(
	const std::basic_string<Char8T, std::char_traits<Char8T>, std::allocator<Char8T> > & src,
	std::basic_string<Char16T, std::char_traits<Char16T>, std::allocator<Char16T> > & dest
) {
	using CodecvtT = std::codecvt_utf8_utf16<
		Char16T, 0x10FFFF, std::codecvt_mode(std::consume_header)
	>;
	__conv_from_bytes<Char8T, Char16T, CodecvtT>(src, dest);
}
	
template<typename Char8T, typename Char16T, unicode_option O>
COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
typename std::enable_if<
	sizeof(Char8T) == sizeof(char) &&
	sizeof(Char16T) == sizeof(char16_t) &&
	O == unicode_conv_gen_bom, void
>::type _conv_utf8_to_utf16(
	const std::basic_string<Char8T, std::char_traits<Char8T>, std::allocator<Char8T> > & src,
	std::basic_string<Char16T, std::char_traits<Char16T>, std::allocator<Char16T> > & dest
) {
	using CodecvtT = std::codecvt_utf8_utf16<
		Char16T, 0x10FFFF, std::codecvt_mode(std::generate_header)
	>;
	__conv_from_bytes<Char8T, Char16T, CodecvtT>(src, dest);
}

template<typename Char8T, typename Char16T,
	typename std::enable_if<
		sizeof(Char8T) == sizeof(char) &&
		sizeof(Char16T) == sizeof(char16_t), void
	>::type* = nullptr
>
COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
void _conv_utf8_to_utf16(
	const std::basic_string<Char8T, std::char_traits<Char8T>, std::allocator<Char8T> > & src,
	std::basic_string<Char16T, std::char_traits<Char16T>, std::allocator<Char16T> > & dest,
	unicode_option option
) {
	switch (option)
	{
		case unicode_conv_del_gen_bom:
			_conv_utf8_to_utf16<Char8T, Char16T, unicode_conv_del_gen_bom>(src, dest);
		break;
		case unicode_conv_del_bom:
			_conv_utf8_to_utf16<Char8T, Char16T, unicode_conv_del_bom>(src, dest);
		break;
		case unicode_conv_gen_bom:
			_conv_utf8_to_utf16<Char8T, Char16T, unicode_conv_gen_bom>(src, dest);
		break;
		default:
			_conv_utf8_to_utf16<Char8T, Char16T, unicode_conv_default>(src, dest);
		break;
	}
}

template<typename Char16T, typename Char8T,
	typename std::enable_if<
		sizeof(Char16T) == sizeof(char16_t) &&
		sizeof(Char8T) == sizeof(char), void
	>::type* = nullptr
>
COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
void _conv_ucs2_to_utf8(
	const std::basic_string<Char16T, std::char_traits<Char16T>, std::allocator<Char16T> > & src,
	std::basic_string<Char8T, std::char_traits<Char8T>, std::allocator<Char8T> > & dest,
	unicode_option option
) {
	using CodecvtT = std::codecvt_utf8<Char16T>;
	__conv_to_bytes<Char16T, Char8T, CodecvtT>(src, dest);
	__utf8_bom<Char8T>(dest, option);
}

template<typename Char16T, typename Char8T,
	typename std::enable_if<
		sizeof(Char16T) == sizeof(char16_t) &&
		sizeof(Char8T) == sizeof(char), void
	>::type* = nullptr
>
COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
void _conv_ucs2_to_utf8(
	const std::basic_string<Char16T, std::char_traits<Char16T>, std::allocator<Char16T> > & src,
	std::basic_string<Char8T, std::char_traits<Char8T>, std::allocator<Char8T> > & dest
) {
	using CodecvtT = std::codecvt_utf8<Char16T>;
	__conv_to_bytes<Char16T, Char8T, CodecvtT>(src, dest);
}
	
template<typename Char8T, typename Char16T, unicode_option O>
COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
typename std::enable_if<
	sizeof(Char8T) == sizeof(char) &&
	sizeof(Char16T) == sizeof(char16_t) &&
	O == unicode_conv_default, void
>::type _conv_utf8_to_ucs2(
	const std::basic_string<Char8T, std::char_traits<Char8T>, std::allocator<Char8T> > & src,
	std::basic_string<Char16T, std::char_traits<Char16T>, std::allocator<Char16T> > & dest
) {
	using CodecvtT = std::codecvt_utf8<Char16T>;
	__conv_from_bytes<Char8T, Char16T, CodecvtT>(src, dest);
}
	
template<typename Char8T, typename Char16T, unicode_option O>
COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
typename std::enable_if<
	sizeof(Char8T) == sizeof(char) &&
	sizeof(Char16T) == sizeof(char16_t) &&
	O == unicode_conv_del_gen_bom, void
>::type _conv_utf8_to_ucs2(
	const std::basic_string<Char8T, std::char_traits<Char8T>, std::allocator<Char8T> > & src,
	std::basic_string<Char16T, std::char_traits<Char16T>, std::allocator<Char16T> > & dest
) {
	using CodecvtT = std::codecvt_utf8<
		Char16T, 0x10FFFF, std::codecvt_mode(std::consume_header|std::generate_header)
	>;
	__conv_from_bytes<Char8T, Char16T, CodecvtT>(src, dest);
}
	
template<typename Char8T, typename Char16T, unicode_option O>
COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
typename std::enable_if<
	sizeof(Char8T) == sizeof(char) &&
	sizeof(Char16T) == sizeof(char16_t) &&
	O == unicode_conv_del_bom, void
>::type _conv_utf8_to_ucs2(
	const std::basic_string<Char8T, std::char_traits<Char8T>, std::allocator<Char8T> > & src,
	std::basic_string<Char16T, std::char_traits<Char16T>, std::allocator<Char16T> > & dest
) {
	using CodecvtT = std::codecvt_utf8<
		Char16T, 0x10FFFF, std::codecvt_mode(std::consume_header)
	>;
	__conv_from_bytes<Char8T, Char16T, CodecvtT>(src, dest);
}
	
template<typename Char8T, typename Char16T, unicode_option O>
COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
typename std::enable_if<
	sizeof(Char8T) == sizeof(char) &&
	sizeof(Char16T) == sizeof(char16_t) &&
	O == unicode_conv_gen_bom, void
>::type _conv_utf8_to_ucs2(
	const std::basic_string<Char8T, std::char_traits<Char8T>, std::allocator<Char8T> > & src,
	std::basic_string<Char16T, std::char_traits<Char16T>, std::allocator<Char16T> > & dest
) {
	using CodecvtT = std::codecvt_utf8<
		Char16T, 0x10FFFF, std::codecvt_mode(std::generate_header)
	>;
	__conv_from_bytes<Char8T, Char16T, CodecvtT>(src, dest);
}

template<typename Char8T, typename Char16T,
	typename std::enable_if<
		sizeof(Char8T) == sizeof(char) &&
		sizeof(Char16T) == sizeof(char16_t), void
	>::type* = nullptr
>
COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
void _conv_utf8_to_ucs2(
	const std::basic_string<Char8T, std::char_traits<Char8T>, std::allocator<Char8T> > & src,
	std::basic_string<Char16T, std::char_traits<Char16T>, std::allocator<Char16T> > & dest,
	unicode_option option
) {
	switch (option)
	{
		case unicode_conv_del_gen_bom:
			_conv_utf8_to_ucs2<Char8T, Char16T, unicode_conv_del_gen_bom>(src, dest);
		break;
		case unicode_conv_del_bom:
			_conv_utf8_to_ucs2<Char8T, Char16T, unicode_conv_del_bom>(src, dest);
		break;
		case unicode_conv_gen_bom:
			_conv_utf8_to_ucs2<Char8T, Char16T, unicode_conv_gen_bom>(src, dest);
		break;
		default:
			_conv_utf8_to_ucs2<Char8T, Char16T, unicode_conv_default>(src, dest);
		break;
	}
}

template<typename Char32T, typename Char8T,
	typename std::enable_if<
		sizeof(Char32T) == sizeof(char32_t) &&
		sizeof(Char8T) == sizeof(char), void
	>::type* = nullptr
>
COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
void _conv_ucs4_to_utf8(
	const std::basic_string<Char32T, std::char_traits<Char32T>, std::allocator<Char32T> > & src,
	std::basic_string<Char8T, std::char_traits<Char8T>, std::allocator<Char8T> > & dest,
	unicode_option option
) {
	using CodecvtT = std::codecvt_utf8<Char32T>;
	__conv_to_bytes<Char32T, Char8T, CodecvtT>(src, dest);
	__utf8_bom<Char8T>(dest, option);
}

template<typename Char32T, typename Char8T,
	typename std::enable_if<
		sizeof(Char32T) == sizeof(char32_t) &&
		sizeof(Char8T) == sizeof(char), void
	>::type* = nullptr
>
COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
void _conv_ucs4_to_utf8(
	const std::basic_string<Char32T, std::char_traits<Char32T>, std::allocator<Char32T> > & src,
	std::basic_string<Char8T, std::char_traits<Char8T>, std::allocator<Char8T> > & dest
) {
	using CodecvtT = std::codecvt_utf8<Char32T>;
	__conv_to_bytes<Char32T, Char8T, CodecvtT>(src, dest);
}
	
template<typename Char8T, typename Char32T, unicode_option O>
COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
typename std::enable_if<
	sizeof(Char8T) == sizeof(char) &&
	sizeof(Char32T) == sizeof(char32_t) &&
	O == unicode_conv_default, void
>::type _conv_utf8_to_ucs4(
	const std::basic_string<Char8T, std::char_traits<Char8T>, std::allocator<Char8T> > & src,
	std::basic_string<Char32T, std::char_traits<Char32T>, std::allocator<Char32T> > & dest
) {
	using CodecvtT = std::codecvt_utf8<Char32T>;
	__conv_from_bytes<Char8T, Char32T, CodecvtT>(src, dest);
}
	
template<typename Char8T, typename Char32T, unicode_option O>
COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
typename std::enable_if<
	sizeof(Char8T) == sizeof(char) &&
	sizeof(Char32T) == sizeof(char32_t) &&
	O == unicode_conv_del_gen_bom, void
>::type _conv_utf8_to_ucs4(
	const std::basic_string<Char8T, std::char_traits<Char8T>, std::allocator<Char8T> > & src,
	std::basic_string<Char32T, std::char_traits<Char32T>, std::allocator<Char32T> > & dest
) {
	using CodecvtT = std::codecvt_utf8<
		Char32T, 0x10FFFF, std::codecvt_mode(std::consume_header|std::generate_header)
	>;
	__conv_from_bytes<Char8T, Char32T, CodecvtT>(src, dest);
}
	
template<typename Char8T, typename Char32T, unicode_option O>
COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
typename std::enable_if<
	sizeof(Char8T) == sizeof(char) &&
	sizeof(Char32T) == sizeof(char32_t) &&
	O == unicode_conv_del_bom, void
>::type _conv_utf8_to_ucs4(
	const std::basic_string<Char8T, std::char_traits<Char8T>, std::allocator<Char8T> > & src,
	std::basic_string<Char32T, std::char_traits<Char32T>, std::allocator<Char32T> > & dest
) {
	using CodecvtT = std::codecvt_utf8<
		Char32T, 0x10FFFF, std::codecvt_mode(std::consume_header)
	>;
	__conv_from_bytes<Char8T, Char32T, CodecvtT>(src, dest);
}
	
template<typename Char8T, typename Char32T, unicode_option O>
COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
typename std::enable_if<
	sizeof(Char8T) == sizeof(char) &&
	sizeof(Char32T) == sizeof(char32_t) &&
	O == unicode_conv_gen_bom, void
>::type _conv_utf8_to_ucs4(
	const std::basic_string<Char8T, std::char_traits<Char8T>, std::allocator<Char8T> > & src,
	std::basic_string<Char32T, std::char_traits<Char32T>, std::allocator<Char32T> > & dest
) {
	using CodecvtT = std::codecvt_utf8<
		Char32T, 0x10FFFF, std::codecvt_mode(std::generate_header)
	>;
	__conv_from_bytes<Char8T, Char32T, CodecvtT>(src, dest);
}

template<typename Char8T, typename Char32T,
	typename std::enable_if<
		sizeof(Char8T) == sizeof(char) &&
		sizeof(Char32T) == sizeof(char32_t), void
	>::type* = nullptr
>
COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
void _conv_utf8_to_ucs4(
	const std::basic_string<Char8T, std::char_traits<Char8T>, std::allocator<Char8T> > & src,
	std::basic_string<Char32T, std::char_traits<Char32T>, std::allocator<Char32T> > & dest,
	unicode_option option
) {
	switch (option)
	{
		case unicode_conv_del_gen_bom:
			_conv_utf8_to_ucs4<Char8T, Char32T, unicode_conv_del_gen_bom>(src, dest);
		break;
		case unicode_conv_del_bom:
			_conv_utf8_to_ucs4<Char8T, Char32T, unicode_conv_del_bom>(src, dest);
		break;
		case unicode_conv_gen_bom:
			_conv_utf8_to_ucs4<Char8T, Char32T, unicode_conv_gen_bom>(src, dest);
		break;
		default:
			_conv_utf8_to_ucs4<Char8T, Char32T, unicode_conv_default>(src, dest);
		break;
	}
}
	
COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
std::u16string utf8_to_utf16(
	const std::string & utf8_in,
	unicode_option option
) {
	std::u16string utf16_out;
	
	using Char8T = std::string::value_type;
	using Char16T = std::u16string::value_type;
	
	_conv_utf8_to_utf16<Char8T, Char16T>(utf8_in, utf16_out, option);
	return utf16_out;
}
	
COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
std::u16string utf8_to_utf16(const std::string & utf8_in)
{
	std::u16string utf16_out;
	
	using Char8T = std::string::value_type;
	using Char16T = std::u16string::value_type;
	
	_conv_utf8_to_utf16<Char8T, Char16T, unicode_conv_default>(utf8_in, utf16_out);
	return utf16_out;
}

COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
std::u16string utf8_to_ucs2(
	const std::string & utf8_in,
	unicode_option option
) {
	std::u16string ucs2_out;
	
	using Char8T = std::string::value_type;
	using Char16T = std::u16string::value_type;
	
	_conv_utf8_to_ucs2<Char8T, Char16T>(utf8_in, ucs2_out, option);
	return ucs2_out;
}

COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
std::u16string utf8_to_ucs2(const std::string & utf8_in) {
	std::u16string ucs2_out;
	
	using Char8T = std::string::value_type;
	using Char16T = std::u16string::value_type;
	
	_conv_utf8_to_ucs2<Char8T, Char16T, unicode_conv_default>(utf8_in, ucs2_out);
	return ucs2_out;
}

COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
std::u32string utf8_to_ucs4(
	const std::string & utf8_in,
	unicode_option option
) {
	std::u32string ucs4_out;
	
	using Char8T = std::string::value_type;
	using Char32T = std::u32string::value_type;
	
	_conv_utf8_to_ucs4<Char8T, Char32T>(utf8_in, ucs4_out, option);
	return ucs4_out;
}

COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
std::u32string utf8_to_ucs4(const std::string & utf8_in)
{
	std::u32string ucs4_out;
	
	using Char8T = std::string::value_type;
	using Char32T = std::u32string::value_type;
	
	_conv_utf8_to_ucs4<Char8T, Char32T, unicode_conv_default>(utf8_in, ucs4_out);
	return ucs4_out;
}

COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
std::u32string utf8_to_utf32(
	const std::string & utf8_in,
	unicode_option option
) { return utf8_to_ucs4(utf8_in, option); }

COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
std::u32string utf8_to_utf32(const std::string & utf8_in)
{ return utf8_to_ucs4(utf8_in); }

COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
std::string utf16_to_utf8(
	const std::u16string & utf16_in,
	unicode_option option
) {
	std::string utf8_out;
	
	using Char16T = std::u16string::value_type;
	using Char8T = std::string::value_type;
	
	_conv_utf16_to_utf8<Char16T, Char8T>(utf16_in, utf8_out, option);
	return utf8_out;
}

COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
std::string utf16_to_utf8(const std::u16string & utf16_in)
{
	std::string utf8_out;
	
	using Char16T = std::u16string::value_type;
	using Char8T = std::string::value_type;
	
	_conv_utf16_to_utf8<Char16T, Char8T>(utf16_in, utf8_out);
	return utf8_out;
}

COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
std::string ucs2_to_utf8(
	const std::u16string & in_ucs2,
	unicode_option option
) {
	std::string utf8_out;
	
	using Char16T = std::u16string::value_type;
	using Char8T = std::string::value_type;
	
	_conv_ucs2_to_utf8<Char16T, Char8T>(in_ucs2, utf8_out, option);
	return utf8_out;
}

COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
std::string ucs2_to_utf8(const std::u16string & in_ucs2)
{
	std::string utf8_out;
	
	using Char16T = std::u16string::value_type;
	using Char8T = std::string::value_type;
	
	_conv_ucs2_to_utf8<Char16T, Char8T>(in_ucs2, utf8_out);
	return utf8_out;
}

COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
std::u16string ucs2_to_utf16(
	const std::u16string & in_ucs2,
	unicode_option option
)
{ return utf8_to_utf16(ucs2_to_utf8(in_ucs2, unicode_conv_del_bom), option); }

COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
std::u16string ucs2_to_utf16(const std::u16string & in_ucs2)
{ return utf8_to_utf16(ucs2_to_utf8(in_ucs2, unicode_conv_del_bom)); }

COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
std::string ucs4_to_utf8(
	const std::u32string & in_ucs4,
	unicode_option option
) {
	std::string utf8_out;
	
	using Char32T = std::u32string::value_type;
	using Char8T = std::string::value_type;
	
	_conv_ucs4_to_utf8<Char32T, Char8T>(in_ucs4, utf8_out, option);
	return utf8_out;
}

COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
std::string ucs4_to_utf8(const std::u32string & in_ucs4)
{
	std::string utf8_out;
	
	using Char32T = std::u32string::value_type;
	using Char8T = std::string::value_type;
	
	_conv_ucs4_to_utf8<Char32T, Char8T>(in_ucs4, utf8_out);
	return utf8_out;
}

COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
std::string utf32_to_utf8(
	const std::u32string & in_utf32,
	unicode_option option
) { return ucs4_to_utf8(in_utf32, option); }

COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
std::string utf32_to_utf8(const std::u32string & in_utf32)
{ return ucs4_to_utf8(in_utf32); }
	
COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
std::string & utf8_add_bom(std::string & in_utf8)
{ __utf8_add_bom(in_utf8); return in_utf8; }
	
COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
std::string & utf8_del_bom(std::string & in_utf8)
{ __utf8_del_bom(in_utf8); return in_utf8; }

COCONUT_PRIVATE COCONUT_VISIBLE
std::wstring utf8_to_wide(const std::string & utf8_in);
	
COCONUT_PRIVATE COCONUT_VISIBLE
std::string wide_to_utf8(const std::wstring & wide_in);

COCONUT_PRIVATE COCONUT_VISIBLE
std::string utf8_to_ansi(const std::string & utf8_in);
	
COCONUT_PRIVATE COCONUT_VISIBLE
std::string ansi_to_utf8(const std::string & ansi_in);
	
}}} /* EONS */

#endif /* !COCONUT_RUNTIME_UNICODE_HPP */

/* EOF */