// Handles all action of birds
#include "Circle.cpp"

class Bird {
  private:
  static const float VELOCITY_DIFF = 2.0f;
  static const float ANGLE_DIFF = 5.0f;
  static const float GRAVITY = 9.8f;
  static const float COFFICENT_OF_FRICTION = 0.3f;
  Circle circle;
  float initial_velocity;
  float velocity_x;
  float velocity_y;
  float projection_angle;
  float bird_center_x;
  float bird_center_y;
  float bird_radius;
  double fly_time;
  bool bird_can_fly;
  bool bird_on_cannon;

  public:
  bool colliding_with_ground;

  int flag;
  public:
  Bird(int x, int y) {
    initial_velocity = 15.0f;
    projection_angle = 45.0f * (M_PI/180.0f);
    bird_can_fly = false;
    bird_on_cannon = false;
    colliding_with_ground = false;
    bird_center_x = x;
    bird_center_y = y;
    bird_radius = 10;
    fly_time = 0.0f;
    circle.initialize(bird_center_x, bird_center_y, bird_radius, true);
  }

  private:
  void change_velcities() {
    velocity_x = velocity_x;
    velocity_y = velocity_y - (GRAVITY * 0.02);
    if (colliding_with_ground && velocity_x > 0) {
      velocity_x -= (COFFICENT_OF_FRICTION * 3);
    }
  }

  void set_velocities() {
    velocity_x = initial_velocity * cos(projection_angle);
    velocity_y = initial_velocity * sin(projection_angle);
  }
  /*void initialize(int x, int y) {
    bird_center_x = x;
    bird_center_y = y;
    circle.initialize(bird_center_x, bird_center_y, 10, true);
    printf("Initialized\n");
  }*/

  public:
  void print(int n) {
    printf("I'm bird no %d\n",n );
    //printf("Deb Flag%d\n",flag );
    printf("%d\n",bird_can_fly );
    printf("%f\n",bird_center_x );
    printf("%f\n",bird_center_y );
    printf("%f\n",projection_angle * (180.0f/M_PI));
  }

  void createBird(glm::mat4 VP) {
    circle.makeCircle(VP,bird_center_x ,bird_center_y);
  }

  bool flyBird() {
    //printf("Pro%f\n",projection_angle * (180.0f/M_PI));
    //printf("Fly%d\n", bird_can_fly);
    //printf("onCannon%d\n", bird_on_cannon);
    if (bird_can_fly && !bird_on_cannon && !is_outside_screen()) {
      fly_time += 0.02;
      change_velcities();
      bird_center_x += velocity_x;
      bird_center_y += velocity_y;
    } else if (is_outside_screen()) {
      bird_on_cannon = false;
    }
  }


  void increase_angle() {
    if (!bird_can_fly && bird_on_cannon) {
      projection_angle += 5 * (M_PI/180.0f);
      printf("%f\n",projection_angle * (180.0f/M_PI));
    }
  }

  void decrease_angle() {
    if (!bird_can_fly && bird_on_cannon) {
      projection_angle -= 5 * (M_PI/180.0f);
      printf("%f\n",projection_angle * (180.0f/M_PI));
    }
  }

  void increase_velocity() {
    if (!bird_can_fly && bird_on_cannon ) {
      initial_velocity += VELOCITY_DIFF;
      printf("%f\n",initial_velocity );
    }
  }

  void decrease_velocity() {
    if (!bird_can_fly && bird_on_cannon ) {
      initial_velocity -= VELOCITY_DIFF;
      printf("%f\n",initial_velocity );
    }
  }

  void set_velocity(float velocity) {
    initial_velocity = velocity;
  }

  float get_velocity() {
    return initial_velocity;
  }

  void set_projection_angle(float angle) {
    projection_angle = angle * (M_PI/180.0f);
  }

  float get_projection_angle() {
    return projection_angle * (180.0f/M_PI);
  }

  void set_fly_status(bool value) {
    if (!bird_can_fly && value) {
      set_velocities();
      printf("set velocities\n" );
    }
    bird_can_fly = value;
  }

  bool get_fly_status() {
    return bird_can_fly;
  }

  void set_deb_flag(int num) {
    flag = num;
  }

  void set_bird_on_cannon(bool on_cannon) {
    bird_on_cannon = on_cannon;
  }

  bool get_bird_on_cannon() {
    return bird_on_cannon;
  }

  void put_on_cannon() {
    bird_center_x = -360;
    bird_center_y = -260;
    printf("%f\n",projection_angle * (180.0f/M_PI));
  }

  void move_forward() {
    bird_center_x += 23;
  }

  float get_center_x() {
    return bird_center_x;
  }

  float get_center_y() {
    return bird_center_y;
  }

  void power_two() {
    velocity_x += 10.0f;
  }

  // cor -> cofficient of restitution
  // collision_angle -> angle made by x-component of velocity of bird with line of impact
  void collision(float collision_angle,float cor = 1.0f) {
    if (collision_angle == 0.0) {
      // bird collided with a wall perpendicular to x-axis
      //printf("Collison with obstacle\n" );
      velocity_x = -(cor * velocity_x);
    } else if (collision_angle == 90.0) {
      // ball collided with ground
      //printf("Collison with ground\n" );
      velocity_y = -(cor * velocity_y);
    }
  }

  bool is_outside_screen() {
    if (bird_center_x - bird_radius > MAX_X) {
      return true;
    } else if (bird_center_x + bird_radius  < MIN_X) {
      return true;
    } else if (bird_center_y - bird_radius  > MAX_Y &&
        bird_center_x - bird_radius > MAX_X &&
        bird_center_x + bird_radius  < MIN_X) {
      return true;
    } else if (bird_center_y + bird_radius  < MIN_Y) {
      return true;
    }
    return false;
  }
};
