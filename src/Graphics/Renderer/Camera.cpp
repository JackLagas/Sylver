#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Sylver{
    Camera::Camera(glm::vec2 position, glm::vec2 size, glm::vec2 depth){
        m_Projection = glm::ortho(0.0f, size.x, 0.0f, size.y, depth.x, depth.y);
        m_View = glm::translate(glm::mat4(1.0f), glm::vec3(-position, 0.0f));
    }
    Camera::~Camera(){

    }
    const glm::mat4& Camera::GetView() const{
        return m_View;
    }
    const glm::mat4& Camera::GetProjection() const{
        return m_Projection;
    }
    
}
