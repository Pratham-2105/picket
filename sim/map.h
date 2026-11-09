#pragma once

#include "sim/common.h"

#include <vector>

namespace picket
{

    struct Cell
    {
        int col = 0;
        int row = 0;

        bool operator==(const Cell &) const = default;
    };

    class Terrain
    {
    public:
        Terrain(int cellsPerSide, float cellSizeM);

        int cellsPerSide() const { return cellsPerSide_; }
        float cellSizeM() const { return cellSizeM_; }
        float worldSizeM() const { return cellsPerSide_ * cellSizeM_; }

        bool isInside(Cell c) const;
        float height(Cell c) const;
        void setHeight(Cell c, float heightM);

        Cell cellAt(Vec2 worldPos) const;
        Vec2 cellCentre(Cell c) const;
        float heightAt(Vec2 worldPos) const;
        bool isSea(Vec2 worldPos) const;

        const std::vector<float> &heights() const { return heights_; };

    private:
        int index(Cell c) const { return c.row * cellsPerSide_ + c.col; }

        int cellsPerSide_;
        float cellSizeM_;
        std::vector<float> heights_;
    };

    Terrain
    makeTestIsland();
} // namespace picket