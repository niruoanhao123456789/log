#pragma once
#include<iostream>
#include<string>
#include<memory>
#include<sstream>
#include<fstream> 
#include<cassert>
#include<ctime>
#include"Util.hpp"
#include"LogMessage.hpp"
#include"Formatter.hpp"

namespace LogModule
{
    // 日志落地
    class LogSink
    {
    public:
        virtual ~LogSink() {}
        virtual void Log(const char* data = nullptr,size_t len = 0) = 0;
    };

    // 标准输出
    class StdOutSink : public LogSink
    {
        // 将日志消息写入标准输出
        void Log(const char* data = nullptr,size_t len = 0) override
        {
            std::cout.write(data,len);
        }
    };

    // 指定文件输出
    class FileSink : public LogSink
    {
    public:
        /***/
        FileSink(const std::string& pathname)
        :_pathname(pathname)
        {
            // 创建指定文件目录
            util::File::CreateDirectory(util::File::GetPath(_pathname));
            // 在该目录下创建并打开日志文件
            _ofs.open(_pathname,std::ios::binary | std::ios::app);
            assert(_ofs.is_open());
        }

        const std::string& File() { return _pathname; }
        void Log(const char* data = nullptr,size_t len = 0) override
        {
            _ofs.write(data,len);
            assert(_ofs.good());
        }

    private:
        std::string _pathname;
        std::ofstream _ofs;
    };

    // 滚动文件的方式输出 可文件大小、时间等方式滚动
    class RollBySizeSink : public LogSink
    {
    public:
        // 构造时传入文件并打开
        /***/
        RollBySizeSink(const std::string& basename,size_t maxsize = 1024*1024)
        :_basename(basename)
        ,_maxsize(maxsize)
        ,_cursize(0)
        ,_recount(0)
        {
            std::string pathname = CreateNewFile();
            // 创建指定文件目录
            util::File::CreateDirectory(util::File::GetPath(pathname));
            // 在该目录下创建并打开日志文件
            _ofs.open(pathname,std::ios::binary | std::ios::app);
            assert(_ofs.is_open());
        }

        void Log(const char* data = nullptr,size_t len = 0) override
        {
            if(_cursize >= _maxsize)
            {
                _ofs.close();
                std::string pathname = CreateNewFile();
                // 创建指定文件目录
                util::File::CreateDirectory(util::File::GetPath(pathname));
                // 在该目录下创建并打开日志文件
                _ofs.open(pathname,std::ios::binary | std::ios::app);
                assert(_ofs.is_open());
                _cursize = 0;
            }
            _ofs.write(data,len);
            _cursize += len;
            assert(_ofs.good());
        }

    private:
        // 进行文件大小检查，超过指定大小则创建新文件
        std::string CreateNewFile()
        {
            time_t time = util::Date::GetCurTime();
            struct tm t;
            localtime_r(&time,&t);
            std::string filename = _basename
            + std::to_string(t.tm_year + 1900)
            + std::to_string(t.tm_mon + 1)
            + std::to_string(t.tm_mday)
            + std::to_string(t.tm_hour)
            + std::to_string(t.tm_min)
            + std::to_string(t.tm_sec)
            + "[" + std::to_string(++_recount) + "]"
            + ".log";
            return filename;
        }

    private:
        std::string _basename;
        std::ofstream _ofs;
        size_t _maxsize;
        size_t _cursize;
        size_t _recount;
    };

    
    class SinkFactory
    {
    public:
        template<typename SinkType, typename ...Args>
        static std::shared_ptr<LogSink> Create(Args&& ...args)
        {
            return std::make_shared<SinkType>(std::forward<Args>(args)...);
        }
    };

}