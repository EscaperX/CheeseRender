#include "log.h"

void print_indent(size_t x)
{
    for (size_t i = 0; i < x; i++)
        std::cerr << "    ";
}
void Record::start_function(std::string info)
{

    print_indent(call_stack.size());
    std::cerr << "================" << std::endl;
    print_indent(call_stack.size());
    std::cerr << "Function Call: " << info << "   Start" << std::endl;
    std::cerr << std::endl;
    start = std::chrono::system_clock::now();
    call_stack.push(info);
}

void Record::end_function(std::string info)
{
    if (info != call_stack.top())
    {
        throw std::runtime_error("Call Stack Conflict!");
    }
    call_stack.pop();
    print_indent(call_stack.size());
    std::cerr << "Function Call: " << info << "   End." << std::endl;
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    print_indent(call_stack.size());
    std::cerr << "Duration: " << elapsed_seconds.count() << std::endl;
    print_indent(call_stack.size());
    std::cerr << "================" << std::endl;
}