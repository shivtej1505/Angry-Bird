class Obstacle {
  private:
  Rectangle rectangle;
  float angle;
  float length;
  float breath;
  float center_x;
  float center_y;

  public:
  bool isCollide;

  public:
  Obstacle(float cen_x, float cen_y, float l, float b, bool fill) {
    angle = 0.0f;
    center_x = cen_x;
    center_y = cen_y;
    length = l;
    breath = b;
    isCollide = false;
    initialize(fill);
  }

  void initialize(bool fill) {
    rectangle.initialize(length, breath, fill);
  }

  void createObstacle(glm::mat4 VP) {
    rectangle.makeRectangle(VP, center_x, center_y, angle, 0, 0);
    //angle += 3.0f;
  }

  void resetObstacle() {
    center_x = rand()%700 - 200;
    center_y = rand()%1000 + 1000;
  }
  void dropObstacle() {
    center_y -= 5.0f;
    //center_x -= 1.0f;
  }
  // vertical left side passing from (center_x, center_y)
  bool isCollidingFromSideA(int x, int y) {
    float dis_x = abs(x - center_x);
    float dis_y = (y - center_y);
    if ( dis_x <= 10 && (dis_y) <= breath  && dis_y >= 0)
      return true;
    return false;
  }

  bool isCollidingFromSideB(int x, int y) {
    float dis_x = x - center_x;
    float dis_y = abs(y - center_y - breath);
    if ((dis_x) <= length  && dis_x >= 0 &&  dis_y <= 10)
      return true;
    return false;
  }

  bool isCollidingFromSideC(int x, int y) {
    float dis_x = abs(x - center_x - length);
    float dis_y = (y - center_y);
    if ( dis_x <= 10 && (dis_y) <= breath  && dis_y >= 0)
      return true;
    return false;
  }

  bool isCollidingFromSideD(int x, int y) {
    float dis_x = x - center_x;
    float dis_y = abs(y - center_y);
    if ((dis_x) <= length  && dis_x >= 0 &&  dis_y <= 10)
      return true;
    return false;
  }
};
