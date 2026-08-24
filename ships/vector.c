//#include"vector.h"


#ifndef VECTOR_TYPES

#define VECTOR_TYPES
//V2i
typedef struct{
	int x;
	int	y;
}V2i;

V2i CV2i(int x,int y);
V2i AddV2i(V2i V1,V2i V2);
V2i SubV2i(V2i V1,V2i V2);
V2i MultV2i(V2i V1,V2i V2);
V2i FlipV2i(V2i V1);
V2i ScaleV2i(V2i V1,float scale);

//V2f
typedef struct{
	float x;
	float y;
}V2f;

V2f CV2f(float x,float y);
V2f AddV2f(V2f V1,V2f V2);
V2f SubV2f(V2f V1,V2f V2);
V2f MultV2f(V2f V1,V2f V2);
V2f FlipV2f(V2f V1);
V2f ScaleV2f(V2f V1,float scale);


typedef struct{
	float x;
	float y;
	float z;
}V3f;

V3f CV3f(float x,float y,float z);
V3f AddV3f(V3f V1,V3f V2);
V3f SubV3f(V3f V1,V3f V2);
V3f MultV3f(V3f V1,V3f V2);
V3f ScaleV3f(V3f V1,float scale);


V2i CV2i(int x,int y){return (V2i){x,y};}
V2i AddV2i(V2i V1,V2i V2){return (V2i){V1.x+V2.x, V1.y+V2.y};}
V2i SubV2i(V2i V1,V2i V2){return (V2i){V1.x-V2.x, V1.y-V2.y};}
V2i MultV2i(V2i V1,V2i V2){return (V2i){V1.x*V2.x, V1.y*V2.y};}
V2i FlipV2i(V2i V1){return (V2i){V1.y,V1.x};}
V2i ScaleV2i(V2i V1,float scale){return (V2i){V1.x*scale,V1.y*scale};}


V2f CV2f(float x,float y){return (V2f){x,y};}
V2f AddV2f(V2f V1,V2f V2){return (V2f){V1.x+V2.x, V1.y+V2.y};}
V2f SubV2f(V2f V1,V2f V2){return (V2f){V1.x-V2.x, V1.y-V2.y};}
V2f MultV2f(V2f V1,V2f V2){return (V2f){V1.x*V2.x, V1.y*V2.y};}
V2f FlipV2f(V2f V1){return (V2f){V1.y,V1.x};}
V2f ScaleV2f(V2f V1,float scale){return (V2f){V1.x*scale,V1.y*scale};}


V3f CV3f(float x,float y,float z){return (V3f){x,y,z};}
V3f AddV3f(V3f V1,V3f V2){return (V3f){V1.x+V2.x, V1.y+V2.y, V1.z+V2.z};}
V3f SubV3f(V3f V1,V3f V2){return (V3f){V1.x-V2.x, V1.y-V2.y, V1.z-V2.z};}
V3f MultV3f(V3f V1,V3f V2){return (V3f){V1.x*V2.x, V1.y*V2.y, V1.z*V2.z};}
V3f ScaleV3f(V3f V1,float scale){return (V3f){V1.x*scale,V1.y*scale,V1.z*scale};}

#endif
