//
// Created by dguco on 18-10-29.
// redis lua 脚本管理
//

#ifndef EXHIREDIS_RSCRIPT_HPP
#define EXHIREDIS_RSCRIPT_HPP

#include <list>
#include "exhiredis/redis_connection.hpp"
#include "rmap.hpp"

namespace exhiredis
{
	class RScript
	{
	public:
		RScript(const shared_ptr<CRedisConnection> pRedisConn);
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
		std::shared_ptr<CRedisConnection> m_pRedisConn;
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
		string scriptCmd = redis_commands::EVAL + to_string(keys.size( )) + " ";
		for (auto str : keys) {
			scriptCmd += str + " ";
		}
		for (auto str : args) {
			scriptCmd += str + " ";
		}

		return m_pRedisConn->RedisAsyncCommand<return_type>(scriptCmd.c_str( ),
															script.c_str( ));

	}

	RScript::RScript(const shared_ptr<CRedisConnection> pRedisConn)
		: m_pRedisConn(pRedisConn)
	{

	}
}
#endif //EXHIREDIS_RSCRIPT_HPP
