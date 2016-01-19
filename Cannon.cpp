#include "Rectangle.cpp"

class Cannon {
  private:
  Rectangle rectangle;
  float cannonAngle;
  public:
  Cannon() {
    cannonAngle = 45.0f;
    printf("Cannon fired\n");
  }
  void initialize() {
    rectangle.initialize(1,0.2);
  }
  void createCannon(glm::mat4 VP) {
    rectangle.makeRectangle(VP, -3.8, -3.8, cannonAngle);
  }

  void decreaseAngle() {
    cannonAngle -= 5.0;
  }

  void increaseAngle() {
    cannonAngle += 5.0;
  }
};
