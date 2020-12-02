#pragma once

#include <memory>

void *operator new(std::size_t)
{
    throw std::bad_alloc();
}
void *operator new(std::size_t, std::nothrow_t) noexcept
{
    std::terminate();
}
void *operator new(std::size_t, std::align_val_t)
{
    throw std::bad_alloc();
}
void *operator new(std::size_t, std::align_val_t, std::nothrow_t) noexcept
{
    std::terminate();
}