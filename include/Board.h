#pragma once
#include <array>
#include <optional>

struct Square { int file{0}; int rank{0}; }; // 0..7

using BoardArray = std::array<std::array<char,8>,8>;

struct Board {
    BoardArray squares{};
    std::optional<Square> selected{};
    Board();
};