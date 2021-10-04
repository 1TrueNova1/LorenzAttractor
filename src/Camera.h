#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>

namespace Engine
{
	struct Camera
	{
		double m_yaw = -90.0;
		double m_pitch = 0.0;
		double m_roll = 0.0;

		glm::vec3 position;
		glm::vec3 direction;
		glm::vec3 right;
		glm::vec3 up;
		glm::vec3 front;
	public:
		Camera(const glm::vec3& position, const glm::vec3& target)
			: position(position)
		{
			front = { 0.0f, 0.0f, -1.0f };
			recalculateDirection(target);
			recalculateRight();
			recalculateUp();
		}
		Camera(const glm::vec3& position)
			: position(position)
		{
			front = { 0.0f, 0.0f, -1.0f };
			recalculateDirection();
			recalculateRight();
			recalculateUp();
		}

		double pitch() const { return m_pitch; }
		double yaw() const { return m_yaw; }
		double roll() const { return m_roll; }

		void setEuler(double yaw, double pitch, double roll = 0.0)
		{
			setYaw(yaw);
			setPitch(pitch);
			setRoll(roll);
			recalculateDirection();
		}
		void setPitch(double pitch)
		{
			m_pitch = pitch;
			if (m_pitch > 89.0f)
				m_pitch = 89.0f;
			else if (m_pitch < -89.0f)
				m_pitch = -89.0f;
		}
		void setYaw(double yaw) { m_yaw = yaw; }
		void setRoll(double roll) { m_roll = roll; }

		void recalculateDirection(const glm::vec3& target)
		{
			direction = glm::normalize(position - target);
		}
		void recalculateDirection()
		{
			front.x = std::cosf(glm::radians(m_yaw)) * std::cosf(glm::radians(m_pitch));
			front.y = std::sinf(glm::radians(m_pitch));
			front.z = std::sinf(glm::radians(m_yaw)) * std::cosf(glm::radians(m_pitch));
			front = glm::normalize(front);
		}
		void recalculateRight()
		{
			glm::vec3 up = { 0.0f, 1.0f, 0.0f };
			right = glm::normalize(glm::cross(up, direction));
		}
		void recalculateUp()
		{
			up = glm::cross(direction, right);
		}

		glm::mat4 getLookAt() const
		{
			return glm::lookAt(position, position + front, up);
		}
	};
}