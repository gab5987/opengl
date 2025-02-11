#ifndef ENGINE_CAMERA_H
#define ENGINE_CAMERA_H

#include <glm/ext/vector_float3.hpp>

namespace engine
{
class camera
{
    static constexpr float SPEED       = 0.1f;
    static constexpr float SENSITIVITY = 0.1f;

    float _yaw   = -90.0f;
    float _pitch = 0.0f;

    glm::vec3 _pos{0.0f, 0.0f, 3.0f};
    glm::vec3 _front{0.0f, 0.0f, -1.0f};
    glm::vec3 _up{0.0f, 1.0f, 0.0f};

    glm::vec3 _direction{};

    void keyboard(uint32_t key);

    public:
    void update();

    void mouse(glm::vec2 deltas, glm::vec2 mouse_pos);

    [[nodiscard]] glm::mat4 get_view_matrix() const;

    explicit camera(class window &win);
};
} // namespace engine

#endif
