#include "queues.hpp"
#include <rover_drive_interfaces/msg/motor_drive.h>
#include <rover_drive_interfaces/msg/motor_feedback.h>

namespace freertos {
void initQueues() {
  for (int i = 0; i < publisherQueues.size(); i++) {
    publisherQueues[i] =
        xQueueCreate(1, sizeof(rover_drive_interfaces__msg__MotorFeedback));

    driveQueues[i] =
        xQueueCreate(1, sizeof(rover_drive_interfaces__msg__MotorDrive));
  }
}

} // namespace freertos
