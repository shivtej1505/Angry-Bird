// Class makes the rectangle
#include "custom_struct.h"
#include "create_draw.h"

class Rectangle {
  VAO *rectangle;
  float length, breath;
  public:
  void initialize(float l, float b, bool fill = true) {
    length = l;
    breath = b;
    createRectangle(length, breath, fill);
  }

  void makeRectangle(glm::mat4 VP, float x = 0.0f, float y = 0.0f, float angle = 0.0f, float rotX = 0, float rotY = 0) {
    glm::mat4 MVP;
    Matrices.model = glm::mat4(1.0f);
    float rotAngle = (float) angle * (M_PI/180.0f);

    glm::mat4 translateRectangle = glm::translate (glm::vec3(rotX, rotY, 0.0f)); // glTranslatef
    glm::mat4 rotateRectangle = glm::rotate(rotAngle, glm::vec3(0,0,1));  // rotate about vector (0,0,1) Rotating about z-axis
    glm::mat4 translateRectangle_1 = glm::translate (glm::vec3(-rotX, -rotY, 0.0f)); // glTranslatef
    glm::mat4 translateRectangle_2 = glm::translate (glm::vec3(x, y, 0.0f)); // glTranslatef

    Matrices.model *= (translateRectangle_2 * translateRectangle * rotateRectangle * translateRectangle_1   );// * scaleTriangle);
    MVP = VP * Matrices.model; // MVP = p * V * M
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    //draw3DObject(triangle1);
    draw3DObject(rectangle);
  }

  private:
  void createRectangle (float l, float b, bool fill) {
    const GLfloat vertex_buffer_data_1 [] = {
      0, 0, 0, // vertex 0
      l, 0, 0, // vertex 1
      l, b, 0, // vertex 2

      0, 0, 0, // vertex 0
      0, b, 0, // vertex 1
      l, b, 0, // vertex 2
    };

    const GLfloat color_buffer_data_1 [] = {
      1,1,1, // color 0
      1,1,1, // color 1
      1,1,1, // color 2

      1,1,1, // color 0
      1,1,1, // color 1
      1,1,1, // color 2
    };

    // create3DObject creates and returns a handle to a VAO that can be used later
    /*if (fill_upper){
      triangle1 = create3DObject(GL_TRIANGLES, 3, vertex_buffer_data_1, color_buffer_data_1, GL_FILL);
    } else {
      triangle1 = create3DObject(GL_TRIANGLES, 3, vertex_buffer_data_1, color_buffer_data_1, GL_LINE);
    }
    const GLfloat vertex_buffer_data_2 [] = {
      0, 0, 0, // vertex 0
      0, b, 0, // vertex 1
      l, b, 0, // vertex 2
    };

    const GLfloat color_buffer_data_2 [] = {
      1,1,1, // color 0
      1,1,1, // color 1
      1,1,1, // color 2
    };*/

    // create3DObject creates and returns a handle to a VAO that can be used later
    if (fill){
      rectangle = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data_1, color_buffer_data_1, GL_FILL);
    } else {
      rectangle = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data_1, color_buffer_data_1, GL_LINE);
    }
  }
};
