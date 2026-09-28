/* Altered libctru copy: completed from MVD reverse engineering; see libctru-mvd/README.md */
/**
 * @file mvd.h
 * @brief MVD service
 */
#pragma once
#include <3ds/types.h>

#ifdef __cplusplus
extern "C"
{
#endif

// New3DS-only, see also: http://3dbrew.org/wiki/MVD_Services

/// @brief Native codec or PP operation completed successfully
#define MVD_STATUS_OK 0x17000
/// @brief Input was consumed without an output-picture event
#define MVD_STATUS_STREAM_PROCESSED 0x17001
/// @brief NextPicture or Peek returned a populated picture record
#define MVD_STATUS_PICTURE_READY 0x17002
/// @brief The decoder completed a picture
#define MVD_STATUS_PICTURE_DECODED 0x17003
/// @brief Stream headers are available for an info query and PP configuration
#define MVD_STATUS_HEADERS_READY 0x17004
/// @brief H.264 decoder reported its advanced-tools event
#define MVD_STATUS_ADVANCED_TOOLS 0x17005
/// @brief H.264 decoder requires pending output to be flushed
#define MVD_STATUS_PENDING_FLUSH 0x17006
/// @brief The decoder skipped a nonreference picture
#define MVD_STATUS_NONREF_SKIPPED 0x17007
/// @brief The VP8-family decoder produced slice output
#define MVD_STATUS_SLICE_READY 0x17038

/// @brief Legacy alias for STREAM_PROCESSED; not restricted to parameter-set input
#define MVD_STATUS_PARAMSET MVD_STATUS_STREAM_PROCESSED
/// @brief Legacy alias for PICTURE_READY; does not mean hardware busy
#define MVD_STATUS_BUSY MVD_STATUS_PICTURE_READY
/// @brief Legacy alias for PICTURE_DECODED; does not mean a picture was dequeued
#define MVD_STATUS_FRAMEREADY MVD_STATUS_PICTURE_DECODED
/// @brief Legacy alias for HEADERS_READY
#define MVD_STATUS_INCOMPLETEPROCESSING MVD_STATUS_HEADERS_READY
/// @brief Legacy alias for NONREF_SKIPPED
#define MVD_STATUS_NALUPROCFLAG MVD_STATUS_NONREF_SKIPPED

/**
 * @brief Tests the legacy high-level H.264 decode-success event set
 *
 * @param[in] result Codec result to compare with the legacy accepted event set
 * @return True for an event accepted by MVD_CHECKNALUPROC_SUCCESS, otherwise false
 */
static inline bool mvdstdIsLegacyDecodeSuccess(Result result)
{
	return result == MVD_STATUS_OK || result == MVD_STATUS_STREAM_PROCESSED || result == MVD_STATUS_PICTURE_DECODED ||
	       result == MVD_STATUS_HEADERS_READY || result == MVD_STATUS_NONREF_SKIPPED;
}
/**
 * @brief Tests the legacy high-level H.264 success event set, evaluating x once
 * @param[in] x Codec result expression to inspect
 * @return True if x belongs to the legacy accepted event set, otherwise false
 */
#define MVD_CHECKNALUPROC_SUCCESS(x) mvdstdIsLegacyDecodeSuccess(x)

/// @brief Invalid native codec or PP parameter
#define MVD_ERROR_PARAMETER ((Result)0xE16170C9)
/// @brief Malformed compressed stream
#define MVD_ERROR_STREAM ((Result)0xD96170CA)
/// @brief Native decoder or service state is not initialized
#define MVD_ERROR_NOT_INITIALIZED ((Result)0xD96170CB)
/// @brief Native allocation failed
#define MVD_ERROR_MEMORY ((Result)0xD86170CC)
/// @brief Decoder or convenience-API initialization failed
#define MVD_ERROR_INITIALIZATION ((Result)0xD96170CD)
/// @brief Invalid compressed-stream headers
#define MVD_ERROR_HEADERS ((Result)0xD96170CE)
/// @brief Compressed stream is unsupported
#define MVD_ERROR_STREAM_UNSUPPORTED ((Result)0xD96170D0)
/// @brief Hardware reservation failed
#define MVD_ERROR_HW_RESERVED ((Result)0xD96171C6)
/// @brief Decoder or postprocessor hardware timed out
#define MVD_ERROR_HW_TIMEOUT ((Result)0xD96171C7)
/// @brief Hardware bus access failed
#define MVD_ERROR_HW_BUS ((Result)0xF96171C8)
/// @brief Native decoder or postprocessor system failure
#define MVD_ERROR_SYSTEM ((Result)0xD96171C9)
/// @brief Decoder wrapper-layer failure
#define MVD_ERROR_DWL ((Result)0xD96171CA)
/// @brief Invalid postprocessor combined-mode state
#define MVD_ERROR_COMBINED_MODE ((Result)0xD96172C8)
/// @brief Linked decoder reported a runtime failure
#define MVD_ERROR_DECODER_RUNTIME ((Result)0xD96172C9)
/// @brief Native library evaluation limit was reached
#define MVD_ERROR_EVALUATION_LIMIT ((Result)0xD9617383)
/// @brief Native codec or output format is unsupported
#define MVD_ERROR_FORMAT_UNSUPPORTED ((Result)0xD9617384)

/// @brief Postprocessor rejected input geometry
#define MVD_ERROR_PP_INPUT_SIZE ((Result)0xD9617108)
/// @brief Postprocessor rejected input bus addresses
#define MVD_ERROR_PP_INPUT_ADDRESS ((Result)0xD9617109)
/// @brief Postprocessor rejected input pixel format
#define MVD_ERROR_PP_INPUT_FORMAT ((Result)0xD961710A)
/// @brief Postprocessor rejected input crop rectangle
#define MVD_ERROR_PP_CROP ((Result)0xD961710B)
/// @brief Postprocessor rejected rotation mode
#define MVD_ERROR_PP_ROTATION ((Result)0xD961710C)
/// @brief Postprocessor rejected output geometry
#define MVD_ERROR_PP_OUTPUT_SIZE ((Result)0xD961710D)
/// @brief Postprocessor rejected output bus addresses
#define MVD_ERROR_PP_OUTPUT_ADDRESS ((Result)0xD961710E)
/// @brief Postprocessor rejected output pixel format
#define MVD_ERROR_PP_OUTPUT_FORMAT ((Result)0xD961710F)
/// @brief Postprocessor rejected brightness, contrast or saturation
#define MVD_ERROR_PP_VIDEO_ADJUSTMENT ((Result)0xD9617110)
/// @brief Postprocessor rejected custom RGB channel masks
#define MVD_ERROR_PP_RGB_MASKS ((Result)0xD9617111)
/// @brief Postprocessor rejected framebuffer placement or geometry
#define MVD_ERROR_PP_FRAMEBUFFER ((Result)0xD9617112)
/// @brief Postprocessor rejected first output mask
#define MVD_ERROR_PP_MASK1 ((Result)0xD9617113)
/// @brief Postprocessor rejected second output mask
#define MVD_ERROR_PP_MASK2 ((Result)0xD9617114)
/// @brief Postprocessor rejected deinterlacing configuration
#define MVD_ERROR_PP_DEINTERLACE ((Result)0xD9617115)
/// @brief Postprocessor rejected input picture/field structure
#define MVD_ERROR_PP_PICTURE_STRUCTURE ((Result)0xD9617116)
/// @brief Postprocessor rejected inherited VC-1 range mapping
#define MVD_ERROR_PP_RANGE_MAPPING ((Result)0xD9617117)
/// @brief Postprocessor does not support alpha blending
#define MVD_ERROR_PP_ALPHA_BLEND_UNSUPPORTED ((Result)0xD9617118)
/// @brief Postprocessor does not support deinterlacing
#define MVD_ERROR_PP_DEINTERLACE_UNSUPPORTED ((Result)0xD9617119)
/// @brief Postprocessor does not support dithering
#define MVD_ERROR_PP_DITHERING_UNSUPPORTED ((Result)0xD961711A)
/// @brief Postprocessor does not support scaling
#define MVD_ERROR_PP_SCALING_UNSUPPORTED ((Result)0xD961711B)

/// @brief Browser-derived default H.264 work-buffer bytes before the convenience margin
#define MVD_DEFAULT_WORKBUF_SIZE 0x9006C8

/// @brief Disables the level-derived allocation candidate
#define MVD_CALC_WITH_LEVEL_FLAG_NONE 0x00
/// @brief Enables the level-derived allocation candidate without additional storage
#define MVD_CALC_WITH_LEVEL_FLAG_ENABLE_CALC 0x01
/// @brief Adds one sixth of the level-derived storage candidate
#define MVD_CALC_WITH_LEVEL_FLAG_ENABLE_EXTRA_OP 0x02
/// @brief Alternative flag bit that also adds one sixth of the level-derived storage
#define MVD_CALC_WITH_LEVEL_FLAG_EXTRA_STORAGE_2 0x04
/// @brief Legacy alias for the alternative extra-storage flag bit
#define MVD_CALC_WITH_LEVEL_FLAG_UNK 0x04

/// @brief Allocation-table index for H.264 level 1.0; not a hardware capability guarantee
#define MVD_H264_LEVEL_1_0 0x00
/// @brief Allocation-table index for H.264 level 1.0B; not a hardware capability guarantee
#define MVD_H264_LEVEL_1_0B 0x01
/// @brief Allocation-table index for H.264 level 1.1; not a hardware capability guarantee
#define MVD_H264_LEVEL_1_1 0x02
/// @brief Allocation-table index for H.264 level 1.2; not a hardware capability guarantee
#define MVD_H264_LEVEL_1_2 0x03
/// @brief Allocation-table index for H.264 level 1.3; not a hardware capability guarantee
#define MVD_H264_LEVEL_1_3 0x04
/// @brief Allocation-table index for H.264 level 2.0; not a hardware capability guarantee
#define MVD_H264_LEVEL_2_0 0x05
/// @brief Allocation-table index for H.264 level 2.1; not a hardware capability guarantee
#define MVD_H264_LEVEL_2_1 0x06
/// @brief Allocation-table index for H.264 level 2.2; not a hardware capability guarantee
#define MVD_H264_LEVEL_2_2 0x07
/// @brief Allocation-table index for H.264 level 3.0; not a hardware capability guarantee
#define MVD_H264_LEVEL_3_0 0x08
/// @brief Allocation-table index for H.264 level 3.1; not a hardware capability guarantee
#define MVD_H264_LEVEL_3_1 0x09
/// @brief Allocation-table index for H.264 level 3.2; not a hardware capability guarantee
#define MVD_H264_LEVEL_3_2 0x0A
/// @brief Allocation-table index for H.264 level 4.0; not a hardware capability guarantee
#define MVD_H264_LEVEL_4_0 0x0B
/// @brief Allocation-table index for H.264 level 4.1; not a hardware capability guarantee
#define MVD_H264_LEVEL_4_1 0x0C
/// @brief Allocation-table index for H.264 level 4.2; not a hardware capability guarantee
#define MVD_H264_LEVEL_4_2 0x0D
/// @brief Allocation-table index for H.264 level 5.0; not a hardware capability guarantee
#define MVD_H264_LEVEL_5_0 0x0E
/// @brief Allocation-table index for H.264 level 5.1; not a hardware capability guarantee
#define MVD_H264_LEVEL_5_1 0x0F
/// @brief Allocation-table index for H.264 level 5.2; not a hardware capability guarantee
#define MVD_H264_LEVEL_5_2 0x10

/**
 * @brief High-level standalone or H.264-plus-PP processing mode
 */
typedef enum
{
	/// @brief Standalone postprocessor color conversion
	MVDMODE_COLORFORMATCONV,
	/// @brief H.264 decoder linked to the postprocessor
	MVDMODE_VIDEOPROCESSING,
} MVDSTD_Mode;

/**
 * @brief Convenience names for decoded pixels entering the postprocessor
 */
typedef enum
{
	/// @brief Packed Y Cb Y Cr input, with one chroma pair per two pixels
	MVD_INPUT_YUYV422 = 0x00010001,
	/// @brief Decoded YCbCr 4:2:0 with separate luma and interleaved CbCr planes
	MVD_INPUT_YUV420SP = 0x00020001,
	/// @brief Legacy alias for decoded YCbCr 4:2:0 semiplanar input; not a codec selector
	MVD_INPUT_H264 = 0x00020001,
} MVDSTD_InputFormat;

/**
 * @brief Legacy libctru PP output-format names
 */
typedef enum
{
	/// @brief Packed Y Cb Y Cr output, with a defined service cache length
	MVD_OUTPUT_YUYV422 = 0x00010001,
	/// @brief Legacy libctru name for Hantro RGB565, with a defined service cache length
	MVD_OUTPUT_BGR565 = 0x00040002,
	/// @brief Legacy libctru name for Hantro BGR565; service cache length is undefined
	MVD_OUTPUT_RGB565 = 0x00040004,
	/// @brief Hantro BGR32 output, with a defined service cache length
	MVD_OUTPUT_BGR32 = 0x00041002,
} MVDSTD_OutputFormat;

/**
 * @brief PP output mask and optional alpha-blend source, exactly 0x2C bytes
 */
typedef struct
{
	u32 enable;             ///< @brief Nonzero to enable this output mask
	s32 origin_x;           ///< @brief Signed mask X origin in output pixels
	s32 origin_y;           ///< @brief Signed mask Y origin in output pixels
	u32 height;             ///< @brief Mask height in output pixels
	u32 width;              ///< @brief Mask width in output pixels
	u32 alpha_blend_enable; ///< @brief Nonzero to alpha-blend the source over this mask
	u32 blend_bus_address;  ///< @brief Nonzero eight-byte-aligned hardware bus address of the blend source
	s32 blend_origin_x;     ///< @brief Signed X origin of the blend-source crop, in pixels
	s32 blend_origin_y;     ///< @brief Signed Y origin of the blend-source crop, in pixels
	u32 blend_width;        ///< @brief Blend-source crop width in pixels
	u32 blend_height;       ///< @brief Blend-source crop height in pixels
} MVDSTD_OutputMask;

/**
 * @brief Hantro postprocessor configuration, exactly 0x11C bytes
 *
 * Addresses are hardware bus addresses; legacy aliases share the recovered fields
 * Only output formats 0x10001, 0x40002 and 0x41002 have defined service cache lengths
 * See RE/postprocessor.md for geometry, format and capability restrictions
 */
typedef struct
{
	MVDSTD_InputFormat
	    input_type; ///< @brief Decoded input pixel format presented to PP, not a compressed codec selector
	/// @brief Shared storage for descriptive fields and legacy aliases
	union
	{
		u32 input_picture_structure; ///< @brief Picture/field layout from MVDSTD_PictureStructure
		u32 unk_x04;                 ///< @brief Legacy alias for input_picture_structure
	};
	/// @brief Shared storage for descriptive fields and legacy aliases
	union
	{
		u32 input_video_range; ///< @brief Zero for limited/studio range or one for full range
		u32 unk_x08;           ///< @brief Legacy alias for input_video_range
	};
	u32 inwidth;                   ///< @brief Input image width in pixels
	u32 inheight;                  ///< @brief Input image height in pixels
	u32 physaddr_colorconv_indata; ///< @brief Hardware bus address of standalone PP packed input or luma
	/// @brief Shared storage for descriptive fields and legacy aliases
	union
	{
		u32 input_cb_bus_address;    ///< @brief Hardware bus address of the input Cb or interleaved chroma plane
		u32 physaddr_colorconv_unk0; ///< @brief Legacy alias for input_cb_bus_address
	};
	/// @brief Shared storage for descriptive fields and legacy aliases
	union
	{
		u32 input_cr_bus_address;    ///< @brief Hardware bus address of the input Cr plane for planar input
		u32 physaddr_colorconv_unk1; ///< @brief Legacy alias for input_cr_bus_address
	};
	/// @brief Shared storage for descriptive fields and legacy aliases
	union
	{
		u32 input_bottom_bus_address; ///< @brief Hardware bus address of bottom-field input luma
		u32 physaddr_colorconv_unk2;  ///< @brief Legacy alias for input_bottom_bus_address
	};
	/// @brief Shared storage for descriptive fields and legacy aliases
	union
	{
		u32 input_bottom_chroma_bus_address; ///< @brief Hardware bus address of bottom-field input chroma
		u32 physaddr_colorconv_unk3;         ///< @brief Legacy alias for input_bottom_chroma_bus_address
	};
	/// @brief Shared storage for descriptive fields and legacy aliases
	union
	{
		u32 unk_x28[6]; ///< @brief Legacy six-word view of the inherited VC-1 input controls
		/// @brief Named view of the inherited postprocessor controls
		struct
		{
			u32 vc1_multires_enable;    ///< @brief Inherited VC-1 multiresolution flag; does not imply an exposed VC-1
			                            ///< decoder
			u32 vc1_range_reduction;    ///< @brief Inherited VC-1 frame range-reduction flag
			u32 vc1_range_map_y_enable; ///< @brief Enables the inherited VC-1 luma range map
			u32 vc1_range_map_y_coefficient; ///< @brief Inherited VC-1 luma range-map coefficient
			u32 vc1_range_map_c_enable;      ///< @brief Enables the inherited VC-1 chroma range map
			u32 vc1_range_map_c_coefficient; ///< @brief Inherited VC-1 chroma range-map coefficient
		};
	};
	u32 enable_cropping;   ///< @brief Nonzero to enable the input crop rectangle
	u32 input_crop_x_pos;  ///< @brief Input crop X origin in pixels, aligned to sixteen pixels
	u32 input_crop_y_pos;  ///< @brief Input crop Y origin in pixels, aligned to sixteen pixels
	u32 input_crop_height; ///< @brief Input crop height in pixels, aligned to eight pixels
	u32 input_crop_width;  ///< @brief Input crop width in pixels, aligned to eight pixels
	/// @brief Shared storage for descriptive fields and legacy aliases
	union
	{
		u32 rotation; ///< @brief Postprocessor orientation from MVDSTD_Rotation
		u32 unk_x54;  ///< @brief Legacy alias for rotation
	};
	MVDSTD_OutputFormat output_type; ///< @brief PP output pixel format; observe the service cache-length restrictions
	u32 outwidth;                    ///< @brief Output image width in pixels
	u32 outheight;                   ///< @brief Output image height in pixels
	u32 physaddr_outdata0;           ///< @brief Hardware bus address of the packed output image or luma plane
	u32 physaddr_outdata1;           ///< @brief Hardware bus address of output chroma for semiplanar format 0x20001
	/// @brief Shared storage for descriptive fields and legacy aliases
	union
	{
		u32 unk_x6c[38]; ///< @brief Legacy 38-word view of RGB controls and the two output masks
		/// @brief Named view of the inherited postprocessor controls
		struct
		{
			u32 rgb_transform; ///< @brief Color transform from MVDSTD_RgbTransform: custom, BT.601 or BT.709
			s32 contrast;      ///< @brief Signed contrast adjustment, from -64 through 64
			s32 brightness;    ///< @brief Signed brightness adjustment, from -128 through 127
			s32 saturation;    ///< @brief Signed saturation adjustment, from -64 through 128
			u32 alpha;         ///< @brief Output alpha value interpreted according to the selected PP RGB format
			u32 transparency;  ///< @brief RGB transparency control interpreted according to the selected output format
			u32 coefficient_a; ///< @brief Custom Y-to-RGB transform coefficient a
			u32 coefficient_b; ///< @brief Custom Cr-to-red transform coefficient b
			u32 coefficient_c; ///< @brief Custom Cr-to-green subtraction coefficient c
			u32 coefficient_d; ///< @brief Custom Cb-to-green subtraction coefficient d
			u32 coefficient_e; ///< @brief Custom Cb-to-blue transform coefficient e
			u32 mask_r;        ///< @brief Red-channel bit mask for a custom RGB format
			u32 mask_g;        ///< @brief Green-channel bit mask for a custom RGB format
			u32 mask_b;        ///< @brief Blue-channel bit mask for a custom RGB format
			u32 mask_alpha;    ///< @brief Alpha-channel bit mask for a custom RGB format
			u32 dithering_enable;           ///< @brief Nonzero to request hardware-supported output dithering
			MVDSTD_OutputMask output_mask1; ///< @brief First output mask and optional alpha-blend source
			MVDSTD_OutputMask output_mask2; ///< @brief Second output mask and optional alpha-blend source
		};
	};
	/// @brief Shared storage for descriptive fields and legacy aliases
	union
	{
		u32 framebuffer_enable; ///< @brief Nonzero to enable framebuffer dimensions and signed output placement
		u32 flag_x104;          ///< @brief Legacy alias for framebuffer_enable
	};
	s32 output_x_pos;           ///< @brief Signed horizontal output origin in the framebuffer, in pixels
	s32 output_y_pos;           ///< @brief Signed vertical output origin in the framebuffer, in pixels
	u32 output_width_override;  ///< @brief Framebuffer width in pixels when framebuffer placement is enabled
	u32 output_height_override; ///< @brief Framebuffer height in pixels when framebuffer placement is enabled
	/// @brief Shared storage for descriptive fields and legacy aliases
	union
	{
		u32 deinterlace_enable; ///< @brief Nonzero to request supported PP deinterlacing
		u32 unk_x118;           ///< @brief Legacy alias for deinterlace_enable
	};
} MVDSTD_Config;

/**
 * @brief H.264 stream-consumption reply, exactly twelve bytes
 */
typedef struct
{
	u32 end_vaddr;      ///< @brief End address in freed module scratch memory; never dereference in the client
	u32 end_physaddr;   ///< @brief Original input bus address plus consumed bytes
	u32 remaining_size; ///< @brief Unconsumed byte count; advance the original input by input_size minus this value
} MVDSTD_ProcessNALUnitOut;

/**
 * @brief Client LINEAR output image/chroma address pair
 */
typedef struct
{
	void* outdata0; ///< @brief Client LINEAR VA of the packed output image or luma plane
	void* outdata1; ///< @brief Client LINEAR VA of the chroma plane for semiplanar output
} MVDSTD_OutputBuffersEntry;

/**
 * @brief Fixed-capacity table of up to seventeen client PP output pairs
 */
typedef struct
{
	u32 total_entries;                     ///< @brief Number of active buffer pairs, from one through seventeen
	MVDSTD_OutputBuffersEntry entries[17]; ///< @brief Fixed-capacity table of client output-buffer address pairs
} MVDSTD_OutputBuffersEntryList;

/**
 * @brief Optional overrides for the convenience H.264 and linked-PP initializer
 */
typedef struct
{
	/// @brief Shared storage for descriptive fields and legacy aliases
	union
	{
		s8 no_output_reordering; ///< @brief Nonzero to disable H.264 output reordering
		s8 cmd5_inval0;          ///< @brief Legacy alias for no_output_reordering
	};
	/// @brief Shared storage for descriptive fields and legacy aliases
	union
	{
		s8 freeze_concealment; ///< @brief Nonzero to request freeze-picture concealment
		s8 cmd5_inval1;        ///< @brief Legacy alias for freeze_concealment
	};
	/// @brief Shared storage for descriptive fields and legacy aliases
	union
	{
		s8 display_smoothing; ///< @brief Nonzero to request additional H.264 display-smoothing buffers
		s8 cmd5_inval2;       ///< @brief Legacy alias for display_smoothing
	};
	/// @brief Shared storage for descriptive fields and legacy aliases
	union
	{
		u32 reference_frame_format; ///< @brief Reference-storage flags such as MVD_REFERENCE_TILED and
		                            ///< MVD_REFERENCE_FIELD_DPB
		u32 cmd5_inval3;            ///< @brief Legacy alias for reference_frame_format
	};
	/// @brief Shared storage for descriptive fields and legacy aliases
	union
	{
		u8 pp_decoder_type; ///< @brief PP attachment selector; the convenience H.264 initializer requires MVD_PP_H264
		u8 cmd1b_inval;     ///< @brief Legacy alias for pp_decoder_type
	};
} MVDSTD_InitStruct;

/**
 * @brief Four-byte H.264 level-based allocation candidate controls
 */
typedef struct
{
	u8 enable; ///< @brief Nonzero to consider the level-based allocation candidate
	u8 flag; ///< @brief Level-candidate flags; zero disables the candidate and bits 1 or 2 add one sixth of its storage
	u8 double_size; ///< @brief Nonzero to double the level-based storage candidate before its fixed overhead
	u8 level;       ///< @brief Level-table index MVD_H264_LEVEL_1_0 through MVD_H264_LEVEL_5_2
} MVDSTD_WithLevel;

/**
 * @brief Two-byte H.264 reference-count allocation candidate controls
 */
typedef struct
{
	u8 enable;     ///< @brief Nonzero to consider this reference-count allocation candidate
	u8 ref_frames; ///< @brief Requested H.264 reference-frame count
} MVDSTD_WithNumOfRefFrames;

/**
 * @brief Packed 48-byte H.264 work-buffer sizing request
 */
typedef struct
{
	u8 unused_0x00;                         ///< @brief Ignored leading request byte; initialize to zero
	MVDSTD_WithLevel level;                 ///< @brief Level-based candidate controls
	MVDSTD_WithNumOfRefFrames ref_frames_a; ///< @brief First reference-count and resolution candidate
	MVDSTD_WithNumOfRefFrames ref_frames_b; ///< @brief Second reference-count and resolution candidate
	u8 unused_0x09[3];                      ///< @brief Ignored request bytes 0x09..0x0B; initialize to zero
	/// @brief Shared storage for descriptive fields and legacy aliases
	union
	{
		u32 ignored_0x0c; ///< @brief Ignored work-size request word at byte offset 0x0c; initialize to zero
		u32 unk_0x0c;     ///< @brief Legacy alias for ignored_0x0c
	};
	/// @brief Shared storage for descriptive fields and legacy aliases
	union
	{
		u32 ignored_0x10; ///< @brief Ignored work-size request word at byte offset 0x10; initialize to zero
		u32 unk_0x10;     ///< @brief Legacy alias for ignored_0x10
	};
	/// @brief Shared storage for descriptive fields and legacy aliases
	union
	{
		u32 ignored_0x14; ///< @brief Ignored work-size request word at byte offset 0x14; initialize to zero
		u32 unk_0x14;     ///< @brief Legacy alias for ignored_0x14
	};
	/// @brief Shared storage for descriptive fields and legacy aliases
	union
	{
		u32 ignored_0x18; ///< @brief Ignored work-size request word at byte offset 0x18; initialize to zero
		u32 unk_0x18;     ///< @brief Legacy alias for ignored_0x18
	};
	/// @brief Shared storage for descriptive fields and legacy aliases
	union
	{
		u32 ignored_0x1c; ///< @brief Ignored work-size request word at byte offset 0x1c; initialize to zero
		u32 unk_0x1c;     ///< @brief Legacy alias for ignored_0x1c
	};
	/// @brief Shared storage for descriptive fields and legacy aliases
	union
	{
		u32 ignored_0x20; ///< @brief Ignored work-size request word at byte offset 0x20; initialize to zero
		u32 unk_0x20;     ///< @brief Legacy alias for ignored_0x20
	};
	/// @brief Shared storage for descriptive fields and legacy aliases
	union
	{
		u32 ignored_0x24; ///< @brief Ignored work-size request word at byte offset 0x24; initialize to zero
		u32 unk_0x24;     ///< @brief Legacy alias for ignored_0x24
	};
	u32 width;  ///< @brief Coded video width in pixels
	u32 height; ///< @brief Coded video height in pixels
} MVDSTD_CalculateWorkBufSizeConfig;

/**
 * @brief Opens MVD and initializes standalone PP or H.264 with linked PP
 *
 * Repeated calls retain the first configuration and increment its reference count
 * Use mvdstdOpen for explicit VP6/VP8/WebP lifecycles; do not mix the two lifecycle APIs
 *
 * @param[in] mode Standalone color conversion or H.264 decoding with postprocessing
 * @param[in] input_type Decoded input pixel format used by the postprocessor
 * @param[in] output_type Postprocessor output format, subject to the service cache-length restrictions
 * @param[in] size Requested work-buffer bytes before the 4096-byte margin; ignored for standalone PP
 * @param[in] initstruct Optional initialization overrides; NULL uses default H.264 settings
 * @return Zero on success, otherwise an initialization or service error
 */
Result mvdstdInit(MVDSTD_Mode mode, MVDSTD_InputFormat input_type, MVDSTD_OutputFormat output_type, u32 size,
                  MVDSTD_InitStruct* initstruct);

/**
 * @brief Releases one convenience-API reference and tears down the last active instance
 */
void mvdstdExit(void);

/**
 * @brief Queries H.264 work-buffer size, temporarily opening the service if necessary
 *
 * @param[in] config H.264 sizing inputs; level.level must not exceed MVD_H264_LEVEL_5_2
 * @param[out] size_out Required destination for the calculated work-buffer byte count
 * @return Zero on the normal success path, otherwise the service or transport error
 */
Result mvdstdCalculateBufferSize(const MVDSTD_CalculateWorkBufSizeConfig* config, u32* size_out);

/**
 * @brief Builds a default postprocessor configuration using the active convenience settings
 *
 * @param[out] config Destination for the complete postprocessor configuration
 * @param[in] input_width Input width in pixels
 * @param[in] input_height Input height in pixels
 * @param[in] output_width Output width in pixels
 * @param[in] output_height Output height in pixels
 * @param[in] vaddr_colorconv_indata Client LINEAR input VA for standalone conversion
 * @param[in] vaddr_outdata0 Client LINEAR output VA for the packed image or luma plane
 * @param[in] vaddr_outdata1 Client LINEAR chroma-plane VA for semiplanar output; otherwise unused
 */
void mvdstdGenerateDefaultConfig(MVDSTD_Config* config, u32 input_width, u32 input_height, u32 output_width,
                                 u32 output_height, u32* vaddr_colorconv_indata, u32* vaddr_outdata0,
                                 u32* vaddr_outdata1);

/**
 * @brief Configures and synchronously runs standalone postprocessing
 *
 * @param[in,out] config PP configuration; the service may rewrite multibuffer output addresses
 * @return Service result or transport/client error; interpret codec events using MVD_STATUS_*
 */
Result mvdstdConvertImage(MVDSTD_Config* config);

/**
 * @brief Submits compressed H.264 input using the convenience picture-ID cycle
 *
 * The helper translates the input to its new LINEAR alias and cycles picture IDs through 0..17
 *
 * @param[in] inbuf_vaddr Client LINEAR VA of compressed H.264 input, including its start-code prefix
 * @param[in] size Compressed payload length in bytes
 * @param[in] flag Nonzero to request skipping nonreference pictures
 * @param[out] out Optional progress record; end_vaddr is a freed module scratch address, not a client pointer
 * @return Service result or transport/client error; interpret codec events using MVD_STATUS_*
 */
Result mvdstdProcessVideoFrame(void* inbuf_vaddr, size_t size, u32 flag, MVDSTD_ProcessNALUnitOut* out);

/**
 * @brief Dequeues decoded H.264 pictures and runs linked postprocessing as needed
 *
 * MVD_STATUS_PICTURE_READY means a picture was dequeued, not that the hardware is busy
 *
 * @param[in,out] config Optional PP configuration; NULL retains the current configuration
 * @param[in] wait True to dequeue until no picture is ready; false to dequeue at most one picture
 * @return Service result or transport/client error; interpret codec events using MVD_STATUS_*
 */
Result mvdstdRenderVideoFrame(MVDSTD_Config* config, bool wait);

/**
 * @brief Submits the complete 0x11C-byte postprocessor configuration
 *
 * Only output formats 0x10001, 0x40002 and 0x41002 have defined cache-maintenance lengths in the service
 * The native PP may overwrite the supplied output addresses when multibuffering is enabled
 *
 * @param[in,out] config PP configuration; the service may rewrite multibuffer output addresses
 * @return Service result or transport/client error; interpret codec events using MVD_STATUS_*
 */
Result MVDSTD_SetConfig(MVDSTD_Config* config);

/**
 * @brief Registers client output-buffer pairs for linked PP multibuffering
 *
 * The client validates the count before IPC because the server translates entries before its own bounds check
 *
 * @param[in] entrylist One to seventeen client LINEAR output-buffer pairs
 * @param[in] bufsize Size of each registered output buffer in bytes
 * @return Service result or transport/client error; interpret codec events using MVD_STATUS_*
 */
Result mvdstdSetupOutputBuffers(MVDSTD_OutputBuffersEntryList* entrylist, u32 bufsize);

/**
 * @brief Replaces the matching buffer pair at the current PP display index
 *
 * The service saves only the first extra replacement mapping; repeated calls do not refresh the entire VA table
 *
 * @param[in] cur_outdata0 Registered client luma or packed-image VA of the current display buffer
 * @param[in] cur_outdata1 Registered client chroma VA of the current display buffer
 * @param[in] new_outdata0 Replacement client LINEAR luma or packed-image VA
 * @param[in] new_outdata1 Replacement client LINEAR chroma VA
 * @return Service result or transport/client error; interpret codec events using MVD_STATUS_*
 */
Result mvdstdOverrideOutputBuffers(void* cur_outdata0, void* cur_outdata1, void* new_outdata0, void* new_outdata1);

/**
 * @brief Opens a raw mvd:STD session without allocating memory or choosing a codec
 *
 * The session holds one decoder family at a time; do not mix Open/Close with Init/Exit
 * @return Zero on success, otherwise an initialization or service error
 */
Result mvdstdOpen(void);

/**
 * @brief Closes the raw session opened by mvdstdOpen
 *
 * Release the decoder and PP, then call MVDSTD_Shutdown before closing
 */
void mvdstdClose(void);

/// @brief Requests tiled decoder reference storage; not a PP pixel-format value
#define MVD_REFERENCE_TILED 1u
/// @brief Enables H.264 field decoded-picture-buffer storage when supported
#define MVD_REFERENCE_FIELD_DPB (1u << 30)

/**
 * @brief Compressed format selector for the shared VP7/VP8/WebP decoder
 */
typedef enum
{
	/// @brief Decode compressed VP7 frames
	MVD_VP7 = 1,
	/// @brief Decode compressed VP8 frames
	MVD_VP8 = 2,
	/// @brief Decode WebP VP8 frame payloads
	MVD_WEBP = 3,
} MVDSTD_Vp8Format;

/**
 * @brief Decoder selectors supported by the combined-mode PP attachment switch
 */
typedef enum
{
	/// @brief Attach PP to H.264
	MVD_PP_H264 = 1,
	/// @brief Attach PP to VP6
	MVD_PP_VP6 = 6,
	/// @brief Attach PP to VP8
	MVD_PP_VP8 = 9,
	/// @brief Attach PP to WebP
	MVD_PP_WEBP = 10,
} MVDSTD_PpDecoder;

/**
 * @brief Hantro pixel-format identifiers, subject to mode and hardware restrictions
 *
 * These values are not an all-modes support matrix; observe the service cache-length restrictions
 */
typedef enum
{
	/// @brief Packed Y Cb Y Cr 4:2:2
	MVD_PIX_YUYV = 0x10001,
	/// @brief Packed Y Cr Y Cb 4:2:2
	MVD_PIX_YVYU = 0x10005,
	/// @brief Packed Cb Y Cr Y 4:2:2
	MVD_PIX_UYVY = 0x10006,
	/// @brief Packed Cr Y Cb Y 4:2:2
	MVD_PIX_VYUY = 0x10007,
	/// @brief YCbCr 4:2:2 semiplanar
	MVD_PIX_YUV422SP = 0x10002,
	/// @brief YCbCr 4:4:0
	MVD_PIX_YUV440 = 0x10004,
	/// @brief Packed Y Cb Y Cr 4:2:2, tiled in 4x4 blocks
	MVD_PIX_YUYV_TILED4 = 0x10008,
	/// @brief Packed Y Cr Y Cb 4:2:2, tiled in 4x4 blocks
	MVD_PIX_YVYU_TILED4 = 0x10009,
	/// @brief Packed Cb Y Cr Y 4:2:2, tiled in 4x4 blocks
	MVD_PIX_UYVY_TILED4 = 0x1000A,
	/// @brief Packed Cr Y Cb Y 4:2:2, tiled in 4x4 blocks
	MVD_PIX_VYUY_TILED4 = 0x1000B,
	/// @brief YCbCr 4:2:0 planar
	MVD_PIX_YUV420P = 0x20000,
	/// @brief YCbCr 4:2:0 semiplanar
	MVD_PIX_YUV420SP = 0x20001,
	/// @brief YCbCr 4:2:0 tiled
	MVD_PIX_YUV420_TILED = 0x20002,
	/// @brief Monochrome luma-only input
	MVD_PIX_MONOCHROME = 0x80000,
	/// @brief YCbCr 4:1:1 semiplanar
	MVD_PIX_YUV411SP = 0x100001,
	/// @brief YCbCr 4:4:4 semiplanar
	MVD_PIX_YUV444SP = 0x200001,
	/// @brief Custom 16-bit RGB channel masks
	MVD_PIX_CUSTOM_RGB16 = 0x40000,
	/// @brief Hantro RGB 5:5:5 layout
	MVD_PIX_RGB555 = 0x40001,
	/// @brief Hantro RGB 5:6:5 layout; named BGR565 in legacy libctru
	MVD_PIX_RGB565 = 0x40002,
	/// @brief Hantro BGR 5:5:5 layout
	MVD_PIX_BGR555 = 0x40003,
	/// @brief Hantro BGR 5:6:5 layout; named RGB565 in legacy libctru
	MVD_PIX_BGR565 = 0x40004,
	/// @brief Custom 32-bit RGB channel masks
	MVD_PIX_CUSTOM_RGB32 = 0x41000,
	/// @brief Hantro RGB32 layout
	MVD_PIX_RGB32 = 0x41001,
	/// @brief Hantro BGR32 layout
	MVD_PIX_BGR32 = 0x41002,
} MVDSTD_PixelFormat;

/**
 * @brief Postprocessor rotation and reflection modes
 */
typedef enum
{
	/// @brief Do not rotate or reflect
	MVD_ROTATION_NONE = 0,
	/// @brief Rotate clockwise by 90 degrees
	MVD_ROTATION_RIGHT_90 = 1,
	/// @brief Rotate counterclockwise by 90 degrees
	MVD_ROTATION_LEFT_90 = 2,
	/// @brief Reflect horizontally
	MVD_ROTATION_FLIP_HORIZONTAL = 3,
	/// @brief Reflect vertically
	MVD_ROTATION_FLIP_VERTICAL = 4,
	/// @brief Rotate by 180 degrees
	MVD_ROTATION_180 = 5,
} MVDSTD_Rotation;

/**
 * @brief Input frame and field arrangements accepted by PP
 */
typedef enum
{
	/// @brief Complete frame or separate top field
	MVD_PICTURE_FRAME_OR_TOP = 0,
	/// @brief Separate bottom field
	MVD_PICTURE_BOTTOM = 1,
	/// @brief Top and bottom fields at separate addresses
	MVD_PICTURE_SEPARATE_FIELDS = 2,
	/// @brief Both fields interleaved in one frame
	MVD_PICTURE_FIELDS_IN_FRAME = 3,
	/// @brief Top field stored in an interleaved frame
	MVD_PICTURE_TOP_IN_FRAME = 4,
	/// @brief Bottom field stored in an interleaved frame
	MVD_PICTURE_BOTTOM_IN_FRAME = 5,
} MVDSTD_PictureStructure;

/**
 * @brief Postprocessor YCbCr-to-RGB transform selection
 */
typedef enum
{
	/// @brief Use the supplied transform coefficients
	MVD_RGB_CUSTOM = 0,
	/// @brief Use BT.601 coefficients selected for the input range
	MVD_RGB_BT601 = 1,
	/// @brief Use BT.709 coefficients selected for the input range
	MVD_RGB_BT709 = 2,
} MVDSTD_RgbTransform;

/// @brief Decoder output uses raster-scan picture storage
#define MVD_LAYOUT_RASTER 0
/// @brief Decoder output uses 8x4 tiled picture storage
#define MVD_LAYOUT_TILED_8X4 1
/// @brief Client sentinel for an output-layout byte left unwritten by Peek
#define MVD_LAYOUT_UNAVAILABLE 0xFF

/**
 * @brief H.264 stream metadata, exactly 0x40 bytes
 */
typedef struct
{
	u32 width;               ///< @brief Stored picture width in pixels
	u32 height;              ///< @brief Stored picture height in pixels
	u32 video_range;         ///< @brief Video range: zero for limited range, one for full range
	u32 matrix_coefficients; ///< @brief H.264 VUI matrix-coefficient identifier
	u32 crop_left;           ///< @brief Left edge of the visible crop in pixels
	u32 crop_width;          ///< @brief Visible cropped width in pixels
	u32 crop_top;            ///< @brief Top edge of the visible crop in pixels
	u32 crop_height;         ///< @brief Visible cropped height in pixels
	u32 output_format;       ///< @brief Native decoded pixel-format identifier; distinct from the output-layout byte
	u32 sar_width;           ///< @brief Sample aspect-ratio numerator
	u32 sar_height;          ///< @brief Sample aspect-ratio denominator
	u32 monochrome;          ///< @brief Nonzero for a monochrome sequence
	u32 interlaced_sequence; ///< @brief Nonzero for an interlaced sequence
	s32 dpb_mode; ///< @brief Native decoded-picture-buffer storage mode; initialized to -1 before sequence setup
	u32 picture_buffer_count; ///< @brief Decoder picture-buffer count
	u32 multibuffer_pp_count; ///< @brief Required multibuffer postprocessor buffer count
} MVDSTD_H264Info;

/**
 * @brief H.264 decoded-picture metadata, exactly 0x40 bytes
 *
 * Normal decoder allocations reside in the client-provided LINEAR work buffer
 * Returned VAs identify client memory; bus addresses identify the same storage to hardware
 * Interpret addresses only for a valid picture, with the reported layout and any plane strides
 * Synchronize CPU access with hardware completion and cache maintenance; decoder buffers may be reused
 */
typedef struct
{
	u32 width;                 ///< @brief Stored picture width in pixels
	u32 height;                ///< @brief Stored picture height in pixels
	u32 crop_left;             ///< @brief Left edge of the visible crop in pixels
	u32 crop_width;            ///< @brief Visible cropped width in pixels
	u32 crop_top;              ///< @brief Top edge of the visible crop in pixels
	u32 crop_height;           ///< @brief Visible cropped height in pixels
	u32 output_vaddr;          ///< @brief Client VA of decoded picture storage in the supplied work buffer
	u32 output_bus_address;    ///< @brief Hardware bus address of decoded picture storage
	u32 picture_id;            ///< @brief Picture identifier reported by the decoder
	u32 is_idr;                ///< @brief Nonzero for an instantaneous decoder refresh picture
	u32 concealed_macroblocks; ///< @brief Number of macroblocks reported as concealed after errors
	u32 interlaced;            ///< @brief Nonzero for interlaced picture storage
	u32 field_picture;         ///< @brief Nonzero when the output represents a field
	u32 top_field;             ///< @brief Nonzero for a top field
	u32 view_id;               ///< @brief MVC view identifier; H264Peek substitutes zero because it is unwritten
	u8 output_layout; ///< @brief MVD_LAYOUT_RASTER, MVD_LAYOUT_TILED_8X4, or the client MVD_LAYOUT_UNAVAILABLE sentinel
	u8 reserved[3];   ///< @brief Wire padding cleared by the client; carries no codec metadata
} MVDSTD_H264Picture;

/**
 * @brief Eight-word compressed VP7/VP8/WebP input record
 */
typedef struct
{
	u32 stream_vaddr; ///< @brief Client VA of the compressed frame payload; use its new LINEAR alias when applicable
	u32 stream_bus_address; ///< @brief Hardware bus address of the compressed frame payload
	u32 size;               ///< @brief Compressed frame-payload length in bytes
	u32 slice_height;       ///< @brief WebP slice height in macroblock rows; zero disables slice output
	u32 luma_vaddr;         ///< @brief Optional WebP output luma VA forwarded without module-VA remapping
	u32 luma_bus_address;   ///< @brief Hardware bus address of the optional WebP luma buffer
	u32 chroma_vaddr;       ///< @brief Optional WebP output chroma VA forwarded without module-VA remapping
	u32 chroma_bus_address; ///< @brief Hardware bus address of the optional WebP chroma buffer
} MVDSTD_Vp8Input;

/**
 * @brief VP7/VP8/WebP stream metadata, exactly 0x28 bytes
 */
typedef struct
{
	u32 version;       ///< @brief Codec bitstream version reported by the native decoder
	u32 profile;       ///< @brief Codec bitstream profile reported by the native decoder
	u32 coded_width;   ///< @brief Coded picture width in pixels
	u32 coded_height;  ///< @brief Coded picture height in pixels
	u32 frame_width;   ///< @brief Stored frame width in pixels, including decoder alignment
	u32 frame_height;  ///< @brief Stored frame height in pixels, including decoder alignment
	u32 scaled_width;  ///< @brief Bitstream-signaled scaled width in pixels
	u32 scaled_height; ///< @brief Bitstream-signaled scaled height in pixels
	u8 constant_zero;  ///< @brief Always zero on successful queries in this build; original field meaning remains
	                   ///< unknown
	u8 reserved[3];    ///< @brief Wire padding cleared by the client; carries no codec metadata
	u32 output_format; ///< @brief Native decoded pixel-format identifier; distinct from the output-layout byte
} MVDSTD_Vp8Info;

/**
 * @brief VP7/VP8/WebP decoded-picture metadata, exactly 0x40 bytes
 *
 * Normal decoder allocations reside in the client-provided LINEAR work buffer
 * Returned VAs identify client memory; bus addresses identify the same storage to hardware
 * Interpret addresses only for a valid picture, with the reported layout and any plane strides
 * Synchronize CPU access with hardware completion and cache maintenance; decoder buffers may be reused
 * Optional WebP user-picture buffers follow a separate path whose accepted modes are not fully established
 */
typedef struct
{
	u32 coded_width;           ///< @brief Coded picture width in pixels
	u32 coded_height;          ///< @brief Coded picture height in pixels
	u32 frame_width;           ///< @brief Stored frame width in pixels, including decoder alignment
	u32 frame_height;          ///< @brief Stored frame height in pixels, including decoder alignment
	u32 luma_stride;           ///< @brief Stored luma row stride in bytes
	u32 chroma_stride;         ///< @brief Stored chroma row stride in bytes
	u32 luma_vaddr;            ///< @brief Client VA of the decoded luma plane in the normal work-buffer path
	u32 luma_bus_address;      ///< @brief Hardware bus address of the decoded luma plane
	u32 chroma_vaddr;          ///< @brief Client VA of the decoded chroma plane in the normal work-buffer path
	u32 chroma_bus_address;    ///< @brief Hardware bus address of the decoded chroma plane
	u32 picture_id;            ///< @brief Picture identifier reported by the decoder
	u32 intra;                 ///< @brief Native intra-frame flag; some output paths explicitly report zero
	u32 golden;                ///< @brief Native golden-frame flag; this build can explicitly report zero
	u32 concealed_macroblocks; ///< @brief Number of macroblocks reported as concealed after errors
	u32 slice_rows;   ///< @brief Number of decoded slice rows; VP8Peek substitutes zero for the unwritten field
	u8 output_layout; ///< @brief MVD_LAYOUT_RASTER, MVD_LAYOUT_TILED_8X4, or the client MVD_LAYOUT_UNAVAILABLE sentinel
	u8 reserved[3];   ///< @brief Wire padding cleared by the client; carries no codec metadata
} MVDSTD_Vp8Picture;

/**
 * @brief VP6 stream metadata, exactly 0x24 bytes
 */
typedef struct
{
	u32 version;       ///< @brief Codec bitstream version reported by the native decoder
	u32 profile;       ///< @brief Codec bitstream profile reported by the native decoder
	u32 frame_width;   ///< @brief Stored frame width in pixels, including decoder alignment
	u32 frame_height;  ///< @brief Stored frame height in pixels, including decoder alignment
	u32 scaled_width;  ///< @brief Bitstream-signaled scaled width in pixels
	u32 scaled_height; ///< @brief Bitstream-signaled scaled height in pixels
	u32 scaling_mode;  ///< @brief Native VP6 scaling-mode identifier
	u8 constant_zero;  ///< @brief Always zero on successful queries in this build; original field meaning remains
	                   ///< unknown
	u8 reserved[3];    ///< @brief Wire padding cleared by the client; carries no codec metadata
	u32 output_format; ///< @brief Native decoded pixel-format identifier; distinct from the output-layout byte
} MVDSTD_Vp6Info;

/**
 * @brief VP6 decoded-picture metadata, exactly 0x24 bytes
 *
 * Normal decoder allocations reside in the client-provided LINEAR work buffer
 * Returned VAs identify client memory; bus addresses identify the same storage to hardware
 * Interpret addresses only for a valid picture, with the reported layout and any plane strides
 * Synchronize CPU access with hardware completion and cache maintenance; decoder buffers may be reused
 */
typedef struct
{
	u32 frame_width;           ///< @brief Stored frame width in pixels, including decoder alignment
	u32 frame_height;          ///< @brief Stored frame height in pixels, including decoder alignment
	u32 output_vaddr;          ///< @brief Client VA of decoded picture storage in the supplied work buffer
	u32 output_bus_address;    ///< @brief Hardware bus address of decoded picture storage
	u32 picture_id;            ///< @brief Picture identifier reported by the decoder
	u32 intra;                 ///< @brief Native intra-frame flag; some output paths explicitly report zero
	u32 golden;                ///< @brief Native golden-frame flag; this build can explicitly report zero
	u32 concealed_macroblocks; ///< @brief Number of macroblocks reported as concealed after errors
	u8 output_layout; ///< @brief MVD_LAYOUT_RASTER, MVD_LAYOUT_TILED_8X4, or the client MVD_LAYOUT_UNAVAILABLE sentinel
	u8 reserved[3];   ///< @brief Wire padding cleared by the client; carries no codec metadata
} MVDSTD_Vp6Picture;

/**
 * @brief Initializes MVD access to the caller-owned work allocation
 *
 * Open a raw session first and keep the work allocation alive until shutdown
 *
 * @param[in] work_buffer New-LINEAR client VA of the work allocation; retain it through Shutdown
 * @param[in] size Work allocation size in bytes
 * @return Zero on the normal success path, otherwise the service or transport error
 */
Result MVDSTD_Initialize(u32* work_buffer, u32 size);

/**
 * @brief Shuts down the service work-buffer state
 *
 * Detach and release any active decoder and PP components before this call
 * @return Zero on the normal success path, otherwise the service or transport error
 */
Result MVDSTD_Shutdown(void);

/**
 * @brief Queries the service H.264 work-buffer sizing helper
 *
 * @param[in] config H.264 sizing inputs; level.level must not exceed MVD_H264_LEVEL_5_2
 * @param[out] size Optional calculated byte count, cleared before the request
 * @return Zero on the normal success path, otherwise the service or transport error
 */
Result MVDSTD_CalculateWorkBufSize(const MVDSTD_CalculateWorkBufSizeConfig* config, u32* size);

/**
 * @brief Calculates the byte size of a wrapper-supported packed output image
 *
 * @param[in] width Image width in pixels
 * @param[in] height Image height in pixels
 * @param[in] format MVD_PIX_YUYV, MVD_PIX_RGB565 or MVD_PIX_BGR32
 * @param[out] size Optional calculated byte count, cleared before the request
 * @return Zero on the normal success path, otherwise the service or transport error
 */
Result MVDSTD_CalculateImageSize(u32 width, u32 height, MVDSTD_PixelFormat format, u32* size);

/**
 * @brief Initializes the session H.264 decoder
 *
 * @param[in] no_output_reordering Nonzero to disable output reordering
 * @param[in] freeze_concealment Nonzero to request freeze-picture error concealment
 * @param[in] display_smoothing Nonzero to allocate additional buffers for display smoothing
 * @param[in] reference_format Reference-storage flags, including MVD_REFERENCE_TILED and MVD_REFERENCE_FIELD_DPB
 * @return Service result or transport/client error; interpret codec events using MVD_STATUS_*
 */
Result MVDSTD_H264Initialize(s8 no_output_reordering, s8 freeze_concealment, s8 display_smoothing,
                             u32 reference_format);

/**
 * @brief Requests MVC mode, which is capability-disabled on the analyzed console
 * @return Service result or transport/client error; interpret codec events using MVD_STATUS_*
 */
Result MVDSTD_H264EnableMvc(void);

/**
 * @brief Releases the session H.264 decoder
 * @return Zero on the normal success path, otherwise the service or transport error
 */
Result MVDSTD_H264Release(void);

/**
 * @brief Submits compressed H.264 input and retrieves stream-consumption progress
 *
 * Progress is copied only for recognized events 0x17000..0x17007; other results leave it zeroed
 *
 * @param[in] stream_vaddr Client stream VA accessible to MVD; use the new LINEAR alias for LINEAR memory
 * @param[in] stream_bus_address Hardware bus address of the same compressed input
 * @param[in] size Compressed payload length in bytes
 * @param[in] picture_id Caller-supplied picture identifier carried through H.264 decoding
 * @param[in] skip_nonreference Nonzero to request skipping nonreference pictures
 * @param[out] out Optional progress record; end_vaddr is a freed module scratch address, not a client pointer
 * @return Service result or transport/client error; interpret codec events using MVD_STATUS_*
 */
Result MVDSTD_ProcessNALUnit(u32 stream_vaddr, u32 stream_bus_address, u32 size, u32 picture_id, u32 skip_nonreference,
                             MVDSTD_ProcessNALUnitOut* out);

/**
 * @brief Dequeues the next available H.264 picture
 *
 * @param[in] end_of_stream Nonzero to flush pending output; zero for normal dequeue
 * @param[out] picture Optional picture record, cleared before the request and populated only on
 * MVD_STATUS_PICTURE_READY
 * @return MVD_STATUS_PICTURE_READY for populated output, MVD_STATUS_OK for no picture, or an error
 */
Result MVDSTD_H264NextPicture(s8 end_of_stream, MVDSTD_H264Picture* picture);

/**
 * @brief Retrieves stream information for H.264
 *
 * @param[out] info Optional stream-information record, cleared before the request and populated only on MVD_STATUS_OK
 * @return MVD_STATUS_OK for valid metadata, otherwise the service or transport error
 */
Result MVDSTD_H264GetInfo(MVDSTD_H264Info* info);

/**
 * @brief Inspects an available picture without dequeuing H.264
 *
 * The unwritten view_id is cleared to a client placeholder; it is not observed metadata
 *
 * @param[out] picture Optional picture record, cleared before the request and populated only on
 * MVD_STATUS_PICTURE_READY
 * @return MVD_STATUS_PICTURE_READY for populated output, MVD_STATUS_OK for no picture, or an error
 */
Result MVDSTD_H264Peek(MVDSTD_H264Picture* picture);

/**
 * @brief Initializes the shared VP7, VP8 or WebP decoder
 *
 * @param[in] format VP7, VP8 or WebP selector
 * @param[in] freeze_concealment Nonzero to request freeze-picture error concealment
 * @param[in] buffer_count Requested frame buffers; clamped to 3..16 for VP6 or VP7, 4..16 for VP8; WebP uses one
 * @param[in] reference_format Reference-storage flags, including MVD_REFERENCE_TILED and MVD_REFERENCE_FIELD_DPB
 * @return Service result or transport/client error; interpret codec events using MVD_STATUS_*
 */
Result MVDSTD_Vp8Initialize(MVDSTD_Vp8Format format, s8 freeze_concealment, u32 buffer_count, u32 reference_format);

/**
 * @brief Releases the shared VP7, VP8 or WebP decoder
 * @return Zero on the normal success path, otherwise the service or transport error
 */
Result MVDSTD_Vp8Release(void);

/**
 * @brief Submits a compressed VP7, VP8 or WebP frame payload
 *
 * Pass a compressed frame payload, not an IVF or RIFF/WebP container
 * The extra native decode reply word is unused and is not exposed
 * Optional WebP picture VAs are forwarded without mapping them into module address space
 *
 * @param[in] input Eight-word VP8 input record with compressed stream and optional WebP output addresses
 * @return Service result or transport/client error; interpret codec events using MVD_STATUS_*
 */
Result MVDSTD_Vp8Decode(const MVDSTD_Vp8Input* input);

/**
 * @brief Dequeues the next available VP7/VP8/WebP picture
 *
 * @param[in] end_of_stream Nonzero to flush pending output; zero for normal dequeue
 * @param[out] picture Optional picture record, cleared before the request and populated only on
 * MVD_STATUS_PICTURE_READY
 * @return MVD_STATUS_PICTURE_READY for populated output, MVD_STATUS_OK for no picture, or an error
 */
Result MVDSTD_Vp8NextPicture(s8 end_of_stream, MVDSTD_Vp8Picture* picture);

/**
 * @brief Retrieves stream information for VP7/VP8/WebP
 *
 * @param[out] info Optional stream-information record, cleared before the request and populated only on MVD_STATUS_OK
 * @return MVD_STATUS_OK for valid metadata, otherwise the service or transport error
 */
Result MVDSTD_Vp8GetInfo(MVDSTD_Vp8Info* info);

/**
 * @brief Inspects an available picture without dequeuing VP7/VP8/WebP
 *
 * The unwritten layout is MVD_LAYOUT_UNAVAILABLE and slice_rows is a zero placeholder
 *
 * @param[out] picture Optional picture record, cleared before the request and populated only on
 * MVD_STATUS_PICTURE_READY
 * @return MVD_STATUS_PICTURE_READY for populated output, MVD_STATUS_OK for no picture, or an error
 */
Result MVDSTD_Vp8Peek(MVDSTD_Vp8Picture* picture);

/**
 * @brief Initializes the session VP6 decoder
 *
 * @param[in] freeze_concealment Nonzero to request freeze-picture error concealment
 * @param[in] buffer_count Requested frame buffers; clamped to 3..16 for VP6 or VP7, 4..16 for VP8; WebP uses one
 * @param[in] reference_format Reference-storage flags, including MVD_REFERENCE_TILED and MVD_REFERENCE_FIELD_DPB
 * @return Service result or transport/client error; interpret codec events using MVD_STATUS_*
 */
Result MVDSTD_Vp6Initialize(s8 freeze_concealment, u32 buffer_count, u32 reference_format);

/**
 * @brief Releases the session VP6 decoder
 * @return Zero on the normal success path, otherwise the service or transport error
 */
Result MVDSTD_Vp6Release(void);

/**
 * @brief Submits a compressed VP6 frame payload
 *
 * Pass a compressed VP6 frame payload, not an FLV container; the extra native output word is unused
 *
 * @param[in] stream_vaddr Client stream VA accessible to MVD; use the new LINEAR alias for LINEAR memory
 * @param[in] stream_bus_address Hardware bus address of the same compressed input
 * @param[in] size Compressed payload length in bytes
 * @return Service result or transport/client error; interpret codec events using MVD_STATUS_*
 */
Result MVDSTD_Vp6Decode(u32 stream_vaddr, u32 stream_bus_address, u32 size);

/**
 * @brief Dequeues the next available VP6 picture
 *
 * @param[in] end_of_stream Nonzero to flush pending output; zero for normal dequeue
 * @param[out] picture Optional picture record, cleared before the request and populated only on
 * MVD_STATUS_PICTURE_READY
 * @return MVD_STATUS_PICTURE_READY for populated output, MVD_STATUS_OK for no picture, or an error
 */
Result MVDSTD_Vp6NextPicture(s8 end_of_stream, MVDSTD_Vp6Picture* picture);

/**
 * @brief Retrieves stream information for VP6
 *
 * @param[out] info Optional stream-information record, cleared before the request and populated only on MVD_STATUS_OK
 * @return MVD_STATUS_OK for valid metadata, otherwise the service or transport error
 */
Result MVDSTD_Vp6GetInfo(MVDSTD_Vp6Info* info);

/**
 * @brief Inspects an available picture without dequeuing VP6
 *
 * The unwritten layout is MVD_LAYOUT_UNAVAILABLE
 *
 * @param[out] picture Optional picture record, cleared before the request and populated only on
 * MVD_STATUS_PICTURE_READY
 * @return MVD_STATUS_PICTURE_READY for populated output, MVD_STATUS_OK for no picture, or an error
 */
Result MVDSTD_Vp6Peek(MVDSTD_Vp6Picture* picture);

/**
 * @brief Initializes the session postprocessor
 * @return Service result or transport/client error; interpret codec events using MVD_STATUS_*
 */
Result MVDSTD_PpInitialize(void);

/**
 * @brief Releases the session postprocessor
 * @return Zero on the normal success path, otherwise the service or transport error
 */
Result MVDSTD_PpRelease(void);

/**
 * @brief Runs standalone PP or retrieves the stored combined-mode result
 *
 * With no decoder attached, the call runs PP and waits for completion
 * Unmapped native statuses, including PP_BUSY, can become raw zero in the service converter
 * @return Service result or transport/client error; interpret codec events using MVD_STATUS_*
 */
Result MVDSTD_PpGetResult(void);

/**
 * @brief Attaches the postprocessor to the active decoder
 *
 * @param[in] decoder_type MVD_PP_H264, MVD_PP_VP6, MVD_PP_VP8 or MVD_PP_WEBP
 * @return Service result or transport/client error; interpret codec events using MVD_STATUS_*
 */
Result MVDSTD_PpEnableCombinedMode(u8 decoder_type);

/**
 * @brief Detaches the postprocessor from the active decoder
 * @return Service result or transport/client error; interpret codec events using MVD_STATUS_*
 */
Result MVDSTD_PpDisableCombinedMode(void);

/**
 * @brief Retrieves the complete 0x11C-byte postprocessor configuration
 *
 * @param[out] config Destination for the complete postprocessor configuration
 * @return Service result or transport/client error; interpret codec events using MVD_STATUS_*
 */
Result MVDSTD_GetConfig(MVDSTD_Config* config);

/**
 * @brief Retrieves the saved client addresses for the selected PP output
 *
 * The service can leave the VA pair unwritten if no mapping matches, even on an OK result
 *
 * @param[out] output Optional client VA pair for the selected PP output; validate against the registered pool
 * @return Service result or transport/client error; interpret codec events using MVD_STATUS_*
 */
Result MVDSTD_GetNextOutput(MVDSTD_OutputBuffersEntry* output);

#ifdef __cplusplus
}
#endif
