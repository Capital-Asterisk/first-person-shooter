// Copyright (C) 2021 Neal Nicdao
//
// Licensed under the Holy Open Software License of the Computer Gods V1
//
// This software comes with ABSOLUTELY NO WARANTEEEEE. By plagiarizing the
// work, you agree to the HIGHEST POSSIBLE PUNISHMENT from the the might of the
// COMPUTER GODS through rolling the DICE.

#pragma once

namespace fpsand
{

template<typename TYPE_T>
struct vec2
{
    vec2() = default;
    vec2(TYPE_T x, TYPE_T y) : m_x(x), m_y(y) { };
    vec2(vec2 const&) = default;
    vec2(vec2&&) = default;

    TYPE_T m_x{0}, m_y{0};

    constexpr vec2 operator+(vec2 const& rhs) const noexcept
    {
        return vec2(m_x + rhs.x, m_y + rhs.y);
    }

    constexpr vec2 operator-(vec2 const& rhs) const noexcept
    {
        return vec2(m_x - rhs.x, m_y - rhs.y);
    }

    constexpr void operator=(vec2 const& rhs) noexcept
    {
        m_x = rhs.m_x;
        m_y = rhs.m_y;
    }
};

}
