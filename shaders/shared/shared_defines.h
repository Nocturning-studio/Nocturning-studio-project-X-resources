////////////////////////////////////////////////////////////////////////////
//	Created		: 09.03.2024
//	Author		: Deathman
//  Nocturning studio for NS Project X
////////////////////////////////////////////////////////////////////////////
#define def_distort float(0.055f) // we get -0.5 .. 0.5 range, this is -512 .. 512 for 1024, so scale it
#define def_aref float(100.0f/255.0f)
////////////////////////////////////////////////////////////////////////////
// ex. Cloudconfig.h
// note: timers has resolution (sec), where x=1, y=10, z=1/10,
#define CLOUD_TILE0 (0.7f)
#define CLOUD_SPEED0 (0.033f)
#define CLOUD_TILE1 (2.5f)
#define CLOUD_SPEED1 (0.033f)
#define CLOUD_FADE (0.5)
////////////////////////////////////////////////////////////////////////////
// ex. Waterconfig.h
#define W_POSITION_SHIFT_HEIGHT (1.f / 60.f) //(1.f/100.f)	//amplitude /50 - small, /10 - large
#define W_POSITION_SHIFT_SPEED (25.f)        //(25.0f)
#define W_DISTORT_BASE_TILE_0 (1.0f)         //(1.0f)
#define W_DISTORT_BASE_TILE_1 (1.1f)         //(1.1f)
#define W_DISTORT_AMP_0 (+0.15f)             //(+0.15f)
#define W_DISTORT_AMP_1 (+0.55f)             //(-0.30f)
#define W_DISTORT_POWER (1.0f)               //(1.0f)
////////////////////////////////////////////////////////////////////////////
// ex. Wmark.h
#define NORMAL_SHIFT 0.007f
#define MIN_SHIFT 0.003f
#define MAX_SHIFT 0.011f
#define RANGE 100.f
////////////////////////////////////////////////////////////////////////////
