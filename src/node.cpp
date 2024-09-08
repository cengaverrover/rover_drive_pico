#include "node.hpp"

#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>

#include <etl/array.h>

#include "BTS7960.hpp"
#include "pinout.hpp"
#include "encoder_substep.hpp"

namespace ros {

static etl::array<rcl_publisher_t, pinout::motorPwmL.size()> publishers{};
static etl::array<rover_drive_interfaces__msg__MotorFeedback, publishers.size()>
    msgs{};
static etl::array<QueueHandle_t, publishers.size()> publisherQueues{};
static QueueHandle_t driveQueue{};

static rover_drive_interfaces__msg__MotorDrive msgDrive = {
    .target_rpm = 0, .rotation_rads = 0.0f};

template <size_t i> void publisherTask(void *arg) {

  motor::BTS7960 motor(pinout::motorPwmL[i], pinout::motorPwmR[i]);
  encoder::EncoderSubstep encoder(pio0, i, pinout::encoderA[i]);

  rover_drive_interfaces__msg__MotorDrive driveMsgReceived{};
  rover_drive_interfaces__msg__MotorFeedback feedbackMsgSent{};
  TickType_t startTick{xTaskGetTickCount()};
  while (true) {
    if (xQueuePeek(driveQueue, &driveMsgReceived, 0) == pdTRUE) {
      feedbackMsgSent.dutycycle = driveMsgReceived.target_rpm / 200.0f;
      feedbackMsgSent.encoder_rpm = driveMsgReceived.target_rpm;
      feedbackMsgSent.current = feedbackMsgSent.dutycycle * 30.0f / 100.0f;
    }
    motor.setSpeed(feedbackMsgSent.dutycycle);
    xQueueOverwrite(publisherQueues[i], &feedbackMsgSent);
    xTaskDelayUntil(&startTick, pdMS_TO_TICKS(50));
  }
}
extern "C" {

static void publisherTimerCallback(rcl_timer_t *timer, int64_t last_call_time) {
  rover_drive_interfaces__msg__MotorFeedback feedbackMsgBuffer{};
  for (int i = 0; i < publishers.size(); i++) {
    if (xQueueReceive(publisherQueues[i], &feedbackMsgBuffer, 0) == pdTRUE) {
      rcl_ret_t ret = rcl_publish(&publishers[i], &feedbackMsgBuffer, NULL);
    }
  }
}

static void driveSubscriberCallback(const void *msgin) {
  if (msgin != NULL) {
    auto msg =
        static_cast<const rover_drive_interfaces__msg__MotorDrive *>(msgin);
    xQueueOverwrite(driveQueue, msg);
  }
}

static void microRosTask(void *arg) {
  vTaskSuspendAll();

  rcl_allocator_t allocator = rcl_get_default_allocator();

  rclc_support_t support{};
  rclc_support_init(&support, 0, NULL, &allocator);

  rcl_node_t node = rcl_get_zero_initialized_node();
  rclc_node_init_default(&node, "pico_node", "drive", &support);

  constexpr etl::array<etl::string_view, 4> names{
      "pico_publisher_0", "pico_publisher_1", "pico_publisher_2",
      "pico_publisher_3"};
  for (int i = 0; i < 4; i++) {
    publishers[i] = rcl_get_zero_initialized_publisher();
    rclc_publisher_init_default(
        &publishers[i], &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(rover_drive_interfaces, msg, MotorFeedback),
        names[i].data());
    publisherQueues[i] =
        xQueueCreate(1, sizeof(rover_drive_interfaces__msg__MotorFeedback));
  }

  rcl_subscription_t driveSubscriber = rcl_get_zero_initialized_subscription();
  driveSubscriber = rcl_get_zero_initialized_subscription();
  rclc_subscription_init_default(
      &driveSubscriber, &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(rover_drive_interfaces, msg, MotorDrive),
      "pico_drive");
  driveQueue = xQueueCreate(1, sizeof(rover_drive_interfaces__msg__MotorDrive));

  rcl_timer_t publisherTimer = rcl_get_zero_initialized_timer();
  publisherTimer = rcl_get_zero_initialized_timer();
  rclc_timer_init_default(&publisherTimer, &support, RCL_MS_TO_NS(100),
                          publisherTimerCallback);

  rclc_parameter_options_t paramServerOptions{};
  paramServerOptions = {.notify_changed_over_dds = true,
                        .max_params = 10,
                        .allow_undeclared_parameters = true,
                        .low_mem_mode = false};

  rclc_parameter_server_t paramServer{};
  rclc_parameter_server_init_with_option(&paramServer, &node,
                                         &paramServerOptions);

  rclc_executor_t executor = rclc_executor_get_zero_initialized_executor();
  rclc_executor_init(&executor, &support.context,
                     RCLC_EXECUTOR_PARAMETER_SERVER_HANDLES + 2, &allocator);

  rclc_executor_add_subscription(&executor, &driveSubscriber, &msgDrive,
                                 driveSubscriberCallback, ON_NEW_DATA);
  rclc_executor_add_timer(&executor, &publisherTimer);
  rclc_executor_add_parameter_server(&executor, &paramServer, NULL);

  rclc_add_parameter(&paramServer, "max_motor_rpm", RCLC_PARAMETER_INT);
  rclc_add_parameter(&paramServer, "max_motor_dutycycle",
                     RCLC_PARAMETER_DOUBLE);
  rclc_add_parameter_constraint_double(&paramServer, "max_motor_dutycycle", 0,
                                       100.0, 0);

  rclc_parameter_set_int(&paramServer, "max_motor_rpm", 20000);
  rclc_parameter_set_double(&paramServer, "max_motor_dutycycle", 100.0);

  xTaskCreateAffinitySet(publisherTask<0>, "publisher_task_0", 500, 0, 3, 0x03,
                         nullptr);
  xTaskCreateAffinitySet(publisherTask<1>, "publisher_task_1", 500, 0, 3, 0x03,
                         nullptr);
  xTaskCreateAffinitySet(publisherTask<2>, "publisher_task_2", 500, 0, 3, 0x03,
                         nullptr);
  xTaskCreateAffinitySet(publisherTask<3>, "publisher_task_3", 500, 0, 3, 0x03,
                         nullptr);

  xTaskResumeAll();
  while (true) {
    // vTaskSuspendAll();
    rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));
    // xTaskResumeAll();
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}
}

void Node::spin() {
  xTaskCreateAffinitySet(microRosTask, "executor_task", 4000, nullptr, 4, 0x01,
                         nullptr);
  vTaskStartScheduler();
}

} // namespace ros