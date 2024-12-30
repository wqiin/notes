#include <iostream>
#include <coroutine>
#include <thread>
#include <chrono>

#include "Coroutine_1.h"
#include "Coroutine_2.h"

#include <thread>
#include <future>

TEST_1::MyCoroutine func1(){
    std::cout << "coroutine 1 continue to run\n";
    co_await std::suspend_always{};

    std::cout << "coroutine resuming to run\n";

    co_yield 12;

    co_return 0;

}

int func(int a, int b){
    return a + b;
}

//get the default value of returned type of in accordance to callable object
template<typename Func, typename... Args>
auto getRetType(Func && func, Args&&... args) -> decltype(func(args...))
{
    using retType = decltype(func(args...));
    return retType{};
}

char * returnPtr(){
    return "";
}

float returnFloat(){
    return 0.0f;
}

int main(int argc, char *argv[])
{
    //a function without no explicit return type could lead to a compilation error,as the below
    // auto nullDefaultVal = getRetType([](){
    //     return ;
    // });

    auto typeFaultVal = getRetType(func, 2, 3);
    std::cout << "ret default val:" << typeFaultVal << std::endl;

    auto floatDefault = getRetType(returnFloat);
    std::cout << "float default val:" << floatDefault << std::endl;

    auto ptrDefaultVal = getRetType(returnPtr);
    //std::cout << "pointer default val:" << static_cast<void*>(ptrDefaultVal) << std::endl;
    std::cout << "pointer default val:" << reinterpret_cast<void*>(ptrDefaultVal) << std::endl;
    return 0;


    //decltype(func) reutrn the type of the fucntioin, not the returned type pf the function. Arguments should be passed when calling the packaged_task object, though arguments has already been bound before
    //std::packaged_task<decltype(func)> pt1(std::bind(func, 2, 3));

    std::packaged_task<int()> pt(std::bind(func, 2, 3));
    auto future = pt.get_future();
    std::thread([task = std::move(pt)]() mutable{
        task();
    }).detach();

    std::cout << "value async:" << future.get() << std::endl;
    return 0;


    std::cout << "threads count:" << std::thread::hardware_concurrency() << std::endl;
    std::thread th1;
    //std::packaged_task<decltype(&func)()> pt(std::bind(func, 1, 2));
    //pt.get_future();

    auto cor1 = func1();
    std::cout << "main begin \n";
    cor1.resume();

    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "affter second sleep\n";

    cor1.resume();

    std::cout << "done:?" << cor1.done() << std::endl;

    cor1.resume();

    std::cout << "main ending\n";
    std::cout << "done:?" << cor1.done() << std::endl;;

    return 0;
}
