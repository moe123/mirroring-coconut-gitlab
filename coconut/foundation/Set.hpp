//
// Set.hpp
//
// Copyright (C) 2015 Cucurbita. All rights reserved.
//

#include <coconut/runtime/Object.hpp>

#ifndef COCONUT_FOUNDATION_SET_HPP
#define COCONUT_FOUNDATION_SET_HPP

namespace coconut
{	
	COCONUT_PUBLIC class COCONUT_EXPORT Set : public Object
	{
	COCONUT_KDCL(coconut.Set, Object.Set)
		
	public:
		Set();
		Set(const Set & set);
		Set & operator = (const Set & set) = default;
		Set(const Set & set, CopyOption option);
		
		Set(Set && set);

		Set(const std::initializer_list< Owning<Any> > & args);
		Set(const std::initializer_list<Any *> & args);
		
		template <typename IterT>
		Set(IterT && beg, IterT && end) :
			Set(std::forward<IterT>(beg), std::forward<IterT>(end), CopyNone)
		{ /* NOP */ }
		
		template <typename IterT>
		Set(IterT && beg, IterT && end, CopyOption option) :
			Object(SetClass),
			m_impl([] (const Owning<Any> & a, const Owning<Any> & b) -> bool
			{ return (a->compare(*b) != OrderedSame); })
		{
			IterT it = beg;
			while (it != end) {
				if ((*it)) {
					if (option != CopyNone) {
						Owning<Any> copy = Object::copyObject((*it), option);
						if (copy) { m_impl.insert(copy); }
					} else {
						m_impl.insert((*it));
					}
				}
				++it;
			}
		}
		
		virtual ~Set();

		COCONUT_KTOR Owning<Set> with(const Set & set);
		COCONUT_KTOR Owning<Set> with(const Set & set, CopyOption option);
		COCONUT_KTOR Owning<Set> with(Set && set);
		COCONUT_KTOR Owning<Set> with(const std::initializer_list< Owning<Any> > & args);
		COCONUT_KTOR Owning<Set> with(const std::initializer_list<Any *> & args);
		
		template <typename IterT>
		COCONUT_KTOR Owning<Set> with(IterT && beg, IterT && end)
		{ return ptr_create<Set>(std::forward<IterT>(beg), std::forward<IterT>(end)); }
		
		template <typename IterT>
		COCONUT_KTOR Owning<Set> with(IterT && beg, IterT && end, CopyOption option)
		{ return ptr_create<Set>(std::forward<IterT>(beg), std::forward<IterT>(end), option); }
		
		virtual std::size_t hash() const
		COCONUT_FINAL_OVERRIDE;
		
		virtual Owning<Any> copy() const
		COCONUT_FINAL_OVERRIDE;
		
		// virtual Owning<Any> mutableCopy() const
		// COCONUT_FINAL_OVERRIDE;
		
		virtual ComparisonResult compare(const Any & ref) const
		COCONUT_FINAL_OVERRIDE;
		
		virtual bool doesContain(const Any & ref) const
		COCONUT_FINAL_OVERRIDE;
		
		virtual Owning<Any> valueForKey(const std::string & utf8_key) const
		COCONUT_FINAL_OVERRIDE;
		
		virtual Owning<Any> valueForKeyPath(const std::string & utf8_keypath) const
		COCONUT_FINAL_OVERRIDE;
		
		virtual std::string stringValue() const
		COCONUT_FINAL_OVERRIDE;
		
		virtual std::size_t size() const
		COCONUT_FINAL_OVERRIDE;
		
		const Array makeObjectsPerformSelectorKey(const std::string & utf8_selkey, Owning<Any> arg = {}) const;
		
		void enumerateObjectsUsingFunction(const std::function<void(const Owning<Any> & obj, bool & stop)> & func) const;
		void enumerateObjectsUsingFunction(const std::function<void(const Owning<Any> & obj, bool & stop)> & func, EnumerationOptions options) const;
		
		bool containsObject(const Any & obj) const;
		bool containsObject(const Owning<Any> & obj) const;
		
		bool intersectsSet(const Set & set) const;
		bool isSubsetOfSet(const Set & set) const;
		
		Owning<Any> member(const Any & obj) const;
		Owning<Any> member(const Owning<Any> & obj) const;
		
		Owning<Any> anyObject() const;
		Owning<Any> firstObject() const;
		Owning<Any> lastObject() const;
		const Array allObjects(CopyOption option = CopyNone) const;
		
		const Set objectsPassingTest(const std::function<bool(const Owning<Any> & obj, bool & stop)> & func) const;
		const Set objectsPassingTest(const std::function<bool(const Owning<Any> & obj, bool & stop)> & func, EnumerationOptions options) const;
		
		bool everyObjectPassingTest(const std::function<bool(const Owning<Any> & obj, bool & stop)> & func) const;
		bool someObjectPassingTest(const std::function<bool(const Owning<Any> & obj, bool & stop)> & func) const;
		
		const Set filteredSetUsingFunction(const std::function<bool(const Owning<Any> & obj, bool & stop)> & func, CopyOption option = CopyNone) const;
		const Set filteredSetUsingFunction(const std::function<bool(const Owning<Any> & obj, bool & stop)> & func, CopyOption option, EnumerationOptions options) const;
		
		const Set setByAddingObject(Owning<Any> ptr, CopyOption option = CopyNone) const;
		const Set setByAddingObjectsFromSet(const Set & set, CopyOption option = CopyNone) const;
		const Set setByAddingObjectsFromOrderedSet(const OrderedSet & set, CopyOption option = CopyNone) const;
		const Set setByAddingObjectsFromArray(const Array & arr, CopyOption option = CopyNone) const;
		
		template <typename IterT>
		const Set setByAddingObjects(IterT && beg, IterT && end) const
		{ return setByAddingObjectsFromSet(Set(std::forward<IterT>(beg), std::forward<IterT>(end), CopyNone)); }
		
		template <typename IterT>
		const Set setByAddingObjects(IterT && beg, IterT && end, CopyOption option) const
		{ return setByAddingObjectsFromSet(Set(std::forward<IterT>(beg), std::forward<IterT>(end)), option); }
	
	protected:
		typedef std::set<Owning<Any>, std::function<bool(const Owning<Any> & a, const Owning<Any> & b)> > impl_type;
		
	public:
		typedef impl_type::iterator iterator;
		typedef impl_type::const_iterator const_iterator;
		
		typedef impl_type::reverse_iterator reverse_iterator;
		typedef impl_type::const_reverse_iterator const_reverse_iterator;
		
		typedef impl_type::value_type value_type;
		typedef impl_type::size_type size_type;
		typedef impl_type::difference_type difference_type;
	
	public:
		iterator begin();
		iterator end();
		
		const_iterator begin() const;
		const_iterator end() const;
		
		const_iterator cbegin() const;
		const_iterator cend() const;
		
		reverse_iterator rbegin();
		reverse_iterator rend();
		
		const_reverse_iterator rbegin() const;
		const_reverse_iterator rend() const;
		
		const_reverse_iterator crbegin() const;
		const_reverse_iterator crend() const;
	
	protected:
		friend class Object;
		friend class Dictionary;
		
	protected:
		impl_type m_impl;
	};
}

#endif /* !COCONUT_FOUNDATION_SET_HPP */

/* EOF */