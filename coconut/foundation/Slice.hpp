//
// Slice.hpp
//
// Copyright (C) 2015 Cucurbita. All rights reserved.
//

#include <coconut/runtime/Object.hpp>

#ifndef COCONUT_FOUNDATION_SLICE_HPP
#define COCONUT_FOUNDATION_SLICE_HPP

namespace coconut
{
	COCONUT_PUBLIC class COCONUT_EXPORT Slice : public Object
	{
	COCONUT_KDCL(coconut.Slice, Object.Slice)
		
	public:
		Slice();
		Slice(const Slice & slc);
		Slice & operator = (const Slice & slc) = default;
		Slice(std::int64_t start, std::int64_t stop);
		Slice(std::int64_t start, std::int64_t stop, std::int64_t step);
		Slice(const Range & rg);
		virtual ~Slice();
		
		COCONUT_KTOR Owning<Slice> with();
		COCONUT_KTOR Owning<Slice> with(const Slice & slc);
		COCONUT_KTOR Owning<Slice> with(std::int64_t start, std::int64_t stop);
		COCONUT_KTOR Owning<Slice> with(std::int64_t start, std::int64_t stop, std::int64_t step);
		COCONUT_KTOR Owning<Slice> with(const Range & rg);
		
		virtual std::size_t hash() const
		COCONUT_FINAL_OVERRIDE;
		
		virtual Owning<Any> copy() const
		COCONUT_FINAL_OVERRIDE;
		
		virtual ComparisonResult compare(const Any & ref) const
		COCONUT_FINAL_OVERRIDE;
		
		virtual std::string stringValue() const
		COCONUT_FINAL_OVERRIDE;
		
		std::int64_t start() const;
		std::int64_t stop() const;
		std::int64_t step() const;
		
	public:
		friend inline std::ostream & operator << (std::ostream & os, const Owning<Slice> ptr)
		{ if (ptr) { os << ptr->stringValue(); } return os; }
	
	protected:
		friend class Array;
		friend class String;
		
	protected:
		typedef runtime::islice impl_type;
	
	protected:
		impl_type m_impl;
	};
}

#endif /* !COCONUT_FOUNDATION_SLICE_HPP */

/* EOF */