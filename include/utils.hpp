#pragma once
#include <array>

template <typename T, std::size_t N>

inline bool arrContains(std::array<T, N> arr, T value) {
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] == value)
            return true;
    }

    return false;
}