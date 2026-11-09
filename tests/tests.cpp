// tests.cpp: every automated check for Picket. Grows with each step.
// Run with: ctest --preset linux-debug

#include <gtest/gtest.h>

#include "sim/common.h"
#include "sim/map.h"

using namespace picket;

// ---------------------------------------------------------------------------
// common.h
// ---------------------------------------------------------------------------

TEST(Vec2, LengthOfThreeFourIsFive) {
    EXPECT_FLOAT_EQ(length(Vec2{3.0f, 4.0f}), 5.0f);
}

TEST(Vec2, LengthSquaredSkipsTheRoot) {
    EXPECT_FLOAT_EQ(lengthSquared(Vec2{3.0f, 4.0f}), 25.0f);
}

TEST(Vec2, NormalizedZeroStaysZero) {
    EXPECT_EQ(normalized(Vec2{0.0f, 0.0f}), (Vec2{0.0f, 0.0f}));
}

// ---------------------------------------------------------------------------
// map.h: the terrain grid
// ---------------------------------------------------------------------------

TEST(Terrain, NewTerrainIsFlatSea) {
    Terrain t(4, 100.0f);
    ASSERT_EQ(t.heights().size(), 16u);
    for (float h : t.heights()) {
        EXPECT_EQ(h, 0.0f);
    }
    EXPECT_TRUE(t.isSea({50.0f, 50.0f}));   // height 0 counts as sea
}

TEST(Terrain, CellIndexIsRowMajor) {
    Terrain t(800, 100.0f);
    t.setHeight({5, 3}, 123.0f);              // column 5, row 3
    EXPECT_FLOAT_EQ(t.heights()[2405], 123.0f); // 3 * 800 + 5
}

TEST(Terrain, CellAtUsesFloorNotTruncation) {
    Terrain t(4, 100.0f);
    EXPECT_EQ(t.cellAt({150.0f, 250.0f}), (Cell{1, 2}));
    EXPECT_EQ(t.cellAt({-50.0f, 0.0f}), (Cell{-1, 0}));   // west of the map, not column 0
}

TEST(Terrain, CellCentreIsHalfACellIn) {
    Terrain t(800, 100.0f);
    EXPECT_EQ(t.cellCentre({5, 3}), (Vec2{550.0f, 350.0f}));
}

TEST(Terrain, OffMapIsSeaLevelAndWritesAreIgnored) {
    Terrain t(4, 100.0f);
    t.setHeight({0, 0}, 50.0f);
    t.setHeight({9, 9}, 100.0f);   // outside the grid: must be ignored

    EXPECT_FLOAT_EQ(t.heightAt({50.0f, 50.0f}), 50.0f);
    EXPECT_FLOAT_EQ(t.heightAt({-1.0f, 50.0f}), 0.0f);
    EXPECT_TRUE(t.isSea({-1.0f, 50.0f}));

    for (std::size_t i = 1; i < t.heights().size(); ++i) {
        EXPECT_EQ(t.heights()[i], 0.0f) << "cell index " << i << " was changed";
    }
}

// ---------------------------------------------------------------------------
// map.h: the test island
// ---------------------------------------------------------------------------

TEST(TestIsland, CentreIsLandAtDomeHeight) {
    Terrain island = makeTestIsland();
    EXPECT_NEAR(island.heightAt({40'000.0f, 40'000.0f}), 400.0f, 1.0f);
}

TEST(TestIsland, RidgeStandsWellAboveCentre) {
    Terrain island = makeTestIsland();
    float centre = island.heightAt({40'000.0f, 40'000.0f});
    float ridge = island.heightAt({46'000.0f, 40'000.0f});
    EXPECT_GT(ridge, centre + 500.0f);
}

TEST(TestIsland, CornersAreSea) {
    Terrain island = makeTestIsland();
    EXPECT_TRUE(island.isSea({100.0f, 100.0f}));
    EXPECT_TRUE(island.isSea({79'900.0f, 79'900.0f}));
}