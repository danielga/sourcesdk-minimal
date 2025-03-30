#ifndef YCOCG_H
#define YCOCG_H


namespace YCoCg
{

/*
    RGB_ to CoCg_Y conversion and back.
    Copyright (C) 2007 Id Software, Inc.
    Written by J.M.P. van Waveren
    
    This code is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.
    
    This code is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.
*/
 
//typedef unsigned char   byte;
 
/* 
    RGB <-> YCoCg
    
    Y  = [ 1/4  1/2  1/4] [R]
    Co = [ 1/2    0 -1/2] [G]
    CG = [-1/4  1/2 -1/4] [B]
    
    R  = [   1    1   -1] [Y]
    G  = [   1    0    1] [Co]
    B  = [   1   -1   -1] [Cg]
    
*/
 
byte CLAMP_BYTE( int x ) { return ( (x) < 0 ? (0) : ( (x) > 255 ? 255 : (x) ) ); }
 
#define RGB_TO_YCOCG_Y( r, g, b )   ( ( (    r +   (g<<1) +  b     ) + 2 ) >> 2 )
#define RGB_TO_YCOCG_CO( r, g, b )  ( ( (   (r<<1)        - (b<<1) ) + 2 ) >> 2 )
#define RGB_TO_YCOCG_CG( r, g, b )  ( ( ( -  r +   (g<<1) -  b     ) + 2 ) >> 2 )
 
#define COCG_TO_R( co, cg )         ( co - cg )
#define COCG_TO_G( co, cg )         ( cg )
#define COCG_TO_B( co, cg )         ( - co - cg )
 
// overwrite src RGBA (32bpp) with CoCg_Y (32bpp)
void ConvertRGBAToCoCg_Y( byte *src, ImageFormat srcImageFormat, int width, int height ) 
{
	int offR, offG, offB, offA;

	switch ( srcImageFormat )
	{
	case IMAGE_FORMAT_RGBA8888:
		offR = 0;
		offG = 1;
		offB = 2;
		offA = 3;
		break;
	case IMAGE_FORMAT_ABGR8888:
		offR = 3;
		offG = 2;
		offB = 1;
		offA = 0;
		break;
	case IMAGE_FORMAT_ARGB8888:
		offR = 1;
		offG = 2;
		offB = 3;
		offA = 0;
		break;
	case IMAGE_FORMAT_BGRA8888:
		offR = 2;
		offG = 1;
		offB = 0;
		offA = 3;
		break;
	default:
		DebuggerBreak();
		break;
	}



    for ( int i = 0; i < width * height; i++ ) 
	{
        int r = src[ i*4 + offR ];
        int g = src[ i*4 + offG ];
        int b = src[ i*4 + offB ];
        int a = src[ i*4 + offA ];
        src[ i*4 + 0 ] = CLAMP_BYTE( RGB_TO_YCOCG_CO( r, g, b ) + 128 );
        src[ i*4 + 1 ] = CLAMP_BYTE( RGB_TO_YCOCG_CG( r, g, b ) + 128 );
        src[ i*4 + 2 ] = a;
        src[ i*4 + 3 ] = CLAMP_BYTE( RGB_TO_YCOCG_Y( r, g, b ) );
    }
}
 
// overwrite src RGB (24bpp) with CoCgY (24bpp)
void ConvertRGBToCoCgY( byte *src, ImageFormat srcImageFormat, int width, int height ) 
{
	int offR, offG, offB;

	switch ( srcImageFormat )
	{
	case IMAGE_FORMAT_RGB888:
		offR = 0;
		offG = 1;
		offB = 2;
		break;
	case IMAGE_FORMAT_BGR888:
		offR = 2;
		offG = 1;
		offB = 0;
		break;
	default:
		DebuggerBreak();
		break;
	}

	for ( int i = 0; i < width * height; i++ ) 
	{
		int r = src[i * 3 + offR];
		int g = src[i * 3 + offG];
		int b = src[i * 3 + offB];
		src[i * 3 + 0] = CLAMP_BYTE( RGB_TO_YCOCG_CO( r, g, b ) + 128 );
		src[i * 3 + 1] = CLAMP_BYTE( RGB_TO_YCOCG_CG( r, g, b ) + 128 );
		src[i * 3 + 2] = CLAMP_BYTE( RGB_TO_YCOCG_Y( r, g, b ) );
	}
}

// 32 bpp CoCg_Y to RGBA
void ConvertCoCg_YToRGBA( byte *src, int width, int height ) 
{
    for ( int i = 0; i < width * height; i++ ) 
	{
        int y  = src[i*4+3];
        int co = src[i*4+0] - 128;
        int cg = src[i*4+1] - 128;
        int a  = src[i*4+2];
        src[i*4+0] = CLAMP_BYTE( y + COCG_TO_R( co, cg ) );
        src[i*4+1] = CLAMP_BYTE( y + COCG_TO_G( co, cg ) );
        src[i*4+2] = CLAMP_BYTE( y + COCG_TO_B( co, cg ) );
        src[i*4+3] = a;
    }
}

/*
    Real-Time YCoCg DXT Compression
    Copyright (C) 2007 Id Software, Inc.
    Written by J.M.P. van Waveren
    
    This code is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.
    
    This code is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.
*/


//typedef unsigned char   byte;
//typedef unsigned short  word;
typedef unsigned int    dword;
 
#define INSET_COLOR_SHIFT       4       // inset color bounding box
#define INSET_ALPHA_SHIFT       5       // inset alpha bounding box
 
#define C565_5_MASK             0xF8    // 0xFF minus last three bits
#define C565_6_MASK             0xFC    // 0xFF minus last two bits
 
#define NVIDIA_G7X_HARDWARE_BUG_FIX     // keep the colors sorted as: max, min
 
byte *globalOutData;
 
word ColorTo565( const byte *color ) 
{
    return ( ( color[ 0 ] >> 3 ) << 11 ) | ( ( color[ 1 ] >> 2 ) << 5 ) | ( color[ 2 ] >> 3 );
}
 
void EmitByte( byte b ) 
{
    globalOutData[0] = b;
    globalOutData += 1;
}
 
void EmitWord( word s ) 
{
    globalOutData[0] = ( s >>  0 ) & 255;
    globalOutData[1] = ( s >>  8 ) & 255;
    globalOutData += 2;
}
 
void EmitDoubleWord( dword i ) 
{
    globalOutData[0] = ( i >>  0 ) & 255;
    globalOutData[1] = ( i >>  8 ) & 255;
    globalOutData[2] = ( i >> 16 ) & 255;
    globalOutData[3] = ( i >> 24 ) & 255;
    globalOutData += 4;
}
 
void ExtractBlock( const byte *inPtr, const int width, const int numInChannels, byte *colorBlock ) 
{
	if ( numInChannels == 3 )
	{
		for ( int j = 0; j < 4; j++ )
		{
			byte *pBlock = &colorBlock[j * 4 * 4];
			const byte *pIn = inPtr;

			for ( int i = 0; i < 4; i++, pBlock += 4, pIn += numInChannels )
			{
				pBlock[0] = pIn[0];
				pBlock[1] = pIn[1];
				pBlock[2] = 0;
				pBlock[3] = pIn[2];
			}

			inPtr += width * numInChannels;
		}
	}
	else
	{
		Assert( numInChannels == 4 );

		for ( int j = 0; j < 4; j++ )
		{
			memcpy( &colorBlock[j * 4 * 4], inPtr, 4 * 4 );
			inPtr += width * 4;
		}
	}
}
 
void GetMinMaxYCoCg( byte *colorBlock, byte *minColor, byte *maxColor ) 
{
    minColor[0] = minColor[1] = minColor[2] = minColor[3] = 255;
    maxColor[0] = maxColor[1] = maxColor[2] = maxColor[3] = 0;
    
    for ( int i = 0; i < 16; i++ ) 
	{
        if ( colorBlock[i*4+0] < minColor[0] )
		{
            minColor[0] = colorBlock[i*4+0];
        }
        if ( colorBlock[i*4+1] < minColor[1] ) 
		{
            minColor[1] = colorBlock[i*4+1];
        }
        if ( colorBlock[i*4+2] < minColor[2] )
		{
            minColor[2] = colorBlock[i*4+2];
        }
        if ( colorBlock[i*4+3] < minColor[3] )
		{
            minColor[3] = colorBlock[i*4+3];
        }
        if ( colorBlock[i*4+0] > maxColor[0] ) 
		{
            maxColor[0] = colorBlock[i*4+0];
        }
        if ( colorBlock[i*4+1] > maxColor[1] ) 
		{
            maxColor[1] = colorBlock[i*4+1];
        }
        if ( colorBlock[i*4+2] > maxColor[2] ) 
		{
            maxColor[2] = colorBlock[i*4+2];
        }
        if ( colorBlock[i*4+3] > maxColor[3] ) 
		{
            maxColor[3] = colorBlock[i*4+3];
        }
    }
}
 
void ScaleYCoCg( byte *colorBlock, byte *minColor, byte *maxColor ) 
{
    int m0 = abs( minColor[0] - 128 );
    int m1 = abs( minColor[1] - 128 );
    int m2 = abs( maxColor[0] - 128 );
    int m3 = abs( maxColor[1] - 128 );
    
    if ( m1 > m0 ) m0 = m1;
    if ( m3 > m2 ) m2 = m3;
    if ( m2 > m0 ) m0 = m2;
    
    const int s0 = 128 / 2 - 1;
    const int s1 = 128 / 4 - 1;
    
    int mask0 = -( m0 <= s0 );
    int mask1 = -( m0 <= s1 );
    int scale = 1 + ( 1 & mask0 ) + ( 2 & mask1 );
    
    minColor[0] = ( minColor[0] - 128 ) * scale + 128;
    minColor[1] = ( minColor[1] - 128 ) * scale + 128;
    minColor[2] = ( scale - 1 ) << 3;
    
    maxColor[0] = ( maxColor[0] - 128 ) * scale + 128;
    maxColor[1] = ( maxColor[1] - 128 ) * scale + 128;
    maxColor[2] = ( scale - 1 ) << 3;
    
    for ( int i = 0; i < 16; i++ ) 
	{
        colorBlock[i*4+0] = ( colorBlock[i*4+0] - 128 ) * scale + 128;
        colorBlock[i*4+1] = ( colorBlock[i*4+1] - 128 ) * scale + 128;
    }
}
 
void InsetYCoCgBBox( byte *minColor, byte *maxColor ) 
{
    int inset[4];
    int mini[4];
    int maxi[4];
    
    inset[0] = ( maxColor[0] - minColor[0] ) - ((1<<(INSET_COLOR_SHIFT-1))-1);
    inset[1] = ( maxColor[1] - minColor[1] ) - ((1<<(INSET_COLOR_SHIFT-1))-1);
    inset[3] = ( maxColor[3] - minColor[3] ) - ((1<<(INSET_ALPHA_SHIFT-1))-1);
    
    mini[0] = ( ( minColor[0] << INSET_COLOR_SHIFT ) + inset[0] ) >> INSET_COLOR_SHIFT;
    mini[1] = ( ( minColor[1] << INSET_COLOR_SHIFT ) + inset[1] ) >> INSET_COLOR_SHIFT;
    mini[3] = ( ( minColor[3] << INSET_ALPHA_SHIFT ) + inset[3] ) >> INSET_ALPHA_SHIFT;
    
    maxi[0] = ( ( maxColor[0] << INSET_COLOR_SHIFT ) - inset[0] ) >> INSET_COLOR_SHIFT;
    maxi[1] = ( ( maxColor[1] << INSET_COLOR_SHIFT ) - inset[1] ) >> INSET_COLOR_SHIFT;
    maxi[3] = ( ( maxColor[3] << INSET_ALPHA_SHIFT ) - inset[3] ) >> INSET_ALPHA_SHIFT;
    
    mini[0] = ( mini[0] >= 0 ) ? mini[0] : 0;
    mini[1] = ( mini[1] >= 0 ) ? mini[1] : 0;
    mini[3] = ( mini[3] >= 0 ) ? mini[3] : 0;
    
    maxi[0] = ( maxi[0] <= 255 ) ? maxi[0] : 255;
    maxi[1] = ( maxi[1] <= 255 ) ? maxi[1] : 255;
    maxi[3] = ( maxi[3] <= 255 ) ? maxi[3] : 255;
    
    minColor[0] = ( mini[0] & C565_5_MASK ) | ( mini[0] >> 5 );
    minColor[1] = ( mini[1] & C565_6_MASK ) | ( mini[1] >> 6 );
    minColor[3] = mini[3];
    
    maxColor[0] = ( maxi[0] & C565_5_MASK ) | ( maxi[0] >> 5 );
    maxColor[1] = ( maxi[1] & C565_6_MASK ) | ( maxi[1] >> 6 );
    maxColor[3] = maxi[3];
}
 
void SelectYCoCgDiagonal( const byte *colorBlock, byte *minColor, byte *maxColor )  
{
    byte mid0 = ( (int) minColor[0] + maxColor[0] + 1 ) >> 1;
    byte mid1 = ( (int) minColor[1] + maxColor[1] + 1 ) >> 1;
    
    byte side = 0;
    for ( int i = 0; i < 16; i++ ) 
	{
        byte b0 = colorBlock[i*4+0] >= mid0;
        byte b1 = colorBlock[i*4+1] >= mid1;
        side += ( b0 ^ b1 );
    }
    
    byte mask = -( side > 8 );
    
#ifdef NVIDIA_7X_HARDWARE_BUG_FIX
    mask &= -( minColor[0] != maxColor[0] );
#endif
    
    byte c0 = minColor[1];
    byte c1 = maxColor[1];
    
    c0 ^= c1 ^= mask &= c0 ^= c1;
    
    minColor[1] = c0;
    maxColor[1] = c1;
}
 
void EmitAlphaIndices( const byte *colorBlock, const byte minAlpha, const byte maxAlpha ) 
{
    
    Assert( maxAlpha >= minAlpha );
    
    const int ALPHA_RANGE = 7;
    
    byte mid, ab1, ab2, ab3, ab4, ab5, ab6, ab7;
    byte indexes[16];
    
    mid = ( maxAlpha - minAlpha ) / ( 2 * ALPHA_RANGE );
    
    ab1 = minAlpha + mid;
    ab2 = ( 6 * maxAlpha + 1 * minAlpha ) / ALPHA_RANGE + mid;
    ab3 = ( 5 * maxAlpha + 2 * minAlpha ) / ALPHA_RANGE + mid;
    ab4 = ( 4 * maxAlpha + 3 * minAlpha ) / ALPHA_RANGE + mid;
    ab5 = ( 3 * maxAlpha + 4 * minAlpha ) / ALPHA_RANGE + mid;
    ab6 = ( 2 * maxAlpha + 5 * minAlpha ) / ALPHA_RANGE + mid;
    ab7 = ( 1 * maxAlpha + 6 * minAlpha ) / ALPHA_RANGE + mid;
    
    for ( int i = 0; i < 16; i++ ) 
	{
        byte a = colorBlock[i*4+3];
        int b1 = ( a <= ab1 );
        int b2 = ( a <= ab2 );
        int b3 = ( a <= ab3 );
        int b4 = ( a <= ab4 );
        int b5 = ( a <= ab5 );
        int b6 = ( a <= ab6 );
        int b7 = ( a <= ab7 );
        int index = ( b1 + b2 + b3 + b4 + b5 + b6 + b7 + 1 ) & 7;
        indexes[i] = index ^ ( 2 > index );
    }
    
    EmitByte( (indexes[ 0] >> 0) | (indexes[ 1] << 3) | (indexes[ 2] << 6) );
    EmitByte( (indexes[ 2] >> 2) | (indexes[ 3] << 1) | (indexes[ 4] << 4) | (indexes[ 5] << 7) );
    EmitByte( (indexes[ 5] >> 1) | (indexes[ 6] << 2) | (indexes[ 7] << 5) );
    
    EmitByte( (indexes[ 8] >> 0) | (indexes[ 9] << 3) | (indexes[10] << 6) );
    EmitByte( (indexes[10] >> 2) | (indexes[11] << 1) | (indexes[12] << 4) | (indexes[13] << 7) );
    EmitByte( (indexes[13] >> 1) | (indexes[14] << 2) | (indexes[15] << 5) );
}
 
void EmitColorIndices( const byte *colorBlock, const byte *minColor, const byte *maxColor ) 
{
    word colors[4][4];
    unsigned int result = 0;
    
    colors[0][0] = ( maxColor[0] & C565_5_MASK ) | ( maxColor[0] >> 5 );
    colors[0][1] = ( maxColor[1] & C565_6_MASK ) | ( maxColor[1] >> 6 );
    colors[0][2] = ( maxColor[2] & C565_5_MASK ) | ( maxColor[2] >> 5 );
    colors[0][3] = 0;
    colors[1][0] = ( minColor[0] & C565_5_MASK ) | ( minColor[0] >> 5 );
    colors[1][1] = ( minColor[1] & C565_6_MASK ) | ( minColor[1] >> 6 );
    colors[1][2] = ( minColor[2] & C565_5_MASK ) | ( minColor[2] >> 5 );
    colors[1][3] = 0;
    colors[2][0] = ( 2 * colors[0][0] + 1 * colors[1][0] ) / 3;
    colors[2][1] = ( 2 * colors[0][1] + 1 * colors[1][1] ) / 3;
    colors[2][2] = ( 2 * colors[0][2] + 1 * colors[1][2] ) / 3;
    colors[2][3] = 0;
    colors[3][0] = ( 1 * colors[0][0] + 2 * colors[1][0] ) / 3;
    colors[3][1] = ( 1 * colors[0][1] + 2 * colors[1][1] ) / 3;
    colors[3][2] = ( 1 * colors[0][2] + 2 * colors[1][2] ) / 3;
    colors[3][3] = 0;
    
    for ( int i = 15; i >= 0; i-- ) 
	{
        int c0 = colorBlock[i*4+0];
        int c1 = colorBlock[i*4+1];
        
        int d0 = abs( colors[0][0] - c0 ) + abs( colors[0][1] - c1 );
        int d1 = abs( colors[1][0] - c0 ) + abs( colors[1][1] - c1 );
        int d2 = abs( colors[2][0] - c0 ) + abs( colors[2][1] - c1 );
        int d3 = abs( colors[3][0] - c0 ) + abs( colors[3][1] - c1 );
        
        bool b0 = d0 > d3;
        bool b1 = d1 > d2;
        bool b2 = d0 > d2;
        bool b3 = d1 > d3;
        bool b4 = d2 > d3;
        
        int x0 = b1 & b2;
        int x1 = b0 & b3;
        int x2 = b0 & b4;
        
        result |= ( x2 | ( ( x0 | x1 ) << 1 ) ) << ( i << 1 );
    }
    
    EmitDoubleWord( result );
}
 
bool CompressYCoCgDXT5( const byte *inBuf, byte *outBuf, int width, int height, int numInChannels, int &outputBytes ) 
{
    byte block[64];
    byte minColor[4];
    byte maxColor[4];
    
    globalOutData = outBuf;
    
    for ( int j = 0; j < height; j += 4, inBuf += width * 4*numInChannels ) 
	{
        for ( int i = 0; i < width; i += 4 ) 
		{
            
            ExtractBlock( inBuf + i * numInChannels, width, numInChannels, block );
            
            GetMinMaxYCoCg( block, minColor, maxColor );
            ScaleYCoCg( block, minColor, maxColor );
            InsetYCoCgBBox( minColor, maxColor );
            SelectYCoCgDiagonal( block, minColor, maxColor );
            
            EmitByte( maxColor[3] );
            EmitByte( minColor[3] );
            
            EmitAlphaIndices( block, minColor[3], maxColor[3] );
            
            EmitWord( ColorTo565( maxColor ) );
            EmitWord( ColorTo565( minColor ) );
            
            EmitColorIndices( block, minColor, maxColor );
        }
    }
    
    outputBytes = globalOutData - outBuf;
    
    return true;
}

} // end namespace YCoCg


#endif