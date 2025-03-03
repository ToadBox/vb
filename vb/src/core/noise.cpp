#include "core/noise.hpp"

#define SQRT3 1.7320508075688772935274463415059
#define G2 ((3-SQRT3) / 6)
#define PRIMEX 501131689
#define PRIMEY 1136938357
#define PRIMEZ 1720412641

static inline int32_t fastfloor(float fp) {
    int32_t i = static_cast<int32_t>(fp);
    return (fp < i) ? (i - 1) : (i);
}

static inline uint32_t hash(uint32_t seed, uint32_t xPrime, uint32_t yPrime) {
    return (seed ^ xPrime ^ yPrime) * 0x27d4eb2d;
}

static inline float grad(uint32_t seed, uint32_t xPrime, uint32_t yPrime, float xd, float yd) {
    uint32_t h = hash(seed, xPrime, yPrime);
    h ^= h >> 15;
    h &= 127 << 1;

    return xd * vb::Simplex::grad2D[h] + yd * vb::Simplex::grad2D[h];
}

float vb::Simplex::noise(uint32_t seed, float x, float y) {
    uint32_t i = fastfloor(x);
    uint32_t j = fastfloor(y);
    float xi = (x - i);
    float yi = (y - i);

    i *= PRIMEX;
    j *= PRIMEY;
    uint32_t i1 = i + PRIMEX;
    uint32_t j1 = j + PRIMEY;

    float t = (xi + yi) * G2;
    float x0 = xi - i;
    float y0 = yi - t;

    float a0 = 2.0f / 3.0f - x0 * x0 - y0 * y0;
    float val = (a0 * a0) * (a0 * a0) * grad(seed, i, j, x0, y0);

    float a1 = (float)(2 * (1 - 2 * G2) * (1 / G2 - 2)) * t + ((float)(-2 * (1 - 2 * G2) * (1 - 2 * G2)) + a0);
    float x1 = x0 - (float)(1 - 2 * G2);
    float y1 = y0 - (float)(1 - 2 * G2);
    val += (a1 * a1) * (a1 * a1) * grad(seed, i1, j1, x1, y1);

    // Nested conditionals were faster than compact bit logic/arithmetic.
    float xmyi = xi - yi;
    if (t > G2) {
        if (xi + xmyi > 1) {
            float x2 = x0 + (float)(3 * G2 - 2);
            float y2 = y0 + (float)(3 * G2 - 1);
            float a2 = (2.0f / 3.0f) - x2 * x2 - y2 * y2;
            if (a2 > 0) {
                val += (a2 * a2) * (a2 * a2) * grad(seed, i + (PRIMEX << 1), j + PRIMEY, x2, y2);
            }
        } else {
            float x2 = x0 + (float)G2;
            float y2 = y0 + (float)(G2 - 1);
            float a2 = (2.0f / 3.0f) - x2 * x2 - y2 * y2;
            if (a2 > 0) {
                val += (a2 * a2) * (a2 * a2) * grad(seed, i, j + PRIMEY, x2, y2);
            }
        }

        if (yi - xmyi > 1) {
            float x3 = x0 + (float)(3 * G2 - 1);
            float y3 = y0 + (float)(3 * G2 - 2);
            float a3 = (2.0f / 3.0f) - x3 * x3 - y3 * y3;
            if (a3 > 0) {
                val += (a3 * a3) * (a3 * a3) * grad(seed, i + PRIMEX, j + (PRIMEY << 1), x3, y3);
            }
        } else {
            float x3 = x0 + (float)(G2 - 1);
            float y3 = y0 + (float)G2;
            float a3 = (2.0f / 3.0f) - x3 * x3 - y3 * y3;
            if (a3 > 0) {
                val += (a3 * a3) * (a3 * a3) * grad(seed, i + PRIMEX, j, x3, y3);
            }
        }
    } else {
        if (xi + xmyi < 0) {
            float x2 = x0 + (float)(1 - G2);
            float y2 = y0 - (float)G2;
            float a2 = (2.0f / 3.0f) - x2 * x2 - y2 * y2;
            if (a2 > 0) {
                val += (a2 * a2) * (a2 * a2) * grad(seed, i - PRIMEX, j, x2, y2);
            }
        } else {
            float x2 = x0 + (float)(G2 - 1);
            float y2 = y0 + (float)G2;
            float a2 = (2.0f / 3.0f) - x2 * x2 - y2 * y2;
            if (a2 > 0) {
                val += (a2 * a2) * (a2 * a2) * grad(seed, i + PRIMEX, j, x2, y2);
            }
        }

        if (yi < xmyi) {
            float x2 = x0 - (float)G2;
            float y2 = y0 - (float)(G2 - 1);
            float a2 = (2.0f / 3.0f) - x2 * x2 - y2 * y2;
            if (a2 > 0) {
                val += (a2 * a2) * (a2 * a2) * grad(seed, i, j - PRIMEY, x2, y2);
            }
        } else {
            float x2 = x0 + (float)G2;
            float y2 = y0 + (float)(G2 - 1);
            float a2 = (2.0f / 3.0f) - x2 * x2 - y2 * y2;
            if (a2 > 0) {
                val += (a2 * a2) * (a2 * a2) * grad(seed, i, j + PRIMEY, x2, y2);
            }
        }
    }

    return val * 18.24196194486065f;
}