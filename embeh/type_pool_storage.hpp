#pragma once

#include <type_traits>
#include <tuple>
#include <memory>

#include "template_avail_block.hpp"

namespace embeh
{
    template <typename... avail_types>
    struct type_pool_storage
    {
        /* avail_list */
        template <typename E>
        avail_list<E> &get_avail_list()
        {
            return std::get<handler<E>>(type_pool).type_list;
        }

        /* head */
        template <typename E>
        E *head()
        {
            return reinterpret_cast<E *>(get_avail_list<E>().head);
        }

        template <typename E>
        void set_head(avail_block<E> *pv)
        {
            get_avail_list<E>().set_head(pv);
        }

        /* next */
        template <typename E>
        E *next()
        {
            if (!get_avail_list<E>().head)
                return nullptr;
            return reinterpret_cast<E *>(get_avail_list<E>().head->next);
        }

        /* single point of creation */
        template <typename E, typename... Args>
        auto create(Args... args)
        {
            auto custom_deleter = [this](void *pv) {
                if (pv == nullptr)
                {
                    return;
                }
                auto tmp_ptr = static_cast<avail_block<E> *>(pv);
                tmp_ptr->next = reinterpret_cast<avail_block<E> *>(head<E>());
                const_cast<type_pool_storage<avail_types...> *>(this)->set_head<E>(tmp_ptr);
            };
            auto *pt = reinterpret_cast<E *>(head<E>());
            if (pt == nullptr)
            {
                return std::unique_ptr<E, decltype(custom_deleter)>(nullptr, custom_deleter);
            }
            set_head<E>(get_avail_list<E>().head->next);

            if (!pt->init(std::forward<Args>(args)...))
            {
                return std::unique_ptr<E, decltype(custom_deleter)>(nullptr, custom_deleter);
            }
            return std::unique_ptr<E, decltype(custom_deleter)>(pt, custom_deleter);
        }

        /* destructor with static asserts on avail_types */
        ~type_pool_storage()
        {
            static_test<(std::is_convertible_v<std::size_t, decltype(avail_types::size)> && ...)>();
            static_test<(not std::is_default_constructible_v<avail_types> && ...)>();
            static_test<(not std::is_copy_assignable_v<avail_types> && ...)>();
            static_test<(not std::is_copy_constructible_v<avail_types> && ...)>();
            static_test<(not std::is_move_assignable_v<avail_types> && ...)>();
            static_test<(not std::is_move_constructible_v<avail_types> && ...)>();
        }

    private:
        /* handler */
        template <typename E>
        struct handler
        {
            avail_list<E> type_list;
        };
        
        std::tuple<handler<avail_types>...> type_pool;
    };
} // namespace embeh
