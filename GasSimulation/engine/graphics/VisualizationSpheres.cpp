#include "VisualizationSpheres.h"


VisualizationSpheres::VisualizationSpheres(vec2f size) {
    init(size);
}

void VisualizationSpheres::changeSize(int w, int h) {
    float ratio = (1.0 * w) / (!h ? 1 : h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(120, ratio, 0.01, 5000);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);

    SCREEN_WIDTH = w, SCREEN_HEIGHT = h;
}

void VisualizationSpheres::changeSize(vec2f size) {
    changeSize(int(size.x), int(size.y));
}


bool VisualizationSpheres::readAndCompileShader(const char* filename, GLuint& hdlr, GLenum shaderType) {
    std::ifstream is(filename, std::ios::in | std::ios::binary | std::ios::ate);
    if (!is.is_open()) {
        std::cerr << "Unable to open file " << filename << std::endl;
        return false;
    }
    long size = is.tellg();
    char* buffer = new char[size + 1];
    is.seekg(0, std::ios::beg);
    is.read(buffer, size);
    is.close();
    buffer[size] = 0;

    hdlr = glCreateShader(shaderType);
    glShaderSource(hdlr, 1, (const GLchar**)&buffer, NULL);
    glCompileShader(hdlr);
    delete[] buffer;
    return true;
}


void VisualizationSpheres::setShaders(GLuint& prog_hdlr, const char* vsfile, const char* fsfile) {
    GLuint vert_hdlr, frag_hdlr;
    readAndCompileShader(vsfile, vert_hdlr, GL_VERTEX_SHADER);
    readAndCompileShader(fsfile, frag_hdlr, GL_FRAGMENT_SHADER);

    prog_hdlr = glCreateProgram();
    glAttachShader(prog_hdlr, frag_hdlr);
    glAttachShader(prog_hdlr, vert_hdlr);

    glLinkProgram(prog_hdlr);
    std::cerr << "info log for the linked program" << std::endl;
}


void VisualizationSpheres::init(vec2f size) {
    glDepthMask(GL_TRUE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
    glDepthRange(0, 1);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

    glewInit();
    if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader && GL_EXT_geometry_shader4)
        std::cout << "Ready for GLSL - vertex, fragment, and geometry units" << std::endl;
    else {
        std::cout << "No GLSL support" << std::endl;
        exit(1);
    }
    setShaders(prog_hdlr, "shaders/vert_shader.glsl", "shaders/frag_shader.glsl");

    location_attribute_0 = glGetAttribLocation(prog_hdlr, "R");          // radius
    location_viewport = glGetUniformLocation(prog_hdlr, "viewport"); // viewport

    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

    changeSize(size);
}

void VisualizationSpheres::draw(Camera& user, Map& map) {
    glLoadIdentity();


    // ======== CAMERA ========
    vec3f cameraPos = user.getPosition();
    vec3f lookAt = cameraPos;
    lookAt.x += sin(user.getRotation().x) * 10.0 * cos(user.getRotation().y);
    lookAt.y += sin(user.getRotation().y) * 10.0;
    lookAt.z += cos(user.getRotation().x) * 10.0 * cos(user.getRotation().y);
    gluLookAt(cameraPos.z, cameraPos.y, cameraPos.x,
        lookAt.z, lookAt.y, lookAt.x,
        0, 1, 0);

    glUseProgram(prog_hdlr);
    GLfloat viewport[4];
    glGetFloatv(GL_VIEWPORT, viewport);
    glUniform4fv(location_viewport, 1, viewport);

    setUniform("u_camera_position", cameraPos.xyz());
    setUniform("u_camera_rotation", user.getRotation());
    setUniform("u_resolution", vec2f(SCREEN_WIDTH, SCREEN_HEIGHT));


    // ======== BOXES ========
    setUniform("u_box_count", int(map.boxes.size()));

    for (int i = 0; i < map.boxes.size(); ++i) {
        auto& item = map.boxes[i];
        std::string index = std::to_string(i);

        // ======== POS ========
        std::string pos = "u_box_pos[" + index + "]";
        setUniform(pos, item.pos);

        // ======== SIZE ========
        std::string size = "u_box_size[" + index + "]";
        setUniform(size, item.size);

        // ========= COLOR ========
        std::string color = "u_box_color[" + index + "]";
        setUniform(color, item.colorIndex);
    }

    // ======== SPHERES ========
    glBegin(GL_POINTS);
    for (auto& sphere : map.spheres) {
        glColor3f(sphere.color.x, sphere.color.y, sphere.color.z);
        glVertexAttrib1f(location_attribute_0, sphere.radius);
        vec3f p = sphere.pos.zyx();
        glVertex3f(p.x, p.y, p.z);
    }
    glEnd();
    glUseProgram(0);
}

void VisualizationSpheres::setUniform(std::string& name, int item) {
    GLint location = glGetUniformLocation(prog_hdlr, name.c_str());
    glUniform1i(location, item);
}
void VisualizationSpheres::setUniform(std::string& name, float item) {
    GLint location = glGetUniformLocation(prog_hdlr, name.c_str());
    glUniform1f(location, item);
}
void VisualizationSpheres::setUniform(std::string& name, vec2f item) {
    GLint location = glGetUniformLocation(prog_hdlr, name.c_str());
    glUniform2f(location, item.x, item.y);
}
void VisualizationSpheres::setUniform(std::string& name, vec3f item) {
    GLint location = glGetUniformLocation(prog_hdlr, name.c_str());
    glUniform3f(location, item.x, item.y, item.z);
}
void VisualizationSpheres::setUniform(const char name[], int item) {
    GLint location = glGetUniformLocation(prog_hdlr, name);
    glUniform1i(location, item);
}
void VisualizationSpheres::setUniform(const char name[], float item) {
    GLint location = glGetUniformLocation(prog_hdlr, name);
    glUniform1f(location, item);
}
void VisualizationSpheres::setUniform(const char name[], vec2f item) {
    GLint location = glGetUniformLocation(prog_hdlr, name);
    glUniform2f(location, item.x, item.y);
}
void VisualizationSpheres::setUniform(const char name[], vec3f item) {
    GLint location = glGetUniformLocation(prog_hdlr, name);
    glUniform3f(location, item.x, item.y, item.z);
}
