//
// Created by dguco on 18-10-29.
// redis lua 脚本管理
//

#ifndef EXHIREDIS_RSCRIPT_HPP
#define EXHIREDIS_RSCRIPT_HPP

#include <list>
#include "exhiredis/redis_conn.hpp"
#include "rmap.hpp"

namespace exhiredis
{
	class RScript
	{
	public:
		RScript(const shared_ptr<CRedisConn> pRedisConn);
	public:
		template<class return_type>
		std::shared_ptr<return_type> Eval(const std::string &script,
										  const std::list<std::string> &keys,
										  const std::list<std::string> &args);

		template<class return_type>
		std::future<std::shared_ptr<return_type>> EvalAsync(const std::string &script,
															const std::list<std::string> &keys,
															const std::list<std::string> &args);
	private:
		std::shared_ptr<CRedisConn> m_pRedisConn;
	};

	template<class return_type>
	std::shared_ptr<return_type> RScript::Eval(const std::string &script,
											   const std::list<std::string> &keys,
											   const std::list<std::string> &args)
	{
		return EvalAsync<return_type>(script, keys, args).get( );

	}

	template<class return_type>
	std::future<std::shared_ptr<return_type>> RScript::EvalAsync(const std::string &script,
																 const std::list<std::string> &keys,
																 const std::list<std::string> &args)
	{
		string scriptCmd = EVAL + to_string(keys.size( ));
		string parm = "";
		for (auto str : keys) {
			parm += str + " ";
		}
		for (auto str : args) {
			parm += str + " ";
		}

		return m_pRedisConn->RedisAsyncCommand<return_type>(scriptCmd.c_str( ),
															script.c_str( ),
															parm);

	}

	RScript::RScript(const shared_ptr<CRedisConn> pRedisConn)
		: m_pRedisConn(pRedisConn)
	{

	}
}
#endif //EXHIREDIS_RSCRIPT_HPP
