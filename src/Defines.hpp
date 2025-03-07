#pragma once

#include <cstdint>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <set>
#include <unordered_map>
#include <concepts>
#include <functional>
#include <memory>
#include <algorithm>
#include <optional>
#include <filesystem>

#include <fmt/core.h>
#include <glm/glm.hpp>


//
// Custom type aliases
//

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using usize = std::size_t;
using uptr = uintptr_t;
using iptr = intptr_t;


// TODO: Switch to using C++23 Fixed Width floating point types when
// there is better compiler support (Clang & MSVC)
using f32 = float;
using f64 = double;

static_assert(sizeof(u8) == 1, "Size of u8 should be 1!");
static_assert(sizeof(u16) == 2, "Size of u16 should be 2!");
static_assert(sizeof(u32) == 4, "Size of u32 should be 4!");
static_assert(sizeof(u64) == 8, "Size of u64 should be 8!");

static_assert(sizeof(i8) == 1, "Size of i8 should be 1!");
static_assert(sizeof(i16) == 2, "Size of i16 should be 2!");
static_assert(sizeof(i32) == 4, "Size of i32 should be 4!");
static_assert(sizeof(i64) == 8, "Size of i64 should be 8!");

static_assert(sizeof(f32) == 4, "Size of f32 should be 4!");
static_assert(sizeof(f64) == 8, "Size of f64 should be 8!");


#if defined(SYLVER_PLATFORM_WINDOWS)

#if defined(SYLVER_DLL_IMPORT)
#define SYLVERAPI __declspec(dllimport)
#else
#define SYLVERAPI __declspec(dllexport)
#endif
#elif defined(SYLVER_PLATFORM_UNIX)
#define SYLVERAPI
#endif

namespace glm {
    template<std::size_t I, auto N, class T, auto Q>
    constexpr auto& get(glm::vec<N, T, Q>& v) noexcept {
        return v[I];
    }

    template<std::size_t I, auto N, class T, auto Q>
    constexpr const auto& get(const glm::vec<N, T, Q>& v) noexcept {
        return v[I];
    }

    template<std::size_t I, auto N, class T, auto Q>
    constexpr auto&& get(glm::vec<N, T, Q>&& v) noexcept {
        return std::move(v[I]);
    }

    template<std::size_t I, auto N, class T, auto Q>
    constexpr const auto&& get(const glm::vec<N, T, Q>&& v) noexcept {
        return std::move(v[I]);
    }

}    // namespace glm
namespace std {
    template<auto N, class T, auto Q>
    struct tuple_size<glm::vec<N, T, Q>> : std::integral_constant<std::size_t, N> {};

    template<std::size_t I, auto N, class T, auto Q>
    struct tuple_element<I, glm::vec<N, T, Q>> {
            using type = decltype(get<I>(declval<glm::vec<N, T, Q>>()));
    };
}    // namespace std
