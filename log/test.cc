#include"Util.hpp"
#include"Level.hpp"
#include"LogMessage.hpp"
#include"Formatter.hpp"
#include<string>

using namespace LogModule;

int main()
{
    LogMessage msg("root","test.cc",10,"testing...",LogLevel::Level::DEBUG);
    Formatter ft("abc%%f[%d{%Y-%m-%d %H:%M:%S}][%p] -%m%nabc");
    std::string s = ft.format(msg);
    std::cout<<s<<std::endl;

    return 0;
}