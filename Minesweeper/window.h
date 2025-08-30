#pragma once



#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "board.h"
#include "shader.h"
#include "config.h"
#include "vertex.h"


class Window
{

	private:

		// ~~~~~~ Window-Based Members ~~~~~~
		GLFWwindow* window;
		unsigned int win_width, win_height;
		const char* win_title;
		bool isInit = false;


		// ~~~~~ Shader-Based Members ~~~~~~
		Shader shader;
		GLuint VAO, VBO, EBO;


		// ~~~~~ Game Logic ~~~~~~
		Board gameBoard;


		// ~~~~~ Vertex Data ~~~~~~
		Vertex vertices[4] = {
			{ {  1.0f,  1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }}, // Add UVs
			{ {  1.0f, -1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f }}, // Add UVs
			{ { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f }}, // Add UVs
			{ { -1.0f,  1.0f, 0.0f }, { 1.0f, 1.0f, 0.0f }, { 0.0f, 0.0f }}  // Add UVs
		};

		unsigned int indices[6] = {
				0, 1, 3,   // First Triangle
				1, 2, 3    // Second Triangle
		};


		/*
	
			Desc: Initializes The Window System Including GLFW, OpenGL Context,
			And All Required Resources For Rendering The Minesweeper Game.

			Preconditions:
				1.) Window Must Not Already Be Initialized
				2.) GLFW And OpenGL Libraries Must Be Available

			Postconditions:
				1.) GLFW Will Be Initialized And Window Created
				2.) OpenGL Context Will Be Established With GLAD
				3.) Shader Program Will Be Loaded And Compiled
				4.) Vertex Data Will Be Initialized For Rendering
				5.) Game Board Will Be Ready For Use
				6.) isInit Flag Will Be Set To true On Success

		*/
		bool initialize()
		{

			if (this->isInit)
			{
				std::cerr << "Window Has Already Been Initialized...\n";
				return false;
			}

			if (!glfwInit())
			{
				std::cerr << "GLFW Initialization Has Failed...\n";
				return false;
			}

			// Seed Randomizer
			srand(static_cast<unsigned int>(time(0)));

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			this->window = glfwCreateWindow(this->win_width, this->win_height, this->win_title, NULL, NULL);

			if (this->window == NULL)
			{
				std::cerr << "GLFW Window Creation Has Failed...\n";
				glfwTerminate();
				return false;
			}

			glfwMakeContextCurrent(this->window);

			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			{
				std::cerr << "GLAD Initialization Has Failed...\n";
				return false;
			}

			glViewport(0, 0, this->win_width, this->win_height);

			// Set Callbacks
			glfwSetWindowSizeCallback(this->window, this->frameBufferSizeCallback);
			glfwSetMouseButtonCallback(this->window, this->mouseButtonCallback);
			glfwSetWindowUserPointer(this->window, this);

			// Load Default Shader
			if (!this->shader.loadShader(CONFIG::SHADER::VERTEX_PATH, CONFIG::SHADER::FRAGMENT_PATH))
			{
				std::cerr << "Default Shader Loading Has Failed...\n";
				return false;
			}

			this->initializeVertexData();

			this->isInit = true;

			return true;

		}


		/*
	
			Desc: Sets Up Vertex Array Objects And Buffers For OpenGL Rendering
			Including Game Board Initialization And Vertex Attribute Configuration.

			Preconditions:
				1.) Window Must Be In Process Of Initialization
				2.) OpenGL Context Must Be Active
				3.) Shader Program Must Be Successfully Loaded

			Postconditions:
				1.) Vertex Array Object (VAO) Will Be Created And Bound
				2.) Vertex And Element Buffer Objects Will Be Generated
				3.) Game Board Will Be Initialized With All Resources
				4.) Vertex Attributes Will Be Configured For Position And Color
				5.) All Vertex Attribute Arrays Will Be Enabled

		*/
		void initializeVertexData()
		{

			if (this->isInit)
			{
				std::cerr << "Window Vertex Data Was Already Initialized...\n";
				return;
			}

			// Build Our Shader Buffers
			glGenVertexArrays(1, &this->VAO);
			glGenBuffers(1, &this->VBO);
			glGenBuffers(1, &this->EBO);

			// Bind Our Buffers
			glBindVertexArray(this->VAO);

			// Initialize Game Board
			this->gameBoard.initialize();

			glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);

