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

int returnInt(int a, int b){
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

TEST_1::Generator sequence(){
    int i = 0;
    while(i < 5){
        co_yield i++;
    }
}

int main(int argc, char *argv[])
{
    //a function without no explicit return type could lead to a compilation error,as the following
    // auto nullDefaultVal = getRetType([](){
    //     return ;
    // });

    auto gen = sequence();
    for(size_t ii = 0; ii < 15; ii++){
        if(gen.has_next())
            std::cout << gen.next() << std::endl;
    }

    /*
    auto strDefault = getRetType([]()->std::string{
        return {};
    });
     std::cout << "ret default string:" << strDefault << std::endl;


    auto typeFaultVal = getRetType(returnInt, 2, 3);
    std::cout << "ret default val:" << typeFaultVal << std::endl;

    auto floatDefault = getRetType(returnFloat);
    std::cout << "float default val:" << floatDefault << std::endl;

    auto ptrDefaultVal = getRetType(returnPtr);
    std::cout << "pointer default val:" << reinterpret_cast<void*>(ptrDefaultVal) << std::endl;
    return 0;


    //decltype(func) reutrn the type of the fucntioin, not the returned type pf the function. Arguments should be passed when calling the packaged_task object, though arguments has already been bound before
    //std::packaged_task<decltype(func)> pt1(std::bind(func, 2, 3));

    std::packaged_task<int()> pt(std::bind(returnInt, 2, 3));
    auto future = pt.get_future();
    std::thread([task = std::move(pt)]() mutable{
        task();
    }).detach();

    // auto asyncResult = std::async(std::launch::async, std::bind(returnInt, 11, 22));
    // auto asyncFuture = asyncResult.get();

    std::cout << "value async:" << future.get() << std::endl;
    return 0;
    */

    // std::cout << "threads count:" << std::thread::hardware_concurrency() << std::endl;
    // std::thread th1;

    /*
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
    */
    return 0;
}
