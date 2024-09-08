#ifndef PINS_HPP
#define PINS_HPP

#include <etl/array.h>
#include <pico/types.h>
#include <hardware/pio.h>
namespace pinout {

constexpr uint led = 25;
constexpr etl::array<uint, 4> motorPwmL{};
constexpr etl::array<uint, 4> motorPwmR{};

const PIO encoderPio = pio0;
constexpr etl::array<uint, 4> encoderA{};
constexpr etl::array<uint, 4> encoderB{};
constexpr etl::array<uint, 4> encoderPioSm{0, 1, 2, 3};
} // namespace pinout

#endif // PINS_HPP