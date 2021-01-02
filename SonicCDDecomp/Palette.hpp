#ifndef PALETTE_H
#define PALETTE_H

#define PALETTE_COUNT (0x8)
#define PALETTE_SIZE  (0x100)

// Palettes (as RGB565 Colours)
extern uint fullPalette32[PALETTE_COUNT][PALETTE_SIZE];
extern uint *activePalette32;

extern byte gfxLineBuffer[SCREEN_YSIZE]; // Pointers to active palette

extern int fadeMode;
extern byte fadeA;
extern byte fadeR;
extern byte fadeG;
extern byte fadeB;

extern int paletteMode;

void LoadPalette(const char *filePath, int paletteID, int startPaletteIndex, int startIndex, int endIndex);

inline void SetActivePalette(byte newActivePal, int startLine, int endLine)
{
    if (newActivePal < PALETTE_COUNT)
        while (startLine++ < endLine) gfxLineBuffer[startLine % SCREEN_YSIZE] = newActivePal;

    activePalette32 = fullPalette32[gfxLineBuffer[0]];
}

inline void SetPaletteEntry(byte paletteIndex, byte index, byte r, byte g, byte b)
{
    if (paletteIndex != 0xFF) {
        fullPalette32[paletteIndex][index] = (0xFF << 24) | (b << 16) | (g << 8) | (r);
    }
    else {
        activePalette32[index] = (0xFF << 24) | (b << 16) | (g << 8) | (r);
    }
}

inline void CopyPalette(byte src, byte dest)
{
    if (src < PALETTE_COUNT && dest < PALETTE_COUNT) {
        for (int i = 0; i < PALETTE_SIZE; ++i) {
            fullPalette32[dest][i] = fullPalette32[src][i];
        }
    }
}

inline void RotatePalette(byte startIndex, byte endIndex, bool right)
{
    if (right) {
        uint startClr32 = activePalette32[endIndex];

        for (int i = endIndex; i > startIndex; --i) {
            activePalette32[i] = activePalette32[i - 1];
        }
        activePalette32[startIndex] = startClr32;
    }
    else {
        uint startClr32 = activePalette32[startIndex];
        for (int i = startIndex; i < endIndex; ++i) {
            activePalette32[i] = activePalette32[i + 1];
        }
        activePalette32[endIndex] = startClr32;
    }
}

inline void SetFade(byte R, byte G, byte B, ushort A)
{
    fadeMode = 1;
    fadeR    = R;
    fadeG    = G;
    fadeB    = B;
    fadeA    = A > 0xFF ? 0xFF : A;
}
void SetLimitedFade(byte paletteID, byte R, byte G, byte B, ushort alpha, int startIndex, int endIndex);

#endif // !PALETTE_H
