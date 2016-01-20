#include "Rectangle.cpp"

class Cannon {
  private:
  Rectangle rectangle;
  float cannonAngle;
  static const float ANGLE_DIFF = 5.0f;
  public:
  Cannon() {
    cannonAngle = 45.0f;
    printf("Cannon fired\n");
  }
  void initialize() {
    rectangle.initialize(80, 30);
  }
  void createCannon(glm::mat4 VP) {
    rectangle.makeRectangle(VP, -380, -275, cannonAngle, 20, 15);
  }

  void decreaseAngle() {
    cannonAngle -= ANGLE_DIFF;
  }

  void increaseAngle() {
    cannonAngle += ANGLE_DIFF;
  }
};
