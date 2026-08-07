#pragma once
#include"Util.hpp"
#include"LogMessage.hpp"
#include"Level.hpp"
#include"Formatter.hpp"
#include"Sink.hpp"
#include<atomic>
#include<mutex>
#include<vector>
#include <cstdarg>

/*
    日志器模块
    1、日志器基类
    2、同步与异步子类
*/
namespace LogModule
{
    class Logger
    {
    public:
        using ptr = std::shared_ptr<Logger>;
        Logger(const std::string& loggername, 
            std::shared_ptr<Formatter>& formatter, 
            LogLevel::Level limitlevel, 
            std::vector<std::shared_ptr<LogSink>>& sinks)
        :_loggername(loggername)
        ,_formatter(formatter)
        ,_limitlevel(limitlevel)
        ,_sinks(sinks.begin(),sinks.end())
        {
        }

        std::string loggerName() { return _loggername; }
        LogLevel::Level loggerLevel() { return _limitlevel; }

        // 用于完成对应等级日志消息的的构建与格式化并输出
        void debug(const std::string& file ,size_t line, const std::string& format, ...)
        {
            // 1、判断当前日志收否达到输出等级
            if(LogLevel::Level::DEBUG < _limitlevel) { return; }

            // 2、对format和不定参进行组织，得到日志消息字符串
            char* res = nullptr;
            va_list al;
            va_start(al,format);
            int n = vasprintf(&res,format.c_str(),al);
            if(n < 0)
            {
                std::cout<<"vasprintf fail!"<<std::endl;
            }
            va_end(al);
            
            Serialize(file,line,LogLevel::Level::DEBUG ,res);
            
            free(res);
            res=nullptr;
        }

        void infor(const std::string& file ,size_t line, const std::string& format, ...)
        {
            if(LogLevel::Level::INFOR < _limitlevel) { return; }

            char* res = nullptr;
            va_list al;
            va_start(al,format);
            int n = vasprintf(&res,format.c_str(),al);
            if(n < 0)
            {
                std::cout<<"vasprintf fail!"<<std::endl;
            }
            va_end(al);
            
            Serialize(file,line,LogLevel::Level::INFOR,res);
            
            free(res);
            res=nullptr;
        }

        void warnning(const std::string& file ,size_t line, const std::string& format, ...)
        {
            if(LogLevel::Level::WARNING < _limitlevel) { return; }

            char* res = nullptr;
            va_list al;
            va_start(al,format);
            int n = vasprintf(&res,format.c_str(),al);
            if(n < 0)
            {
                std::cout<<"vasprintf fail!"<<std::endl;
            }
            va_end(al);
            
            Serialize(file,line,LogLevel::Level::WARNING,res);
            
            free(res);
            res=nullptr;
        }

        void error(const std::string& file ,size_t line, const std::string& format, ...)
        {
            if(LogLevel::Level::ERROR < _limitlevel) { return; }

            char* res = nullptr;
            va_list al;
            va_start(al,format);
            int n = vasprintf(&res,format.c_str(),al);
            if(n < 0)
            {
                std::cout<<"vasprintf fail!"<<std::endl;
            }
            va_end(al);
            
            Serialize(file,line,LogLevel::Level::ERROR,res);
            
            free(res);
            res=nullptr;
        }

        void fatal(const std::string& file ,size_t line, const std::string& format, ...)
        {
            if(LogLevel::Level::FATAL < _limitlevel) { return; }

            char* res = nullptr;
            va_list al;
            va_start(al,format);
            int n = vasprintf(&res,format.c_str(),al);
            if(n < 0)
            {
                std::cout<<"vasprintf fail!"<<std::endl;
            }
            va_end(al);
            
            Serialize(file,line,LogLevel::Level::FATAL,res);
            
            free(res);
            res=nullptr;
        }

    protected:
        void Serialize(const std::string& file ,size_t line ,LogLevel::Level level,const char* res)
        {
            // 3、根据消息字符串构建LogMessage对象
            LogMessage msg(_loggername,file,line,res,level);

            // 4、对LogMessage对象进行格式化，得到格式化字符串
            std::stringstream ss;
            _formatter->format(ss,msg);

            // 5、日志落地
            log(ss.str().c_str(),ss.str().size());
        }

        // 用于实际日志落地输出
        virtual void log(const char* data, size_t len) = 0;
        
    protected:
        std::mutex _lock;
        std::string _loggername;
        std::shared_ptr<Formatter> _formatter;
        std::atomic<LogLevel::Level> _limitlevel;
        std::vector<std::shared_ptr<LogSink>> _sinks;
    };

    class SyncLogger : public Logger
    {
    public:
        SyncLogger(const std::string& loggername,
            std::shared_ptr<Formatter>& formatter, 
            LogLevel::Level limitlevel, 
            std::vector<std::shared_ptr<LogSink>>& sinks)
        :Logger(loggername,formatter,limitlevel,sinks)
        {}

    protected:
        // 同步日志器，是将日志直接通过落地模块句柄进行日志落地
        virtual void log(const char* data, size_t len) override
        {
            std::unique_lock<std::mutex> lock(_lock);
            if(_sinks.empty()) return;
            for(auto& sink:_sinks)
            {
                sink->Log(data,len);
            }
        }
    };
}