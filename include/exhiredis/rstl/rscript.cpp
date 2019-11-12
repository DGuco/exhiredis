//
// Created by dguco on 19-1-3.
//

#include <fstream>
#include "rscript.h"
#include "exhiredis/redis_commands.h"

namespace exhiredis {

    // Rotate an integer value to left.
    inline unsigned int Rol(const unsigned int value,
                              const unsigned int steps)
    {
        return ((value << steps) | (value >> (32 - steps)));
    }

    RScript::RScript(shared_ptr<CConnectionManager> &pRedisConn)
            : m_pConnectionManager(pRedisConn)
    {

    }

    int RScript::EvalReturnInt(const string &script,
                                     const list<string> &keys,
                                     const list<string> &args)
    {
        return EvalScript<int>(script, keys, args);
    }

    future<int> RScript::AsyncEvalReturnInt(const string &script,
                                            const list<string> &keys,
                                            const list<string> &args)
    {
        return AsyncEvalScript<int>(script, keys, args);
    }

    bool RScript::EvalReturnBool(const string &script,
                                             const list<string> &keys,
                                             const list<string> &args)
    {
        return EvalScript<bool>(script, keys, args);
    }

    future<bool> RScript::AsyncEvalReturnBool(const string &script,
                                              const list<string> &keys,
                                              const list<string> &args)
    {
        return AsyncEvalScript<bool>(script, keys, args);
    }

    string RScript::EvalReturnString(const string &script,
                                     const list<string> &keys,
                                    const list<string> &args)
    {
        return EvalScript<string>(script, keys, args);
    }

    future<string> RScript::AsyncEvalReturnString(const string &script,
                                                  const list<string> &keys,
                                                  const list<string> &args)
    {
        return AsyncEvalScript<string>(script, keys, args);
    }

    int RScript::EvalshaReturnInt(const string &script,
                                  const list<string> &keys,
                                  const list<string> &args)
    {
        return EvalshaScript<int>(script, keys, args);
    }

    future<int> RScript::AsyncEvalshaReturnInt(const string &script,
                                               const list<string> &keys,
                                               const list<string> &args)
    {
        return AsyncEvalshaScript<int>(script, keys, args);
    }

    bool RScript::EvalshaReturnBool(const string &script,
                                    const list<string> &keys,
                                    const list<string> &args)
    {
        return EvalshaScript<bool>(script, keys, args);
    }

    future<bool> RScript::AsyncEvalshaReturnBool(const string &script,
                                                 const list<string> &keys,
                                                 const list<string> &args)
    {
        return AsyncEvalshaScript<bool>(script, keys, args);
    }


    string RScript::EvalshaReturnString(const string &script,
                                     const list<string> &keys,
                                     const list<string> &args)
    {
        return EvalshaScript<string>(script, keys, args);
    }

    future<string> RScript::AsyncEvalshaReturnString(const string &script,
                                                  const list<string> &keys,
                                                  const list<string> &args)
    {
        return AsyncEvalshaScript<string>(script, keys, args);
    }

    template<class return_type>
    return_type RScript::EvalScript(const string &scriptCmd,
                                    const list<string> &keys,
                                    const list<string> &args)
    {
        return m_pConnectionManager->ExecuteCommand<return_type>(BuildScriptCmd(CRedisCommands::EVAL,scriptCmd,keys,args));
    }

    template<class return_type>
    future<return_type> RScript::AsyncEvalScript(const string &scriptCmd,
                                                 const list<string> &keys,
                                                 const list<string> &args)
    {
        return  std::async([this,scriptCmd,keys,args]() -> return_type
                           {
                               return m_pConnectionManager->ExecuteCommand<return_type>(BuildScriptCmd(CRedisCommands::EVAL,scriptCmd,keys,args));
                           });
    }

    template<class return_type>
    return_type RScript::EvalScriptFile(const string &path,
                                        const list<string> &keys,
                                        const list<string> &args)
    {
        std::string script_content = ReadFile(path);
        return m_pConnectionManager->ExecuteCommand<return_type>(BuildScriptCmd(CRedisCommands::EVAL,script_content,keys,args));
    }

