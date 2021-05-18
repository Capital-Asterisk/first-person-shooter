// Copyright (C) 2021 Neal Nicdao
//
// Licensed under the Holy Open Software License of the Computer Gods V1
//
// This software comes with ABSOLUTELY NO WARANTEEEEE. By plagiarizing the
// work, you agree to the HIGHEST POSSIBLE PUNISHMENT from the the might of the
// COMPUTER GODS through rolling the DICE.

#pragma once

#include "../types.h"
#include "../vecmath.h"

#include <cstdint>

namespace fpsand
{

struct SgtParticleMap
{
    std::vector<Ent> m_map;
};

struct CmpPosition
{
    vec2<coord_t> m_pos;
};

struct CmpSubPosition
{
    // used for smooth diagonal motion
    vec2<int8_t> m_subpos;
};

struct CmpVelocity
{
    // subpixels per frame
    vec2<int16_t> m_vel;
};

}
