#pragma once
#include <iostream>

#ifdef __unix__

#define SET_ERROR_COLOR std::cerr<<"\033[31m";
#define SET_INFO_COLOR std::cout<<"\033[32m";
#define SET_DEBUG_COLOR std::cout<<"\033[0m";
#define SET_DEFAULT_ERROR_COLOR std::cerr<<"\033[0m";
#define SET_DEFAULT_INFO_COLOR std::cout<<"\033[0m";

#elif defined(_WIN32) || defined(WIN32)

#include <windows.h>

#define SET_ERROR_COLOR SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),12);
#define SET_INFO_COLOR SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),2);
#define SET_DEBUG_COLOR SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),6);
#define SET_DEFAULT_ERROR_COLOR SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7);
#define SET_DEFAULT_INFO_COLOR SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7);

#endif


#define LOG_MAX_THRESHOLD 3
#define LOG_ERROR_THRESHOLD 1
#define LOG_INFO_THRESHOLD 2
#define LOG_DEBUG_THRESHOLD 3

#define LOG_ERROR logger::LOGGER_ERROR(__PRETTY_FUNCTION__)
#define LOG_INFO  logger::LOGGER_INFO(__PRETTY_FUNCTION__)
#define LOG_DEBUG logger::LOGGER_DEBUG(__PRETTY_FUNCTION__)

#define ASSERT_LOG_LEVEL_ERROR if(Logging::log_level()<LOG_ERROR_THRESHOLD)return;
#define ASSERT_LOG_LEVEL_INFO  if(Logging::log_level()<LOG_INFO_THRESHOLD)return;
#define ASSERT_LOG_LEVEL_DEBUG if(Logging::log_level()<LOG_DEBUG_THRESHOLD)return;

namespace logger
{
struct Logging
{
    enum Level
    {
        Nothing = 0,
        Error_only = 1,
        Info = 2,
        Everything = 3
    };

    explicit Logging() = delete;
    explicit Logging(Level x)
    {
        if (x <= LOG_MAX_THRESHOLD)
        {
            std::cerr << "[ LOGGER ] Log level set to " << x << std::endl;
            logging_level_ = x;
        }
        else
        {
            std::cerr << "[ LOGGER ] Log level cannot be set to level " << x
                      << " max level is " << LOG_MAX_THRESHOLD << std::endl;
        }
    }
    static unsigned log_level() { return logging_level_; }

private:
    static unsigned logging_level_;
};

struct LOGGER_ERROR
{
public:
    LOGGER_ERROR(const char* function_name) : function_name(function_name) {}
    template<typename ...Args>
    void operator()(Args&&... args)
    {
        ASSERT_LOG_LEVEL_ERROR
        SET_ERROR_COLOR
        std::cerr << "[ERR/" << function_name << "]";
        SET_DEFAULT_ERROR_COLOR
        std::cerr << ": ";
        using expander = int[];
        (void)expander{0, (void(std::cerr << std::forward<Args>(args)), 0)...};
        std::cerr << std::endl;
    }
private:
    const char* function_name;
};

struct LOGGER_INFO
{
    LOGGER_INFO(const char* function_name) : function_name(function_name) {}
    template<typename ...Args>
    void operator()(Args&&... args)
    {
        ASSERT_LOG_LEVEL_INFO
        SET_INFO_COLOR
        std::cout << "[INFO/" << function_name << "]";
        SET_DEFAULT_INFO_COLOR
        std::cout << ": ";
        using expander = int[];
        (void)expander{0, (void(std::cout << std::forward<Args>(args)), 0)...};
        std::cout << std::endl;
    }
private:
     const char* function_name;
};

struct LOGGER_DEBUG
{
    LOGGER_DEBUG(const char* function_name) : function_name(function_name) {}
    template<typename ...Args>
    void operator()(Args&&... args)
    {
        ASSERT_LOG_LEVEL_DEBUG
        SET_DEBUG_COLOR
        std::cout << "[DEBUG/" << function_name << "]: ";
        SET_DEFAULT_INFO_COLOR
        using expander = int[];
        (void)expander{0, (void(std::cout << std::forward<Args>(args)), 0)...};
        std::cout << std::endl;
    }
private:
     const char* function_name;
};

}  // namespace logger
