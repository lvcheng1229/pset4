#include <stdint.h>
enum ETileMode : uint32_t
{
	// Depth modes (for depth buffers)
	kTileModeDepth_2dThin_64 = 0x00000000, ///< Recommended for depth targets with one fragment per pixel.
	kTileModeDepth_2dThin_128 = 0x00000001, ///< Recommended for depth targets with two or four fragments per pixel, or texture-readable.
	kTileModeDepth_2dThin_256 = 0x00000002, ///< Recommended for depth targets with eight fragments per pixel.
	kTileModeDepth_2dThin_512 = 0x00000003, ///< Recommended for depth targets with 512-byte tiles.
	kTileModeDepth_2dThin_1K = 0x00000004, ///< Recommended for depth targets with 1024-byte tiled.
	kTileModeDepth_1dThin = 0x00000005, ///< Not used; included only for completeness.
	kTileModeDepth_2dThinPrt_256 = 0x00000006, ///< Recommended for partially-resident depth surfaces. Does not support aliasing multiple virtual texture pages to the same physical page.
	kTileModeDepth_2dThinPrt_1K = 0x00000007, ///< Not used; included only for completeness.
	// Display modes
	kTileModeDisplay_LinearAligned = 0x00000008, ///< Recommended for any surface to be easily accessed on the CPU.
	kTileModeDisplay_1dThin = 0x00000009, ///< Not used; included only for completeness.
	kTileModeDisplay_2dThin = 0x0000000A, ///< Recommended mode for displayable render targets.
	kTileModeDisplay_ThinPrt = 0x0000000B, ///< Supports aliasing multiple virtual texture pages to the same physical page.
	kTileModeDisplay_2dThinPrt = 0x0000000C, ///< Does not support aliasing multiple virtual texture pages to the same physical page.
	// Thin modes (for non-displayable 1D/2D/3D surfaces)
	kTileModeThin_1dThin = 0x0000000D, ///< Recommended for read-only non-volume textures.
	kTileModeThin_2dThin = 0x0000000E, ///< Recommended for non-displayable intermediate render targets and read/write non-volume textures.
	kTileModeThin_3dThin = 0x0000000F, ///< Not used; included only for completeness.
	kTileModeThin_ThinPrt = 0x00000010, ///< Recommended for partially-resident textures (PRTs). Supports aliasing multiple virtual texture pages to the same physical page.
	kTileModeThin_2dThinPrt = 0x00000011, ///< Does not support aliasing multiple virtual texture pages to the same physical page.
	kTileModeThin_3dThinPrt = 0x00000012, ///< Does not support aliasing multiple virtual texture pages to the same physical page.
	// Thick modes (for 3D textures)
	kTileModeThick_1dThick = 0x00000013, ///< Recommended for read-only volume textures.
	kTileModeThick_2dThick = 0x00000014, ///< Recommended for volume textures to which pixel shaders will write.
	kTileModeThick_3dThick = 0x00000015, ///< Not used; included only for completeness.
	kTileModeThick_ThickPrt = 0x00000016, ///< Supports aliasing multiple virtual texture pages to the same physical page.
	kTileModeThick_2dThickPrt = 0x00000017, ///< Does not support aliasing multiple virtual texture pages to the same physical page.
	kTileModeThick_3dThickPrt = 0x00000018, ///< Does not support aliasing multiple virtual texture pages to the same physical page.
	kTileModeThick_2dXThick = 0x00000019, ///< Recommended for volume textures to which pixel shaders will write.
	kTileModeThick_3dXThick = 0x0000001A, ///< Not used; included only for completeness.
	// Hugely inefficient linear display mode - do not use!
	kTileModeDisplay_LinearGeneral = 0x0000001F, ///< Unsupported; do not use!
};