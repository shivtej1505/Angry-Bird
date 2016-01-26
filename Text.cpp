class Text {
  private:
  int scoreFontScale;
  float scoreFontScaleValue;
  glm::vec3 scoreFontColor;

  int speedFontScale;
  float speedFontScaleValue;
  glm::vec3 speedFontColor;

  int textFontScale;
  float textFontScaleValue;
  glm::vec3 textFontColor;

  private:
  glm::vec3 getRGBfromHue (int hue) {
  	float intp;
  	float fracp = modff(hue/60.0, &intp);
  	float x = 1.0 - abs((float)((int)intp%2)+fracp-1.0);

  	if (hue < 60) // Yellow
  		return glm::vec3(1,x,0);
  	else if (hue < 120) // Green
  		return glm::vec3(x,1,0);
  	else if (hue < 180)
  		return glm::vec3(0,1,x);
  	else if (hue < 240)
  		return glm::vec3(0,x,1);
  	else if (hue < 300) // Pink
  		return glm::vec3(x,0,1);
  	else
  		return glm::vec3(1,0,x);
  }

  public:
  void set_score_display(int fontScale, float fontScaleValue) {
    scoreFontScale = fontScale;
    scoreFontScaleValue = fontScaleValue;
    scoreFontColor = getRGBfromHue (fontScale);
  }

  void set_speed_display(int fontScale, float fontScaleValue) {
    speedFontScale = fontScale;
    speedFontScaleValue = fontScaleValue;
    speedFontColor = getRGBfromHue (fontScale);
  }

  void set_text_display(int fontScale, float fontScaleValue) {
    textFontScale = fontScale;
    textFontScaleValue = fontScaleValue;
    textFontColor = getRGBfromHue (fontScale);
  }

  void drawScore(int score) {
    glm::mat4 MVP;
    Matrices.model = glm::mat4(1.0f);
  	glm::mat4 translateText = glm::translate(glm::vec3(-490,275,0));
  	glm::mat4 scaleText = glm::scale(glm::vec3(scoreFontScaleValue,
      scoreFontScaleValue,scoreFontScaleValue));

    Matrices.model *= (translateText * scaleText);
  	MVP = Matrices.projection * Matrices.view * Matrices.model;
  	// send font's MVP and font color to fond shaders
  	glUniformMatrix4fv(GL3Font.fontMatrixID, 1, GL_FALSE, &MVP[0][0]);
  	glUniform3fv(GL3Font.fontColorID, 1, &scoreFontColor[0]);

  	char s[10];
  	strcpy(s,"Score: ");
  	char buffer[100];
  	sprintf(buffer, "%d", score);
  	strcat(s, buffer);
  	// Render font
  	GL3Font.font->Render(s);
  }

  void drawSpeed(float speed) {
    glm::mat4 MVP;
    Matrices.model = glm::mat4(1.0f);
  	glm::mat4 translateText = glm::translate(glm::vec3(-490,250,0));
  	glm::mat4 scaleText = glm::scale(glm::vec3(scoreFontScaleValue,
      scoreFontScaleValue,scoreFontScaleValue));

    Matrices.model *= (translateText * scaleText);
  	MVP = Matrices.projection * Matrices.view * Matrices.model;
  	// send font's MVP and font color to fond shaders
  	glUniformMatrix4fv(GL3Font.fontMatrixID, 1, GL_FALSE, &MVP[0][0]);
  	glUniform3fv(GL3Font.fontColorID, 1, &speedFontColor[0]);

  	char s[10];
  	strcpy(s,"Speed: ");
  	char buffer[100];
  	sprintf(buffer, "%0.2f", speed);
  	strcat(s, buffer);
  	// Render font
  	GL3Font.font->Render(s);
  }

  void drawText(char *text) {
    glm::mat4 MVP;
    Matrices.model = glm::mat4(1.0f);
  	glm::mat4 translateText = glm::translate(glm::vec3(-100,200,0));
  	glm::mat4 scaleText = glm::scale(glm::vec3(textFontScaleValue,textFontScaleValue,textFontScaleValue));
  	Matrices.model *= (translateText * scaleText);
  	MVP = Matrices.projection * Matrices.view * Matrices.model;
  	// send font's MVP and font color to fond shaders
  	glUniformMatrix4fv(GL3Font.fontMatrixID, 1, GL_FALSE, &MVP[0][0]);
  	glUniform3fv(GL3Font.fontColorID, 1, &textFontColor[0]);

  	GL3Font.font->Render(text);
  }
};
