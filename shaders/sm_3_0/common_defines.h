////////////////////////////////////////////////////////////////////////////
//	Created		: 27.07.2022
//	Author		: Deathman
//  Nocturning studio for NS Project X
////////////////////////////////////////////////////////////////////////////
#ifndef COMMON_DEFINES_H_INCLUDED
#define COMMON_DEFINES_H_INCLUDED
////////////////////////////////////////////////////////////////////////////
// Defines
#define def_gloss float(0.0h)  //
#define def_aref float(200.f / 255.f) //
#define def_dbumph float(0.333f)      //
#define def_virtualh float(.05f)      // 5cm
#define def_distort float(0.055f)      // we get -0.5 .. 0.5 range, this is -512 .. 512 for 1024, so scale it
#define def_hdr half(7.5h)             // hight luminance range half(3.h)
#define LUMINANCE_float half3(0.3f, 0.38f, 0.22f)
/////////////////////////////////////////////////////////////////////////////////////////
// ex. Cloudconfig.h
// note: timers has resolution (sec), where x=1, y=10, z=1/10,
#define CLOUD_TILE0 (0.7f)
#define CLOUD_SPEED0 (0.033f)
#define CLOUD_TILE1 (2.5f)
#define CLOUD_SPEED1 (0.033f)
#define CLOUD_FADE (0.5)
/////////////////////////////////////////////////////////////////////////////////////////
// ex. Waterconfig.h
#define W_POSITION_SHIFT_HEIGHT (1.f / 60.f) //(1.f/100.f)	//amplitude /50 - small, /10 - large
#define W_POSITION_SHIFT_SPEED (25.f)        //(25.0h)
#define W_DISTORT_BASE_TILE_0 (1.0h)         //(1.0h)
#define W_DISTORT_BASE_TILE_1 (1.1f)         //(1.1f)
#define W_DISTORT_AMP_0 (+0.15f)             //(+0.15f)
#define W_DISTORT_AMP_1 (+0.55f)             //(-0.30f)
#define W_DISTORT_POWER (1.0h)               //(1.0h)
/////////////////////////////////////////////////////////////////////////////////////////
// ex. Wmark.h
#define NORMAL_SHIFT 0.007f
#define MIN_SHIFT 0.003f
#define MAX_SHIFT 0.011f
#define RANGE 100.f
//////////////////////////////////////////////////////////////////////////////////////////
#define EPSDEPTH 0.0001f
//////////////////////////////////////////////////////////////////////////////////////////
// Волны по траве From x0.0007(rc1) by K.D., RvP, Shadows, Real Wolf, Phoenix
#define GRASS_WAVE_POWER float(2.0) // "яркость", заметность волн
#define GRASS_WAVE_FREQ float(0.7)  // частота появления волн
//////////////////////////////////////////////////////////////////////////////////////////
#define AO_BRIGHTNESS 0.75f
//////////////////////////////////////////////////////////////////////////////////////////
//#define USE_POSITION_WITH_NORMALS_BIAS
//////////////////////////////////////////////////////////////////////////////////////////
#endif // COMMON_DEFINES_H_INCLUDED
//////////////////////////////////////////////////////////////////////////////////////////
