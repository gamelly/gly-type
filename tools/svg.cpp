#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <ranges>
#include <regex>
#include <string>
#include <sstream>
#include <math.h>

#include "gly_type_render.h"
std::ofstream file;

const std::string svg_header = R"(
<?xml version="1.0" standalone="no"?>
<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN" "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">
<svg xmlns="http://www.w3.org/2000/svg" width="100%" height="100%" viewBox="0 0 1000 1000">
<metadata>Powered by Gamely</metadata>
<defs>
<font horiz-adv-x="1000">
<font-face font-family="Gamely MonoRetro" font-weight="400" font-stretch="normal" units-per-em="1000" ascent="800" descent="-50"/>
)";

const std::string svg_footer = R"(</font>
</defs>
</svg>
)";

void draw_line(int x1, int y1, int x2, int y2)
{
    float offset = 16.0f;
    float angle = std::atan2(y2 - y1, x2 - x1);
    float dx = offset * std::cos(angle + M_PI_2);
    float dy = offset * std::sin(angle + M_PI_2);

    file << " M " << std::dec << x1 + dx << " " << std::dec << y1 + dy
         << " L " << std::dec << x1 - dx << " " << std::dec << y1 - dy
         << " L " << std::dec << x2 - dx << " " << std::dec << y2 - dy
         << " L " << std::dec << x2 + dx << " " << std::dec << y2 + dy
         << " Z ";
}

int main(int argc, char* argv[]) {
    const auto ascii = std::views::iota(0x20, 0x7e);
    const auto text = std::string(ascii.begin(), ascii.end());
    
    file.open("font.svg");

    if (!file.is_open()) {
        std::cerr << "error creating SVG file!" << std::endl;
        return 1;
    }

    file << svg_header;
    for (uint8_t i = 1; i < text.size(); i++) {
        auto unicode = text[i];
        file << "<glyph unicode=\"&#x00" << std::hex << static_cast<int>(unicode) << ";\" ";
        if (unicode != '"') {
            file << "glyph-name=\"" << char(unicode) << "\" ";
        }
        file << "d=\"";
        gly_type_render<int>(100, 15, -600, &text[i], 1, draw_line);
        file << "\" horiz-adv-x=\"800\"/>\n";
    }
    file << svg_footer;

    return 0;
}
