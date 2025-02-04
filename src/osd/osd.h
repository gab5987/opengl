#ifndef OSD_OSD_H
#define OSD_OSD_H

#include <functional>
#include <string>

// foward declare window.
namespace engine
{
class window;
};

namespace osd
{

struct ctx
{
    const std::string _id;
    const std::string _name;

    std::function<void(struct ImGuiIO &)> draw;
};

class osd
{
    private:
    std::vector<ctx> _ctxs;

    struct ImGuiIO &_io;

    static ImGuiIO &init_imgui();

    public:
    void draw();

    std::string add_ctx(const std::string &name, decltype(ctx::draw) &&);

    explicit osd(engine::window &win);
    ~osd();
};
}; // namespace osd

#endif

