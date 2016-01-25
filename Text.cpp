class Text {
  /*private:
  static int fontScale;
  float fontScaleValue;*/

  public:
  void drawScore(int score,glm::vec3 fontColor, float fontScaleValue) {
    glm::mat4 MVP;
    Matrices.model = glm::mat4(1.0f);
  	glm::mat4 translateText = glm::translate(glm::vec3(-490,275,0));
  	glm::mat4 scaleText = glm::scale(glm::vec3(fontScaleValue,fontScaleValue,fontScaleValue));

    Matrices.model *= (translateText * scaleText);
  	MVP = Matrices.projection * Matrices.view * Matrices.model;
  	// send font's MVP and font color to fond shaders
  	glUniformMatrix4fv(GL3Font.fontMatrixID, 1, GL_FALSE, &MVP[0][0]);
  	glUniform3fv(GL3Font.fontColorID, 1, &fontColor[0]);

  	char s[10];
  	strcpy(s,"Score: ");
  	char buffer[100];
  	sprintf(buffer, "%d", score);
  	strcat(s, buffer);
  	// Render font
  	GL3Font.font->Render(s);
  }

  void drawText(char *text, glm::vec3 fontColor, float fontScaleValue) {
    glm::mat4 MVP;
    Matrices.model = glm::mat4(1.0f);
  	glm::mat4 translateText = glm::translate(glm::vec3(-100,200,0));
  	glm::mat4 scaleText = glm::scale(glm::vec3(fontScaleValue,fontScaleValue,fontScaleValue));
  	Matrices.model *= (translateText * scaleText);
  	MVP = Matrices.projection * Matrices.view * Matrices.model;
  	// send font's MVP and font color to fond shaders
  	glUniformMatrix4fv(GL3Font.fontMatrixID, 1, GL_FALSE, &MVP[0][0]);
  	glUniform3fv(GL3Font.fontColorID, 1, &fontColor[0]);

  	GL3Font.font->Render(text);
  }
};
