#ifndef ENGINE_RENDER_H
#define ENGINE_RENDER_H

namespace engine
{
class render
{
    public:
    void clear() const;
    void draw(
        const class varr &vao, const class ibuff &ebo,
        const class program &prog) const;
};
}; // namespace engine

#endif
