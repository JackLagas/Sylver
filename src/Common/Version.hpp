#pragma once

#include <Defines.hpp>
#include <ostream>
#include <regex>
#include <fmt/core.h>


namespace Sylver {
    /**
     * @brief Semantic Versioning complient version object.
     */
    struct Version {
            u32 Major{ 0 };
            u32 Minor{ 1 };
            u32 Patch{ 0 };
            std::string Prerelease{};
            std::string Build{};


            Version(u32 major = 0, u32 minor = 1, u32 patch = 0, std::string prerelease = "", std::string build = "") :
                Major(major), Minor(minor), Patch(patch), Prerelease(prerelease), Build(build) {}

            /*
             * @brief returns a value consistant with vulkans VK_MAKE_API_VERSION macro
             */
            u32 vkVersion() {
                return ((u32)(0) << 29U) | ((u32)(Major) << 22U) | ((u32)(Minor) << 12U) | ((u32)(Patch));
            }

            /**
             * @brief Converts string to Version type
             *
             * @param semVer version string
             *
             * @return Returns 0.0.0 for invalid inputs
             */
            inline static std::optional<Version> FromString(std::string semVer) {
                if (!Validate(semVer)) {
                    return std::optional<Version>();
                }
                std::smatch match;
                if (std::regex_search(semVer, match, RegularExpression)) {

                    return Version(
                        std::stoi(match.str(1)),
                        std::stoi(match.str(2)),
                        std::stoi(match.str(3)),
                        match.str(4),
                        match.str(5));
                }
                return std::optional<Version>();
            }

            inline static std::regex RegularExpression = std::regex("^(0|[1-9]\\d*)\\.(0|[1-9]\\d*)\\.(0|[1-9]\\d*)(?:-((?:0|[1-9]\\d*|\\d*[a-zA-Z-][0-9a-zA-Z-]*)(?:\\.(?:0|[1-9]\\d*|\\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?(?:\\+([0-9a-zA-Z-]+(?:\\.[0-9a-zA-Z-]+)*))?$");

            // ********************
            // Helper Functions
            // ********************

            inline static bool Validate(std::string semVer) {
                return std::regex_match(semVer, RegularExpression);
            }


            std::string ToString() const {
                return fmt::format("{}.{}.{}{}{}",
                                   Major,
                                   Minor,
                                   Patch,
                                   Prerelease != "" ? fmt::format("-{}", Prerelease) : "",
                                   Build != "" ? fmt::format("+{}", Build) : "");
            }
            operator std::string() const {
                return ToString();
            }


            bool operator<(const Version& other) {
                if (Major > other.Major) {
                    return false;
                }
                if (Major < other.Major) {
                    return true;
                }

                if (Minor > other.Minor) {
                    return false;
                }
                if (Minor < other.Minor) {
                    return true;
                }

                if (Patch < other.Patch) {
                    return true;
                }
                return false;
            }
            bool operator<=(const Version& other) {
                if (Major > other.Major) {
                    return false;
                }
                if (Major < other.Major) {
                    return true;
                }

                if (Minor > other.Minor) {
                    return false;
                }
                if (Minor < other.Minor) {
                    return true;
                }

                if (Patch <= other.Patch) {
                    return true;
                }
                return false;
            }
            bool operator>(const Version& other) {
                if (Major > other.Major) {
                    return true;
                }
                if (Major < other.Major) {
                    return false;
                }

                if (Minor > other.Minor) {
                    return true;
                }
                if (Minor < other.Minor) {
                    return false;
                }

                if (Patch > other.Patch) {
                    return true;
                }
                return false;
            }
            bool operator>=(const Version& other) {
                if (Major > other.Major) {
                    return true;
                }
                if (Major < other.Major) {
                    return false;
                }

                if (Minor > other.Minor) {
                    return true;
                }
                if (Minor < other.Minor) {
                    return false;
                }

                if (Patch >= other.Patch) {
                    return true;
                }
                return false;
            }
            bool operator==(const Version& other) {
                if (Major != other.Major) {
                    return false;
                }
                if (Minor != other.Minor) {
                    return false;
                }
                if (Patch != other.Patch) {
                    return false;
                }
                return true;
            }
            bool operator!=(const Version& other) {
                if (Major != other.Major) {
                    return true;
                }
                if (Minor != other.Minor) {
                    return true;
                }
                if (Patch != other.Patch) {
                    return true;
                }
                return false;
            }
    };
    static std::ostream& operator<<(std::ostream& stream, const Version& version) {
        return stream << version.ToString();
    }
}    // namespace Sylver
