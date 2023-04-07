#ifndef CHEESE_LOG_H
#define CHEESE_LOG_H
#include <string>
#include <iostream>
#include <chrono>
#include <stack>
#include <ctime>
class Record
{
public:
    void start_function(std::string info);
    void end_function(std::string info);
    static Record &instance()
    {
        static Record _instance;
        return _instance;
    }

private:
    Record() {}
    Record(const Record &other) = delete;
    Record &operator=(const Record &) = delete;
    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::stack<std::string> call_stack;
};

#endif