#ifndef OSD_SCENE_H
#define OSD_SCENE_H

namespace osd
{
class scene
{
    bool _wireframe = false;

    void draw(struct ImGuiIO &);

    public:
    explicit scene(class osd &);
};
}; // namespace osd

#endif

