//
// JobBroker.hpp
//
// Copyright (C) 2015-2016 Cucurbita. All rights reserved.
//

#include <coconut/runtime/Types.hpp>

#ifndef COCONUT_RUNTIME_TO_FOUNDATION_JOBBROKER_HPP
#define COCONUT_RUNTIME_TO_FOUNDATION_JOBBROKER_HPP

namespace coconut
{
	template <typename CallableT>
	class COCONUT_VISIBLE JobReturn COCONUT_FINAL
	{
	public:
		JobReturn(const JobReturn &) = delete;
		JobReturn & operator = (const JobReturn &) = delete;
		
		~JobReturn() { /* NOP */ }
		
		JobReturn(JobReturn && jret) noexcept :
			m_fut{std::move(jret.m_fut)}
		{ /* NOP */ }
		
		JobReturn & operator = (JobReturn && jret) noexcept
		{ JobReturn(std::move(jret)).swap(*this); return *this; }
		
		explicit JobReturn(std::future<CallableT> && fut) noexcept :
			m_fut{std::move(fut)}
		{ /* NOP */ }
		
		CallableT operator () () noexcept { return m_fut.get(); }
		
	private:
		std::future<CallableT> m_fut;
	};
	
	template <typename FuncT, typename... ArgsT>
	inline auto _JobExec(JobPolicyOption option, FuncT && func, ArgsT &&... args)
		-> decltype(runtime::async::exec(option, std::forward<FuncT>(func), std::forward<ArgsT>(args)...))
	{ return runtime::async::exec(option, std::forward<FuncT>(func), std::forward<ArgsT>(args)...); }
	
	template <typename FuncT, typename... ArgsT>
	inline auto JobExec(JobPolicyOption option, FuncT && func, ArgsT &&... args)
		-> JobReturn<typename std::result_of<FuncT(ArgsT...)>::type>
	{
		return JobReturn<typename std::result_of<FuncT(ArgsT...)>::type>
		( _JobExec(option, std::forward<FuncT>(func), std::forward<ArgsT>(args)...) );
	}
	
	template <typename FuncT, typename... ArgsT>
	inline auto JobExec(FuncT && func, ArgsT &&... args)
		-> JobReturn<typename std::result_of<FuncT(ArgsT...)>::type>
	{
		return JobReturn<typename std::result_of<FuncT(ArgsT...)>::type>
		( _JobExec(JobPolicyAsync, std::forward<FuncT>(func), std::forward<ArgsT>(args)...) );
	}
	
	template <typename FuncT, typename... ArgsT>
	inline auto JobRun(FuncT && func, ArgsT &&... args)
		-> typename std::result_of<FuncT(ArgsT...)>::type
	{ auto job = JobExec(std::forward<FuncT>(func), std::forward<ArgsT>(args)...); return job(); }
	
	template <typename FuncT, typename... ArgsT>
	inline auto JobDetach(FuncT && func, ArgsT &&... args)
		-> void
	{ runtime::async::detach(std::forward<FuncT>(func), std::forward<ArgsT>(args)...); }
}

#endif /* !COCONUT_RUNTIME_TO_FOUNDATION_JOBBROKER_HPP */

/* EOF */