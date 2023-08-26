#include <GL/glew.h>
#include "ViewportPanel.h"
#include "FileDropTarget.h"
#include <wx/image.h>

//todo add checkerboard

BEGIN_EVENT_TABLE(ViewportPanel, wxGLCanvas)
    EVT_PAINT(ViewportPanel::render)
    EVT_SIZE(ViewportPanel::OnSize)
END_EVENT_TABLE()

ViewportPanel::ViewportPanel(wxWindow* parent) : wxGLCanvas(parent, wxID_ANY, nullptr, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE) {
    wxInitAllImageHandlers();
    
    context = new wxGLContext(this);

    SetCurrent(*context);
    glClearColor(0.2109375f, 0.22265625f, 0.2421875f, 1.0);  // Set clear color to #36393e
    
    image.LoadFile("C:/Users/marco/Desktop/ahdjahda.png", wxBITMAP_TYPE_ANY); // test
    
    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;
    

    std::cout << glGetString(GL_VERSION) << std::endl;

    constexpr float positions[6] = {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };
    
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

ViewportPanel::~ViewportPanel() {
    delete context;
}

void ViewportPanel::render(wxPaintEvent& e) {
    if (!IsShown()) return;
    //SetCurrent(*context); // unnecessary because there is only 1 context?

    // Clear the canvas
    glClear(GL_COLOR_BUFFER_BIT);
    
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
    SwapBuffers();
}

void ViewportPanel::OnSize(wxSizeEvent& e) {
    if (initialized) return;
    
    wxSize viewport = GetSize();
    glViewport(0, 0, viewport.GetWidth(), viewport.GetHeight());
    if (viewport.GetWidth() > 10) initialized = true;
}
