#ifndef SYLVER_RESULT_HPP
#define SYLVER_RESULT_HPP

namespace Sylver {
    enum Result {
        OK = 0,
        FILE_NOT_FOUND,
        FILE_CORRUPTED,
        FILE_EXISTS,
        NO_SUCH_DIRECTORY,
        DUPLICATE,
        INVALID_INPUT,
    };

}

#endif
