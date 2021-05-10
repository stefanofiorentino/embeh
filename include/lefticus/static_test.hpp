#pragma once

template<bool B> 
inline constexpr auto static_test(){
  static_assert(B);
  return B;
}