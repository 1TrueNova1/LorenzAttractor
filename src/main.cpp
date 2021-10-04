#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>

#include "Debugger.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include <imgui/imgui_impl_opengl3.h>

glm::vec3 getNextPoint(glm::vec3 point, double dt);
void recalculateVector(size_t id);
void recalculatePositions();
void reset();

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);


int pointsPerFrame = 20;

int width = 1920;
int height = 1080;

double lastX = width / 2.0f;
double lastY = height / 2.0f;

bool mousePressed = false;

GLFWwindow* window;

Engine::Camera camera({ 0.0f, 0.0f, 50.0f }, { 0.0f, 0.0f, 0.0f });
float dt = 0.0002f;

std::vector<std::vector<glm::vec3>> newPoints;
std::vector<glm::vec4> colors = {
	{1.0f, 0.0f, 0.0f, 1.0f},
	{0.0f, 1.0f, 0.0f, 1.0f},
	{0.0f, 0.0f, 1.0f, 1.0f}
};

std::vector<glm::vec3> points = {
	{10.0f, 10.0f, 25.0f}
};
float sigma = 10.0f;
float beta = 8 / 3.0f;
float r = 28.0f;

bool mouseAllowed = true;

int main(void)
{
	{
		newPoints.push_back({ {10.0f, 10.0f, 26.0f} });
		newPoints.push_back({ {10.0f, 10.0f, 26.01f} });
		newPoints[0].reserve(1000000);
		newPoints[1].reserve(1000000);

		/* Initialize the library */
		if (!glfwInit())
			return -1;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_SAMPLES, 4);

		/* Create a windowed mode window and its OpenGL context */
		window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
		if (!window)
		{
			glfwTerminate();
			return -1;
		}

		/* Make the window's context current */
		glfwMakeContextCurrent(window);
		glfwSwapInterval(1);

		if (glewInit() != GLEW_OK) {
			std::cout << "GLEW not initialized\n";
		}
		std::cout << "GLEW initialized successfully\n";

		std::cout << glGetString(GL_VERSION) << '\n';

		//learn more about later
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_MULTISAMPLE);

		glPointSize(1);

		int posFloatsPerVertex = 3;

		glm::mat4 proj = glm::perspective(glm::radians(60.0f), (float)width / height, 0.00001f, 1000000.0f);

		VertexArray vao;

		VertexBuffer vbo;
		vbo.reserve(100000000, GL_DYNAMIC_DRAW);
		VertexBufferLayout layout;
		layout.push<float>(posFloatsPerVertex);
		vao.addBuffer(vbo, layout);

		Shader shader(
			"..\\res\\shaders\\vertexShader.glsl",
			"..\\res\\shaders\\fragmentShader.glsl");
		shader.bind();
		shader.setUniform4f("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);
		shader.setUniformMat4f("u_Proj", proj);

		glfwSetKeyCallback(window, keyboard_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);
		glfwSetScrollCallback(window, scroll_callback);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glPointSize(1);

		Renderer renderer;

		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
		ImGui::StyleColorsDark();

		bool show_demo_window = true;
		bool show_another_window = true;
		std::vector<ImVec4> clear_colors = {
			{1.0f, 0.0f, 0.0f, 1.0f},
			{0.0f, 1.0f, 0.0f, 1.0f},
			{0.0f, 0.0f, 1.0f, 1.0f}
		};
		float f = 0.0f;
		float dtTmp = dt * 100000.0f;
		int nofLines = newPoints.size();

		std::chrono::steady_clock::time_point previousTime = std::chrono::steady_clock::now();
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			renderer.clear();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			processInput(window);

			std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
			std::chrono::duration<double> delta = currentTime - previousTime;
			previousTime = currentTime;


			for (size_t j = 0; j < newPoints.size(); ++j)
			{
				for (int i = 0; i < pointsPerFrame; ++i)
				{
					glm::vec3 point = newPoints[j].back();
					point = getNextPoint(point, dt);
					newPoints[j].push_back(point);
				}
			}

			shader.setUniformMat4f("u_View", camera.getLookAt());
			for (size_t i = 0; i < newPoints.size(); ++i)
			{
				vbo.addData(newPoints[i].data(), newPoints[i].size() * sizeof(glm::vec3), 0);
				shader.setUniform4f("u_Color", clear_colors[i].x, clear_colors[i].y, clear_colors[i].z, clear_colors[i].w);
				renderer.draw(vao, shader, newPoints[i].size(), GL_POINTS);
			}

			float rTmp = r;
			ImGui::SliderFloat("R parameter", &rTmp, -200.0f, 200.0f);
			if (rTmp != r)
			{
				r = rTmp;
				recalculatePositions();
			}
			ImGui::SliderFloat("Delta time", &dtTmp, 1.0f, 1000.0f);
			dt = dtTmp / 100000.0f;
			ImGui::SliderInt("Points per frame", &pointsPerFrame, 0, 100);
			int linesTmp = nofLines;
			ImGui::SliderInt("Number of lines", &linesTmp, 1, 3);
			if (linesTmp != nofLines)
			{
				if (linesTmp > nofLines)
				{
					do {
						glm::vec3 startingPoint = newPoints.back()[0];
						startingPoint.z += 0.01f;
						std::vector<glm::vec3> vector;
						vector.resize(newPoints.back().size());
						vector[0] = startingPoint;
						newPoints.push_back(std::move(vector));
						recalculateVector(nofLines);
					} while (++nofLines != linesTmp);
				}
				else if (linesTmp < nofLines)
				{
					do {
						newPoints.pop_back();
					} while (--nofLines != linesTmp);
				}
				nofLines = linesTmp;
			}
			ImGui::ColorEdit3("First line", (float*)&(clear_colors[0]));
			if (newPoints.size() > 1)
				ImGui::ColorEdit3("Second line", (float*)&(clear_colors[1]));
			if (newPoints.size() > 2)
				ImGui::ColorEdit3("Third line", (float*)&(clear_colors[2]));

			ImGui::Button("Reset");
			if (ImGui::IsItemActive())
			{
				reset();
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	glfwTerminate();
	return 0;
}

glm::vec3 getNextPoint(glm::vec3 point, double dt)
{
	float dx = sigma * (point.y - point.x);
	float dy = point.x * (r - point.z) - point.y;
	float dz = point.x * point.y - beta * point.z;

	return point + glm::vec3(dx * dt, dy * dt, dz * dt);
}

void recalculatePositions()
{
	for (size_t i = 0; i < newPoints.size(); ++i)
	{
		recalculateVector(i);
	}
}

void recalculateVector(size_t id)
{
	for (size_t i = 0; i < newPoints[id].size() - 1; ++i)
	{
		newPoints[id][i + 1] = getNextPoint(newPoints[id][i], dt);
	}
}

void reset()
{
	for (auto& vector : newPoints)
	{
		glm::vec3 point = vector[0];
		vector.clear();
		vector.push_back(point);
	}
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	static bool firstMouse = true;
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;
	if (!mouseAllowed)
		return;

	const double sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	camera.setEuler(camera.yaw() + xoffset, camera.pitch() + yoffset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			mousePressed = true;
			//std::cout << "x:" << xpos << ";y:" << ypos << '\n';
		}
		else if (action == GLFW_RELEASE)
		{
			mousePressed = false;
		}
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (yoffset > 0)
		dt *= 1.1f;
	else if (yoffset < 0)
		dt /= 1.1f;
	std::cout << "dt=" << dt << '\n';
}

float baseCameraSpeed = 0.05f;
float cameraSpeed = 0.05f;
float maxCameraSpeed = 1.0f;
void processInput(GLFWwindow* window)
{
	cameraSpeed *= 1.01f;
	if (cameraSpeed > maxCameraSpeed)
		cameraSpeed = maxCameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		cameraSpeed = baseCameraSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera.position += camera.front * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera.position -= camera.front * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		camera.position -= glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		camera.position += glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.position -= camera.up * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.position += camera.up * cameraSpeed;

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		r -= 0.01f;
		recalculatePositions();
	}
	else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		r += 0.01f;
		recalculatePositions();
	}
}

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_Q: {
			if (pointsPerFrame > 1)
				pointsPerFrame--;
			break;
		}
		case GLFW_KEY_E: {
			pointsPerFrame++;
			break;
		}
		case GLFW_KEY_G: {
			mouseAllowed = !mouseAllowed;
			if (mouseAllowed)
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			else
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		}
	}
	else if (action == GLFW_RELEASE)
	{
	}
}

