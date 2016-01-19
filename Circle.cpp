// Class makes the circle
struct VAO {
    GLuint VertexArrayID;
    GLuint VertexBuffer;
    GLuint ColorBuffer;

    GLenum PrimitiveMode;
    GLenum FillMode;
    int NumVertices;
};
struct GLMatrices {
	glm::mat4 projection;
	glm::mat4 model;
	glm::mat4 view;
	GLuint MatrixID;
} Matrices;

typedef struct VAO VAO;
VAO *triangle;

class Circle {

  int center_x, center_y;
  public:
  void initialize(int x, int y, float radius) {
    center_x = x;
    center_y = y;
    createTriangle(radius);
  }

  private:
  void createTriangle (float radius = 1.0f) {
    printf("Called\n");
    /* ONLY vertices between the bounds specified in glm::ortho will be visible on screen */

    /* Define vertex array as used in glBegin (GL_TRIANGLES) */
    static const GLfloat vertex_buffer_data [] = {
      0, 0,0, // vertex 0
      radius,0,0, // vertex 1
      0,radius,0, // vertex 2
    };

    static const GLfloat color_buffer_data [] = {
      1,1,1, // color 0
      1,1,1, // color 1
      1,1,1, // color 2
    };

    // create3DObject creates and returns a handle to a VAO that can be used later
    triangle = create3DObject(GL_TRIANGLES, 3, vertex_buffer_data, color_buffer_data, GL_FILL);
  }
  struct VAO* create3DObject (GLenum primitive_mode, int numVertices,
    const GLfloat* vertex_buffer_data, const GLfloat* color_buffer_data, GLenum fill_mode=GL_FILL) {
      struct VAO* vao = new struct VAO;
      vao->PrimitiveMode = primitive_mode;
      vao->NumVertices = numVertices;
      vao->FillMode = fill_mode;

      // Create Vertex Array Object
      // Should be done after CreateWindow and before any other GL calls
      glGenVertexArrays(1, &(vao->VertexArrayID)); // VAO
      glGenBuffers (1, &(vao->VertexBuffer)); // VBO - vertices
      glGenBuffers (1, &(vao->ColorBuffer));  // VBO - colors

      glBindVertexArray (vao->VertexArrayID); // Bind the VAO
      glBindBuffer (GL_ARRAY_BUFFER, vao->VertexBuffer); // Bind the VBO vertices
      glBufferData (GL_ARRAY_BUFFER, 3*numVertices*sizeof(GLfloat), vertex_buffer_data, GL_STATIC_DRAW); // Copy the vertices into VBO
      glVertexAttribPointer(
                            0,                  // attribute 0. Vertices
                            3,                  // size (x,y,z)
                            GL_FLOAT,           // type
                            GL_FALSE,           // normalized?
                            0,                  // stride
                            (void*)0            // array buffer offset
                            );

      glBindBuffer (GL_ARRAY_BUFFER, vao->ColorBuffer); // Bind the VBO colors
      glBufferData (GL_ARRAY_BUFFER, 3*numVertices*sizeof(GLfloat), color_buffer_data, GL_STATIC_DRAW);  // Copy the vertex colors
      glVertexAttribPointer(
                            1,                  // attribute 1. Color
                            3,                  // size (r,g,b)
                            GL_FLOAT,           // type
                            GL_FALSE,           // normalized?
                            0,                  // stride
                            (void*)0            // array buffer offset
                            );

      return vao;
  }
  /* Generate VAO, VBOs and return VAO handle - Common Color for all vertices */
  struct VAO* create3DObject (GLenum primitive_mode, int numVertices,
    const GLfloat* vertex_buffer_data, const GLfloat red, const GLfloat green, const GLfloat blue, GLenum fill_mode=GL_FILL) {
      GLfloat* color_buffer_data = new GLfloat [3*numVertices];
      for (int i=0; i<numVertices; i++) {
          color_buffer_data [3*i] = red;
          color_buffer_data [3*i + 1] = green;
          color_buffer_data [3*i + 2] = blue;
      }

      return create3DObject(primitive_mode, numVertices, vertex_buffer_data, color_buffer_data, fill_mode);
  }

  public:
  void makeCircle(glm::mat4 VP, float x = -3.5f, float y = -3.5f) {
    glm::mat4 MVP;
    for(int i=0; i<720; i++) {
      Matrices.model = glm::mat4(1.0f);
      glm::mat4 scaleTriangle = glm::mat4(1.0f);
      float rotAngle = (float)(i * M_PI/360);
      glm::mat4 translateTriangle = glm::translate (glm::vec3(x, y, 0.0f)); // glTranslatef
      glm::mat4 rotateTriangle = glm::rotate(rotAngle, glm::vec3(0,0,1));  // rotate about vector (0,0,1) Rotating about z-axis
      Matrices.model *= (translateTriangle * rotateTriangle);// * scaleTriangle);
      MVP = VP * Matrices.model; // MVP = p * V * M
      glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
      draw3DObject(triangle);
    }
  }

  void draw3DObject (struct VAO* vao) {
      // Change the Fill Mode for this object
      glPolygonMode (GL_FRONT_AND_BACK, vao->FillMode);

      // Bind the VAO to use
      glBindVertexArray (vao->VertexArrayID);

      // Enable Vertex Attribute 0 - 3d Vertices
      glEnableVertexAttribArray(0);
      // Bind the VBO to use
      glBindBuffer(GL_ARRAY_BUFFER, vao->VertexBuffer);

      // Enable Vertex Attribute 1 - Color
      glEnableVertexAttribArray(1);
      // Bind the VBO to use
      glBindBuffer(GL_ARRAY_BUFFER, vao->ColorBuffer);

      // Draw the geometry !
      glDrawArrays(vao->PrimitiveMode, 0, vao->NumVertices); // Starting from vertex 0; 3 vertices total -> 1 triangle
  }
};
