#pragma once

#include <typeinfo>

namespace norlit {
namespace meta {

template<typename T>
struct DestructorInvocation {
    static void Invoke(void* arg) {
        T* t = (T*)arg;
        t->T::~T();
    }
};

template<typename T>
struct CopyConstructorInvocation {
    static void Invoke(void* dest, void* from) {
        new (dest)T(*(T*)from);
    }
};

template<typename T>
struct MoveConstructorInvocation {
    static void Invoke(void* dest, void* from) {
        new (dest)T(std::move(*(T*)from));
    }
};

template<typename T>
struct TypeIDInvocation {
    static const std::type_info& Invoke() {
        return typeid(T);
    }
};


}
}