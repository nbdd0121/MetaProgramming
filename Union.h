#pragma once

#include "Compare.h"
#include "Variadic.h"
#include "StaticDispatch.h"
#include "StaticInvocation.h"

namespace norlit {
namespace meta {

template<typename... Types>
class Union {
    using TemplateList = VariadicTemplate<Types...>;
    using DestructorDispatcher = StaticDispatch<DestructorInvocation<Types>...>;
    using CopyConstructorDispatcher = StaticDispatch<CopyConstructorInvocation<Types>...>;
    using MoveConstructorDispatcher = StaticDispatch<MoveConstructorInvocation<Types>...>;
    using TypeIDDispatcher = StaticDispatch<TypeIDInvocation<Types>...>;

    char data[SizeComparator<sizeof(Types)...>::max];
    unsigned int tag;

    void Destruct() {
        if (tag != -1)
            DestructorDispatcher::template Invoke<void>(tag, (void*)data);
    }

  public:
    Union() {
        tag = -1;
    }

    Union(const Union& from) {
        tag = from.tag;
        if (tag != -1) {
            CopyConstructorDispatcher::template Invoke<void>(tag, (void*)data, (void*)from.data);
        }
    }

    Union(Union&& from) {
        tag = from.tag;
        if (tag != -1) {
            MoveConstructorDispatcher::template Invoke<void>(tag, (void*)data, (void*)from.data);
            from.tag = -1;
        }
    }

    void operator =(const Union& from) {
        Destruct();
        tag = from.tag;
        if (tag != -1) {
            CopyConstructorDispatcher::template Invoke<void>(tag, (void*)data, (void*)from.data);
        }
    }

    void operator =(Union&& from) {
        Destruct();
        tag = from.tag;
        if (tag != -1) {
            MoveConstructorDispatcher::template Invoke<void>(tag, (void*)data, (void*)from.data);
            from.tag = -1;
        }
    }

    template<unsigned I>
    void Set(const typename TemplateList::template Type<I>& value) {
        Destruct();
        new ((void*)data) typename TemplateList::template Type<I>(value);
        tag = I;
    }

    template<typename T>
    void Set(const T& value) {
        Set<TemplateList::template Index<T>::Value>(value);
    }

    template<unsigned I>
    void Set(typename TemplateList::template Type<I>&& value) {
        Destruct();
        new ((void*)data) typename TemplateList::template Type<I>(std::move(value));
        tag = I;
    }

    template<typename T>
    void Set(T&& value) {
        Set<TemplateList::template Index<T>::Value>(std::move(value));
    }

    template<unsigned I>
    typename TemplateList::template Type<I> UncheckedGet() {
        return *(typename TemplateList::template Type<I>*)data;
    }

    template<typename T>
    T UncheckedGet() {
        return UncheckedGet<TemplateList::template Index<T>::Value>();
    }

    template<unsigned I>
    typename TemplateList::template Type<I> Get() {
        if (tag != I) {
            throw std::logic_error("Invalid Get Operation: Type mismatch");
        }
        return UncheckedGet<I>();
    }

    template<typename T>
    T Get() {
        return Get<TemplateList::template Index<T>::Value>();
    }

    bool IsEmpty() {
        return tag == -1;
    }

    void Empty() {
        Destruct();
        tag = -1;
    }

    const std::type_info& Type() {
        if (tag == -1) {
            throw std::bad_typeid();
        }
        return TypeIDDispatcher::template Invoke<const std::type_info&>(tag);
    }

    ~Union() {
        Destruct();
    }
};

}
}