#ifndef SYLVER_DEFINES_HPP
#define SYLVER_DEFINES_HPP

#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>
#include <set>
#include <cstdint>
#include <concepts>
#include <functional>
#include <memory>
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
using usize = std::size_t;
using uptr = uintptr_t;

using s8 = int8_t;
using s16 = int16_t;
using s32 = int32_t;
using s64 = int64_t;
using sptr = intptr_t;

using b8 = u8;
using b16 = u16;
using b32 = u32;
using b64 = u64;

// TODO: Switch to using C++23 Fixed Width floating point types when
// there is better compiler support (Clang & MSVC)
using f32 = float;
using f64 = double;


#if defined(SYLVER_PLATFORM_WINDOWS)

#if defined(SYLVER_DLL_IMPORT)
#define SYLVERAPI __declspec(dllimport)
#else
#define SYLVERAPI __declspec(dllexport)
#endif
#elif defined(SYLVER_PLATFORM_LINUX) || defined(SYLVER_PLATFORM_MAC)
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


#endif
