#pragma once

#include <Defines.hpp>

namespace Sylver{
    class Camera {
        private:
            glm::mat4 m_View{};
            glm::mat4 m_Projection{};
        public:
            Camera(glm::vec2 position, glm::vec2 size, glm::vec2 depth = {0.1f, 1000.0f});
            ~Camera();

            const glm::mat4& GetView() const;
            const glm::mat4& GetProjection() const;




    };
}
