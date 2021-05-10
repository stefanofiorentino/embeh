#pragma once

namespace embeh
{
    template <typename T>
    union avail_block
    {
        void set_next(avail_block *abp)
        {
            m_next = abp;
        }
        avail_block *next()
        {
            return m_next;
        }
    private:
        avail_block *m_next;
        alignas(alignof(T)) char memory[sizeof(T)];        
    };

    template <typename T>
    struct avail_list
    {
        static constexpr std::size_t num_blocks = T::size;

        avail_list()
            : m_head(blocks)
        {
            for (auto i = 0u; i < num_blocks - 1; ++i)
            {
                blocks[i].set_next(&blocks[i + 1]);
            }
            blocks[num_blocks - 1].set_next(nullptr);
        }

        void set_head(avail_block<T> *pv)
        {
            m_head = pv;
        }

        avail_block<T> *head()
        {
            return m_head;
        }

    private:
        avail_block<T> blocks[num_blocks];
        avail_block<T> *m_head;
    };
} // namespace embeh
