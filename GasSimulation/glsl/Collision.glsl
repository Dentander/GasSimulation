#version 330

out vec4 fragColor;

uniform vec3 u_sphere_pos[471];
uniform float u_sphere_radius[471];

uniform int u_box_count;
uniform vec3 u_box_pos[25];
uniform vec3 u_box_size[25];

bool insideBox3D(vec3 v, vec3 bottomLeft, vec3 topRight) {
    if (v.x <= topRight.x && v.y <= topRight.y && v.z <= topRight.z &&
        v.x >= bottomLeft.x && v.y >= bottomLeft.y && v.z >= bottomLeft.z) { return true; }
    return false;
}

vec4 boxCollision() {
    int index = int(gl_FragCoord.x);
    if (index < 471) {
        for (int i = 0; i < u_box_count; ++i) {
            vec3 deltaSize = u_box_size[i] / 2 + vec3(u_sphere_radius[index]);
            vec3 bottomLeft = u_box_pos[i] - deltaSize;
            vec3 topRight = u_box_pos[i] + deltaSize;

            if (insideBox3D(u_sphere_pos[index], bottomLeft, topRight)) {
                float color = float(i) / 255;
                return vec4(sqrt(color));
            }
        }
        return vec4(0.1, 0.2, 0.3, 0.4);
    }
    return vec4(0.1, 0.2, 0.3, 0.4);
}

void main() {
    if (gl_FragCoord.y == 1) {

    } else {
        vec4 color = boxCollision();
        fragColor = color;
    }
}