    template<class return_type>
    future<return_type> RScript::AsyncEvalScriptFile(const string &path,
                                                     const list<string> &keys,
                                                     const list<string> &args)
    {
        return std::async([this,path,keys,args]() -> return_type
                           {
                               std::string script_content = ReadFile(path);
                               m_pConnectionManager->ExecuteCommand<return_type>(BuildScriptCmd(CRedisCommands::EVAL,script_content,keys,args));
                           });
    }


    template<class return_type>
    return_type RScript::EvalshaScript(const string &scriptCmd,
                                        const list<string> &keys,
                                        const list<string> &args)
    {
        unsigned char hash[20];
        char hexstring[41];
        Calc(scriptCmd.c_str(),(int)scriptCmd.size(),hash);
        ToHexString(hash, hexstring);
        string scriptArg;
        scriptArg.assign(hexstring);
        return m_pConnectionManager->ExecuteCommand<return_type>(BuildScriptCmd(CRedisCommands::EVALSHA,scriptArg,keys,args));
    }

    template<class return_type>
    future<return_type> RScript::AsyncEvalshaScript(const string &scriptCmd,
                                                 const list<string> &keys,
                                                 const list<string> &args)
    {
        return  std::async([this,scriptCmd,keys,args]() -> return_type
                           {
                               unsigned char hash[20];
                               char hexstring[41];
                               Calc(scriptCmd.c_str(),(int)scriptCmd.size(),hash);
                               ToHexString(hash, hexstring);
                               string scriptArg;
                               scriptArg.assign(hexstring);
                               m_pConnectionManager->ExecuteCommand<return_type>(BuildScriptCmd(CRedisCommands::EVALSHA,scriptArg,keys,args));
                           });
    }

    template<class return_type>
    return_type RScript::EvalshaScriptFile(const string &path,
                                        const list<string> &keys,
                                        const list<string> &args)
    {
        unsigned char hash[20];
        char hexstring[41];

        std::string script_content = ReadFile(path);
        Calc(script_content.c_str(),(int)script_content.size(),hash);
        ToHexString(hash, hexstring);
        string scriptArg;
        scriptArg.assign(hexstring);
        return m_pConnectionManager->ExecuteCommand<return_type>(BuildScriptCmd(CRedisCommands::EVALSHA,scriptArg,keys,args));
    }

    template<class return_type>
    future<return_type> RScript::AsyncEvalshaScriptFile(const string &path,
                                                     const list<string> &keys,
                                                     const list<string> &args)
    {
        return  std::async([this,path,keys,args]() -> return_type
                           {
                               unsigned char hash[20];
                               char hexstring[41];

                               std::string script_content = ReadFile(path);
                               Calc(script_content.c_str(),(int)script_content.size(),hash);
                               ToHexString(hash, hexstring);
                               string scriptArg;
                               scriptArg.assign(hexstring);
                               m_pConnectionManager->ExecuteCommand<return_type>(BuildScriptCmd(CRedisCommands::EVALSHA,scriptArg,keys,args));
                           });
    }

    vector<string> RScript::BuildScriptCmd(const string redisCmd,
                                           const string scriptArg,
                                           const list<string> &keys,
                                           const list<string> &args)
    {
        std::vector<std::string> commands;
        commands.reserve(3 + keys.size() + args.size());

        commands.push_back(redisCmd);
        commands.push_back(scriptArg);
        commands.push_back(std::to_string(keys.size()));
        commands.insert(commands.end(), keys.begin(), keys.end());
        commands.insert(commands.end(), args.begin(), args.end());
        return  std::move(commands);
    }

    string RScript::ReadFile(const string &path)
    {
        std::ifstream ifs(path.c_str());
        return std::string((std::istreambuf_iterator<char>(ifs)),
                           (std::istreambuf_iterator<char>()));
    }

    // Sets the first 16 integers in the buffert to zero.
    // Used for clearing the W buffert.
    void RScript::ClearWBuffert(unsigned int* buffert)
    {
        for (int pos = 16; --pos >= 0;)
        {
            buffert[pos] = 0;
        }
    }

