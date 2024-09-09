#include "node.hpp"

#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>

#include <etl/array.h>

#include "parameters.hpp"
#include "publisher.hpp"
#include "queues.hpp"

namespace ros {

static etl::array<rover_drive_interfaces__msg__MotorDrive, 4> driveMsgs{};

template <size_t i> static void driveSubscriberCallback(const void *msgin) {
  if (msgin != NULL) {
    auto msg =
        static_cast<const rover_drive_interfaces__msg__MotorDrive *>(msgin);
    xQueueOverwrite(driveQueues[i], msg);
  }
}

extern "C" {

static void microRosTask(void *arg) {
  vTaskSuspendAll();

  rcl_allocator_t allocator = rcl_get_default_allocator();

  rclc_support_t support{};
  rclc_support_init(&support, 0, NULL, &allocator);

  rcl_node_t node = rcl_get_zero_initialized_node();
  rclc_node_init_default(&node, "pico_node", "drive", &support);

  initPublishers(&node);

  constexpr etl::array<etl::string_view, 4> subscriberNames{
      "pico_subscriber_0", "pico_subscriber_1", "pico_subscriber_2",
      "pico_subscriber_3"};
  etl::array<rcl_subscription_t, 4> driveSubscribers{};
  for (int i = 0; i < 4; i++) {
    driveSubscribers[i] = rcl_get_zero_initialized_subscription();
    rclc_subscription_init_default(
        &driveSubscribers[i], &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(rover_drive_interfaces, msg, MotorDrive),
        subscriberNames[i].data());

    publisherQueues[i] =
        xQueueCreate(1, sizeof(rover_drive_interfaces__msg__MotorFeedback));

    driveQueues[i] =
        xQueueCreate(1, sizeof(rover_drive_interfaces__msg__MotorDrive));
  }

  rcl_timer_t publisherTimer = rcl_get_zero_initialized_timer();
  publisherTimer = rcl_get_zero_initialized_timer();
  rclc_timer_init_default(&publisherTimer, &support, RCL_MS_TO_NS(100),
                          publisherTimerCallback);

  rclc_parameter_options_t paramServerOptions{};
  paramServerOptions = {.notify_changed_over_dds = true,
                        .max_params = 10,
                        .allow_undeclared_parameters = true,
                        .low_mem_mode = false};

  parameter::Server paramServer(&node, &paramServerOptions);

  rclc_executor_t executor = rclc_executor_get_zero_initialized_executor();
  rclc_executor_init(&executor, &support.context, 5, &allocator);

  rclc_executor_add_subscription(&executor, &driveSubscribers[0], &driveMsgs[0],
                                 driveSubscriberCallback<0>, ON_NEW_DATA);
  rclc_executor_add_subscription(&executor, &driveSubscribers[1], &driveMsgs[1],
                                 driveSubscriberCallback<1>, ON_NEW_DATA);
  rclc_executor_add_subscription(&executor, &driveSubscribers[2], &driveMsgs[2],
                                 driveSubscriberCallback<2>, ON_NEW_DATA);
  rclc_executor_add_subscription(&executor, &driveSubscribers[3], &driveMsgs[3],
                                 driveSubscriberCallback<3>, ON_NEW_DATA);
  rclc_executor_add_timer(&executor, &publisherTimer);

  rclc_executor_t paramServerExecutor =
      rclc_executor_get_zero_initialized_executor();
  rclc_executor_init(&paramServerExecutor, &support.context,
                     RCLC_EXECUTOR_PARAMETER_SERVER_HANDLES, &allocator);
  paramServer.addToExecutor(&paramServerExecutor);
  paramServer.initParameters();

  createPublisherTasks(500, 3, 0x03);

  xTaskResumeAll();
  while (true) {
    // vTaskSuspendAll();
    rclc_executor_spin_some(&executor, RCL_MS_TO_NS(1));
    rclc_executor_spin_some(&paramServerExecutor, RCL_MS_TO_NS(1));
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