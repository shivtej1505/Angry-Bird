class Ground {
  private:
  Rectangle line;
  public:
  void initialize() {
    line.initialize(800,5);
  }

  void createGround(glm::mat4 VP) {
    line.makeRectangle(VP, -400, -300, 0, 0, 0);
  }
};
