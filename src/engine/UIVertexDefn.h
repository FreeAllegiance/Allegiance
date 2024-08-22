#pragma once

#include <cstdint>

// UI vertex, single textured uncoloured vertices.
struct UIVERTEX
{
    float x, y, z, rhw; // The transformed position for the vertex
	float fU, fV;		// Texture coords.
};

// FVF for the above vertex defn.
#define D3DFVF_UIVERTEX ( D3DFVF_XYZRHW | D3DFVF_TEX1)


// UI colour fill verterx, used to render blank coloured squares.
struct UICOLOURFILLVERTEX
{
	float x, y, z, rhw;	// The transformed position for the vertex
    uint32_t color;		// Colour value.
};

// FVF for the above vertex defn.
#define D3DFVF_UICOLOURVERT ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE )


// UI vertex, single textured uncoloured vertices. Not affected by transforms or lighting.
struct UIFONTVERTEX
{
    float x, y, z, rhw; // The transformed position for the vertex
    uint32_t color;		// Colour value.
	float fU, fV;		// Texture coords.
};

// FVF for the above vertex defn.
#define D3DFVF_UIFONTVERTEX ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )


// Vertex that is affected by transforms and lighting
struct UIFONTVERTEX2
{
    float x, y, z; // The transformed position for the vertex
    uint32_t color;		// Colour value.
    float fU, fV;		// Texture coords.
};

// FVF for the above vertex defn.
#define D3DFVF_UIFONTVERTEX2 ( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 )