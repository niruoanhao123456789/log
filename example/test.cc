#include"../log/Log.hpp"

using namespace LogModule;

void test1()
{
    std::unique_ptr<LoggerBuilder> builder = std::make_unique<LocalLoggerBuilder>();
    builder->BuildLoggerName("Async_Logger");
    builder->BUildLoggerLevel(LogLevel::Level::WARNING);
    builder->BUildLoggerSink<StdOutSink>();
    builder->BUildLoggerSink<FileSink>("./logfile/test.log");
    builder->BUildLoggerSink<RollBySizeSink>("./logfile/test.log",1024*1024);
    builder->BuildLoggerType(LoggerType::LOGGER_ASYNC);

    Logger::ptr logger = builder->Build();

    logger->debug(__FILE__,__LINE__,"%s","testing...");
    logger->error(__FILE__,__LINE__,"%s","testing...");
    logger->fatal(__FILE__,__LINE__,"%s","testing...");
    logger->infor(__FILE__,__LINE__,"%s","testing...");
    logger->warnning(__FILE__,__LINE__,"%s","testing...");
    size_t i = 0;
    while(i<1024*1024)
    {
        std::string s = "testing...";
        logger->fatal(__FILE__,__LINE__,"%s","testing...");
        i+=s.size();
    }
}

void test2()
{
    Logger::ptr logger = LoggerManager::getInstance().GetLogger("Async_Logger");
    logger->debug(__FILE__,__LINE__,"%s","testing...");
    logger->error(__FILE__,__LINE__,"%s","testing...");
    logger->fatal(__FILE__,__LINE__,"%s","testing...");
    logger->infor(__FILE__,__LINE__,"%s","testing...");
    logger->warnning(__FILE__,__LINE__,"%s","testing...");

    size_t i = 0;
    while(i<1024*1024)
    {
        std::string s = "testing...";
        logger->fatal(__FILE__,__LINE__,"%s","testing...");
        i+=s.size();
    }
}

void test3()
{
    Logger::ptr logger = LoggerManager::getInstance().GetLogger("Async_Logger");
    logger->debug("%s","testing...");
    logger->error("%s","testing...");
    logger->fatal("%s","testing...");
    logger->infor("%s","testing...");
    logger->warnning("%s","testing...");

    size_t i = 0;
    while(i<1024*1024)
    {
        std::string s = "testing...";
        logger->fatal("%s","testing...");
        i+=s.size();
    }
}

void loggerTest(const std::string &logger_name) 
{
    LogModule::Logger::ptr lp = LogModule::GetLogger(logger_name);
    assert(lp.get());
    LOGF("------------example--------------------");
    lp->debug("%s", "logger->debug");
    lp->infor("%s", "logger->info");
    lp->warnning("%s", "logger->warn");
    lp->error("%s", "logger->error");
    lp->fatal("%s", "logger->fatal");
    LOG_DEBUG(lp, "%s", "LOG_DEBUG");
    LOG_INFOR(lp, "%s", "LOG_INFO");
    LOG_WARNNING(lp, "%s", "LOG_WARN");
    LOG_ERROR(lp, "%s", "LOG_ERROR");
    LOG_FATAL(lp, "%s", "LOG_FATAL");
    LOGF("---------------------------------------");
    
    std::string log_msg = "hello bitejiuyeke-";
    size_t fsize = 0;
    size_t count = 0;
    while(count < 1000000) {
        std::string msg = log_msg + std::to_string(count++);
        lp->error("%s", msg.c_str());
    }
}

void functional_test() 
{
    std::unique_ptr<LoggerBuilder> lbp(new LogModule::GobalLoggerBuilder);
    lbp->BuildLoggerName("all_sink_logger");
    lbp->BuildLoggerFormatter("[%d][%c][%f:%l][%p] %m%n");
    lbp->BUildLoggerLevel(LogModule::LogLevel::Level::DEBUG);
    lbp->BUildLoggerSink<LogModule::StdOutSink>();
    lbp->BUildLoggerSink<LogModule::FileSink>("./logs/sync.log");
    lbp->BUildLoggerSink<LogModule::RollBySizeSink>("./logs/roll-", 10 * 1024 * 1024);
    lbp->BuildLoggerType(LogModule::LoggerType::LOGGER_ASYNC);
    lbp->Build();
    loggerTest("all_sink_logger");
}

int main()
{
    

    return 0;
}