class Ground {
  private:
  Rectangle line;
  public:
  void initialize() {
    line.initialize(1000,5);
  }

  void createGround(glm::mat4 VP) {
    line.makeRectangle(VP, -500, -300, 0, 0, 0);
  }
};
