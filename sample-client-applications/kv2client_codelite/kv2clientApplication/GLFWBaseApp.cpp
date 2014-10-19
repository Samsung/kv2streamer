/*
 * Copyright (c) 2014 Samsung Electronics Corporation.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *    http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "GLFWBaseApp.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

GLFWBaseApp::GLFWBaseApp(int WIDTH, int HEIGHT, const char* AppName)
{
	glfwSetErrorCallback(ErrorCallback);
	if (!glfwInit())
		exit(EXIT_FAILURE);
	
	window = glfwCreateWindow(WIDTH, HEIGHT, AppName, NULL, NULL);
	
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, KeyCallback);
	
	isSetup = false;	
	
	{
	GLint majorVersion = 0;
	GLint minorVersion = 0;
	
		glGetIntegerv(GL_MAJOR_VERSION,&majorVersion);
		glGetIntegerv(GL_MINOR_VERSION,&minorVersion);
		
		std::cout << "OpenGL V" << majorVersion << "." << minorVersion << std::endl;
		
		printf("%s %s\n", glGetString(GL_RENDERER), glGetString(GL_VERSION));

	}

}

GLFWBaseApp::~GLFWBaseApp()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void GLFWBaseApp::Run()
{	
	while (!glfwWindowShouldClose(window))
	{
		if (!isSetup) 
		{
			Setup();
			isSetup = true;
		}
		
		Update();
		Draw();
	}
	
	Destroy();
}

void GLFWBaseApp::ErrorCallback(int error, const char* description)
{
	fputs(description, stderr);
}

void GLFWBaseApp::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
