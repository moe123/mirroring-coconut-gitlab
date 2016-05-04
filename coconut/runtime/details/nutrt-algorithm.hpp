//
// nutrt-algorithm.hpp
//
// Copyright (C) 2015-2016 Cucurbita. All rights reserved.
//

#include <coconut/runtime/details/nutrt-defines.hpp>

#ifndef COCONUT_RUNTIME_ALGORITHM_HPP
#define COCONUT_RUNTIME_ALGORITHM_HPP

namespace coconut {
	namespace runtime {
		namespace algorithm {

template <typename CharT>
COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
bool starts_with(
	const std::basic_string<CharT, std::char_traits<CharT>, std::allocator<CharT> > & haystack,
	const std::basic_string<CharT, std::char_traits<CharT>, std::allocator<CharT> > & needle
) {
	return needle.size() <= haystack.size() &&
		std::equal(needle.cbegin(), needle.cend(), haystack.cbegin());
}

template <typename CharT>
COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
bool ends_with(
	const std::basic_string<CharT, std::char_traits<CharT>, std::allocator<CharT> > & haystack,
	const std::basic_string<CharT, std::char_traits<CharT>, std::allocator<CharT> > & needle
) {
	return needle.size() <= haystack.size() &&
		std::equal(needle.crbegin(), needle.crend(), haystack.crbegin());
}

COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
bool is_alpha(char a)
{
	std::locale loc;
	return std::use_facet< std::ctype<char> >(loc).is(std::ctype<char>::alpha, a);
}

COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
bool is_ascii(const std::string & in)
{
	for (std::string::const_iterator it= in.cbegin(); it!= in.cend(); ++it) {
		if (!((static_cast<int>(*it) & ~0x7F) == 0)) { return false; }
	}
	return true;
}

template <typename CharT>
COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
std::basic_string<CharT, std::char_traits<CharT>, std::allocator<CharT> > & ltrim(
	std::basic_string<CharT, std::char_traits<CharT>, std::allocator<CharT> > & s
) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

template <typename CharT>
COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
std::basic_string<CharT, std::char_traits<CharT>, std::allocator<CharT> > & rtrim(
	std::basic_string<CharT, std::char_traits<CharT>, std::allocator<CharT> > & s
) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

template <typename CharT>
COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
std::basic_string<CharT, std::char_traits<CharT>, std::allocator<CharT> > & trim(
	std::basic_string<CharT, std::char_traits<CharT>, std::allocator<CharT> > & s
) {
	return ltrim<std::basic_string<CharT, std::char_traits<CharT>, std::allocator<CharT> > >(
		rtrim<std::basic_string<CharT, std::char_traits<CharT>, std::allocator<CharT> > >(s)
	);
}

COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
std::string to_upper(const std::string & in)
{
	std::locale loc;
	std::string out;
	std::transform(in.cbegin(), in.cend(), back_inserter(out), [&loc](char c) {
		return std::toupper(c, loc);
	});
	return out;
}

COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
std::string to_lower(const std::string & in)
{
	std::locale loc;
	std::string out;
	std::transform(in.cbegin(), in.cend(), back_inserter(out), [&loc](char c) {
		return std::tolower(c, loc);
	});
	return out;
}

COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
bool cmp(const std::string & left, const std::string & right)
{
	return left.compare(right);
}

COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
int icmp(const std::string & left, const std::string & right)
{
	return cmp(to_upper(left), to_upper(right));
}

COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
bool is_integer(const std::string & in, bool is_unsigned = false)
{
	bool result;
	try {
		std::regex regex;
		if (is_unsigned) {
			regex.assign("[[:digit:]]+");
		} else {
			regex.assign("(\\+|-)?[[:digit:]]+");
		}
		result = std::regex_match(in, regex);
	}
	catch (std::regex_error e) { result = false; }
	return result;
}

COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
bool is_integer(const std::wstring & in, bool is_unsigned = false)
{
	bool result;
	try {
		std::wregex regex;
		if (is_unsigned) {
			regex.assign(L"[[:digit:]]+");
		} else {
			regex.assign(L"(\\+|-)?[[:digit:]]+");
		}
		result = std::regex_match(in, regex);
	}
	catch (std::regex_error e) { result = false; }
	return result;
}

COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
bool is_number(const std::string & in)
{
	bool result;
	try {
		result = std::regex_match(in, std::regex("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?"));
	}
	catch (std::regex_error e) { result = false; }
	return result;
}

COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
bool is_number(const std::wstring & in)
{
	bool result;
	try {
		result = std::regex_match(in, std::wregex(L"((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?"));
	}
	catch (std::regex_error e) { result = false; }
	return result;
}

COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
bool is_numeric(const std::string & in)
{
	bool result;
	try {
		result = std::regex_match(in,
			std::regex("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?((e|E)((\\+|-)?)[[:digit:]]+)?"));
	}
	catch (std::regex_error e) { result = false; }
	return result;
}

COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
bool is_numeric(const std::wstring & in)
{
	bool result;
	try {
		result = std::regex_match(in,
			std::wregex(L"((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?((e|E)((\\+|-)?)[[:digit:]]+)?"));
	}
	catch (std::regex_error e) { result = false; }
	return result;
}

template <typename NumT>
COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
std::string to_binary(const NumT & x)
{ std::bitset<(sizeof(x) * CHAR_BIT)> bits(x); return bits.to_string(); }

template <typename NumT, typename StrT>
COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
NumT to_numeric(const StrT & in)
{
	NumT t = 0;
	using char_type = typename StrT::value_type;
	using traits_type = typename StrT::traits_type;
	using allocator_type = typename StrT::allocator_type;
	
	typedef std::basic_stringstream<char_type, traits_type, allocator_type> stringstream_type;
	
	if (in.size() && is_numeric(in)) {
		stringstream_type buf;
		buf << in;
		buf >> t;
	}
	return t;
}

template <typename StrT, typename NumT>
COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
StrT to_string(const NumT & n, std::size_t p = 6)
{
	using char_type = typename StrT::value_type;
	using traits_type = typename StrT::traits_type;
	using allocator_type = typename StrT::allocator_type;
	
	using ostringstream_type = std::basic_ostringstream<char_type, traits_type, allocator_type>;
	
	ostringstream_type ostr;
	ostr.setf(std::ios::fixed, std::ios::floatfield);
	ostr.precision(static_cast<std::streamsize>(p));
	ostr << n;
	return ostr.str();
}

template <typename StrT, typename IterT>
COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
StrT joiner(IterT && beg, IterT && end, const StrT & sep)
{
	using char_type = typename StrT::value_type;
	using traits_type = typename StrT::traits_type;
	using allocator_type = typename StrT::allocator_type;
	
	using ostringstream_type = std::basic_ostringstream<char_type, traits_type, allocator_type>;
	
	ostringstream_type result;

	if (beg != end) {
		result << *beg++;
	}
	while (beg != end) {
		result << sep;
		result << *beg++;
	}
	return result.str();
}

template <typename StrT, typename VecT>
COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
void tokenizer(const StrT & in, VecT & tokens, const StrT & delimiter)
{
	using value_type = typename VecT::value_type;
	using size_type = typename VecT::size_type;
	using string_size_type = typename StrT::size_type;
	
	string_size_type pos, last_pos = 0;
	
	while (true) {
		pos = in.find_first_of(delimiter, last_pos);
		if (pos == StrT::npos) {
			pos = in.length();
			if (pos != last_pos) {
				tokens.push_back(value_type(in.data() + last_pos, static_cast<size_type>(pos) - last_pos ));
			}
			break;
		} else {
			if (pos != last_pos) {
				tokens.push_back(value_type(in.data() + last_pos, static_cast<size_type>(pos) - last_pos ));
			}
		}
		last_pos = pos + 1;
	}
}

template <typename StrT>
COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
std::vector<StrT> split(const StrT & in, const StrT & delimiter)
{
	std::vector<StrT> out;
	tokenizer<StrT>(in, out, delimiter);
	return out;
}

COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
std::size_t explode(
	std::vector<std::string> & out,
	const std::string & delimiter,
	const std::string & in,
	int limit = std::numeric_limits<int>::max()
) {
	std::vector<std::string>::size_type begin = 0;
	std::vector<std::string>::size_type end = 0;
	
	if (delimiter.empty()) {
		return 0;
	}
	if (limit == 0) {
		limit = 1;
	}
	if (in.find(delimiter) == std::string::npos) {
		if (limit > 0) {
			out.push_back(in);
		}
		return out.size();
	}
	
	while (end != std::string::npos) {
		if (limit > 0 && static_cast<int>(out.size()) == limit - 1) {
			out.push_back(in.substr(begin));
			break;
		}
		std::vector<std::string>::size_type substr_len = std::string::npos;
		std::vector<std::string>::size_type next_tok = std::string::npos;
		end = in.find_first_of(delimiter, begin);
		if (end != std::string::npos) {
			substr_len = end - begin;
			next_tok = end + delimiter.size();
		}
		out.push_back(in.substr(begin, substr_len));
		begin = next_tok;
	}
	if (limit < 0) {
		limit = std::abs(limit);
		if (limit < static_cast<int>(out.size())) {
			out.resize(out.size() - static_cast<std::size_t>(limit));
		} else {
			out.clear();
		}
	}
	return out.size();
}

template <typename StrT>
COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
StrT join(const std::vector<StrT> & parts, const StrT & separator)
{ return joiner(parts.cbegin(), parts.cend(), separator); }

template <typename... ArgsT>
COCONUT_PRIVATE COCONUT_ALWAYS_INLINE
std::string format(const char * fmt, ArgsT &&... args)
{
	int sz = std::snprintf(nullptr, 0, fmt, std::forward<ArgsT>(args)...);
	if (sz) {
		std::vector<char> buf(static_cast<std::size_t>(sz));
		std::snprintf(buf.data(), buf.size(), fmt, std::forward<ArgsT>(args)...);
		if (buf.back() == '\0') { buf.pop_back(); }
		return std::string(buf.begin(), buf.end());
	}
	return {};
}

}}} /* EONS */

#endif /* !COCONUT_RUNTIME_ALGORITHM_HPP */

/* EOF */