#include "sim/map.h"

#include <algorithm>
#include <cmath>
#include <cstddef>

namespace picket
{

    Terrain::Terrain(int cellsPerSide, float cellSizeM) : cellsPerSide_(cellsPerSide),
                                                          cellSizeM_(cellSizeM),
                                                          heights_(static_cast<std::size_t>(cellsPerSide) * cellsPerSide, 0.0f) {}

    bool Terrain::isInside(Cell c) const
    {
        return c.col >= 0 && c.col < cellsPerSide_ && c.row >= 0 && c.row < cellsPerSide_;
    }

    float Terrain::height(Cell c) const
    {
        if (!isInside(c))
            return 0.0f;

        return heights_[index(c)];
    }

    void Terrain::setHeight(Cell c, float heightM)
    {
        if (isInside(c))
        {
            heights_[index(c)] = heightM;
        }
    }

    Cell Terrain::cellAt(Vec2 worldPos) const
    {
        int col = static_cast<int>(std::floor(worldPos.x / cellSizeM_));
        int row = static_cast<int>(std::floor(worldPos.y / cellSizeM_));
        return {col, row};
    }

    Vec2 Terrain::cellCentre(Cell c) const
    {
        return {(c.col + 0.5f) * cellSizeM_, (c.row + 0.5f) * cellSizeM_};
    }

    float Terrain::heightAt(Vec2 worldPos) const
    {
        return height(cellAt(worldPos));
    }

    bool Terrain::isSea(Vec2 worldPos) const
    {
        return heightAt(worldPos) <= 0.0f;
    }

    Terrain makeTestIsland()
    {
        Terrain terrain(kGridCells, kCellSizeM);

        const Vec2 centre{kWorldSizeM / 2.0f, kWorldSizeM / 2.0f};
        const float islandRadiusM = 30'000.0f;
        const float domeHeightM = 400.0f;
        const float ridgeX = 46'000.0f;
        const float ridgeHalfWidthM = 3'000.0f;
        const float ridgeHeightM = 800.0f;

        for (int row = 0; row < terrain.cellsPerSide(); ++row)
        {
            for (int col = 0; col < terrain.cellsPerSide(); ++col)
            {
                Cell cell{col, row};
                Vec2 p = terrain.cellCentre(cell);

                float r = distance(p, centre) / islandRadiusM;
                if (r >= 1.0f)
                {
                    terrain.setHeight(cell, -50.0f);
                    continue;
                }

                float dome = domeHeightM * (1.0f - r * r);
                float ridgeFactor = 1.0f - std::fabs(p.x - ridgeX) / ridgeHalfWidthM;
                float ridge = ridgeHeightM * std::max(0.0f, ridgeFactor);
                terrain.setHeight(cell, dome + ridge);
            }
        }

        return terrain;
    }
} // namespace picket