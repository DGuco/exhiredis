//
// Created by dguco on 18-10-24.
//
//

#include "command.h"


namespace exhiredis
{

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