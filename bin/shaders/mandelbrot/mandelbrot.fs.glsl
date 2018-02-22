#version 410 core
precision highp float;
#define MAX_ITERATION 300
#define COLOR_STEP 301
#define COLOR_FIRST vec3(74, 205, 250) / 256.0f
#define COLOR_SECOND vec3(24, 64, 127) / 256.0f

struct Complex {
    double r, i;
};

in vec2 FragPos;
out vec4 FragColor;
uniform dmat4 invMat;

Complex multiply(Complex c1, Complex c2) {
    Complex result;
    result.r = c1.r * c2.r - c1.i * c2.i;
    result.i = c1.r * c2.i + c1.i * c2.r;
    return result;
}

Complex add(Complex c1, Complex c2) {
    Complex result;
    result.r = c1.r + c2.r;
    result.i = c1.i + c2.i;
    return result;
}

double modulusSquare(Complex c) {
    return c.r * c.r + c.i * c.i;
}

int mand(Complex c) {
    Complex z;
    z.r = z.i = 0;
    for (int t = 0; t < MAX_ITERATION; t++) {
        if (modulusSquare(z) > 4) return t;
        z = add(multiply(z, z), c);
    }
    return MAX_ITERATION;
}

void main() {
    dvec4 original = invMat * dvec4(FragPos, 0, 1);
    Complex c;
    c.r = original.x;
    c.i = original.y;
    int time = mand(c) % COLOR_STEP;
    float interp = float(time) / COLOR_STEP;
    vec3 color = (1-interp) * COLOR_FIRST + interp * COLOR_SECOND;
    FragColor = vec4(color, 1);
}
