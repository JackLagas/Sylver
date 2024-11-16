#include "ResourceManager.hpp"


namespace Sylver{
    Result ResourceManager::Add(const std::string& name, const ResourceType type, const std::filesystem::path& path, const b8 load){
        if(s_Resources.find(name) != s_Resources.end()){
            return Result::DUPLICATE;
        }
        s_Resources[name] = new Resource(name, type, path, load);
        return Result::OK;
    }
    Result ResourceManager::Add(const std::string& name, const ResourceType type, const std::filesystem::path& path, const u64 offset, const u64 size, const b8 load){
        if(s_Resources.find(name) == s_Resources.end()){
            return Result::DUPLICATE;
        }
        s_Resources[name] = new Resource(name, type, path, offset, size, load);
        return Result::OK;
    }

    b8 ResourceManager::Remove(const std::string& name){
        if(s_Resources.find(name) == s_Resources.end()){
            return false;
        }
        delete s_Resources[name];
        s_Resources.erase(name);
        return true;
    }

    b8 ResourceManager::Load(const std::string& name){
        if(s_Resources.find(name) == s_Resources.end()){
            return false;
        }
        return s_Resources[name]->Load();
    }
    b8 ResourceManager::Unload(const std::string& name){
        if(s_Resources.find(name) == s_Resources.end()){
            return false;
        }
        return s_Resources[name]->Unload();
    }
    Resource* ResourceManager::Get(const std::string& name){
        if(s_Resources.find(name) == s_Resources.end()){
            return nullptr;
        }
        return s_Resources[name]; 
    }

    void ResourceManager::Cleanup(){
        std::erase_if(s_Resources, [] (auto& kv) { 
                delete kv.second(); 
                return true;
            }
        );
    }


}
