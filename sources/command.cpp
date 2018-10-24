//
// Created by dguco on 18-10-24.
//
//

#include "command.h"


namespace exhiredis
{

	unsigned long CCommand::_id = 0l;

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

	IRobject &CCommand::GetObj() const
	{
		return m_oObj;
	}

	unsigned long CCommand::GenCommandId()
	{
		return ++_id;
	}
}