// Copyright (C) 2021 Neal Nicdao
//
// Licensed under the Holy Open Software License of the Computer Gods V1
//
// This software comes with ABSOLUTELY NO WARANTEEEEE. By plagiarizing the
// work, you agree to the HIGHEST POSSIBLE PUNISHMENT from the the might of the
// COMPUTER GODS through rolling the DICE.

#pragma once

#include "tasks.h"
#include "types.h"

#include <entt/core/family.hpp>
#include <entt/core/any.hpp>

#include <cstring>
#include <variant>

namespace fpsand
{

class World : public entt::basic_registry<Ent>
{
    // Used to generate sequential IDs at runtime
    using singlecomp_t = entt::family<struct singlecomp_type>;

public:
    World(unsigned int numSingletons);

    template<typename COMP_T>
    COMP_T& get_singleton()
    {
        entt::any &singleton = m_singletons.at(singlecomp_t::type<COMP_T>);
        return entt::any_cast< COMP_T& >(singleton);
    }

    template<typename COMP_T, typename ... ARGS_T>
    COMP_T& emplace_singleton(ARGS_T ... args)
    {
        entt::any &singleton = m_singletons.at(singlecomp_t::type<COMP_T>);
        singleton.emplace<COMP_T>(std::forward<ARGS_T>(args) ...);
        return entt::any_cast< COMP_T& >(singleton);
    }

private:
    std::vector<entt::any> m_singletons;

};

struct TagFPSand
{
    struct Writes { entt::id_type m_modify; };
    struct Reads { entt::id_type m_read; };
    struct Name { std::string_view m_name; };

    std::variant<Writes, Reads, Name> m_value;

    bool operator<(TagFPSand const& rhs) const
    {
        return std::memcmp(this, &rhs, sizeof(TagFPSand)) < 0;
    }
};


template<typename COMP_T>
constexpr TagFPSand tag_writes() noexcept
{
    return { TagFPSand::Writes{entt::type_seq<COMP_T>::value()} };
}

template<typename COMP_T>
constexpr TagFPSand tag_reads() noexcept
{
    return { TagFPSand::Reads{entt::type_seq<COMP_T>::value()} };
}

constexpr TagFPSand tag_name(std::string_view name) noexcept
{
    return { TagFPSand::Name{name} };
}


class Systems
{
    using SystemTags_t = TagRegistry<TagFPSand>;
public:
    Systems() = default;

    //void add(std::initializer_list<TagFPSand> nicestack,);

private:

    SystemTags_t m_tagSys;
};


}
