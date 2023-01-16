#include "Visualization.h"


float rand_minus_one_one_() {
    return (float)rand() / (float)RAND_MAX * (rand() > RAND_MAX / 2 ? 1 : -1);
}

float rand_zero_one_() {
    return (float)rand() / (float)RAND_MAX;
}


Visualization::Visualization(vec2f size) {
    init(size);
}

void Visualization::changeSize(int w, int h) {
    float ratio = (1.0 * w) / (!h ? 1 : h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(120, ratio, 0.01, 5000);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);

    SCREEN_WIDTH = w, SCREEN_HEIGHT = h;
}

void Visualization::changeSize(vec2f size) {
    changeSize(int(size.x), int(size.y));
}


bool Visualization::readAndCompileShader(const char* filename, GLuint& hdlr, GLenum shaderType) {
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


void Visualization::setShaders(GLuint& prog_hdlr, const char* vsfile, const char* fsfile) {
    GLuint vert_hdlr, frag_hdlr;
    readAndCompileShader(vsfile, vert_hdlr, GL_VERTEX_SHADER);
    readAndCompileShader(fsfile, frag_hdlr, GL_FRAGMENT_SHADER);

    prog_hdlr = glCreateProgram();
    glAttachShader(prog_hdlr, frag_hdlr);
    glAttachShader(prog_hdlr, vert_hdlr);

    glLinkProgram(prog_hdlr);
    std::cerr << "info log for the linked program" << std::endl;
}


void Visualization::init(vec2f size) {
    for (int i = 0; i < NATOMS; i++) {
        std::vector<float> tmp;
        for (int c = 0; c < 3; c++) {
            tmp.push_back(rand_minus_one_one_() / 2);      // xyz
        }
        tmp.push_back(rand_zero_one_() / 8.0); // radius
        for (int c = 0; c < 3; c++) {
            tmp.push_back(rand_zero_one_()); // rgb
        }
        atoms.push_back(tmp);
    }

    glDepthMask(GL_TRUE);
    glEnable(GL_COLOR_MATERIAL);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDepthRange(0, 1);
    glDepthFunc(GL_LESS);
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
    location_player_pos = glGetUniformLocation(prog_hdlr, "u_camera_position");
    location_player_rot = glGetUniformLocation(prog_hdlr, "u_camera_rotation");
    location_resolution = glGetUniformLocation(prog_hdlr, "u_resolution");

    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

    changeSize(size);
}

void Visualization::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    cur_camera[0] = cos(angle) * camera[0] + sin(angle) * camera[2];
    cur_camera[1] = camera[1];
    cur_camera[2] = cos(angle) * camera[2] - sin(angle) * camera[0];

    glUseProgram(prog_hdlr);
    GLfloat viewport[4];
    glGetFloatv(GL_VIEWPORT, viewport);
    glUniform4fv(location_viewport, 1, viewport);
    glBegin(GL_POINTS);
    for (int i = 0; i < NATOMS; i++) {
        glColor3f(atoms[i][4], atoms[i][5], atoms[i][6]);
        glVertexAttrib1f(location_attribute_0, atoms[i][3]);
        glVertex3f(atoms[i][0], atoms[i][1], atoms[i][2]);
    }
    glEnd();
    glUseProgram(0);
    glClearColor(.0f, 1.0f, .0f, 1.0f);
}

void Visualization::draw(Camera& user, Map& map) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glClearColor(1, 1, 1, 1);

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
    
    glUniform3f(location_player_pos, cameraPos.x, cameraPos.y, cameraPos.z);
    glUniform2f(location_player_rot, user.getRotation().x, user.getRotation().y );
    glUniform2f(location_resolution, SCREEN_WIDTH, SCREEN_HEIGHT);

    std::cout << glGetError() << std::endl;

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
