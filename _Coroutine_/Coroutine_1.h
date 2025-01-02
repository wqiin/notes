#ifndef COROUTINE_1_H
#define COROUTINE_1_H

#include <coroutine>
#include <thread>
#include <future>
#include <iostream>

#include <utility>  // std::exchange

namespace TEST_1{

class MyCoroutine{
public:
    struct promise_type{//the name is a MUST
        MyCoroutine get_return_object(){
            return MyCoroutine(handle_t::from_promise(*this));
        }

        std::suspend_never initial_suspend(){
            return {};
        }

        std::suspend_always final_suspend() noexcept{
            return {};
        }

        std::suspend_always yield_value(int val){
            std::cout << "yield calling\n";
            return {};
        }

        //void return_void(){}//only one of the both can be defined
        void return_value(int val){
            std::cout << "return value calling\n";
        }

        void unhandled_exception(){//a MUST complememtion
            std::exit(1);
        }
    };

    using handle_t = std::coroutine_handle<promise_type>;
    MyCoroutine(handle_t h):handle(h){};
    ~MyCoroutine(){
        if(handle)
            handle.destroy();
    }

    void resume(){
        if(handle)
            handle.resume();
    }

    bool done(){
        if(handle)
            return handle.done();

        return false;
    }

private:
    handle_t handle;
};


class Generator{
public:
    struct promise_type{//the name is a MUST
    public:
        int value = 0;
        bool bIsReady = false;

        Generator get_return_object(){
            return Generator(handle_t::from_promise(*this));
        }

        std::suspend_never initial_suspend(){
            return {};
        }

        std::suspend_always final_suspend() noexcept{
            return {};
        }

        //suspended when pass the value
        //after await_transform being defined，co_await expr means to calling co_await promise.await_transform(expr)
        std::suspend_always await_transform(int value) {
            this->value = value;
            return {};
        }


        std::suspend_always yield_value(int val){
            std::cout << "yield calling\n";
            this->value = val;
            this->bIsReady = true;
            return {};
        }

        //void return_void(){}//only one of the both can be defined
        void return_value(int val){
            std::cout << "return value calling\n";
        }

        void unhandled_exception(){//a MUST complememtion
            std::exit(1);
        }
    };

    using handle_t = std::coroutine_handle<promise_type>;
    Generator(handle_t h):handle(h){};

    //make Generator to control the coroutine object, in case of illegal pointer access
    ~Generator(){
        if(handle)
            handle.destroy();
    }

    Generator(Generator && other) noexcept : handle(std::exchange(other.handle, {}))
    {}
    Generator & operator = (Generator && other){
        this->handle = std::exchange(other.handle, {});
    }

    Generator(const Generator &) = delete;
    Generator & operator = (const Generator &) = delete;

    void resume(){
        if(handle)
            handle.resume();
    }

    bool done(){
        if(handle)
            return handle.done();

        return false;
    }


    bool has_next(){
        if(!handle || handle.done())
            return false;

        if(!handle.promise().bIsReady)
            handle.resume();

        if(handle.done())
            return false;

        return true;
    }

    int next(){
        if(!has_next())
            throw std::runtime_error("No Value");

        handle.promise().bIsReady = false;
        return handle.promise().value;
    }

private:
    handle_t handle;
};

}
#endif // COROUTINE_1_H
