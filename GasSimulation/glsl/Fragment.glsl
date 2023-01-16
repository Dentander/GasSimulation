#version 330

out vec4 fragColor;

uniform vec2 u_resolution;
uniform vec3 u_camera_position;
uniform vec2 u_camera_rotation;
uniform vec3 u_background_color = vec3(0.5, 0.8, 0.9);

uniform int u_box_count;
uniform vec3 u_box_pos[200];
uniform vec3 u_box_size[200];
uniform float u_box_color[200];

const float FOV = 0.5775;
const int MAX_STEPS = 100;
const int MAX_REFLECTIONS = 100;
const float MAX_DIST = 5000;
const float EPSILON = 0.01;

mat2 rot(const float a) {
    float s = sin(a);
    float c = cos(a);
    return mat2(c, -s, s, c);
}


float boxIntersection(in vec3 ro, in vec3 rd, in vec3 rad, out vec3 oN) {
    vec3 m = 1.0 / rd;
    vec3 n = m * ro;
    vec3 k = abs(m) * rad;
    vec3 t1 = -n - k;
    vec3 t2 = -n + k;
    float tN = max(max(t1.x, t1.y), t1.z);
    float tF = min(min(t2.x, t2.y), t2.z);
    if (tN > tF || tF < 0.0) return -1.0;
    oN = -sign(rd) * step(t1.yzx, t1.xyz) * step(t1.zxy, t1.xyz);
    return min(tN, tF);
}

vec4 unionObjects(const vec4 obj1, const vec4 obj2) {
    return (obj1.x > obj2.x && obj2.x > 0) ? obj2 : obj1;
}

vec4 getColor(int index) {
    if (index == 1) {
        return vec4(1);
    }
    else if (index == 2) {
        return vec4(1, 0, 0, 1);
    }
    else if (index == 3) {
        return vec4(0, 1, 0, 1);
    }
    else if (index == 4) {
        return vec4(0, 0, 1, 1);
    }
    else if (index == 5) {
        return vec4(1, 1, 1, 0.5);
    }
    return vec4(0);
}

vec3 getLight(vec3 p, vec3 rd, vec3 color, vec3 N) {
    vec3 lightPos = vec3(2, 4, -3) * 1000;
    vec3 L = normalize(lightPos - p);

    vec3 diffuse = color * clamp(dot(L, N), 0.07, 1.0);
    return diffuse;
}

vec4 castRay(const vec3 ro, const vec3 rd, const vec3 backgroundColor) {
    float distTransparent = 1000000;
    vec4 result = vec4(1000000, backgroundColor);
    vec3 finalNormal, transparentNormal;

    for (int i = 0; i < u_box_count; ++i) {
        // ======== DIST & NORMAL =======
        vec3 boxNormal;
        float boxDist = boxIntersection(ro - u_box_pos[i].xyz, rd, u_box_size[i], boxNormal);
        if (result.x > boxDist && boxDist >= 0) {
            // ========= NOT TRANSPERENT ========
            if (u_box_color[i] != 5) {
                result = vec4(boxDist, getColor(int(u_box_color[i])));
                finalNormal = boxNormal;
            }
            // ========= TRANSPERENT ========
            else if (boxDist < distTransparent) {
                distTransparent = boxDist;
                transparentNormal = boxNormal;
            }
        }
    }
    if (result.x < MAX_DIST) {
        result.yzw = getLight(ro + rd * result.x, rd, result.yzw, finalNormal);
    }
    if (result.x > distTransparent) {
        result.yzw *= getLight(ro + rd * distTransparent, rd, vec3(0.7), transparentNormal);
        result.x = distTransparent;
    }
    return result;
}

vec3 render(const vec2 uv) {
    vec3 ro = u_camera_position;
    vec3 rd = normalize(vec3(uv, FOV));

    // rotate
    rd.zy *= rot(u_camera_rotation.y);
    rd.xz *= rot(-u_camera_rotation.x);

    //vec3 color = u_background_color - max(0.6 * rd.y, 0.0);
    vec3 color = vec3(1);

    // raycasting
    vec4 object = castRay(ro, rd, color);

    // hit detection
    if (object.x < MAX_DIST && object.x >= -1) {
        color = object.yzw;
        return color;
    }
    return color;
}

vec2 getUV(const vec2 offset) {
    vec2 uv = (2.0 * (gl_FragCoord.xy + offset) - u_resolution.xy) / u_resolution.y;
    return uv;
}

vec3 AAx4() {
    vec4 e = vec4(0);
    vec3 colorAAx4 = render(getUV(e.xz));
    return colorAAx4;
}

void main() {
    vec3 color = AAx4();
    // gamma correctiom
    color = pow(color, vec3(0.4545));
    fragColor = vec4(color, 1.0);
}