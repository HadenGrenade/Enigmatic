void FX(ImDrawList* d, ImVec2 a, ImVec2 b, ImVec2 sz, ImVec4 m, float) {
#define el(p,rx,ry,c){for(int s=0;s<36;s++){d->PathLineTo(ImVec2(p.x+sinf((IM_PI/18)*s)*rx,p.y+cosf((IM_PI/18)*s)*ry));}d->PathFillConvex(c);}
#define v2 ImVec2
    v2 c((a + b) * 0.5f);
    v2 mp(a.x + m.x * sz.x, a.y + m.y * sz.y);
    //d->AddRectFilled(a, b, 0xFF665544);
    v2 eye[2] = { {c.x - 50,c.y},{c.x + 50,c.y} };
    for (int i = 0; i < 2; i++) {
        el(eye[i], 40, 70, 0xFF000000);
        el(eye[i], 35, 65, 0xFFFFFFFF);
        v2 v(mp - eye[i]);
        float l = sqrtf(v.x * v.x + v.y * v.y);
        v2 n(v / l);
        if (l > 20)l = 20;
        v2 p(eye[i] + n * l);
        el(p, 10, 10, 0xFF000000);
    }
}





#define V2 ImVec2
#define F float
F k;
int i{};
F r() { return F(rand() / 32768.f) * 2.f - 1.f; };
struct P {
    F x, y, z, a, b, c; void A() {
        x += a * k; y += b * k; z += c * k;
        F ng{ 0.008f }; z -= 5.f; F xp = x * cosf(ng) - z * sinf(ng); F zp = x * sinf(ng) + z * cosf(ng);
        x = xp; z = zp + 5.f; a -= x * k + r() * k; b -= y * k + r() * k; c -= (z - 5.0f) * k + r() * k;
    }
};
P p[64];
void FX2(ImDrawList* d, V2 o, V2 b, V2 sz, ImVec4, F t)
{
    int j{};
    if (!i) { i = 1; for (P& a : p) { a = { r(),r(),r() + 5.f,r(),r(),r() }; } }
    for (P& a : p) {
        if (a.z < 0.001) continue;
        V2 s((a.x / a.z) * sz.x * 2.f + sz.x * 0.5f + o.x, (a.y / a.z) * sz.y * 2.f + sz.y * 0.5f + o.y);
        int x = (j++) % 16;
        k = cosf((j / 64.f) * 3.14592f) * 0.002f + 0.002f;
        F dist = fabsf(a.z - 5.f) / 2.5f, sc = (10.f + dist * 100.f) / a.z;
        int tr = int(ImMin(dist * 128.f, 128.f) + 127) << 24;
        ImColor col = ImColor::HSV(k * 9.f + 0.06f, 0.8f, 1.f, 1.f - sqrtf(dist));
        d->AddCircleFilled(s, sc, col, 12); a.A();
    }
}



#define min(x,y) ((x)<(y)?x:y)
#define wh(a) ImColor(1.f,1.f,1.f,a)
void FX3(ImDrawList* d, ImVec2 a, ImVec2 b, ImVec2 sz, float t)
{
    static float fl;
    if ((rand() % 500) == 0) fl = t;
    if ((t - fl) > 0)
    {
        auto ft = 0.25f;
        //d->AddRectFilled(a, b, wh((ft - (t - fl)) / ft));
    }

    for (int i = 0; i < 2000; ++i) {
        unsigned h = ImGui::GetID(d + i + int(t / 4));
        auto f = fmodf(t + fmodf(h / 777.f, 99), 99);
        auto tx = h % (int)sz.x;
        auto ty = h % (int)sz.y;
        if (f < 1) {
            auto py = ty - 1000 * (1 - f);
            d->AddLine({ a.x + tx, a.y + py }, { a.x + tx, a.y + min(py + 10,ty) }, (ImU32)-1);
        }
        else if (f < 1.2f)
            d->AddCircle({ a.x + tx, a.y + ty }, (f - 1) * 10 + h % 5, wh(1 - (f - 1) * 5.f));
    }
}