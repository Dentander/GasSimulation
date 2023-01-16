#version 120

uniform vec2 u_resolution;
uniform vec3 u_camera_position;
uniform vec2 u_camera_rotation;

uniform int u_box_count;
uniform vec3 u_box_pos[200];
uniform vec3 u_box_size[200];
uniform float u_box_color[200];

varying float sphereRadius;
varying vec3 spherePos;

const float FOV = 0.5775;
const int MAX_STEPS = 100;
const int MAX_REFLECTIONS = 100;
const float MAX_DIST = 1000;
const float EPSILON = 0.001;



void main() {
    //vec3 color = AAx4();
    // gamma correctiom
    color = vec3(0, 1, 0);
    gl_FragColor = vec4(color, 1.0);
    //gl_FragColor = vec4(u_box_pos[0], 1);
}
