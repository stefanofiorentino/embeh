#pragma once

namespace embeh
{
    template <typename T>
    union avail_block
    {
        avail_block *next;
        alignas(alignof(T)) char memory[sizeof(T)];
    };

    template <typename T>
    struct avail_list
    {
        static constexpr std::size_t num_blocks = T::size;

        avail_list()
            : head(blocks)
        {
            for (auto i = 0u; i < num_blocks - 1; ++i)
            {
                blocks[i].next = &blocks[i + 1];
            }
            blocks[num_blocks - 1].next = nullptr;
        }

        void set_head(avail_block<T> *pv)
        {
            head = pv;
        }
        avail_block<T> blocks[num_blocks];
        avail_block<T> *head;
    };
} // namespace embeh
