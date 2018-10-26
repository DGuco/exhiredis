//
// Created by dguco on 18-10-24.
// redis cmd
//

#ifndef EXHIREDIS_COMMAND_H
#define EXHIREDIS_COMMAND_H

#include <future>
#include "robject/robject.hpp"

namespace exhiredis
{
	class CCommandState
	{
	public:
		static const int NO_REPLY = -1;   // No reply yet
		static const int OK_REPLY = 0;    // Successful reply of the expected type
		static const int NIL_REPLY = 1;   // Got a nil reply
		static const int ERROR_REPLY = 2; // Got an error reply
		static const int SEND_ERROR = 3;  // Could not send to server
		static const int WRONG_TYPE = 4;  // Got reply, but it was not the expected type
		static const int TIMEOUT = 5;     // No reply, timed out
	};

	template<class reply_type>
	class CCommand
	{
	public:
		//构造函数
		CCommand(unsigned long id);
		//获取cmd id
		const unsigned long GetCommandId() const;
		//获取cmd reply promise
		const promise<reply_type> &GetPromise() const;
		//获取cmd 状态
		const int &GetCommState() const;
		//设置cmd 状态
		void SetCommState(const int iCommState);
	private:
		const unsigned long m_iCommandId;  //cmd id
		std::atomic_int m_iCommState;
		std::promise<reply_type> m_promise;
	};

	template<class reply_type>
	CCommand<reply_type>::CCommand(unsigned long id)
		:m_iCommandId(id)
	{
		m_iCommState.store(CCommandState::NO_REPLY);
	}

	template<class reply_type>
	const promise<reply_type> &CCommand<reply_type>::GetPromise() const
	{
		return m_promise;
	}

	template<class reply_type>
	const unsigned long CCommand<reply_type>::GetCommandId() const
	{
		return m_iCommandId;
	}

	template<class reply_type>
	const int &CCommand<reply_type>::GetCommState() const
	{
		return m_iCommState.load( );
	}

	template<class reply_type>
	void CCommand<reply_type>::SetCommState(const int iCommState)
	{
		m_iCommState.store(iCommState);
	}
}
#endif //EXHIREDIS_COMMAND_H
