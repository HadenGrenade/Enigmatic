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

using V = ImVec2;
using I = int;
using F = float;
const I t = 10, s = 50;
F p1 = IM_PI, p2 = 2 * p1, lb = 10;
V T[t][s];
V e(10, 0);
I bk = 0xFF000000;
F NA(F f) { return f - p2 * floor((f + p1) / p2); }
void FX2(ImDrawList* d, V a, V z, V sz, ImVec4 m, F u)
{
	V vP = a + V(m.x, m.y - .3f) * sz;
	for (I i = 0; i < t; ++i)
	{
		F aT = i * p2 / t, apv = aT, l = lb;
		V vDT(cos(aT), sin(aT)), vpv = vP;
		for (I j = 0; j < s; ++j)
		{
			V& vC = T[i][j];
			V vab = vC - vpv;
			V vAB = vab * ImInvLength(vab, 1);
			F aS = atan2(vAB.y, vAB.x);
			F cl = ImClamp(NA(aS - apv), -p1, p1);
			aS = apv + ImClamp(cl - cl * .6f, -p1, p1);
			V vD(cos(aS), sin(aS));
			vC = vpv + vD * l;
			d->AddLine(vpv, vC, j & 1 ? 0xFF800000 : 0xFF000040, 2 * l);
			d->AddLine(vpv, vC, j & 1 ? 0xFFE08020 : 0xFF4020E0, 2 * l / 3);
			apv = aS;
			vpv = vC;
			l *= i & 1 ? .9f : .85f;
		}
	}
	F cs = pow(abs(sin(u)), .3);
	d->PushClipRect(vP - V(99, 10) * cs, vP + V(99, 10) * cs, 1);
	d->AddCircleFilled(vP + e, 9, -1);
	d->AddCircleFilled(vP - e, 9, -1);
	V o = e * .5 * sin(u * .8);
	d->AddCircleFilled(vP + e + o, 4, bk);
	d->AddCircleFilled(vP - e + o, 4, bk);
	d->PopClipRect();
}