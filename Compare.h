#pragma once

#include <cstddef>

namespace norlit {
namespace meta {

template<size_t... var>
struct SizeComparator {};

template<size_t V>
struct SizeComparator<V> {
    static const size_t max = V;
};

template<size_t V, size_t... VRest>
struct SizeComparator<V, VRest...> {
    static const size_t max = V > SizeComparator<VRest...>::max ? V : SizeComparator<VRest...>::max;
};

}
}
