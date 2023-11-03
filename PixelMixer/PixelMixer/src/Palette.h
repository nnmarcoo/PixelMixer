#pragma once
#include <wx/colour.h>

// https://www.color-hex.com/color-palette/28549
namespace Palette {
    const wxColour text("#bfbfbf");         // off-white
    const wxColour border("#1e1f22");       // edges      (darkest)
    const wxColour bhover("#2e436e");       // blue hover
    const wxColour ghover("#424549");       // gray hover (brightest)
    const wxColour red("#ff3333");          // red
    const wxColour viewport("#313338");     // lightest color
    const wxColour config("#2b2d31");
    const wxColour linebreak("#646464");
    const wxColour clickable("#285fd0");
}
namespace Font {
    const wxFont read(13, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    const wxFont click(13, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    const wxFont dropdown(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_THIN);
}