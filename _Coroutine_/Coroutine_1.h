#ifndef COROUTINE_1_H
#define COROUTINE_1_H

#include <coroutine>
#include <thread>
#include <future>
#include <iostream>

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


}
#endif // COROUTINE_1_H
