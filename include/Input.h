#pragma once
#include <optional>
#include <raylib.h>
#include "Layout.h"
#include "Board.h"

std::optional<Square> MouseToSquare(const Layout& L, Vector2 mouse);
void HandleClick(Board& board, const Layout& L, Vector2 mouse);