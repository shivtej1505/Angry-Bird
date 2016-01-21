// Handles all action of birds
#include "Circle.cpp"

class Bird {
  private:
  static const float VELOCITY_DIFF = 5.0f;
  static const float ANGLE_DIFF = 5.0f;
  static const float GRAVITY = 5.0f;
  Circle circle;
  float initial_velocity;
  float projection_angle;
  float bird_center_x;
  float bird_center_y;
  double fly_time;
  bool bird_can_fly;

  int flag;
  public:
  Bird(int x, int y) {
    initial_velocity = 10.0f;
    projection_angle = 45.0f * (M_PI/180.0f);
    bird_can_fly = true;
    bird_center_x = x;
    bird_center_y = y;
    fly_time = 0.0f;
    circle.initialize(bird_center_x, bird_center_y, 10, true);
  }

  /*private:
  void initialize(int x, int y) {
    bird_center_x = x;
    bird_center_y = y;
    circle.initialize(bird_center_x, bird_center_y, 10, true);
    printf("Initialized\n");
  }*/

  public:
  void print(int n) {
    printf("I'm bird no %d\n",n );
    printf("Deb Flag%d\n",flag );
    printf("%d\n",bird_can_fly );
    printf("%f\n",bird_center_x );
    printf("%f\n",bird_center_y );
  }


  void createBird(glm::mat4 VP) {
    circle.makeCircle(VP,bird_center_x ,bird_center_y);
  }

  void flyBird() {
    //if (bird_can_fly) {
      fly_time += 0.05;
      bird_center_x += 10;// (initial_velocity * cos(projection_angle));
      bird_center_y += 10;//(initial_velocity * sin(projection_angle) - GRAVITY * fly_time);
      //printf("%f\n", bird_center_x);
      //printf("%f\n", bird_center_x);
    //}
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
      initial_velocity += VELOCITY_DIFF;
      printf("%f\n",initial_velocity );
    }
  }

  void decrease_velocity() {
    if (!bird_can_fly) {
      initial_velocity -= VELOCITY_DIFF;
      printf("%f\n",initial_velocity );
    }
  }

  void set_fly_status() {
    bird_can_fly = !bird_can_fly;
  }

  bool get_fly_status() {
    return true;
  }

  void set_deb_flag(int num) {
    flag = num;
  }
};