			// Load Data Into Buffers
			glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices), this->vertices, GL_STATIC_DRAW);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->indices), this->indices, GL_STATIC_DRAW);

			// Set Vertex Attribute Pointers
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

			// Enable Vertex Attributes
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);

		}


		/*
	
			Desc: Static Callback Function For Handling Mouse Button Events
			Distinguishing Between Left And Right Clicks For Game Interaction.

			Preconditions:
				1.) Window Must Have Valid User Pointer Set
				2.) GLFW Must Be Properly Initialized
				3.) Mouse Button Event Must Have Occurred

			Postconditions:
				1.) Left Mouse Button Press Will Trigger Cell Revelation
				2.) Right Mouse Button Press Will Trigger Flag Toggle
				3.) Appropriate handleMouseClick Will Be Called With Click Type

		*/
		static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
		{

			if (action == GLFW_PRESS)
			{
				Window* windowInstance = static_cast<Window*>(glfwGetWindowUserPointer(window));
				if (windowInstance)
				{
					if (button == GLFW_MOUSE_BUTTON_LEFT)
					{
						windowInstance->handleMouseClick(false); // Left Click = Reveal
					}
					else if (button == GLFW_MOUSE_BUTTON_RIGHT)
					{
						windowInstance->handleMouseClick(true); // Right Click = Flag
					}
				}
			}

		}


		/*
	
			Desc: Processes Mouse Click Events By Converting Screen Coordinates
			To Normalized Device Coordinates And Forwarding To Game Board.

			Preconditions:
				1.) Window Must Be Properly Initialized
				2.) Game Board Must Be Active And Ready
				3.) Mouse Cursor Must Be Within Window Bounds

			Postconditions:
				1.) Mouse Position Will Be Retrieved From GLFW
				2.) Screen Coordinates Will Be Converted To Normalized Coordinates
				3.) Game Board handleClick Will Be Called With Converted Coordinates

		*/
		void handleMouseClick(bool isRightClick = false)
		{

			double xpos, ypos;
			glfwGetCursorPos(this->window, &xpos, &ypos);

			// Convert Screen Coordinates To Normalized Device Coordinates
			float normalizedX = (2.0f * xpos) / this->win_width - 1.0f;
			float normalizedY = 1.0f - (2.0f * ypos) / this->win_height;

			// Convert To Board Coordinates And Update Cell
			this->gameBoard.handleClick(normalizedX, normalizedY, isRightClick);

		}


		/*
	
			Desc: Clears The Screen Buffer With The Configured Background Color
			Preparing For The Next Frame Rendering.

			Preconditions:
				1.) OpenGL Context Must Be Active
				2.) CONFIG Color Values Must Be Available

			Postconditions:
				1.) Color Buffer Will Be Cleared With Background Color
				2.) Screen Will Be Ready For New Frame Rendering

		*/
		void clear()
		{

			glClearColor(CONFIG::SHADER::COLORS::CLEAR_COLOR_R,
				CONFIG::SHADER::COLORS::CLEAR_COLOR_G,
				CONFIG::SHADER::COLORS::CLEAR_COLOR_B, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

		}


		/*
	
			Desc: Renders The Current Frame Including Background And Game Board
			With Proper Shader Uniforms And Texture Binding.

			Preconditions:
				1.) OpenGL Context Must Be Active
				2.) Shader Program Must Be Bound And Ready
				3.) All Vertex Data Must Be Current

			Postconditions:
				1.) Background Quad Will Be Rendered
				2.) Shader Uniforms Will Be Set For Board Rendering
				3.) Game Board Will Be Rendered With Textures

		*/
		void render()
		{

			// Bind Our Buffers
			glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 3));

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			// Set Shader Uniforms For Board Rendering
			glUniform1i(glGetUniformLocation(this->shader.getProgID(), "numberAtlas"), 0);
			glUniform1i(glGetUniformLocation(this->shader.getProgID(), "useTexture"), 1);

			this->gameBoard.render();

		}


		/*
	
			Desc: Cleanup Routine For Window Resources Including OpenGL Objects
			And GLFW Window Termination.

			Preconditions:
				1.) Window Must Have Been Initialized
				2.) OpenGL Objects Must Exist

			Postconditions:
				1.) All OpenGL Objects Will Be Deleted
				2.) GLFW Window Will Be Destroyed
				3.) GLFW System Will Be Terminated
				4.) isInit Flag Will Be Set To false

		*/
		void terminate()
		{

			if (!this->isInit)
			{
				std::cerr << "Window Has Not Been Initialized...\n";
				return;
			}

			glDeleteVertexArrays(1, &this->VAO);
			glDeleteBuffers(1, &this->VBO);
			glDeleteBuffers(1, &this->EBO);

			glfwDestroyWindow(this->window);
			glfwTerminate();

			this->isInit = false;

		}


		/*
	
			Desc: Static Callback Function For Handling Window Resize Events
			Updating The OpenGL Viewport To Match New Window Dimensions.

			Preconditions:
				1.) GLFW Must Be Properly Initialized
				2.) Window Resize Event Must Have Occurred
				3.) width And height Must Be Valid Positive Values

			Postconditions:
				1.) OpenGL Viewport Will Be Updated To New Dimensions
				2.) Rendering Will Adapt To New Window Size

		*/
		static void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
		{

			glViewport(0, 0, width, height);

		}



	public:

		/*
	
			Desc: Constructs A New Window Instance With Specified Dimensions
			And Title, Automatically Initializing All Systems.

			Preconditions:
				1.) All Parameters Must Be Valid
				2.) GLFW And OpenGL Libraries Must Be Available

			Postconditions:
				1.) Window Instance Will Be Created With Specified Properties
				2.) All Initialization Will Be Attempted
				3.) Error Message Will Be Displayed If Initialization Fails

		*/
		Window(const unsigned int& width = CONFIG::WINDOW::WIDTH,
			const unsigned int& height = CONFIG::WINDOW::HEIGHT,
			const char* title = CONFIG::WINDOW::TITLE) :
			win_width(width), win_height(height), win_title(title)
		{

			if (!initialize())
			{
				std::cerr << "Window Initialization Has Failed (Consult Dependencies)...\n";
			}

		}


		/*
	
			Desc: Executes The Main Game Loop Handling Events, Rendering,
			And Buffer Swapping Until The Window Should Close.

			Preconditions:
				1.) Window Must Be Properly Initialized
				2.) OpenGL Context Must Be Active
				3.) Shader Program Must Be Ready

			Postconditions:
				1.) Game Loop Will Run Until Window Close Is Requested
				2.) All Events Will Be Processed Each Frame
				3.) Screen Will Be Cleared And Rendered Each Frame
				4.) Buffers Will Be Swapped For Smooth Display
				5.) Window Will Be Terminated When Loop Exits

		*/
		void mainLoop()
		{

			if (!this->isInit)
			{
				std::cerr << "Window Has Not Been Initialized...\n";
				return;
			}

			this->shader.use();

			glBindVertexArray(this->VAO);

			while (!glfwWindowShouldClose(this->window))
			{
				glfwPollEvents();

				this->clear();
				this->render();

				glfwSwapBuffers(this->window);
			}

			this->terminate();

		}


		/*
	
			Desc: Destructor For Window Class Ensuring Proper Cleanup
			Of All Resources If Still Initialized.

			Preconditions:
				1.) Window Instance Must Exist

			Postconditions:
				1.) If Window Is Still Initialized, terminate() Will Be Called
				2.) All Resources Will Be Properly Cleaned Up

		*/
		~Window()
		{

			if (this->isInit)
			{
				this->terminate();
			}

		}

};