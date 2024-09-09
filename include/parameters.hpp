#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

#include <pico/types.h>
#include <rclc_parameter/rclc_parameter.h>

namespace ros {

namespace parameter {

class Server {
public:
  Server(rcl_node_t *node,
                  rclc_parameter_options_t *serverOptions = nullptr);

  rcl_ret_t addToExecutor(rclc_executor_t *executor);
  rcl_ret_t initParameters();

private:
  rclc_parameter_server_t paramServer_{};
};


inline int32_t maxMotorRpm = 20000;

inline float maxMotorDutyCycle = 100.0f;
constexpr inline float maxMotorDutyCycleUpperConstraint = 100.0f;
constexpr inline float maxMotorDutyCycleLowerConstraint = 0.0f;


inline float maxMotorCurrent = 50.0f;

inline int32_t motorPidLoopPeriodMs = 50;

inline int32_t motorTimeoutMs = 1000;

inline bool motorPidMode = true;

inline int32_t motorFeedbackPeriodMs = 100;

} // namespace parameter

} // namespace ros

#endif // PARAMETERS_HPP