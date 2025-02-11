#include <GLFW/glfw3.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/camera.h"
#include "engine/window.h"

using namespace engine;

void camera::update()
{
    this->_direction.x =
        std::cos(glm::radians(this->_yaw)) * cos(glm::radians(this->_pitch));
    this->_direction.y = std::sin(glm::radians(this->_pitch));
    this->_direction.z =
        std::sin(glm::radians(this->_yaw)) * cos(glm::radians(this->_pitch));
    this->_front = glm::normalize(this->_direction);
}

void camera::keyboard(const uint32_t key)
{
    if ((key & key::front) != 0)
    {
        this->_pos += SPEED * this->_front;
    }
    if ((key & key::back) != 0)
    {
        this->_pos -= SPEED * this->_front;
    }
    if ((key & key::left) != 0)
    {
        this->_pos -=
            glm::normalize(glm::cross(this->_front, this->_up)) * SPEED;
    }
    if ((key & key::right) != 0)
    {
        this->_pos +=
            glm::normalize(glm::cross(this->_front, this->_up)) * SPEED;
    }
}

void camera::mouse(glm::vec2 deltas, glm::vec2 mouse_pos)
{
    static constexpr float sensitivity = 0.1f;
    deltas.x *= sensitivity;
    deltas.y *= sensitivity;

    this->_yaw += deltas.x;
    this->_pitch += deltas.y;

    this->_pitch = std::clamp(this->_pitch, -89.0f, 89.0f);
}

glm::mat4 camera::get_view_matrix() const
{
    return glm::lookAt(this->_pos, this->_pos + this->_front, this->_up);
}

camera::camera(window &win)
{
    win.add_keypad_callback(
        [this](const uint32_t key) -> void { this->keyboard(key); });

    win.add_mouse_callback(
        [this](const glm::vec2 deltas, const glm::vec2 pos) -> void {
            this->mouse(deltas, pos);
        });
}