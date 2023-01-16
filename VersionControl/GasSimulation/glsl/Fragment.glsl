#version 330

out vec4 fragColor;

uniform vec2 u_resolution;
uniform vec3 u_camera_position;
uniform vec2 u_camera_rotation;
uniform vec3 u_background_color=vec3(0.5, 0.8, 0.9);

uniform int u_sphere_count;
uniform vec3 u_sphere_pos[471];
uniform float u_sphere_radius[471];
uniform vec3 u_sphere_color;

uniform int u_box_count;
uniform vec3 u_box_pos[25];
uniform vec3 u_box_size[25];
uniform float u_box_color[25];

const float FOV = 0.58;
const int MAX_STEPS = 100;
const int MAX_REFLECTIONS = 100;
const float MAX_DIST = 200;
const float EPSILON = 0.01;

mat2 rot(const float a) {
    float s = sin(a);
    float c = cos(a);
    return mat2(c, -s, s, c);
}

float raySphereIntersect(vec3 r0, vec3 rd, vec3 s0, float sr) {
    // - r0: ray origin
    // - rd: normalized ray direction
    // - s0: sphere center
    // - sr: sphere radius
    // - Returns distance from r0 to first intersecion with sphere,
    //   or -1.0 if no intersection.
    float a = dot(rd, rd);
    vec3 s0_r0 = r0 - s0;
    float b = 2.0 * dot(rd, s0_r0);
    float c = dot(s0_r0, s0_r0) - (sr * sr);
    if (b*b - 4.0*a*c < 0.0) {
        return -1.0;
    }
    return (-b - sqrt((b*b) - 4.0*a*c)) / (2.0*a);
}

float boxIntersection(in vec3 ro, in vec3 rd, in vec3 rad)  {
    vec3 oN;
    vec3 m = 1.0 / rd;
    vec3 n = m * ro;
    vec3 k = abs(m) * rad;
    vec3 t1 = -n - k;
    vec3 t2 = -n + k;
    float tN = max(max(t1.x, t1.y), t1.z);
    float tF = min(min(t2.x, t2.y), t2.z);
    if(tN > tF || tF < 0.0) return -1.0;
    oN = -sign(rd) * step(t1.yzx, t1.xyz) * step(t1.zxy, t1.xyz);
    return min(tN, tF);
}

vec4 unionObjects(const vec4 obj1, const vec4 obj2) {
    return (obj1.x > obj2.x && obj2.x > 0) ? obj2 : obj1;
}

vec4 getColor(int index) {
    if (index == 1) {
        return vec4(1);
    } else if (index == 2) {
        return vec4(1, 0, 0, 1);
    } else if (index == 3) {
        return vec4(0, 1, 0, 1);
    } else if (index == 4) {
        return vec4(0, 0, 1, 1);
    } else if (index == 5) {
        return vec4(1, 1, 1, 0.5);
    }
    return vec4(0);
}

vec4 castRay(const vec3 ro, const vec3 rd) {
    vec4 result = vec4(10000, 1, 1, 1);
    float d = 100000;
    for (int i = 0; i < u_sphere_count; ++i) {
        float s = raySphereIntersect(ro, rd, u_sphere_pos[i], u_sphere_radius[i]);
        result = unionObjects(result, vec4(s, u_sphere_color));
    }
    for (int i = 0; i < u_box_count; ++i) {
        float b = boxIntersection(ro - u_box_pos[i], rd, u_box_size[i]);
        if (result.x > b && b != -1) {
            if (u_box_color[i] != 5) {
                result = vec4(b, getColor(int(u_box_color[i])).xyz);
            } else {
                d = b;
            }
        }
    }
    if (result.x > d) { result *= 0.5; }
    return result;
}

vec3 render(const vec2 uv) {
    vec3 ro = u_camera_position;
    vec3 rd = normalize(vec3(uv, FOV));

    // rotate
    rd.zy *= rot(u_camera_rotation.y);
    rd.xz *= rot(-u_camera_rotation.x);

    vec3 color = u_background_color - max(0.6 * rd.y, 0.0);

    // raycasting
    vec4 object = castRay(ro, rd);

    // hit detection
    if (object.x < MAX_DIST && object.x > 0) {
        color = object.yzw;
        color = mix(color, u_background_color, 1 - exp(-0.0001 * object.x * object.x));
        return color;
    } if (object.yzw == vec3(0.5)) {
        return color * object.yzw;
    }
    return color;
}

vec2 getUV(const vec2 offset) {
    vec2 uv =  (2.0 * (gl_FragCoord.xy + offset) - u_resolution.xy) / u_resolution.y;
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