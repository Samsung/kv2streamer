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

#pragma once

#include <GLFW/glfw3.h>

class glTexture2DWrapper
{
public:
	glTexture2DWrapper(){Initialize();};
	
	~glTexture2DWrapper(){};

	void Initialize()
	{
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	};
	
	void Update(int width, int height, GLenum format, GLenum type, GLvoid* data)
	{
		glBindTexture(GL_TEXTURE_2D, tex);
	
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, type, data);
	};
	
	void Draw(int x, int y, int w, int h, int z)
	{
		glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, tex);
			glBegin(GL_QUADS);
			
				glNormal3d(0,0,1);
				glVertex3d(x, 	y, 	z); glTexCoord2f(1,1); 
				glVertex3d(x+w, y,	z); glTexCoord2f(1,0);
				glVertex3d(x+w, y+h,z); glTexCoord2f(0,0);
				glVertex3d(x, 	y+h,z); glTexCoord2f(0,1);
				
			glEnd();
		glDisable(GL_TEXTURE_2D);

	};
	
private:
	
	GLuint tex;
};
