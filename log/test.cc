#include"Util.hpp"
#include"Level.hpp"
#include"LogMessage.hpp"
#include"Formatter.hpp"
#include"Sink.hpp"
#include<string>

using namespace LogModule;

int main()
{
    LogMessage msg("root","test.cc",10,"testing...",LogLevel::Level::DEBUG);
    Formatter ft;
    std::string s = ft.format(msg);
    /***/
    std::shared_ptr<LogSink> stdoutlsp = SinkFactory::Create<StdOutSink>();
    // std::shared_ptr<LogSink> filelsp = SinkFactory::Create<FileSink>("./logfile/test.log");
    // std::shared_ptr<LogSink> rolllsp = SinkFactory::Create<RollBySizeSink>("./logfile/test.log",1024*1024);

    stdoutlsp->Log(s.c_str(),s.size());
    // filelsp->Log(s.c_str(),s.size());
    // rolllsp->Log(s.c_str(),s.size());
    // size_t i = 0;
    // while(i<1024*1024*10)
    // {
    //     rolllsp->Log(s.c_str(),s.size());
    //     i+=s.size();
    // }
    /***/

    return 0;
}