#pragma once

#include <Defines.hpp>
#include <entt/entt.hpp>

namespace Sylver{
    class EntityManager;
    class Entity{
        private:
            friend EntityManager;
            entt::entity m_EntityID;
        public:
        private:
            Entity(entt::entity id): m_EntityID(id){}
            
            operator entt::entity(){ return m_EntityID; }
    };
    class EntityManager{
        private:
            inline static EntityManager* s_Instance{nullptr};
            entt::registry m_Registry;
        public:
            inline static void Init(){
                s_Instance = new EntityManager();
            }
            inline static void Destroy(){
                delete s_Instance;
            }
            inline static Entity Create(){
                if(s_Instance == nullptr) Init();
                return Entity(s_Instance->m_Registry.create());
            }
            template<typename T, typename... Args>
            inline static void Add(Entity entity, Args&&... args){
                if(s_Instance == nullptr) Init(); 
                s_Instance->m_Registry.emplace<T>(entity, std::forward<Args>(args)...);
            }
            template<typename T>
            inline static void Get(Entity entity){
                if(s_Instance == nullptr) Init(); 
                s_Instance->m_Registry.get<T>(entity);
            }
        private:
            EntityManager(){}
            ~EntityManager(){}
    };
}
