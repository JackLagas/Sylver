#ifndef SYLVER_EVENT_HPP
#define SYLVER_EVENT_HPP

#include <Defines.hpp>
#include "Event/Keys.hpp"

namespace Sylver {
    // TODO: Joystick/Controller Events
    enum class EventType {
        COMMON_EVENT,
        WINDOW_CLOSE_EVENT,
        WINDOW_RESIZE_EVENT,
        WINDOW_MOVE_EVENT,
        WINDOW_FOCUS_EVENT,
        WINDOW_MINIMIZE_EVENT,
        WINDOW_MAXIMIZE_EVENT,
        KEYBOARD_EVENT,
        MOUSE_MOVE_EVENT,
        MOUSE_CLICK_EVENT,
        MOUSE_SCROLL_EVENT,
    };

    struct CommonEvent {
            EventType type{ EventType::COMMON_EVENT };
    };
    struct WindowCloseEvent {
            EventType type{ EventType::WINDOW_CLOSE_EVENT };
            void* window_id;
    };
    struct WindowResizeEvent {
            EventType type{ EventType::WINDOW_RESIZE_EVENT };
            void* window_id;
            u32 x;
            u32 y;
    };
    struct WindowMoveEvent {
            EventType type{ EventType::WINDOW_MOVE_EVENT };
            void* window_id;
            u32 x;
            u32 y;
    };
    struct WindowFocusEvent {
            EventType type{ EventType::WINDOW_FOCUS_EVENT };
            void* window_id;
            u32 gained_focus;
    };
    struct WindowMinimizeEvent {
            EventType type{ EventType::WINDOW_MINIMIZE_EVENT };
            void* window_id;
            u32 minimized;
    };
    struct WindowMaximizeEvent {
            EventType type{ EventType::WINDOW_MAXIMIZE_EVENT };
            void* window_id;
            u32 maximized;
    };
    struct KeyboardEvent {
            EventType type{ EventType::KEYBOARD_EVENT };
            void* window_id;
            Keys key;
            ButtonAction action;
    };
    struct MouseMoveEvent {
            EventType type{ EventType::MOUSE_MOVE_EVENT };
            void* window_id;
            f64 x;
            f64 y;
    };
    struct MouseClickEvent {
            EventType type{ EventType::MOUSE_CLICK_EVENT };
            void* window_id;
            MouseButton button;
            ButtonAction action;
    };
    struct MouseScrollEvent {
            EventType type{ EventType::MOUSE_SCROLL_EVENT };
            void* window_id;
            f64 x_offset;
            f64 y_offset;
    };

    template<typename T>
    concept Event_T = requires(T a) {
        { a.type } -> std::convertible_to<EventType>;
    };


    template<Event_T T>
    class EventListener {
        private:
            inline static std::vector<T> s_EventQueue = {};

            std::function<b8(T)> m_EventCallback = []() { return false; };

        public:
            EventListener();
            EventListener(std::function<b8(T)> callback) :
                m_EventCallback(callback) {}

            void SetCallback(std::function<b8(T)> callback) {
                m_EventCallback = callback;
            }

            void Handle() {
                std::erase_if(s_EventQueue, [&](T a) {
                    return m_EventCallback(a);
                });
            }

            static void AddEvent(T event) {
                s_EventQueue.push_back(event);
            }
    };

}    // namespace Sylver
#endif
