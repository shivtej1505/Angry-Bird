#include "Rectangle.cpp"

class Cannon {
  private:
  Rectangle cannon_pipe;
  Circle cannon_stand;
  float cannonAngle;
  static const float ANGLE_DIFF = 5.0f;
  public:
  Cannon() {
    cannonAngle = 45.0f;
  }
  
  void initialize() {
    cannon_pipe.initialize(80, 30);
    cannon_stand.initialize(-360, -260, 32, false);
  }
  void createCannon(glm::mat4 VP) {
    cannon_pipe.makeRectangle(VP, -380, -275, cannonAngle, 20, 15);
    cannon_stand.makeCircle(VP, -360, -260);
  }

  void decreaseAngle() {
    cannonAngle -= ANGLE_DIFF;
  }

  void increaseAngle() {
    cannonAngle += ANGLE_DIFF;
  }
};
