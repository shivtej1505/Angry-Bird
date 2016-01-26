class Ground {
  private:
  Rectangle line;
  public:
  void initialize() {
    line.initialize(2000,5);
  }

  void createGround(glm::mat4 VP) {
    line.makeRectangle(VP, -1000, -300, 0, 0, 0);
  }
};
