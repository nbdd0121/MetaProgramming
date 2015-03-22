#pragma once

namespace norlit {
namespace meta {

namespace internal {

template<unsigned V>
struct AlwaysFalse {
    static const bool Value = false;
};

template<unsigned I, unsigned V, typename... T>
struct TypeExtractor {
    static_assert(AlwaysFalse<V>::Value, "Index out of bound when looking up in the variadic template argument");
    using Type = void;
};

template<unsigned I, unsigned V, typename T, typename... TRest>
struct TypeExtractor<I, V, T, TRest...> {
    using Type = typename TypeExtractor<I, V + 1, TRest...>::Type;
};

template<unsigned V, typename T, typename... TRest>
struct TypeExtractor<V, V, T, TRest...> {
    using Type = T;
};

template<typename U, unsigned V, typename... T>
struct IndexExtractor {
    static_assert(AlwaysFalse<V>::Value, "Type is contained in the variadic template argument");
    static const int Value = -1;
};

template<typename U, unsigned V, typename T, typename... TRest>
struct IndexExtractor<U, V, T, TRest...> {
    static const int Value = IndexExtractor<U, V + 1, TRest...>::Value;
};

template<unsigned V, typename T, typename... TRest>
struct IndexExtractor<T, V, T, TRest...> {
    static const int Value = V;
};

}

template <typename... T>
class VariadicTemplate {
  public:
    template<unsigned V>
    using Type = typename internal::TypeExtractor<V, 0, T...>::Type;
    template<typename U>
    struct Index {
        static const unsigned Value = internal::IndexExtractor<U, 0, T...>::Value;
    };
};


}
}