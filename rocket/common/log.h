#ifndef ROCKET_COMMON_LOG_H
#define ROCKET_COMMON_LOG_H
#include <queue>
#include <string>
#include <memory>
#include "rocket/common/config.h"
#include "rocket/common/mutex.h"

namespace rocket
{

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"

template<typename... Args>
std::string formatString(const char *str,Args&&... args)
{
    int size = snprintf(nullptr,0,str,args...);
    std::string result;
    if(size >0)
    {
        result.resize(size);
        snprintf(&result[0],size+1,str,args...);
    }
    return result;
}

#define DEBUGLOG(str,...)\
    if(rocket::Logger::GetGlobalLogger()->GetLogLevel() <= rocket::Debug)\
    {\
        rocket::Logger::GetGlobalLogger()->pushLog(rocket::LogEvent(rocket::LogLevel::Debug).toString()\
        + "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]\t" + rocket::formatString(str,##__VA_ARGS__) + "\n");\
        rocket::Logger::GetGlobalLogger()->log();\
    }\

#define INFOLOG(str,...)\
    if(rocket::Logger::GetGlobalLogger()->GetLogLevel() <= rocket::Info)\
    {\
        rocket::Logger::GetGlobalLogger()->pushLog(rocket::LogEvent(rocket::LogLevel::Info).toString()\
        + "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]\t" + rocket::formatString(str,##__VA_ARGS__) + "\n");\
        rocket::Logger::GetGlobalLogger()->log();\
    }\

#define ERRORLOG(str,...)\
    if(rocket::Logger::GetGlobalLogger()->GetLogLevel() <= rocket::Error)\
    {\
        rocket::Logger::GetGlobalLogger()->pushLog(rocket::LogEvent(rocket::LogLevel::Error).toString()\
        + "[" + std::string(__FILE__) + ":" + std::to_string(__LINE__) + "]\t" + rocket::formatString(str,##__VA_ARGS__) + "\n");\
        rocket::Logger::GetGlobalLogger()->log();\
    }\

#pragma GCC diagnostic pop

enum LogLevel
{
    Debug = 1,
    Info = 2,
    Error = 3,
    Unknown = 4
};

class Logger
{
public:
    typedef std::shared_ptr<Logger> s_ptr;

    Logger(LogLevel level):m_set_level(level) {}

    void pushLog(const std::string& msg);

    void log();

    LogLevel GetLogLevel() const 
    {
        return m_set_level;
    }

public:
    static Logger* GetGlobalLogger();

    static void InitGlobalLogger();

private:
    Mutex m_mutex;
    LogLevel m_set_level;
    std::queue<std::string> m_buffer;
};

std::string LogLevelToString(LogLevel level);

LogLevel StringToLogLevel(const std::string& level);

class LogEvent
{
    public:

        LogEvent(LogLevel level):m_level(level){}


        std::string getFileName() const 
        {
            return m_file_name;
        }

        LogLevel getLogLevel() const 
        {
        return m_level;
        }

        std::string toString();

    private:
        std::string m_file_name; //文件名
        int32_t m_file_line;     //行号
        int32_t m_pid;           //进程号
        int32_t m_thread_id;     //线程号

        LogLevel m_level;        //日志级别
};

}



#endif