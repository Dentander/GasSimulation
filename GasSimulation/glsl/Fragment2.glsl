#version 330

out vec4 fragColor;

uniform vec2 u_resolution;
uniform vec3 u_camera_position;
uniform vec2 u_camera_rotation;
uniform vec3 u_background_color = vec3(0.5, 0.8, 0.9);

const float FOV = 1;
const int MAX_STEPS = 100;
const int MAX_REFLECTIONS = 100;
const float MAX_DIST = 200;
const float EPSILON = 0.1;

mat2 rot(float a) {
    float s = sin(a);
    float c = cos(a);
    return mat2(c, -s, s, c);
}
float vmax(vec2 v) {
    return max(v.x, v.y);
}
float vmax(vec3 v) {
    return max(max(v.x, v.y), v.z);
}
float vmax(vec4 v) {
    return max(max(v.x, v.y), max(v.z, v.w));
}
float fSphere(vec3 p, float r) {
    return length(p) - r;
}
float fPlane(vec3 p, vec3 n, float distanceFromOrigin) {
    return dot(p, n) + distanceFromOrigin;
}
float fBox(vec3 p, vec3 b) {
    vec3 d = abs(p) - b;
    return length(max(d, vec3(0))) + vmax(min(d, vec3(0)));
}


vec4 map(vec3 p) {
    vec4 result = vec4(1000000, 0, 0, 0);
    for(int i = 0; i < 2000; ++i) {
        float sphereDist = fSphere(vec3(p.x, p.y + i, p.z), 1);
        result.x = min(result.x, sphereDist);
    }
    return result;
}

vec4 rayMarch(vec3 ro, vec3 rd) {
    vec4 object, hit;
    for (int i = 0; i < MAX_STEPS; ++i) {
        vec3 p = ro + object.x * rd;
        hit = map(p);
        object.x += hit.x;
        object.yzw = hit.yzw;
        if (abs(hit.x) < EPSILON || object.x > MAX_DIST) { break; }
    }
    return object;
}

vec3 render(vec2 uv) {
    vec3 color;
    vec3 ro = u_camera_position;
    vec3 rd = normalize(vec3(uv, FOV));

    // rotate
    rd.zy *= rot(u_camera_rotation.y);
    rd.xz *= rot(-u_camera_rotation.x);

    // raymarching
    vec4 object = rayMarch(ro, rd);

    // hit detection
    if (object.x < MAX_DIST) {
        vec3 p = ro + object.x * rd;
        color = object.yzw;
        color = mix(color, u_background_color, 1 - exp(-0.0004 * object.x * object.x));
    } else {
        color = u_background_color - max(0.6 * rd.y, 0.0);
    }
    return color;
}

vec2 getUV(vec2 offset) {
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