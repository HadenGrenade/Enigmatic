//v1.1
#define V2 ImVec2
void FX(ImDrawList* d, V2 a, V2, V2 sz, ImVec4, float t)
{
    static struct { int y, h, c; float t, s; } m[40] = { 0 };
    static int S = 0x1234;
    static float t0 = t;
    float ZI = t * .07f, Z = ZI + 1.f;
    for (int x = 0; x < 40; x++)
    {
        auto& M = m[x];
        int i = x >= 15 && x < 25;
        if (M.s == 0.f || M.y > 16)
        {
            M.h = (t < 7.f || i) * ((int)(2 + t * .5f) + S % (int)(6 + (t * 0.3f)));
            M.y = (M.s == 0.f) * -(S % 15) - M.h;
            M.c += S;
            M.s = (5 + (S % 14)) * (.01f - t * .001f);
            if (t > 5.f && i)
            {
                M.c = (340003872375972UL >> (x * 5 - 75)) & 31;
                M.h = i ? (x != 19) : 0;
            }
        }
        if ((M.t -= t - t0) < 0.f)
        {
            if (t < 6.f || !i || M.y != 6)
                M.y++;
            M.t += M.s;
        }
        char c = 64 | M.c % 42;
        for (int j = 0; j < M.h; j++, c = 64 | (c ^ M.c + M.h ^ j) % 42)
            for (int f = (j + 1 == M.h) ? 13 : 4 + (M.c & 1); f--;)
                d->AddText(0, 13 * (i ? Z : -Z), V2(a.x - (sz.x * .5f * ZI) + x * 8 * Z + sin(j + t * f), a.y - (sz.y * .5f * ZI) + (M.y + j) * 13 * Z + cos(x * f - t)), 0x3c68bb5b, &c, &c + 1);
        S |= ((S & 1) ^ ((S & 8) >> 2)) << 16;
        S >>= 1;
    }
    t0 = t;
}