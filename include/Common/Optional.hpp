#ifndef SYLVER_OPTIONAL_HPP
#define SYLVER_OPTIONAL_HPP

#include <Defines.hpp>


/*
 * Used as reference: http://www.club.cc.cmu.edu/%7Eajo/disseminate/2017-02-15-Optional-From-Scratch.pdf
 */



namespace Sylver {
    template<typename T>
    class [[deprecated("Not working")]]Optional {
        private:
            b8 m_HasValue{ false };
            union {
                    u8 dummy;
                    T m_Value;
            };

        public:
            Optional() :
                m_HasValue(false) {}
            Optional(const Optional& copy){
                if((m_HasValue = copy.m_HasValue)){
                    m_Value = copy.m_Value;
                }
            };
            Optional(T value) :
                m_Value(value), m_HasValue(true) {}
            
            template<typename... Args>
            explicit constexpr Optional(std::in_place_t, Args&&... args) :
                m_Value(std::forward<Args>(args)...), m_HasValue(true) {}
            template<typename U, typename... Args>
            explicit constexpr Optional(std::in_place_t, std::initializer_list<U> list, Args&&... args):
                m_Value(list, std::forward<Args>(args)...), m_HasValue(true) {}
            
            Optional(std::nullptr_t nullValue) :
                m_HasValue(false) {}
            ~Optional() {
                if (m_HasValue)
                    m_Value.~T();
            }
            T& Get() {
                return m_Value;
            }
            const T& Get() const {
                return m_Value;
            }
            void Set(const T& value) {
                m_Value = value;
                m_HasValue = true;
            }
            bool HasValue() const {
                return m_HasValue;
            }

            void Reset(){
                m_HasValue.~T();
                m_HasValue = false;
            }

            operator T&() {
                return m_Value;
            }

            Optional& operator=(const T& value) noexcept{
                m_Value = value;
                m_HasValue = true;
                return *this;
            }
            T& operator*(){
                return m_Value;
            }
            T& operator->(){
                return m_Value;
            }

            explicit operator b8() const noexcept {
                return m_HasValue;
            }
    };
}    // namespace Sylver

#endif
