//
// Object.hpp
//
// Copyright (C) 2015-2016 Cucurbita. All rights reserved.
//

#if defined(COCONUT_HAVE_PRAGMA_ONCE) && COCONUT_HAVE_PRAGMA_ONCE
	#pragma once
#endif

#include <coconut/runtime/Types.hpp>
#include <coconut/runtime/Features.hpp>
#include <coconut/runtime/JobBroker.hpp>
#include <coconut/runtime/Printifier.hpp>
#include <coconut/runtime/ByteOrder.hpp>

#ifndef COCONUT_RUNTIME_TO_FOUNDATION_OBJECT_HPP
#define COCONUT_RUNTIME_TO_FOUNDATION_OBJECT_HPP

namespace coconut
{
	COCONUT_PROTECTED class COCONUT_ABSTRACT COCONUT_VISIBLE Object : public Any
	{
	COCONUT_CLASSDECLARE(coconut.Object, Any.Object)
		
	public:
	/*
	* Bridge adapter late and early runtime bindings.
	*/
		Object();
		virtual ~Object();
		
	protected:
		Object(const Object & obj);
		Object & operator = (const Object & obj);
		explicit Object(ClassKind kind);
	
	public:
		COCONUT_CLASSMETHOD Owning<Any> copyObject(const Any & obj, CopyOption option);
		COCONUT_CLASSMETHOD Owning<Any> copyObject(const Owning<Any> & obj, CopyOption option);
	
	public:
	/*
	* @iKeyValueCoding compliant.
	*/
		virtual Owning<Any> valueForKey(const std::string & utf8_key) const
		COCONUT_OVERRIDE;
		
		virtual void setValueForKey(Owning<Any> ptr, const std::string & utf8_key)
		COCONUT_OVERRIDE;
		
		virtual Owning<Any> valueForSelectorKey(const std::string & utf8_selkey, Owning<Any> arg = {}) const
		COCONUT_OVERRIDE;
		
	private:
	/*
	* @iKeyValueCoding collection operators.
	*/
		Owning<Any> KVClen(const std::string & utf8_key) const;
		Owning<Any> KVCsum(const std::string & utf8_key) const;
		Owning<Any> KVCmin(const std::string & utf8_key) const;
		Owning<Any> KVCmax(const std::string & utf8_key) const;
		Owning<Any> KVCavg(const std::string & utf8_key) const;
		
		Owning<Any> KVCdistinctUnionOfObjects(const std::string & utf8_key) const;
		Owning<Any> KVCunionOfObjects(const std::string & utf8_key) const;
		Owning<Any> KVCdistinctUnionOfArrays(const std::string & utf8_key) const;
		Owning<Any> KVCdistinctUnionOfOrderedSets(const std::string & utf8_key) const;
		Owning<Any> KVCdistinctUnionOfSets(const std::string & utf8_key) const;
		Owning<Any> KVCunionOfArrays(const std::string & utf8_key) const;
		Owning<Any> KVCunionOfOrderedSets(const std::string & utf8_key) const;
		Owning<Any> KVCunionOfSets(const std::string & utf8_key) const;
	};
}

#endif /* !COCONUT_RUNTIME_TO_FOUNDATION_OBJECT_HPP */

/* EOF */