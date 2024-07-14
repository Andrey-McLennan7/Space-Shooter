#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

// Directions
enum Camera_Movement
{
	Forward,
	Backward,
	Left,
	Right,
	Up,
	Down
};

Camera::Camera(	glm::vec3 position, glm::vec3 up, float yaw, float pitch, float speed)

	: m_position{ position }, m_world_up{ up }, m_yaw{ yaw }, m_pitch{ pitch },

	m_front{ glm::vec3(0.0f, 0.0f, -1.0f) },
	m_sensitivity{ DCV::SENSITIVITY },
	m_zoom{ DCV::ZOOM }, m_speed{ speed },
	m_move{ false }
{
	this->UpdateCameraVectors();
}

Camera::~Camera()
{}

void Camera::processMouseMovement(float xoffset, float yoffset)
{
	xoffset *= this->m_sensitivity;
	yoffset *= this->m_sensitivity;

	this->m_yaw += xoffset;
	this->m_pitch += yoffset;

	if (this->m_pitch >= 89.0f)
		this->m_pitch = 89.0f;

	if (this->m_pitch <= -89.0f)
		this->m_pitch = -89.0f;

	this->UpdateCameraVectors();
}

void Camera::processKeyboard(enum Camera_Movement direction, float deltaTime)
{
	const float velocity{ this->m_speed * deltaTime };

	if (direction == Forward)
		this->m_position += this->m_front * velocity;

	if (direction == Backward)
		this->m_position -= this->m_front * velocity;

	if (direction == Right)
		this->m_position += this->m_right * velocity;

	if (direction == Left)
		this->m_position -= this->m_right * velocity;

	if (direction == Up)
		this->m_position += this->m_up * velocity;

	if (direction == Down)
		this->m_position -= this->m_up * velocity;
}

void Camera::ProcessMouseScroll(float yoffset)
{
	if (this->m_move)
	{
		this->m_zoom -= yoffset;

		if (this->m_zoom >= 45.0f)
			this->m_zoom = 45.0f;

		if (this->m_zoom <= 20.0f)
			this->m_zoom = 20.0f;
	}
}

void Camera::UpdateCameraVectors()
{
	// Recalculate View Direction
	glm::vec3 direction
	{
		std::cos(glm::radians(this->m_yaw)) * std::cos(glm::radians(this->m_pitch)),
		std::sin(glm::radians(this->m_pitch)),
		std::sin(glm::radians(this->m_yaw)) * std::cos(glm::radians(this->m_pitch))
	};

	this->m_front = glm::normalize(direction);

	// Recalculate right and up vectors
	this->m_right = glm::normalize(glm::cross(this->m_front, this->m_world_up));
	this->m_up = glm::normalize(glm::cross(this->m_right, this->m_front));
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(this->m_position, this->m_position + this->m_front, this->m_up);
}