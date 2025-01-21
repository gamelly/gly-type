#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <ranges>
#include <regex>

#include "gly_type_render.h"

static int32_t width, height;

static std::vector<uint8_t> bitmap;

constexpr std::array<uint8_t, 3> padding = {
    0x00, 0x00, 0x00
};

constexpr std::array<uint8_t, 8> palette = {
    0xFF, 0xFF, 0xFF, 0x00,
    0x00, 0x00, 0x00, 0x00
};

struct BMPFileHeader {
    uint16_t bfType{0x4D42};
    uint32_t bfSize;
    uint16_t bfReserved1{0};
    uint16_t bfReserved2{0};
    uint32_t bfOffBits;
} __attribute__((packed));

struct BMPInfoHeader {
    uint32_t biSize{sizeof(BMPInfoHeader)};
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes{1};
    uint16_t biBitCount{1};
    uint32_t biCompression{0};
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter{2835};
    int32_t biYPelsPerMeter{2835};
    uint32_t biClrUsed{2};
    uint32_t biClrImportant{2};
} __attribute__((packed));

void draw_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
    int16_t dx = std::abs(x2 - x1);
    int16_t dy = std::abs(y2 - y1);
    int16_t sx = (x1 < x2) ? 1 : -1;
    int16_t sy = (y1 < y2) ? 1 : -1;
    int16_t err = dx - dy;
    while (true) {
        if (y1 < height && x1 < width) {
            int16_t byte_index = (y1 * width + x1) / 8;
            int16_t bit_index = (y1 * width + x1) % 8;
            bitmap[byte_index] |= (1 << (7 - bit_index));
        }
        if (x1 == x2 && y1 == y2) break;
        int16_t err2 = err * 2;
        if (err2 > -dy) { err -= dy; x1 += sx; }
        if (err2 < dx) { err += dx; y1 += sy; }
    }
}

int main(int argc, char* argv[]) {
    const auto ascii = std::views::iota(0x20, 0x7c);
    const auto pattern = std::regex(R"((\d+)x(\d+))");

    std::smatch match;
    std::string arg_size = (argc > 1) ? argv[1] : "64x96";
    std::string arg_file = (argc > 2) ? argv[2] : "font.bmp";
    std::string arg_text = (argc > 3) ? argv[3] : std::string(ascii.begin(), ascii.end());
    std::ofstream file(arg_file, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "error creating image file!" << std::endl;
        return 1;
    }

    if (!std::regex_match(arg_size, match, pattern) && argc > 1) {
        std::cerr << "image size is invalid format!" << std::endl;
        return 1;
    }

    width = std::stoi(match[1].str());
    height = std::stoi(match[2].str());
    bitmap.resize(height * width/8, 0);

    if (width % 8 != 0 || height % 8 != 0) {
        std::cerr << "image size must be divisible by 8!" << std::endl;
        return 1;
    }

    const uint32_t row_size = (width + 7) / 8;
    const uint32_t padded_row_size = (row_size + 3) & ~3;
    const uint32_t data_size = padded_row_size * height;

    BMPFileHeader fileHeader{
        .bfSize = static_cast<uint32_t>(sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + palette.size() + data_size),
        .bfOffBits = static_cast<uint32_t>(sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + palette.size())
    };

    BMPInfoHeader infoHeader{
        .biWidth=width,
        .biHeight=-height,
        .biSizeImage = data_size,
    };

    file.write(reinterpret_cast<const char*>(&fileHeader), sizeof(fileHeader));
    file.write(reinterpret_cast<const char*>(&infoHeader), sizeof(infoHeader));
    file.write(reinterpret_cast<const char*>(palette.data()), palette.size());

    for (uint8_t i = 0; i < arg_text.size(); i++) {
        uint8_t x = i % (width / 8) * 8;
        uint8_t y = i / (width / 8) * 8;
        if (y >= height) break;
        gly_type_render<uint8_t>(x, y, 7, &arg_text[i], 1, draw_line);
    }

    for (int y = 0; y < height; ++y) {
        file.write(reinterpret_cast<const char*>(&bitmap[y * row_size]), row_size);
        file.write(reinterpret_cast<const char*>(padding.data()), padded_row_size - row_size);
    }

    return 0;
}
