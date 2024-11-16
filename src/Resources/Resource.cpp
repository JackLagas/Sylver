#include "Resource.hpp"


#include <Core/Logger.hpp>
#include <exception>
#include <fstream>


namespace Sylver {

    b8 Resource::Load() {
        if (m_Loaded) {
            return true;
        }
        try {
            std::ifstream fileStream(m_FilePath, std::ios::in | std::ios::binary | std::ios::ate);
            std::streamsize fileSize;
            if (m_Size == 0) {
                fileSize = fileStream.tellg();
            } else {
                fileSize = m_Size;
            }
            fileStream.seekg(m_Offset);
            m_Data = new u8(fileSize);
            fileStream.read((char*)m_Data, fileSize);
            m_Loaded = true;
        } catch (std::exception& e) {
            Logger::Warn("Error loading file: {} error: {}", m_FilePath.string(), e.what());
            return false;
        }
        return true;
    }
    b8 Resource::Unload() {
        if (!m_Loaded) {
            return true;
        } else {
            delete m_Data;
            m_Loaded = false;
        }
        return true;
    }
}    // namespace Sylver
