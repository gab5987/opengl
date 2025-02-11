#include <imgui.h>
#include <spdlog/spdlog.h>

#include "engine/window.h"
#include "osd/osd.h"
#include "osd/scene.h"

void osd::scene::draw(ImGuiIO &imgui_io)
{
    ImGui::Text(
        "Application average %.3f ms/frame (%.1f FPS)",
        1000.0F / imgui_io.Framerate, imgui_io.Framerate);

    if (ImGui::Button("WireframeMode"))
    {
        this->_wireframe = !this->_wireframe;
        if (this->_wireframe)
        {
            engine::window::wireframe_mode();
        }
        else
        {
            engine::window::normal_mode();
        }
    }
}

osd::scene::scene(osd &osd)
{
    osd.add_ctx("SceneSettings", [this](auto &_io) { this->draw(_io); });
}
