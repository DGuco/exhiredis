//
// Created by dguco on 18-10-24.
// redis cmd
//

#ifndef EXHIREDIS_COMMAND_H
#define EXHIREDIS_COMMAND_H

#include <future>
#include "robject/robject.h"

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
		IRobject &GetObj() const;
	public:
		static unsigned long GenCommandId();
	private:
		static unsigned long _id;
		unsigned long m_iCommandId;
		const string m_sCmd;
		std::promise<IRobject> m_promise;
		IRobject m_oObj;
	};
}
#endif //EXHIREDIS_COMMAND_H
