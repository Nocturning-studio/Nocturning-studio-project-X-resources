#include "common.h"

vector4 	main	( vector4 P:	POSITION )	: POSITION
{
	return	mul	(m_WVP,	P	);
}
