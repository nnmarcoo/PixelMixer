#include "ViewportPanel.h"
#include "FileDropTarget.h"
#include <wx/image.h>

// include OpenGL
#ifdef __WXMAC__
#include "OpenGL/glu.h"
#include "OpenGL/gl.h"
#else
#include <GL/glu.h>
#include <GL/gl.h>
#endif

//todo add checkerboard

BEGIN_EVENT_TABLE(ViewportPanel, wxGLCanvas)
    EVT_PAINT(ViewportPanel::render)
END_EVENT_TABLE()


ViewportPanel::ViewportPanel(wxWindow* parent) : wxGLCanvas(parent, wxID_ANY, nullptr, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE) {
    wxInitAllImageHandlers();
    
    context = new wxGLContext(this);
    SetCurrent(*context);
    glClearColor(0.2109375f, 0.22265625f, 0.2421875f, 1.0);  // Set clear color to #36393e
    
    image.LoadFile("C:/Users/marco/Desktop/ahdjahda.png", wxBITMAP_TYPE_ANY);
}

ViewportPanel::~ViewportPanel() {
    delete context;
}

void ViewportPanel::render(wxPaintEvent& e) {
    if (!IsShown()) return;

    // Set up OpenGL context
    SetCurrent(*context);

    // Clear the canvas
    glClear(GL_COLOR_BUFFER_BIT);

    // Render the image
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(-1, -1);
    glTexCoord2f(1, 0); glVertex2f(1, -1);
    glTexCoord2f(1, 1); glVertex2f(1, 1);
    glTexCoord2f(0, 1); glVertex2f(-1, 1);
    glEnd();

    // Swap buffers
    SwapBuffers();
}
