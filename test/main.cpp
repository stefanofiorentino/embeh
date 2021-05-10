#include <cassert>
#include <type_traits>
#include <memory>

#include <include/lefticus/static_test.hpp>
#include <embeh/type_pool_storage.hpp>

using embeh::avail_list;
using embeh::type_pool_storage;

template <std::size_t N=1>	
struct type_pool_storage_base	
{	
    static constexpr std::size_t size = N;	

    virtual ~type_pool_storage_base() noexcept = default;	

    bool init()	
    {	
        return true;	
    }	

private:	
    type_pool_storage_base() = default;	
    type_pool_storage_base(type_pool_storage_base const &) = delete;	
    type_pool_storage_base &operator=(type_pool_storage_base const &) = delete;	
    type_pool_storage_base(type_pool_storage_base &&) = delete;	
    type_pool_storage_base &operator=(type_pool_storage_base &&) = delete;	
};

struct dummy_type : public type_pool_storage_base<>
{
};

static bool type_pool_templatization()
{
    bool status{true};
    avail_list<dummy_type> dummy_avail_list;
    status &= dummy_avail_list.head()->next() == nullptr;
    return status;
}

struct dummy_type_1 : public type_pool_storage_base<>
{
    double m_d{};
    float m_f{};
    int m_i{};
};

bool type_pool_type_pool_storage()
{
    bool status{true};
    type_pool_storage<dummy_type, dummy_type_1> tps;
    status &= (nullptr == tps.get_next<dummy_type_1>());
    auto dt1 = tps.create<dummy_type_1>();
    status &= (nullptr != dt1.get());
    status &= (nullptr == tps.get_head<dummy_type_1>());
    return status;
}

bool type_pool_size_is_one()
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

struct dummy_type_2 : public type_pool_storage_base<10>
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

bool type_pool_accessing_members()
{
    bool status{true};
    type_pool_storage<dummy_type, dummy_type_1, dummy_type_2> tps;
    auto dt1 = tps.create<dummy_type_2>(1., 1.f, 1);
    static_assert(std::is_same_v<decltype(dt1->m_d), double>);
    static_assert(std::is_same_v<decltype(dt1->m_f), float>);
    static_assert(std::is_same_v<decltype(dt1->m_i), int>);

    static_test<1 == dummy_type::size>();
    static_test<10 == dummy_type_2::size>();

    status &= (std::abs(1. - dt1->m_d) < 0.0000001f);
    status &= (std::abs(1.f - dt1->m_f) < 0.001f);
    status &= (1 == dt1->m_i);
    return status;
}

bool type_pool_type_out_of_block()
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
    assert(type_pool_type_out_of_block());
    return 0;
}