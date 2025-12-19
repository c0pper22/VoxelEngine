#include "Crosshair.h"

Crosshair::Crosshair()
{
	texture = std::make_unique<Texture>("assets/crosshair.png");
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	// Texture coord attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void Crosshair::draw(Shader & shader, int SCR_WIDTH, int SCR_HEIGHT)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	shader.use();

	glm::mat4 projection = glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT);

	float crosshairSize = 50.0f;
	float centerX = (SCR_WIDTH - crosshairSize) / 2.0f;
	float centerY = (SCR_HEIGHT - crosshairSize) / 2.0f;

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(centerX, centerY, 0.0f));
	model = glm::scale(model, glm::vec3(crosshairSize, crosshairSize, 1.0f));

	// 6. Set Uniforms
	shader.setMat4("projection", projection);
	shader.setMat4("model", model);

	// 7. Bind Texture and Draw
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->ID);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// 8. Cleanup state for next frame
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}