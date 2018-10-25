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
	template<class reply_type>
	class ICommand
	{

	};

	class CCommand: public ICommand<IRobject>
	{
	public:
		CCommand(const string &sCmd, unsigned long id);
		CCommand(const char *cmd, unsigned long id);
		unsigned long GetCommandId() const;
		const promise<IRobject> &GetPromise() const;
	public:
		static unsigned long GenCommandId();
	private:
		static atomic_ulong _id;
		unsigned long m_iCommandId;
		const string m_sCmd;
		std::promise<IRobject> m_promise;
	};

	atomic_ulong CCommand::_id = {0};

	CCommand::CCommand(const string &sCmd, unsigned long id)
		: m_sCmd(sCmd),
		  m_iCommandId(id)
	{

	}

	CCommand::CCommand(const char *cmd, unsigned long id)
		: m_sCmd(cmd),
		  m_iCommandId(id)
	{

	}

	const promise<IRobject> &CCommand::GetPromise() const
	{
		return m_promise;
	}

	unsigned long CCommand::GetCommandId() const
	{
		return m_iCommandId;
	}

	unsigned long CCommand::GenCommandId()
	{
		return ++_id;
	}
}
#endif //EXHIREDIS_COMMAND_H
