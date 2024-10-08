/**
 * @file parameter.cpp
 * @author Alper Tunga Güven (alpert.guven@gmail.com)
 * @brief Source file of MicroROS parameter related functions.
 * @version 0.1
 * @date 2024-09-09
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "parameters.hpp"
#include <etl/string_view.h>
#include <etl/unordered_map.h>

namespace ros {

namespace parameter {

constexpr static etl::string_view maxMotorRpmName{ "motor_max_rpm" };
constexpr static etl::string_view maxMotorDutyCycleName{ "motor_max_dutycycle" };
constexpr static etl::string_view maxMotorCurrentName{ "motor_max_current" };
constexpr static etl::string_view motorPidLoopPeriodMsName{ "motor_pid_loop_period_ms" };
constexpr static etl::string_view executorSpinPeriodMsName{ "executor_spin_period_ms" };
constexpr static etl::string_view motorTimeoutMsName{ "motor_timeout_ms" };
constexpr static etl::string_view motorFeedbackPeriodMsName{ "motor_feedback_period_ms" };
constexpr static etl::string_view motorPidModeName{ "motor_pid_mode" };
constexpr static etl::string_view motorPidKpName{ "motor_pid_kp" };
constexpr static etl::string_view motorPidKiName{ "motor_pid_ki" };
constexpr static etl::string_view motorPidKdName{ "motor_pid_kd" };

static etl::unordered_map<etl::string_view, void*, 11> parameters{
    {maxMotorRpmName,            &maxMotorRpm          },
    { maxMotorDutyCycleName,     &maxMotorDutyCycle    },
    { maxMotorCurrentName,       &maxMotorCurrent      },
    { motorPidLoopPeriodMsName,  &motorPidLoopPeriodMs },
    { executorSpinPeriodMsName,  &executorSpinPeriodMs },
    { motorTimeoutMsName,        &motorTimeoutMs       },
    { motorFeedbackPeriodMsName, &motorFeedbackPeriodMs},
    { motorPidModeName,          &motorPidMode         },
    { motorPidKpName,            &motorPidKp           },
    { motorPidKiName,            &motorPidKi           },
    { motorPidKdName,            &motorPidKd           },
};

extern "C" bool onParameterChange(
    const Parameter* oldParam, const Parameter* newParam, void* context);

Server::Server(rcl_node_t* node, rclc_parameter_options_t* serverOptions) {
    if (serverOptions == nullptr) {
        rclc_parameter_server_init_default(&paramServer_, node);
    } else {
        rclc_parameter_server_init_with_option(&paramServer_, node, serverOptions);
    }
}

Server::Server(rcl_node_t* node, bool notify_changed_over_dds, uint32_t max_params,
    bool allow_undeclared_parameters, bool low_mem_mode) {
    rclc_parameter_options_t options{ .notify_changed_over_dds = notify_changed_over_dds,
        .max_params = max_params,
        .allow_undeclared_parameters = allow_undeclared_parameters,
        .low_mem_mode = low_mem_mode };
    rclc_parameter_server_init_with_option(&paramServer_, node, &options);
}

rcl_ret_t Server::addToExecutor(rclc_executor_t* executor) {
    rcl_ret_t ret = rclc_executor_add_parameter_server_with_context(
        executor, &paramServer_, onParameterChange, this);
    ret += initParameters();
    return ret;
}

rcl_ret_t Server::addParameter(etl::string_view paramName, rclc_parameter_type_t paramType) {
    return rclc_add_parameter(&paramServer_, paramName.data(), paramType);
}

rcl_ret_t Server::addParameter(etl::string_view paramName, int32_t value) {
    rcl_ret_t ret = addParameter(paramName, RCLC_PARAMETER_INT);
    ret += setParameter(paramName, value);
    return ret;
}

rcl_ret_t Server::addParameter(etl::string_view paramName, float value) {
    rcl_ret_t ret = addParameter(paramName, RCLC_PARAMETER_DOUBLE);
    ret += setParameter(paramName, value);
    return ret;
}

rcl_ret_t Server::addParameter(etl::string_view paramName, bool value) {
    rcl_ret_t ret = addParameter(paramName, RCLC_PARAMETER_BOOL);
    ret += setParameter(paramName, value);
    return ret;
}

rcl_ret_t Server::addParameterConstraint(
    etl::string_view paramName, int32_t lower, int32_t upper, int32_t step) {
    return rclc_add_parameter_constraint_integer(
        &paramServer_, paramName.data(), lower, upper, step);
}

rcl_ret_t Server::addParameterConstraint(
    etl::string_view paramName, float lower, float upper, float step) {
    return rclc_add_parameter_constraint_double(
        &paramServer_, paramName.data(), lower, upper, step);
}


rcl_ret_t Server::setParameter(etl::string_view paramName, int32_t paramValue) {
    return rclc_parameter_set_int(&paramServer_, paramName.data(), paramValue);
}

rcl_ret_t Server::setParameter(etl::string_view paramName, float paramValue) {
    return rclc_parameter_set_double(&paramServer_, paramName.data(), paramValue);
}

rcl_ret_t Server::setParameter(etl::string_view paramName, bool paramValue) {
    return rclc_parameter_set_bool(&paramServer_, paramName.data(), paramValue);
}


rcl_ret_t Server::initParameters() {
    rcl_ret_t ret = 0;
    // Add parameters to the server.
    ret += addParameter(maxMotorRpmName, maxMotorRpm);
    ret += addParameter(maxMotorDutyCycleName, maxMotorDutyCycle);
    ret += addParameterConstraint(maxMotorDutyCycleName, maxMotorDutyCycleLowerConstraint,
        maxMotorDutyCycleUpperConstraint, 0.0f);

    ret += addParameter(maxMotorCurrentName, maxMotorCurrent);
    ret += addParameter(motorPidLoopPeriodMsName, motorPidLoopPeriodMs);
    ret += addParameter(executorSpinPeriodMsName, executorSpinPeriodMs);
    ret += addParameter(motorTimeoutMsName, motorTimeoutMs);
    ret += addParameter(motorFeedbackPeriodMsName, motorFeedbackPeriodMs);
    ret += addParameter(motorPidModeName, motorPidMode);

    ret += addParameter(motorPidKpName, motorPidKp);
    ret += addParameter(motorPidKdName, motorPidKi);
    ret += addParameter(motorPidKiName, motorPidKd);
    return ret;
}

bool onParameterChange(const Parameter* oldParam, const Parameter* newParam, void* context) {
    auto paramServer = static_cast<parameter::Server*>(context);
    if (oldParam == nullptr || newParam == nullptr || paramServer == nullptr) {
        return false;
    }

    void* const param = parameters[newParam->name.data];
    if (param != nullptr) {
        // Amazing typesafety...
        switch (newParam->value.type) {
            case RCLC_PARAMETER_INT:
                *static_cast<int32_t*>(param) = newParam->value.integer_value;
                break;
            case RCLC_PARAMETER_DOUBLE:
                *static_cast<float*>(param) = newParam->value.double_value;
                break;
            case RCLC_PARAMETER_BOOL:
                *static_cast<bool*>(param) = newParam->value.bool_value;
                break;
        }
        return true;
    }
    return false;
}

} // namespace parameter
} // namespace ros