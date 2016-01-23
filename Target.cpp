class Target {
  private:
  Rectangle rectangle;
  float angle;

  public:
  Target() {
    angle = 0.0f;
    //rectangle.initialize(1, 1);
  }

  void initialize() {
    rectangle.initialize(100, 100);
  }

  void createTarget(glm::mat4 VP) {
    rectangle.makeRectangle(VP, 200, 200, angle, 10, 10);
    angle += 1.0f;
  }
};
