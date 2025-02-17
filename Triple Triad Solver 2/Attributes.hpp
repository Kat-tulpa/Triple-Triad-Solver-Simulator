#pragma once
#include "TextureCache.hpp"
#include "Graphics.hpp"

namespace Attributes {
    static constexpr float OFFSET_MULTIPLIER = 0.6f; // 60% of the way to each edge
    static constexpr float VERTICAL_OFFSET = OFFSET_MULTIPLIER * TextureCache::CARD_HALF_HEIGHT;
    static constexpr float HORIZONTAL_OFFSET = OFFSET_MULTIPLIER * TextureCache::CARD_HALF_WIDTH;

    static void draw(float x, float y, const int* attributes) {
        for (int edge = 0; edge < 4; edge++) {
            float xAfterOffset = x;
            float yAfterOffset = y;

            // Apply vertical position modifications to vertical edges
            if (edge == Edge::TOP)
                yAfterOffset -= VERTICAL_OFFSET / 3.5f;
            else if (edge == Edge::BOTTOM)
                yAfterOffset += VERTICAL_OFFSET / 3.5f;
            else if (edge == Edge::LEFT)
                xAfterOffset -= HORIZONTAL_OFFSET;
            else if (edge == Edge::RIGHT)
                xAfterOffset += HORIZONTAL_OFFSET;

            std::string fg = attributeToString(attributes[edge]);
            GraphicsSDL::drawTextWithBackground(xAfterOffset, yAfterOffset + (TextureCache::CARD_HALF_HEIGHT * 0.55f), GraphicsSDL::FONT_SIZE, fg);
        }
    }
}