    void RScript::InnerHash(unsigned int* result, unsigned int* w)
    {
        unsigned int a = result[0];
        unsigned int b = result[1];
        unsigned int c = result[2];
        unsigned int d = result[3];
        unsigned int e = result[4];

        int round = 0;

#define sha1macro(func,val) \
			{ \
                const unsigned int t = Rol(a, 5) + (func) + e + val + w[round]; \
				e = d; \
				d = c; \
				c = Rol(b, 30); \
				b = a; \
				a = t; \
			}

        while (round < 16)
        {
            sha1macro((b & c) | (~b & d), 0x5a827999)
            ++round;
        }
        while (round < 20)
        {
            w[round] = Rol((w[round - 3] ^ w[round - 8] ^ w[round - 14] ^ w[round - 16]), 1);
            sha1macro((b & c) | (~b & d), 0x5a827999)
            ++round;
        }
        while (round < 40)
        {
            w[round] = Rol((w[round - 3] ^ w[round - 8] ^ w[round - 14] ^ w[round - 16]), 1);
            sha1macro(b ^ c ^ d, 0x6ed9eba1)
            ++round;
        }
        while (round < 60)
        {
            w[round] = Rol((w[round - 3] ^ w[round - 8] ^ w[round - 14] ^ w[round - 16]), 1);
            sha1macro((b & c) | (b & d) | (c & d), 0x8f1bbcdc)
            ++round;
        }
        while (round < 80)
        {
            w[round] = Rol((w[round - 3] ^ w[round - 8] ^ w[round - 14] ^ w[round - 16]), 1);
            sha1macro(b ^ c ^ d, 0xca62c1d6)
            ++round;
        }

#undef sha1macro

        result[0] += a;
        result[1] += b;
        result[2] += c;
        result[3] += d;
        result[4] += e;
    }


    void RScript::Calc(const void* src, const int bytelength, unsigned char* hash)
    {
        // Init the result array.
        unsigned int result[5] = { 0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476, 0xc3d2e1f0 };

        // Cast the void src pointer to be the byte array we can work with.
        const unsigned char* sarray = (const unsigned char*) src;

        // The reusable round buffer
        unsigned int w[80];

        // Loop through all complete 64byte blocks.
        const int endOfFullBlocks = bytelength - 64;
        int endCurrentBlock;
        int currentBlock = 0;

        while (currentBlock <= endOfFullBlocks)
        {
            endCurrentBlock = currentBlock + 64;

            // Init the round buffer with the 64 byte block data.
            for (int roundPos = 0; currentBlock < endCurrentBlock; currentBlock += 4)
            {
                // This line will swap endian on big endian and keep endian on little endian.
                w[roundPos++] = (unsigned int) sarray[currentBlock + 3]
                                | (((unsigned int) sarray[currentBlock + 2]) << 8)
                                | (((unsigned int) sarray[currentBlock + 1]) << 16)
                                | (((unsigned int) sarray[currentBlock]) << 24);
            }
            InnerHash(result, w);
        }

        // Handle the last and not full 64 byte block if existing.
        endCurrentBlock = bytelength - currentBlock;
        ClearWBuffert(w);
        int lastBlockBytes = 0;
        for (;lastBlockBytes < endCurrentBlock; ++lastBlockBytes)
        {
            w[lastBlockBytes >> 2] |= (unsigned int) sarray[lastBlockBytes + currentBlock] << ((3 - (lastBlockBytes & 3)) << 3);
        }
        w[lastBlockBytes >> 2] |= 0x80 << ((3 - (lastBlockBytes & 3)) << 3);
        if (endCurrentBlock >= 56)
        {
            InnerHash(result, w);
            ClearWBuffert(w);
        }
        w[15] = bytelength << 3;
        InnerHash(result, w);

        // Store hash in result pointer, and make sure we get in in the correct order on both endian models.
        for (int hashByte = 20; --hashByte >= 0;)
        {
            hash[hashByte] = (result[hashByte >> 2] >> (((3 - hashByte) & 0x3) << 3)) & 0xff;
        }
    }

    void RScript::ToHexString(const unsigned char* hash, char* hexstring)
    {
        const char hexDigits[] = { "0123456789abcdef" };

        for (int hashByte = 20; --hashByte >= 0;)
        {
            hexstring[hashByte << 1] = hexDigits[(hash[hashByte] >> 4) & 0xf];
            hexstring[(hashByte << 1) + 1] = hexDigits[hash[hashByte] & 0xf];
        }
        hexstring[40] = 0;
    }
}