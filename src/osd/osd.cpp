#include <algorithm>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <spdlog/spdlog.h>
// #include <backends/imgui_impl_opengl3_loader.h>

#include "osd/osd.h"
#include "render/window.h"
#include "util.h"

std::string osd::osd::add_ctx(
    const std::string &name, decltype(ctx::draw) &&drawer)
{
    const std::string rid = util::randstr(5);
    this->_ctxs.emplace_back(rid, name, drawer);

    spdlog::info("osd::add_ctx(\"{}\"): rid:\"{}\"", name, rid);

    return rid;
}

void osd::osd::draw()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    std::ranges::for_each(this->_ctxs, [this](ctx &ctx) {
        ImGui::Begin(ctx._name.c_str());
        ctx.draw(this->_io);
        ImGui::End();
    });

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

ImGuiIO &osd::osd::init_imgui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    return ImGui::GetIO();
}

osd::osd::osd(engine::window &win) : _io(init_imgui())
{
    this->_io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    this->_io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
    this->_io.FontGlobalScale = win.get_dpi_scale();

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(win.get(), true);
    ImGui_ImplOpenGL3_Init("#version 130");

    this->_ctxs.reserve(10);
}

osd::osd::~osd()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
