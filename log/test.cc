#include"Util.hpp"
#include"Level.hpp"
#include"LogMessage.hpp"
#include"Formatter.hpp"
#include"Sink.hpp"
#include"Logger.hpp"
#include<string>

using namespace LogModule;

int main()
{
    // LogMessage msg("root","test.cc",10,"testing...",LogLevel::Level::DEBUG);
    // Formatter ft;
    // std::string s = ft.format(msg);
    
    std::shared_ptr<LogSink> stdoutlsp = SinkFactory::Create<StdOutSink>();
    std::shared_ptr<LogSink> filelsp = SinkFactory::Create<FileSink>("./logfile/test.log");
    std::shared_ptr<LogSink> rlsp = SinkFactory::Create<RollBySizeSink>("./logfile/test.log",1024*1024);
    std::shared_ptr<LogSink> tlsp = SinkFactory::Create<RollByTimeSink>("./logfile/test.log",TimeGap::GAP_SECOND);

    // stdoutlsp->Log(s.c_str(),s.size());
    // filelsp->Log(s.c_str(),s.size());
    // rlsp->Log(s.c_str(),s.size());
    // size_t i = 0;
    // while(i<1024*1024*10)
    // {
    //     rlsp->Log(s.c_str(),s.size());
    //     i+=s.size();
    // }
    
    // for(int i=0;i<10;i++)
    // {
    //     tlsp->Log(s.c_str(),s.size());
    // }

    auto ft = std::make_shared<Formatter>();
    std::vector<std::shared_ptr<LogSink>> sinks = {stdoutlsp,filelsp,tlsp};
    Logger::ptr logger(new SyncLogger("Syn_Logger",ft,LogLevel::Level::WARNING,sinks));
    
    logger->debug(__FILE__,__LINE__,"%s","testing...");
    logger->error(__FILE__,__LINE__,"%s","testing...");
    logger->fatal(__FILE__,__LINE__,"%s","testing...");
    logger->infor(__FILE__,__LINE__,"%s","testing...");
    logger->warnning(__FILE__,__LINE__,"%s","testing...");
    // size_t i = 0;
    // while(i<1024*1024)
    // {
    //     std::string s = "testing...";
    //     logger->fatal(__FILE__,__LINE__,"%s","testing...");
    //     i+=s.size();
    // }

    return 0;
}