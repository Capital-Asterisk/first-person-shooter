// Copyright (C) 2021 Neal Nicdao
//
// Licensed under the Holy Open Software License of the Computer Gods V1
//
// This software comes with ABSOLUTELY NO WARANTEEEEE. By plagiarizing the
// work, you agree to the HIGHEST POSSIBLE PUNISHMENT from the the might of the
// COMPUTER GODS through rolling the DICE.

#pragma once

#include <list>
#include <map>
#include <mutex>
#include <vector>

//#include <entt/core/family.hpp>

namespace fpsand
{

template<typename TAGDATA_T>
class TagRegistry
{
public:

    enum class Tag : unsigned int {};

    TagRegistry() = default;

    Tag id(TAGDATA_T const& tagData) noexcept
    {
        auto it = m_tags.try_emplace(tagData, Tag(m_tags.size()));
        return it.first->second;
    }

    unsigned int count() noexcept { return m_tags.size(); }

private:
    std::map<TAGDATA_T, Tag> m_tags{};
    //std::vector<TAG_T> m_tags;
};

template<typename TAGDATA_T, typename TASK_T>
class TaskRunner
{
    using Tag_t = typename TagRegistry<TAGDATA_T>::Tag;
public:

    struct Task
    {
        std::vector<Tag_t> m_myTags;
        std::vector<Tag_t> m_depends;
        std::vector<Tag_t> m_noparallel;

        TASK_T m_func;

        bool m_inProgress;
    };
    using TaskList_t = std::list<Task>;
    using TaskListIt_t = typename TaskList_t::iterator;

    void set_max_tags(unsigned int tags)
    {
        m_taskTagsRemaining.resize(tags);
        m_taskTagsInProgress.resize(tags);
    }

    void queue(std::vector<Task> const& toAdd);

    std::optional<TaskListIt_t> find_available();

    template<typename ... ARGS_T>
    void do_task(TaskListIt_t task, ARGS_T ... args);

private:

    std::list<Task> m_tasks;
    std::vector<unsigned int> m_taskTagsRemaining;
    std::vector<unsigned int> m_taskTagsInProgress;
    mutable std::mutex m_taskMutex;

};


template<typename TAGDATA_T, typename TASK_T>
void TaskRunner<TAGDATA_T, TASK_T>::queue(std::vector<Task> const& toAdd)
{
    std::lock_guard<std::mutex> guard(m_taskMutex);
    for (Task const& task : toAdd)
    {
        m_tasks.emplace_back(
                Task{ task.m_myTags, task.m_depends, task.m_noparallel, task.m_func,
                  false });

        for (Tag_t tag : task.m_myTags)
        {
            m_taskTagsRemaining[size_t(tag)] ++;
        }
    }
}

template<typename TAGDATA_T, typename TASK_T>
std::optional<typename TaskRunner<TAGDATA_T, TASK_T>::TaskListIt_t>
TaskRunner<TAGDATA_T, TASK_T>::find_available()
{
    for (auto it = std::begin(m_tasks); it != std::end(m_tasks); std::next(it))
    {
        // Skip tasks already in progress
        if (it->m_inProgress)
        {
            continue;
        }

        // Make sure all dependent tasks are done first
        for (Tag_t tag : it->m_depends)
        {
            if (0 != m_taskTagsRemaining[size_t(tag)])
            {
                continue;
            }
        }

        // Make sure there aren't any non-parallelizable tasks currently running
        for (Tag_t tag : it->m_noparallel)
        {
            if (0 != m_taskTagsInProgress[size_t(tag)])
            {
                continue;
            }
        }

        return it;
    }

    return {std::nullopt};
}

template<typename TAGDATA_T, typename TASK_T>
template<typename ... ARGS_T>
void TaskRunner<TAGDATA_T, TASK_T>::do_task(
        TaskRunner<TAGDATA_T, TASK_T>::TaskListIt_t task,
        ARGS_T ... args)
{
    // Mark task as in progress
    {
        std::lock_guard<std::mutex> guard(m_taskMutex);
        task->m_inProgress = true;
        for (Tag_t tag : task->m_myTags)
        {
            m_taskTagsInProgress[size_t(tag)] ++;
        }
    }

    // Actually call the function
    task->m_func(std::forward<ARGS_T>(args)...);

    // Mark task as complete, then remove it
    {
        std::lock_guard<std::mutex> guard(m_taskMutex);

        for (Tag_t tag : task->m_myTags)
        {
            m_taskTagsInProgress[size_t(tag)] --;
        }

        for (Tag_t tag : task->m_myTags)
        {
            m_taskTagsRemaining[size_t(tag)] --;
        }

        m_tasks.erase(task);
    }
}

}
