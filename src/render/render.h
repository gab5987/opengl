#ifndef RENDER_RENDER_H
#define RENDER_RENDER_H

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

