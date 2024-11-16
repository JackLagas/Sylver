#ifndef SYLVER_OPENGL_RENDERER_HPP
#define SYLVER_OPENGL_RENDERER_HPP

#include <Graphics/Renderer.hpp>


namespace Sylver {
    class OpenGLRenderer : public Renderer {
        private:
        public:
            OpenGLRenderer(Window* window);
            ~OpenGLRenderer();

            void BeginFrame();
            void Submit(const Object& object);
            void EndFrame();
    };
}    // namespace Sylver

#endif
