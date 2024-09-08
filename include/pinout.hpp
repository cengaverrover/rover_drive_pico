#ifndef PINS_HPP
#define PINS_HPP

#include <etl/array.h>
#include <pico/types.h>
namespace pinout {

constexpr uint led = 25;
constexpr etl::array<uint, 4> motorPwmL{};
constexpr etl::array<uint, 4> motorPwmR{};
} // namespace pinout

#endif // PINS_HPP