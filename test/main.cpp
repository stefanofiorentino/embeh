#include <cassert>
#include <type_traits>
#include <memory>

#include <embeh/type_pool_storage.hpp>

using embeh::avail_list;
using embeh::type_pool_storage;

template <std::size_t N=1>	
struct sample_base_class	
{	
    static constexpr std::size_t size = N;	

    virtual ~sample_base_class() noexcept = default;	

    bool init()	
    {	
        return true;	
    }	

private:	
    sample_base_class() = default;	
    sample_base_class(sample_base_class const &) = delete;	
    sample_base_class &operator=(sample_base_class const &) = delete;	
    sample_base_class(sample_base_class &&) = delete;	
    sample_base_class &operator=(sample_base_class &&) = delete;	
};

struct dummy_type : public sample_base_class<>
{
};

static bool type_pool_templatization()
{
    bool status{true};
    avail_list<dummy_type> dummy_avail_list;
    status &= dummy_avail_list.head()->next() == nullptr;
    return status;
}

struct dummy_type_1 : public sample_base_class<>
{
    double m_d{};
    float m_f{};
    int m_i{};
};

static bool type_pool_type_pool_storage()
{
    bool status{true};
    type_pool_storage<dummy_type, dummy_type_1> tps;
    status &= (nullptr == tps.get_next<dummy_type_1>());
    auto dt1 = tps.create<dummy_type_1>();
    status &= (nullptr != dt1.get());
    status &= (nullptr == tps.get_head<dummy_type_1>());
    return status;
}

static bool type_pool_size_is_one()
{
    bool status{true};
    type_pool_storage<dummy_type, dummy_type_1> tps;
    status &= (nullptr == tps.get_next<dummy_type_1>());
    status &= (nullptr != tps.get_head<dummy_type_1>());
    auto dt1 = tps.create<dummy_type_1>();
    status &= (nullptr == tps.get_next<dummy_type_1>());
    status &= (nullptr == tps.get_head<dummy_type_1>());
    return status;
}

struct dummy_type_2 : public sample_base_class<10>
{
    bool init(double d, float f, int i)
    {
        m_d = d;
        m_f = f;
        m_i = i;
        return true;
    }

    double m_d{};
    float m_f{};
    int m_i{};
};

static bool type_pool_accessing_members()
{
    bool status{true};
    type_pool_storage<dummy_type, dummy_type_1, dummy_type_2> tps;
    auto dt1 = tps.create<dummy_type_2>(1., 1.f, 1);
    static_assert(std::is_same_v<decltype(dt1->m_d), double>);
    static_assert(std::is_same_v<decltype(dt1->m_f), float>);
    static_assert(std::is_same_v<decltype(dt1->m_i), int>);

    static_assert(1 == dummy_type::size);
    static_assert(10 == dummy_type_2::size);

    status &= (std::abs(1. - dt1->m_d) < 0.0000001f);
    status &= (std::abs(1.f - dt1->m_f) < 0.001f);
    status &= (1 == dt1->m_i);
    return status;
}

static bool type_pool_type_out_of_block()
{
    bool status{true};
    type_pool_storage<dummy_type> tps;
    auto dt1 = tps.create<dummy_type>();
    status &= (dt1.get() != nullptr);
    auto dt2 = tps.create<dummy_type>();
    status &= (dt2.get() == nullptr);
    return status;
}

int main()
{
    assert(type_pool_templatization());
    assert(type_pool_type_pool_storage());
    assert(type_pool_size_is_one());
    assert(type_pool_accessing_members());
    assert(type_pool_type_out_of_block());
    return 0;
}