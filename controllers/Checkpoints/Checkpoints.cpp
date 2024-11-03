#include <deque>
#include <tuple>

#include <webots/Keyboard.hpp>
#include <webots/Motor.hpp>
#include <webots/Robot.hpp>

constexpr double maxEpuckMotorSpeed{6.28};

void move(webots::Motor* left, webots::Motor* right, double speed) {
  left->setPosition(INFINITY);
  right->setPosition(INFINITY);
  left->setVelocity(speed);
  right->setVelocity(speed);
}

void turn(webots::Motor* left, webots::Motor* right, double speed) {
  left->setPosition(INFINITY);
  right->setPosition(INFINITY);
  left->setVelocity(-speed);
  right->setVelocity(speed);
}

// Waits for a duration (in milliseconds).
void wait(webots::Robot* robot, int duration) {
  const double start{robot->getTime()};
  const double timeStep{robot->getBasicTimeStep()};
  while (robot->getTime() - start < duration * 0.001) {
    robot->step(timeStep);
  }
}

std::deque<std::tuple<double, double, double>> checkpoints{
  {0, 0.8, M_PI / 2},
  {-0.3, 0.5, 0},
  {0, 0, 3 * M_PI / 2},
};

int main() {
  webots::Robot robot;
  webots::Motor* leftMotor{robot.getMotor("left wheel motor")};
  webots::Motor* rightMotor{robot.getMotor("right wheel motor")};  
  webots::Keyboard* keyboard{robot.getKeyboard()};
  
  double timeStep{robot.getBasicTimeStep()};
  
  keyboard->enable(timeStep);
  
  while (robot.step(timeStep) != -1) {
    // Control robot movement.
    char const key{static_cast<char>(keyboard->getKey())};
    switch (key) {
      case 'W':
        move(leftMotor, rightMotor, maxEpuckMotorSpeed);
        break;
      case 'A':
        turn(leftMotor, rightMotor, maxEpuckMotorSpeed / 2);
        break;
      case 'S':
        move(leftMotor, rightMotor, -maxEpuckMotorSpeed);
        break;
      case 'D':
        turn(leftMotor, rightMotor, -maxEpuckMotorSpeed / 2);
        break;
      default:
        move(leftMotor, rightMotor, 0);
    }
  }
}