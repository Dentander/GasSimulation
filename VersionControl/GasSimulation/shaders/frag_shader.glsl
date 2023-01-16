#version 120

uniform vec4 viewport;

uniform vec2 u_resolution;
uniform vec3 u_camera_position;
uniform vec2 u_camera_rotation;

varying float sphereRadius;
varying vec3 spherePos;

const float FOV = 0.58;
const int MAX_STEPS = 100;
const int MAX_REFLECTIONS = 100;
const float MAX_DIST = 1000;
const float EPSILON = 0.001;

mat2 rot(const float a) {
    float s = sin(a);
    float c = cos(a);
    return mat2(c, -s, s, c);
}

float raySphereIntersect(vec3 r0, vec3 rd, vec3 s0, float sr) {
    float a = dot(rd, rd);
    vec3 s0_r0 = r0 - s0;
    float b = 2.0 * dot(rd, s0_r0);
    float c = dot(s0_r0, s0_r0) - (sr * sr);
    float d = b * b - 4.0 * a * c;
    if (d < 0.0) {
        return -1.0;
    }
    return (-b - sqrt(d)) / (2.0 * a);
}

float boxIntersection(in vec3 ro, in vec3 rd, in vec3 rad) {
    vec3 oN;
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

vec4 castRay(const vec3 ro, const vec3 rd) {
    vec4 result = vec4(1000000, 1, 1, 1);

    float s = raySphereIntersect(ro, rd, spherePos.zyx, sphereRadius);
    result = unionObjects(result, vec4(s, gl_Color));

    return result;
}

vec3 render(const vec2 uv) {
    vec3 ro = u_camera_position;
    vec3 rd = normalize(vec3(uv, FOV));

    // rotate
    rd.zy *= rot(u_camera_rotation.y);
    rd.xz *= rot(-u_camera_rotation.x);

    // raycasting
    vec4 object = castRay(ro, rd);

    vec3 color = vec3(0);

    // hit detection
    if (object.x < MAX_DIST && object.x > 0) {
        return gl_Color.xyz;
    } else {
        discard;
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
    gl_FragColor = vec4(color, 1.0);
}
