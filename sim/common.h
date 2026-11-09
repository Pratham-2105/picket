#pragma once

#include <cmath>

namespace picket
{
    inline constexpr float kWorldSizeM = 80'000.0f;

    inline constexpr float kCellSizeM = 100.0f;
    inline constexpr int kGridCells = 800;

    inline constexpr float kGridSquareM = 8'000.0f;
    inline constexpr float kGridSquares = 10;

    inline constexpr float kTickSeconds = 0.1f;

    static_assert(kGridCells * kCellSizeM == kWorldSizeM, "terrain cells must cover the world exactly");
    static_assert(kGridSquares * kGridSquareM == kWorldSizeM, "grid squares must cover the world exactly");

    struct Vec2
    {
        float x = 0.0f;
        float y = 0.0f;

        bool operator==(const Vec2 &) const = default;
    };

    inline Vec2 operator+(Vec2 a, Vec2 b) { return {a.x + b.x, a.y + b.y}; }
    inline Vec2 operator-(Vec2 a, Vec2 b) { return {a.x - b.x, a.y - b.y}; }
    inline Vec2 operator*(Vec2 v, float s) { return {v.x * s, v.y * s}; }
    inline Vec2 operator*(float s, Vec2 v) { return {v * s}; }

    inline Vec2 &operator+=(Vec2 &a, Vec2 b)
    {
        a.x += b.x;
        a.y += b.y;
        return a;
    }

    inline float dot(Vec2 a, Vec2 b) { return a.x * b.x + a.y * b.y; }
    inline float length(Vec2 v) { return std::sqrt(dot(v, v)); }
    inline float distance(Vec2 a, Vec2 b) { return length(b - a); }
    inline float lengthSquared(Vec2 v) { return dot(v, v); }

    inline Vec2 normalized(Vec2 v)
    {
        float len = length(v);
        if (len == 0.0f)
        {
            return {0.0f, 0.0f};
        }
        return {v.x / len, v.y / len};
    }
} // namespace picket