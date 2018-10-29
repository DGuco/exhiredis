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

	class CCommand
	{
	public:
		//构造函数
		CCommand(unsigned long id);
		//获取cmd id
		const unsigned long GetCommandId() const;
		//获取cmd reply promise
		shared_ptr<promise<redisReply *>> &GetPromise();
		//获取cmd 状态
		const int GetCommState() const;
		//设置cmd 状态
		void SetCommState(const int iCommState);
	private:
		const unsigned long m_iCommandId;  //cmd id
		std::atomic_int m_iCommState;
		std::shared_ptr<promise<redisReply *>> m_pPromise;
	};

	CCommand::CCommand(unsigned long id)
		: m_iCommandId(id),
		  m_pPromise(std::make_shared<promise<redisReply *>>(std::promise<redisReply *>( )))
	{
		m_iCommState.store(CCommandState::NO_REPLY);
	}

	const unsigned long CCommand::GetCommandId() const
	{
		return m_iCommandId;
	}

	const int CCommand::GetCommState() const
	{
		return m_iCommState.load( );
	}

	void CCommand::SetCommState(const int iCommState)
	{
		m_iCommState.store(iCommState);
	}

	shared_ptr<promise<redisReply *>> &CCommand::GetPromise()
	{
		return m_pPromise;
	}
}
#endif //EXHIREDIS_COMMAND_H
