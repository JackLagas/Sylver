#pragma once

#include <Defines.hpp>


namespace Sylver{
    class Texture{
        public:
            virtual ~Texture(){}

            virtual void LoadCPU() = 0;
            virtual void LoadGPU() = 0;

            virtual void FreeGPU() = 0;
            virtual void FreeCPU() = 0;

            virtual bool InGPU() const = 0;
            virtual bool InCPU() const = 0;

            virtual void Bind() const = 0;
            virtual void Unbind() const = 0;


    };
}
