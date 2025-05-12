#include <Core/Application.hpp>
#include <Core/Version.hpp>


int main(int argc, char** argv) {
    std::vector<std::string> args(argv, argv + argc);
    Sylver::Application app(args);
}
