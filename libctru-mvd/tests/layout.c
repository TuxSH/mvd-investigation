/**
 * @file layout.c
 * @brief Compile-time checks of public wire layouts and legacy field aliases
 */
#include <stddef.h>
#include <3ds/services/mvd.h>
#include <3ds/services/l2b.h>
#include <3ds/services/y2r2.h>
#include <3ds/services/y2r.h>
#include <3ds/ipc.h>

#ifdef __cplusplus
/// @brief C++ compile-time assertion primitive
#define CHECK static_assert
#else
/// @brief C compile-time assertion primitive
#define CHECK _Static_assert
#endif
/**
 * @brief Asserts an exact structure size
 * @param[in] type Type whose wire size is checked
 * @param[in] size Expected byte size
 */
#define SIZE(type, size) CHECK(sizeof(type) == size, #type)
/**
 * @brief Asserts a wire-field byte offset
 * @param[in] type Containing structure type
 * @param[in] field Member whose offset is checked
 * @param[in] offset Expected byte offset
 */
#define OFFSET(type, field, offset) CHECK(offsetof(type, field) == offset, #type "." #field)

SIZE(MVDSTD_Config, 0x11C);
SIZE(MVDSTD_OutputMask, 0x2C);
OFFSET(MVDSTD_Config, input_picture_structure, 0x04);
OFFSET(MVDSTD_Config, input_video_range, 0x08);
OFFSET(MVDSTD_Config, vc1_multires_enable, 0x28);
OFFSET(MVDSTD_Config, rotation, 0x54);
OFFSET(MVDSTD_Config, rgb_transform, 0x6C);
OFFSET(MVDSTD_Config, contrast, 0x70);
OFFSET(MVDSTD_Config, coefficient_a, 0x84);
OFFSET(MVDSTD_Config, output_mask1, 0xAC);
OFFSET(MVDSTD_Config, output_mask2, 0xD8);
OFFSET(MVDSTD_Config, output_x_pos, 0x108);
OFFSET(MVDSTD_Config, deinterlace_enable, 0x118);
SIZE(MVDSTD_ProcessNALUnitOut, 12);
SIZE(MVDSTD_CalculateWorkBufSizeConfig, 48);
OFFSET(MVDSTD_CalculateWorkBufSizeConfig, width, 0x28);
SIZE(MVDSTD_InitStruct, 12);
OFFSET(MVDSTD_InitStruct, reference_frame_format, 4);
OFFSET(MVDSTD_InitStruct, pp_decoder_type, 8);
SIZE(MVDSTD_H264Info, 64);
SIZE(MVDSTD_H264Picture, 64);
OFFSET(MVDSTD_H264Info, dpb_mode, 0x34);
OFFSET(MVDSTD_H264Picture, output_layout, 0x3C);
SIZE(MVDSTD_Vp8Input, 32);
SIZE(MVDSTD_Vp8Info, 40);
SIZE(MVDSTD_Vp8Picture, 64);
OFFSET(MVDSTD_Vp8Info, constant_zero, 0x20);
OFFSET(MVDSTD_Vp8Info, output_format, 0x24);
SIZE(MVDSTD_Vp6Info, 36);
SIZE(MVDSTD_Vp6Picture, 36);
OFFSET(MVDSTD_Vp6Info, constant_zero, 0x1C);
OFFSET(MVDSTD_Vp6Info, output_format, 0x20);
SIZE(Y2R2U_ConversionParams, 12);
OFFSET(Y2R2U_ConversionParams, input_line_width, 4);
OFFSET(Y2R2U_ConversionParams, standard_coefficient, 8);
OFFSET(Y2R2U_ConversionParams, alpha, 10);
SIZE(Y2R2U_ColorCoefficients, 16);
SIZE(Y2R2U_DitheringWeightParams, 32);
SIZE(L2BU_ConversionParams, 8);
OFFSET(L2BU_ConversionParams, input_lines, 4);
OFFSET(L2BU_ConversionParams, alpha, 6);
#if UINTPTR_MAX == UINT32_MAX
SIZE(MVDSTD_OutputBuffersEntry, 8);
SIZE(MVDSTD_OutputBuffersEntryList, 140);
#endif

/**
 * @brief Compiles both recovered and legacy member names with signed adjustments
 * @param[in,out] config Configuration receiving representative field assignments
 */
void layout_example(MVDSTD_Config* config)
{
	config->unk_x6c[0] = MVD_RGB_BT601;
	config->rgb_transform = MVD_RGB_BT709;
	config->contrast = -32;
	config->output_x_pos = -16;
}
