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
		/**
		 *
		 * @param script
		 * @param keys
		 * @param args
		 * @return lua script return int value
		 */
		std::shared_ptr<long long> EvalReturnInt(const std::string &script,
												 const std::list<std::string> &keys,
												 const std::list<std::string> &args);

		/**
		 *
		 * @param script
		 * @param keys
		 * @param args
		 * @return lua script async return int value
		 */
		std::future<std::shared_ptr<long long>> EvalReturnIntAsync(const std::string &script,
																   const std::list<std::string> &keys,
																   const std::list<std::string> &args);

		/**
		 *
		 * @param script
		 * @param keys
		 * @param args
		 * @return lua script return bool value
		 */
		std::shared_ptr<bool> EvalReturnBool(const std::string &script,
											 const std::list<std::string> &keys,
											 const std::list<std::string> &args);

		/**
		 *
		 * @param script
		 * @param keys
		 * @param args
		 * @return lua script async return bool value
		 */
		std::future<std::shared_ptr<bool>> EvalReturnBoolAsync(const std::string &script,
															   const std::list<std::string> &keys,
															   const std::list<std::string> &args);

		/**
		 *
		 * @tparam return_type
		 * @param script
		 * @param keys
		 * @param args
		 * @return lua script return return_type value
		 */
		template<class return_type>
		std::shared_ptr<return_type> Eval(const std::string &script,
										  const std::list<std::string> &keys,
										  const std::list<std::string> &args);

		/**
		 *
		 * @tparam return_type
		 * @param script
		 * @param keys
		 * @param args
		 * @return lua script async return return_type value
		 */
		template<class return_type>
		std::future<std::shared_ptr<return_type>> EvalAsync(const std::string &script,
															const std::list<std::string> &keys,
															const std::list<std::string> &args);
	private:
		const string BuildScriptCmd(const std::list<std::string> &keys, const std::list<std::string> &args);
	private:
		std::shared_ptr<CRedisConnection> m_pRedisConn;
	};

	RScript::RScript(const shared_ptr<CRedisConnection> pRedisConn)
		: m_pRedisConn(pRedisConn)
	{

	}

	std::shared_ptr<long long> RScript::EvalReturnInt(const std::string &script,
													  const std::list<std::string> &keys,
													  const std::list<std::string> &args)
	{
		return EvalReturnIntAsync(script, keys, args).get( );
	}

	std::future<std::shared_ptr<long long>> RScript::EvalReturnIntAsync(const std::string &script,
																		const std::list<std::string> &keys,
																		const std::list<std::string> &args)
	{
		return m_pRedisConn->RedisAsyncReturnIntCommand(BuildScriptCmd(keys, args).c_str( ),
														script.c_str( ));
	}

	std::shared_ptr<bool> RScript::EvalReturnBool(const std::string &script,
												  const std::list<std::string> &keys,
												  const std::list<std::string> &args)
	{
		return EvalReturnBoolAsync(script, keys, args).get( );
	}

	std::future<std::shared_ptr<bool>> RScript::EvalReturnBoolAsync(const std::string &script,
																	const std::list<std::string> &keys,
																	const std::list<std::string> &args)
	{
		return m_pRedisConn->RedisAsyncReturnBoolCommand(BuildScriptCmd(keys, args).c_str( ),
														 script.c_str( ));
	}

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
		return m_pRedisConn->RedisAsyncCommand<return_type>(BuildScriptCmd(keys, args).c_str( ),
															script.c_str( ));
	}

	const string RScript::BuildScriptCmd(const std::list<std::string> &keys, const std::list<std::string> &args)
	{
		string scriptCmd = redis_commands::EVAL + to_string(keys.size( )) + " ";
		for (auto str : keys) {
			scriptCmd += (" " + str);
		}
		for (auto str : args) {
			scriptCmd += (" " + str);
		}
		return scriptCmd;
	}
}
#endif //EXHIREDIS_RSCRIPT_HPP
