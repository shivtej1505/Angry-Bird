// Handles all action of birds
#include "Circle.cpp"
class Bird {
  private:
  Circle circle;
  float initial_velocity;
  float projection_angle;
  float bird_center_x;
  float bird_center_y;
  bool bird_can_fly;

  public:
  Bird() {
    initial_velocity = 0.0f;
    projection_angle = 4 * (M_PI/180.0f);
    bird_can_fly = false;
    bird_center_x = -3.5f;
    bird_center_y = -3.5f;
  }
  void initialize() {
    printf("LOL\n");
    circle.initialize(1,1, 0.3);
  }

  void createBird(glm::mat4 VP) {
    if (bird_can_fly) {
      circle.makeCircle(VP, bird_center_x - (initial_velocity * 0.01),
        bird_center_y - (initial_velocity * 0.01));
    }
  }

  void flyBird() {
    //printf("%f\n", increase_velocity);
  }

  void increase_velocity() {
    initial_velocity += 0.5;
  }

  void decrease_velocity() {
    initial_velocity -= 0.5;
  }

  void set_fly_status() {
    bird_can_fly = !bird_can_fly;
  }
};
