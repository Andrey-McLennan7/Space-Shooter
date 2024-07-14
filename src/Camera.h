#pragma once

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <glm/glm.hpp>

// Default Camera Values (DCV)
namespace DCV
{
	const float YAW{ -90.0f };
	const float PITCH{ 0.0f };
	const float SPEED{ 2.5f };
	const float SENSITIVITY{ 0.1f };
	const float ZOOM{ 45.0f };
}

class Camera
{
private:

	// Camera Attributes
	glm::vec3 m_position;
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_world_up;
	glm::vec3 m_right;

	// Euler Angles
	float m_yaw;
	float m_pitch;

	// Camera Options
	float m_speed;
	float m_sensitivity;
	float m_zoom;
	bool m_move;

	void UpdateCameraVectors();

public:

	// Constructor/Destructor
	Camera
	(
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		
		float yaw = DCV::YAW,
		float pitch = DCV::PITCH,
		float speed = DCV::SPEED
	);

	~Camera();

	// Getters
	glm::vec3 getPostion()	  const { return this->m_position; }
	glm::vec3 getFront()	  const { return this->m_front;	   }
	float	  getYaw()		  const { return this->m_yaw;	   }
	float	  getPitch()	  const { return this->m_pitch;	   }
	float	  getZoom()		  const { return this->m_zoom;	   }
	bool	  isMoving()	  const { return this->m_move;     }

	glm::mat4 getViewMatrix() const;

	// Setters
	void setPostion(glm::vec3 position) { this->m_position = position; }
	void setYaw(float yaw)				{ this->m_yaw = yaw;		   }
	void setPitch(float pitch)			{ this->m_pitch = pitch;	   }
	void setZoom(float zoom)			{ this->m_zoom = zoom;		   }
	void setMovement(bool move)			{ this->m_move = move;		   }
	void setSpeed(float speed)			{ this->m_speed = speed;	   }

	// Process Input
	void processMouseMovement(float xoffset, float yoffset);
	void processKeyboard(enum Camera_Movement direction, float deltaTime);
	void ProcessMouseScroll(float yoffset);
};

#endif