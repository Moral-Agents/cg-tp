#ifndef figures_h
#define figures_h

struct Rgb {
	f32 r, g, b;
};

class Object {
protected:
	f32* vertices;
	u32* indices;
	u32  nv;
	u32  ni;

public:
	Object(u32 n, u32 m) : vertices(new f32[n]), indices(new u32[m]), nv(n), ni(m) {}

	f32* getVertices() {
		return vertices;
	}
	u32* getIndices() {
		return indices;
	}
	u32 getVSize() {
		return nv;
	}
	u32 getISize() {
		return ni;
	}
};

class Cube : public Object {
	f32  width;
	f32  height;
	f32  depth;

public:
	Cube(f32 width=1.0f, f32 height=1.0f, f32 depth=1.0f, Rgb a={1.0f, 1.0f, 1.0f}, Rgb b={1.0f, 0.0f, 0.0f}, Rgb c={1.0f, 1.0f, 0.0f}, Rgb d={0.0f, 1.0f, 1.0f}, Rgb e={1.0f, 0.0f, 1.0f}, Rgb f={0.0f, 1.0f, 0.0f}) : Object(16*8, 6*6), width(width), height(height), depth(depth) {
        f32 wm =  width / 2.0f;
        f32 hm = height / 2.0f;
        f32 dm =  depth / 2.0f;
        f32 temp[] = {
            //Posiciones        //Colores           //Texturas
            -wm,  hm,  dm,      1.0f, 0.0, 0.0,     0.0f, 1.0f,  //0
             wm,  hm,  dm,      1.0f, 0.0, 0.0,     1.0f, 1.0f,  //1
            -wm, -hm,  dm,      1.0f, 0.0, 0.0,     0.0f, 0.0f,  //2
             wm, -hm,  dm,      1.0f, 0.0, 0.0,     1.0f, 0.0f,  //3
            
            -wm,  hm, -dm,      1.0f, 0.0, 0.0,     1.0f, 1.0f,  //4
             wm,  hm, -dm,      1.0f, 0.0, 0.0,     0.0f, 1.0f,  //5
            -wm, -hm, -dm,      1.0f, 0.0, 0.0,     1.0f, 0.0f,  //6
             wm, -hm, -dm,      1.0f, 0.0, 0.0,     0.0f, 0.0f,  //7
            -wm,  hm,  dm,      1.0f, 0.0, 0.0,     0.0f, 0.0f,  //8
             wm,  hm,  dm,      1.0f, 0.0, 0.0,     1.0f, 0.0f,  //9
            -wm, -hm,  dm,      1.0f, 0.0, 0.0,     0.0f, 0.0f,  //10
             wm, -hm,  dm,      1.0f, 0.0, 0.0,     1.0f, 0.0f,  //11
            -wm,  hm, -dm,      1.0f, 0.0, 0.0,     0.0f, 1.0f,  //12
             wm,  hm, -dm,      1.0f, 0.0, 0.0,     1.0f, 1.0f,  //13
            -wm, -hm, -dm,      1.0f, 0.0, 0.0,     0.0f, 1.0f,  //14
             wm, -hm, -dm,      1.0f, 0.0, 0.0,     1.0f, 1.0f   //15
        };
        u32 temp2[] = {
            0, 1, 2,    1, 2, 3,
            8, 9, 12,   9, 12, 13,
            1, 5, 3,    3, 5, 7,
            11, 14, 15, 10, 11, 14,
            0, 4, 6,    0, 2, 6,
            4, 5, 6,    5, 6, 7
        };
        for (u32 i = 0; i < nv; ++i) vertices[i] = temp[i];
        for (u32 i = 0; i < ni; ++i) indices[i] = temp2[i];
	}
};

#endif
