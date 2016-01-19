// Handles all action of birds
#include "Circle.cpp"
class Bird {
  private:
  Circle circle;
  float initial_velocity;
  float projection_angle;
  float bird_center_x;
  float bird_center_y;
  double fly_time;
  bool bird_can_fly;

  public:
  Bird() {
    initial_velocity = 0.0f;
    projection_angle = 45.0f * (M_PI/180.0f);
    bird_can_fly = false;
    bird_center_x = -3.5f;
    bird_center_y = -3.5f;
    fly_time = 0.0f;
  }
  void initialize() {
    printf("LOL\n");
    circle.initialize(1,1, 0.1);
  }

  void createBird(glm::mat4 VP) {
    circle.makeCircle(VP,bird_center_x ,bird_center_y);
  }

  void flyBird() {
    if (bird_can_fly) {
      fly_time += 0.05;
      bird_center_x += (initial_velocity * cos(projection_angle));
      bird_center_y += (initial_velocity * sin(projection_angle) - 0.1 * fly_time);
      //printf("%f\n", increase_velocity);
    }
  }

  void increase_angle() {
    if (!bird_can_fly) {
      projection_angle += 5 * (M_PI/180.0f);
      printf("%f\n",sin(projection_angle) );
      printf("%f\n",projection_angle * (180.0f/M_PI));
    }
  }

  void decrease_angle() {
    if (!bird_can_fly) {
      projection_angle -= 5 * (M_PI/180.0f);
      printf("%f\n",projection_angle * (180.0f/M_PI));
    }
  }

  void increase_velocity() {
    if (!bird_can_fly) {
      initial_velocity += 0.08;
      printf("%f\n",initial_velocity );
    }
  }

  void decrease_velocity() {
    if (!bird_can_fly) {
      initial_velocity -= 0.08;
      printf("%f\n",initial_velocity );
    }
  }

  void set_fly_status() {
    bird_can_fly = !bird_can_fly;
  }
};
