#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using i64 = int64_t;

#ifdef DEBUG
    #define DBG(x) std::cout << x << "\n";
#else
    #define DBG(x) 
#endif

inline std::string copy_qasm_code(std::string& path) {
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("ERR: the path \"" + path + "\" to the source code file is invalid");
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    if (file.fail()) {
        throw std::runtime_error("ERR: failed to read from source code file \"" + path + "\"");
    }
    return buffer.str();
}

inline std::vector<u8> to_bytes(u16 value) {
    std::vector<u8> bytes(2);
    for (int i = 0; i < 2; ++i) {
        bytes[1 - i] = value & 0xFF;
        value >>= 8;
    }
    return bytes;
}

inline std::vector<u8> to_bytes(u32 value) {
    std::vector<u8> bytes(4);
    for (int i = 0; i < 4; ++i) {
        bytes[3 - i] = value & 0xFF;
        value >>= 8;
    }
    return bytes;
}

inline std::vector<u8> to_bytes(u64 value) {
    std::vector<u8> bytes(8);
    for (int i = 0; i < 8; ++i) {
        bytes[7 - i] = value & 0xFF;
        value >>= 8;
    }
    return bytes;
}

template <typename T>
T sign_to_unsign(i64 value) {
    // if (value < 0) {
    //     return static_cast<T>(-value) | (1ull << (sizeof(T) * 8 - 1));
    // } else {
    //     return static_cast<T>(value) | (0ull << (sizeof(T) * 8 - 1)); // TODO:
    // }

    return static_cast<T>(value);
}
