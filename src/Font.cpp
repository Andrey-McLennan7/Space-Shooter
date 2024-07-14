#include "Font.h"
#include "Shader.h"

#include <iostream>
#include <ft2build.h>
#include FT_FREETYPE_H

Font::Font(std::string path) :
	m_success{ true }
{
	FT_Library ft;

	if (FT_Init_FreeType(&ft))
	{
		this->m_error = "COULD NOT INITIALISE FREETYPE LIBRARY";
		this->m_success = false;
		return;
	}

	FT_Face face;
	if (FT_New_Face(ft, path.c_str(), 0, &face))
	{
		this->m_error = "FAILED TO LOAD FONT";
		this->m_success = false;
		return;
	}

	FT_Set_Pixel_Sizes(face, 0, 48);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (GLubyte c{ 0 }; c < 128; ++c)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cerr << "ERROR: FAILED TO LOAD GLYMPH" << std::endl;
			continue;
		}

		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);

		glBindTexture(GL_TEXTURE_2D, texture);

		glTexImage2D
		(
			GL_TEXTURE_2D, 0, GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0, GL_RED, GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		// Set Texture Options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Store characters for later use
		Character character =
		{
			texture,
			face->glyph->advance.x,

			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top)
		};

		this->m_characters.insert(std::pair<char, Character>(c, character));
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void Font::draw(GLuint vao, GLuint vbo, Shader* shader, const std::string& text, glm::vec2 position, float scale)
{
	shader->use();
	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(vao);

	// Iterate through all characters
	for (std::string::const_iterator c = text.begin(); c != text.end(); ++c)
	{
		Character ch = this->m_characters[*c];

		float xpos{ position.x + ch.bearing.x * scale };
		float ypos{ position.y - (ch.size.y - ch.bearing.y) * scale };

		float w{ ch.size.x * scale };
		float h{ ch.size.y * scale };

		float vertices[6][4] =
		{
			{ xpos,     ypos + h, 0.0f, 0.0f },
			{ xpos,     ypos,	  0.0f, 1.0f },
			{ xpos + w, ypos,	  1.0f, 1.0f },

			{ xpos,     ypos + h, 0.0f, 0.0f },
			{ xpos + w, ypos,	  1.0f, 1.0f },
			{ xpos + w, ypos + h, 1.0f, 0.0f }
		};

		// Render glymth texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.textureID);

		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Render Quad
		glDrawArrays(GL_TRIANGLES, 0, 6);

		position.x += (ch.advance >> 6) * scale;
	}

	glBindVertexArray(vao);
	glBindTexture(GL_TEXTURE_2D, 0);

	shader->unuse();
}