#include <Core/Application.hpp>
#include <Common/Version.hpp>


int main(int argc, char** argv) {
    std::vector<std::string> args(argv, argv + argc);
    Sylver::Application app(args);
}
