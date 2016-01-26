#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glad/glad.h>
#include <FTGL/ftgl.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

#include "game_config.h"
#include "Bird.cpp"
#include "Cannon.cpp"
#include "Ground.cpp"
#include "Pig.cpp"
#include "Obstacle.cpp"
#include "Text.cpp"

using namespace std;

bool is_rules_seen = false;

vector <Bird> birds;
int bird_number = 0;
float zoom = 1.0f;

int power_one = false;
int power_two = false;
int power_three = false;

int score = 0;
float velocity = 15.0f;
string info;

float target_x = 0.0f;
float target_y = 0.0f;

GLuint programID, fontProgramID, textureProgramID;;
Cannon cannon;
Ground ground;
Text text;

vector<Pig> pigs;
vector<Obstacle> obstacles;
/* Function to load Shaders - Use it as it is */
GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open())
	{
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	cout << "Compiling shader : " <<  vertex_file_path << endl;
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> VertexShaderErrorMessage( max(InfoLogLength, int(1)) );
	glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
	cout << VertexShaderErrorMessage.data() << endl;

	// Compile Fragment Shader
	cout << "Compiling shader : " << fragment_file_path << endl;
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> FragmentShaderErrorMessage( max(InfoLogLength, int(1)) );
	glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
	cout << FragmentShaderErrorMessage.data() << endl;

	// Link the program
	cout << "Linking program" << endl;
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> ProgramErrorMessage( max(InfoLogLength, int(1)) );
	glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
	cout << ProgramErrorMessage.data() << endl;

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

static void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

Bird add_new_bird(int center_x, int center_y, bool is_on_cannon = false) {
	Bird bird(center_x, center_y);
	if (is_on_cannon) {
		bird.set_bird_on_cannon(true);
	}
	printf("%d\n",bird.get_bird_on_cannon() );
	birds.push_back(bird);
	return bird;
}

void add_new_pig(int center_x, int center_y, bool fill = true) {
	Pig pig(center_x, center_y, 50,50, fill);
	pigs.push_back(pig);
}

void add_new_obstacle(int center_x, int center_y, bool fill = true) {
	Obstacle obstacle(center_x, center_y, 5, 100, fill);
	obstacles.push_back(obstacle);
}

void activate_power_one() {
 	if(!power_one) {
		float cen_x = birds.at(bird_number-1).get_center_x();
		float cen_y = birds.at(bird_number-1).get_center_y();
		power_one = true;
		printf("Power one activated\n" );
	} else {
		printf("Power one used\n" );
	}
}

void activate_power_two() {
 	if(!power_two) {
		birds.at(bird_number-1).power_two();
		power_two = true;
		printf("Power two activated\n" );
	} else {
		printf("Power two used\n" );
	}
}

void activate_power_three() {
 	if(!power_three) {
		power_three = true;
		printf("Power three activated\n" );
	} else {
		printf("Power three used\n" );
	}
}

void next_bird(float velocity, float angle) {
	if (!birds.at(bird_number).get_bird_on_cannon()) {
		printf("next bird now!\n" );
		bird_number++;
		if (bird_number == birds.size()) {
			printf("No bird left\n" );
			return;
		}
		birds.at(bird_number).put_on_cannon();
		birds.at(bird_number).set_bird_on_cannon(true);
		birds.at(bird_number).set_velocity(velocity);
		birds.at(bird_number).set_projection_angle(angle);
		printf("Bird number%d\n", bird_number);
		for(int bird_num = bird_number+1; bird_num < birds.size(); bird_num++) {
			(birds.at(bird_num)).move_forward();// Move forward in queue
			//printf("%d %d\n",bird_num, (birds.at(bird_num)).get_bird_on_cannon() );
		}
		//reset();
	}
}

// reset cannon and bird velocity and angle
void reset() {
	//birds.at(bird_number).print(bird_number);
	//cannon.reset();
	if (bird_number < birds.size()) {
		float velocity = birds.at(bird_number).get_velocity();
		float angle = birds.at(bird_number).get_projection_angle();
		//printf("Bird number%d\n", bird_number);
		printf("Velocity %f Angle %f\n",velocity, angle );
		next_bird(velocity, angle);
	} else
		info = "All bird used";
		printf("All bird used\n" );
}

void reset_obstacles() {
	vector<Obstacle>::iterator obstacle;
	for(obstacle = obstacles.begin(); obstacle != obstacles.end(); obstacle++) {
		(*obstacle).resetObstacle();
	}
}

void initialize_elements(int number_bird = 12) {
	add_new_bird(-360, -260, true);
	//int x_increment =
	for (int i=0; i<number_bird; i++) {
		add_new_bird(-408 - (i*23), -280);
	}
	cannon.initialize();
	ground.initialize();
	for (int i=0; i<6; i++) {
		if (i<3)
			add_new_pig(300 + i * 50 , -300);
		else if (i<5)
			add_new_pig(325 + (i-3) * 50, -250);
		else
			add_new_pig(350 + (i-5) * 50, -200);
	}
	for (int i=0; i<3; i++) {
		add_new_obstacle(rand()%700 - 200, rand()%1000 + 100);
	}
	//static int fontScale = 0;
	//float fontScaleValue = 40;
	//glm::vec3 fontColor = getRGBfromHue (fontScale);

	text.set_score_display(220, 40);
	text.set_speed_display(50, 40);

	info = "";
}

void quit(GLFWwindow *window) {
		glfwSetCursor(window, NULL);
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void create_pigs(glm::mat4 VP) {
	vector<Pig>::iterator pig;
	for(pig = pigs.begin(); pig != pigs.end(); pig++) {
		(*pig).createPig(VP);
	}
}

void create_obstacles(glm::mat4 VP) {
	vector<Obstacle>::iterator obstacle;
	for(obstacle = obstacles.begin(); obstacle != obstacles.end(); obstacle++) {
		(*obstacle).createObstacle(VP);
	}
}

void set_fly_status_birds(bool value) {
	if (bird_number >= birds.size()) {
		printf("No bird left\n" );
		return;
	}
	birds.at(bird_number).set_fly_status(true);
	birds.at(bird_number).set_bird_on_cannon(false);
	reset();
}

void create_birds(glm::mat4 VP) {
	vector<Bird>::iterator bird;
	for(bird = birds.begin(); bird != birds.end(); bird++) {
		(*bird).createBird(VP);
	}
}

void decrease_velocity_birds() {
	if (bird_number >= birds.size()) {
		return;
	}
	if (velocity > 2.0f) {
		birds.at(bird_number).decrease_velocity();
		velocity -= 2.0f;
	}
}

void increase_velocity_birds() {
	if (bird_number >= birds.size()) {
		return;
	}
	if (velocity < 60.0f) {
		birds.at(bird_number).increase_velocity();
		velocity += 2.0f;
	}
}

void decrease_angle_birds() {
	if (bird_number >= birds.size()) {
		return;
	}
	birds.at(bird_number).decrease_angle();
}

void increase_angle_birds() {
	if (bird_number >= birds.size()) {
		return;
	}
	birds.at(bird_number).increase_angle();
}

void fly_birds() {
	for (int bird_num = 0 ; bird_num < birds.size(); bird_num ++) {
		birds.at(bird_num).flyBird();
	}
}

void drop_obstacles() {
	vector<Obstacle>::iterator obstacle;
	for(obstacle = obstacles.begin(); obstacle != obstacles.end(); obstacle++) {
		(*obstacle).dropObstacle();
	}
}

void check_collision() {
	// bird and ground
	//Bird bird = birds.at(bird_number);
	for (int bird_num = 0 ; bird_num < birds.size(); bird_num ++) {
		float bird_center_x = birds.at(bird_num).get_center_x();
		float bird_center_y = birds.at(bird_num).get_center_y();
		if (-295.0 <= bird_center_y && bird_center_y <= -285.0 &&
				!birds.at(bird_num).colliding_with_ground) {
			printf("Collison with ground\n" );
			birds.at(bird_num).collision(90.0f, 0.8f);
			birds.at(bird_num).colliding_with_ground = true;
		} else {
			birds.at(bird_num).colliding_with_ground = false;
		}
		vector<Pig>::iterator pig;
		for(pig = pigs.begin(); pig != pigs.end(); pig++) {
			bool collidingWithAorC = (*pig).isCollidingFromSideA(bird_center_x,
				bird_center_y) || (*pig).isCollidingFromSideC(bird_center_x, bird_center_y);
			bool collidingWithBorD = (*pig).isCollidingFromSideB(bird_center_x,
				bird_center_y) || (*pig).isCollidingFromSideD(bird_center_x, bird_center_y);
			//printf("Dis %f\n", dis );
			if (!(*pig).isCollide ) {
				if (collidingWithAorC) {
					birds.at(bird_num).collision(0.0f, 0.75f);
					(*pig).isCollide = true;
					(*pig).reduceSize();
					score += 10;
				} else if (collidingWithBorD) {
					birds.at(bird_num).collision(90.0f, 0.75f);
					(*pig).isCollide = true;
					(*pig).reduceSize();
					score += 10;
				}

			} else {
				(*pig).isCollide = false;
			}
		}

		vector<Obstacle>::iterator obstacle;
		for(obstacle = obstacles.begin(); obstacle != obstacles.end(); obstacle++) {
			bool collidingWithAorC = (*obstacle).isCollidingFromSideA(bird_center_x,
				bird_center_y) || (*obstacle).isCollidingFromSideC(bird_center_x, bird_center_y);
			bool collidingWithBorD = (*obstacle).isCollidingFromSideB(bird_center_x,
				bird_center_y) || (*obstacle).isCollidingFromSideD(bird_center_x, bird_center_y);
			//printf("Dis %f\n", dis );
			if (!(*obstacle).isCollide ) {
				if (collidingWithAorC) {
					birds.at(bird_num).collision(0.0f, 0.8f);
					(*obstacle).isCollide = true;
				} else if (collidingWithBorD) {
					birds.at(bird_num).collision(90.0f, 0.8f);
					(*obstacle).isCollide = true;
				}
			} else {
				(*obstacle).isCollide = false;
			}
		}
	}
}

float camera_rotation_angle = 90;

/* Executed when a regular key is pressed/released/held-down */
/* Prefered for Keyboard events */
// mods --> which key has pressed, can be used to differentiate b/w Shift + c and c
void keyboard (GLFWwindow* window, int key, int scancode, int action, int mods) {
	 // Function is called first on GLFW_PRESS.
	if (action == GLFW_RELEASE || action == GLFW_REPEAT) {
	    switch (key) {
	        case GLFW_KEY_F:
							if (is_rules_seen)
	            	increase_velocity_birds();
	            break;
	        case GLFW_KEY_S:
							if (is_rules_seen)
	            	decrease_velocity_birds();
	            break;
					case GLFW_KEY_A:
							if (is_rules_seen) {
								increase_angle_birds();
								cannon.increaseAngle();
							}
							break;
					case GLFW_KEY_B:
							if (is_rules_seen) {
								decrease_angle_birds();
								cannon.decreaseAngle();
	            }
							break;
					case GLFW_KEY_R:
							if (is_rules_seen)
								set_fly_status_birds(true);
	            break;
					case GLFW_KEY_SPACE:
							if (!is_rules_seen) {
								is_rules_seen = true;
								break;
							}
							// Pause the game
	            set_fly_status_birds(false);
	            break;
					case GLFW_KEY_1:
							if (is_rules_seen)
								activate_power_one();
							break;
					case GLFW_KEY_2:
							if (is_rules_seen)
								activate_power_two();
							break;
					case GLFW_KEY_3:
							if (is_rules_seen)
								activate_power_three();
							break;
					case GLFW_KEY_KP_ADD:
							if (zoom > 0.5f && is_rules_seen)
								zoom = (zoom / 2.0f);
							break;
					case GLFW_KEY_KP_SUBTRACT:
							if (zoom < 2 && is_rules_seen)
								zoom = (zoom * 2.0f);
							break;
	        default:
	            break;
	    }
	}
	else if (action == GLFW_PRESS) {
	    switch (key) {
	        case GLFW_KEY_ESCAPE:
	            quit(window);
	            break;
	        default:
	            break;
	    }
	}
}

/* Executed for character input (like in text boxes) */
void keyboardChar (GLFWwindow* window, unsigned int key) {
	switch (key) {
		case 'Q':
		case 'q':
            quit(window);
            break;
		default:
			break;
	}
}

/* Executed when a mouse button is pressed/released */
void mouseButton (GLFWwindow* window, int button, int action, int mods) {
    switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            if (action == GLFW_RELEASE)
                set_fly_status_birds(true);
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            if (action == GLFW_RELEASE) {
                //rectangle_rot_dir *= 50; // also change the rotation speed
            }
            break;
        default:
            break;
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	float x = xpos - 140;
	float y = 560 - ypos;
	float angle = atan(y/x) * (180.0f/M_PI);
	cannon.set_cannon_angle(angle);
	if (bird_number < birds.size())
		birds.at(bird_number).set_projection_angle(angle);
	//printf("%f\n",angle );
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	if (yoffset > 0)
		increase_velocity_birds();
	else if (yoffset < 0)
		decrease_velocity_birds();
}

/* Executed when window is resized to 'width' and 'height' */
/* Modify the bounds of the screen here in glm::ortho or Field of View in glm::Perspective */
void reshapeWindow (GLFWwindow* window, int width, int height) {
    int fbwidth=width, fbheight=height;
    /* With Retina display on Mac OS X, GLFW's FramebufferSize
     is different from WindowSize */
    glfwGetFramebufferSize(window, &fbwidth, &fbheight);

	GLfloat fov = 90.0f;

	// sets the viewport of openGL renderer
	glViewport (0, 0, (GLsizei) fbwidth, (GLsizei) fbheight);

	// set the projection matrix as perspective
	/* glMatrixMode (GL_PROJECTION);
	   glLoadIdentity ();
	   gluPerspective (fov, (GLfloat) fbwidth / (GLfloat) fbheight, 0.1, 500.0); */
	// Store the projection matrix in a variable for future use
    // Perspective projection for 3D views
    // Matrices.projection = glm::perspective (fov, (GLfloat) fbwidth / (GLfloat) fbheight, 0.1f, 500.0f);

    // Ortho projection for 2D views
    // first and second parameter : bound for x-axis
    // third and fourth parameter : bound for y-axis
    // fifth parameter : how much near you can see
    // sixth parameter : how much futher you can see, objects after them won't be visible
    Matrices.projection = glm::ortho(-500.0f * zoom , 500.0f * zoom , -300.0f * zoom , 300.0f * zoom , 0.1f, 500.0f);
}

void showText(string msg, float xpos = -100, float ypos = 100,
		float scaleFontValue = 40, float fontScale = 0) {
	text.drawText(strdup(msg.c_str()), xpos, ypos, scaleFontValue, fontScale);
}
/* Edit this function according to your assignment */
void draw () {
  // clear the color and depth in the frame buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // use the loaded shader program
  // Don't change unless you know what you are doing
  glUseProgram (programID);

  // Eye - Location of camera. Don't change unless you are sure!!
  glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
  // Target - Where is the camera looking at.  Don't change unless you are sure!!
  glm::vec3 target (target_x, 0, 0);
  // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
  glm::vec3 up (0, 1, 0);

	static float c = 0;
	c++;
	//Matrices.view = glm::lookAt(glm::vec3(0,0,3), glm::vec3(0,0,0), glm::vec3(sinf(c*M_PI/180.0),3*cosf(c*M_PI/180.0),0)); // Fixed camera for 2D (ortho) in XY plane
	Matrices.view = glm::lookAt(glm::vec3(0,0,3), glm::vec3(0,0,0), glm::vec3(0,1,0)); // Fixed camera for 2D (ortho) in XY plane
	// Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
	//  Don't change unless you are sure!!

  glm::mat4 VP = Matrices.projection * Matrices.view;

	target_x += 10.0f;

	if (is_rules_seen) {
		create_obstacles(VP);
		create_pigs(VP);
		create_birds(VP);
		cannon.createCannon(VP);
		ground.createGround(VP);

		// Use font Shaders for next part of code
		glUseProgram(fontProgramID);
		//Matrices.view = glm::lookAt(glm::vec3(0,0,3), glm::vec3(0,0,0), glm::vec3(0,1,0)); // Fixed camera for 2D (ortho) in XY plane

		//fontScale = (fontScale + 1) % 360;
		text.drawScore(score);
		text.drawSpeed(velocity);
		if (score >= 140) {
			info = "You won! :)";
			showText(info, -150, 200, 100, 300);
		}
	} else {
		glUseProgram(fontProgramID);
		string title = "ANGRY BIRD";
		showText(title, -220, 180, 150, 350);
		string developer = "By :- Shivang Nagaria";
		showText(developer, 150, 130, 50, 150);
		// GAME CONTROLS
		string control_string = "GAME CONTROLS";
		showText(control_string, -350, 70, 70, 100);
		// CONTROL ONE
		string control_line_one = "Press Cursor or Keys A, B to change angle";
		showText(control_line_one, -350, 30, 50, 240);
		// CONTROL TWO
		string control_line_two = "Press keys F, S to change speed of bird";
		showText(control_line_two, -350, 0, 50, 240);
		// CONTROL THREE
		string control_line_three = "Press key R to release the bird";
		showText(control_line_three, -350, -30, 50, 240);
		// CONTROL FOUR
		string control_line_four = "Press key 1,2,3 to use powers";
		showText(control_line_four, -350, -60, 50, 240);
		// CONTROL FIVE
		string control_line_five = "Press key + to zoom in";
		showText(control_line_five, -350, -90, 50, 240);
		// CONTROL SIX
		string control_line_six = "Press key - to zoom out";
		showText(control_line_six, -350, -120, 50, 240);
		// CONTROL SEVEN
		string control_line_seven = "Press key Q to exit";
		showText(control_line_seven, -350, -150, 50, 240);
		// CONTROL EIGHT
		string control_line_eight = "Press spacebar to continue";
		showText(control_line_eight, -350, -260, 50, 240);
	}
}

/* Initialise glfw window, I/O callbacks and the renderer to use */
/* Nothing to Edit here */
GLFWwindow* initGLFW (int width, int height) {
	GLFWwindow* window; // window desciptor/handle

	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, "Angry Bird", NULL, NULL);

	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
	glfwSwapInterval( 1 );

	/* --- register callbacks with GLFW --- */

	/* Register function to handle window resizes */
	/* With Retina display on Mac OS X GLFW's FramebufferSize
	 is different from WindowSize */
	glfwSetFramebufferSizeCallback(window, reshapeWindow);
	glfwSetWindowSizeCallback(window, reshapeWindow);

	/* Register function to handle window close */
	glfwSetWindowCloseCallback(window, quit);

	/* Register function to handle keyboard input */
	glfwSetKeyCallback(window, keyboard);      // general keyboard input
	glfwSetCharCallback(window, keyboardChar);  // simpler specific character handling

	/* Register function to handle mouse click */
	glfwSetMouseButtonCallback(window, mouseButton);  // mouse button clicks

	GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);

	glfwSetCursor(window, cursor);
	// Set cursor callback function
	glfwSetCursorPosCallback(window, cursor_position_callback);
	// Set scroll callback funcion
	glfwSetScrollCallback(window, scroll_callback);

	return window;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL (GLFWwindow* window, int width, int height) {
	// Load Textures
	// Enable Texture0 as current texture memory
	glActiveTexture(GL_TEXTURE0);
	// load an image file directly as a new OpenGL texture
	// GLuint texID = SOIL_load_OGL_texture ("beach.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_TEXTURE_REPEATS); // Buggy for OpenGL3
	//GLuint textureID = createTexture("beach2.png");
	// check for an error during the load process
	//if(textureID == 0 )
	//	cout << "SOIL loading error: '" << SOIL_last_result() << "'" << endl;

	// Create and compile our GLSL program from the texture shaders
	textureProgramID = LoadShaders( "TextureRender.vert", "TextureRender.frag" );
	// Get a handle for our "MVP" uniform
	Matrices.TexMatrixID = glGetUniformLocation(textureProgramID, "MVP");


	/* Objects should be created before any other gl function and shaders */
	// Create the models
	//createTriangle (); // Generate the VAO, VBOs, vertices data & copy into the array buffer
	//createRectangle (textureID);


	// Create and compile our GLSL program from the shaders
	programID = LoadShaders( "Sample_GL.vert", "Sample_GL.frag" );
	// Get a handle for our "MVP" uniform
	Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


	reshapeWindow (window, width, height);

	// Background color of the scene
	glClearColor (0.3f, 0.3f, 0.3f, 0.0f); // R, G, B, A
	glClearDepth (1.0f);

	//glEnable (GL_DEPTH_TEST);
	//glDepthFunc (GL_LEQUAL);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Initialise FTGL stuff
	const char* fontfile = "arial.ttf";
	GL3Font.font = new FTExtrudeFont(fontfile); // 3D extrude style rendering

	if(GL3Font.font->Error())
	{
		cout << "Error: Could not load font `" << fontfile << "'" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Create and compile our GLSL program from the font shaders
	fontProgramID = LoadShaders( "fontrender.vert", "fontrender.frag" );
	GLint fontVertexCoordAttrib, fontVertexNormalAttrib, fontVertexOffsetUniform;
	fontVertexCoordAttrib = glGetAttribLocation(fontProgramID, "vertexPosition");
	fontVertexNormalAttrib = glGetAttribLocation(fontProgramID, "vertexNormal");
	fontVertexOffsetUniform = glGetUniformLocation(fontProgramID, "pen");
	GL3Font.fontMatrixID = glGetUniformLocation(fontProgramID, "MVP");
	GL3Font.fontColorID = glGetUniformLocation(fontProgramID, "fontColor");

	GL3Font.font->ShaderLocations(fontVertexCoordAttrib, fontVertexNormalAttrib, fontVertexOffsetUniform);
	GL3Font.font->FaceSize(1);
	GL3Font.font->Depth(0);
	GL3Font.font->Outset(0, 0);
	GL3Font.font->CharMap(ft_encoding_unicode);

	cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
	cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
	cout << "VERSION: " << glGetString(GL_VERSION) << endl;
	cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

}

int main (int argc, char** argv) {
    GLFWwindow* window = initGLFW(WIDTH, HEIGHT);
	  initGL (window, WIDTH, HEIGHT);

		initialize_elements();
    double last_update_time = glfwGetTime(), current_time;
		double last_update_time_1 = glfwGetTime(), current_time_1;
    double total_time_elapsed = 0;
    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // OpenGL Draw commands
				reshapeWindow(window, WIDTH, HEIGHT);
        draw();
        // Swap Frame Buffer in double buffering
        glfwSwapBuffers(window);
        // Poll for Keyboard and mouse events
        glfwPollEvents();

        // Control based on time (Time based transformation like 5 degrees rotation every 0.5s)
        current_time = glfwGetTime(); // Time in seconds
        if ((current_time - last_update_time) >= 0.01 && is_rules_seen) { // atleast 0.5s elapsed since last frame
            // do something every 0.02 seconds ..
            last_update_time = current_time;
            total_time_elapsed += 0.02;
						check_collision();
						fly_birds();
						drop_obstacles();
						//next_bird();
        }
				current_time_1 = glfwGetTime();
				if ((current_time_1 - last_update_time_1) >= 5 && is_rules_seen) {
					// after each 5 sec add 5 new obstacles
					last_update_time_1 = current_time_1;
					reset_obstacles();
					printf("Added new obstacles\n" );
				}
    }

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
