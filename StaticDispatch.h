#pragma once

#include <stdexcept>

namespace norlit {
namespace meta {

template<unsigned I, typename... Dispatcher>
struct StaticDispatchHelper {

};

template<unsigned I>
struct StaticDispatchHelper<I> {
    template<typename R, typename... Args>
    static R Invoke(unsigned id, Args... args) {
        throw std::logic_error("No suitable invocation handler found");
    }
};

template<unsigned I, typename T, typename... TRest>
struct StaticDispatchHelper<I, T, TRest...> {
    template<typename R, typename... Args>
    static R Invoke(unsigned id, Args... args) {
        if (id == I) {
            return T::Invoke(args...);
        } else {
            return StaticDispatchHelper<I + 1, TRest...>::template Invoke<R, Args...>(id, args...);
        }
    }
};

template<typename... T>
using StaticDispatch = StaticDispatchHelper<0, T...>;

}
}
