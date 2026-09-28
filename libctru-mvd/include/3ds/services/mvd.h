/* Altered libctru copy: completed from MVD reverse engineering; see libctru-mvd/README.md */
/**
 * @file mvd.h
 * @brief MVD service
 */
#pragma once
#include <3ds/types.h>

#ifdef __cplusplus
extern "C" {
#endif

//New3DS-only, see also: http://3dbrew.org/wiki/MVD_Services

/** Codec events are positive Result values, not ordinary zero-only IPC success
 * Lifecycle and sizing calls can return zero; unknown native PP statuses also map to zero
 */
#define MVD_STATUS_OK                 0x17000
#define MVD_STATUS_STREAM_PROCESSED   0x17001
#define MVD_STATUS_PICTURE_READY      0x17002
#define MVD_STATUS_PICTURE_DECODED    0x17003
#define MVD_STATUS_HEADERS_READY      0x17004
#define MVD_STATUS_ADVANCED_TOOLS     0x17005
#define MVD_STATUS_PENDING_FLUSH      0x17006
#define MVD_STATUS_NONREF_SKIPPED     0x17007
#define MVD_STATUS_SLICE_READY        0x17038

/// Legacy aliases: BUSY actually means that NextPicture returned a picture
#define MVD_STATUS_PARAMSET MVD_STATUS_STREAM_PROCESSED
#define MVD_STATUS_BUSY MVD_STATUS_PICTURE_READY
#define MVD_STATUS_FRAMEREADY MVD_STATUS_PICTURE_DECODED
#define MVD_STATUS_INCOMPLETEPROCESSING MVD_STATUS_HEADERS_READY
#define MVD_STATUS_NALUPROCFLAG MVD_STATUS_NONREF_SKIPPED

static inline bool mvdstdIsLegacyDecodeSuccess(Result result)
{
 return result == MVD_STATUS_OK || result == MVD_STATUS_STREAM_PROCESSED ||
        result == MVD_STATUS_PICTURE_DECODED || result == MVD_STATUS_HEADERS_READY ||
        result == MVD_STATUS_NONREF_SKIPPED;
}
#define MVD_CHECKNALUPROC_SUCCESS(x) mvdstdIsLegacyDecodeSuccess(x)

#define MVD_ERROR_PARAMETER          ((Result)0xE16170C9)
#define MVD_ERROR_STREAM             ((Result)0xD96170CA)
#define MVD_ERROR_NOT_INITIALIZED    ((Result)0xD96170CB)
#define MVD_ERROR_MEMORY             ((Result)0xD86170CC)
#define MVD_ERROR_INITIALIZATION     ((Result)0xD96170CD)
#define MVD_ERROR_HEADERS            ((Result)0xD96170CE)
#define MVD_ERROR_STREAM_UNSUPPORTED ((Result)0xD96170D0)
#define MVD_ERROR_HW_RESERVED        ((Result)0xD96171C6)
#define MVD_ERROR_HW_TIMEOUT         ((Result)0xD96171C7)
#define MVD_ERROR_HW_BUS             ((Result)0xF96171C8)
#define MVD_ERROR_SYSTEM             ((Result)0xD96171C9)
#define MVD_ERROR_DWL                ((Result)0xD96171CA)
#define MVD_ERROR_COMBINED_MODE      ((Result)0xD96172C8)
#define MVD_ERROR_DECODER_RUNTIME    ((Result)0xD96172C9)
#define MVD_ERROR_EVALUATION_LIMIT   ((Result)0xD9617383)
#define MVD_ERROR_FORMAT_UNSUPPORTED ((Result)0xD9617384)


/// PP configuration errors returned by the native-to-IPC converter
#define MVD_ERROR_PP_INPUT_SIZE ((Result)0xD9617108)
#define MVD_ERROR_PP_INPUT_ADDRESS ((Result)0xD9617109)
#define MVD_ERROR_PP_INPUT_FORMAT ((Result)0xD961710A)
#define MVD_ERROR_PP_CROP ((Result)0xD961710B)
#define MVD_ERROR_PP_ROTATION ((Result)0xD961710C)
#define MVD_ERROR_PP_OUTPUT_SIZE ((Result)0xD961710D)
#define MVD_ERROR_PP_OUTPUT_ADDRESS ((Result)0xD961710E)
#define MVD_ERROR_PP_OUTPUT_FORMAT ((Result)0xD961710F)
#define MVD_ERROR_PP_VIDEO_ADJUSTMENT ((Result)0xD9617110)
#define MVD_ERROR_PP_RGB_MASKS ((Result)0xD9617111)
#define MVD_ERROR_PP_FRAMEBUFFER ((Result)0xD9617112)
#define MVD_ERROR_PP_MASK1 ((Result)0xD9617113)
#define MVD_ERROR_PP_MASK2 ((Result)0xD9617114)
#define MVD_ERROR_PP_DEINTERLACE ((Result)0xD9617115)
#define MVD_ERROR_PP_PICTURE_STRUCTURE ((Result)0xD9617116)
#define MVD_ERROR_PP_RANGE_MAPPING ((Result)0xD9617117)
#define MVD_ERROR_PP_ALPHA_BLEND_UNSUPPORTED ((Result)0xD9617118)
#define MVD_ERROR_PP_DEINTERLACE_UNSUPPORTED ((Result)0xD9617119)
#define MVD_ERROR_PP_DITHERING_UNSUPPORTED ((Result)0xD961711A)
#define MVD_ERROR_PP_SCALING_UNSUPPORTED ((Result)0xD961711B)

/// Default input size for mvdstdInit(). This is what the New3DS Internet Browser uses, from the MVDSTD:CalculateWorkBufSize output
#define MVD_DEFAULT_WORKBUF_SIZE 0x9006C8

#define MVD_CALC_WITH_LEVEL_FLAG_NONE				0x00	//Nothing
#define MVD_CALC_WITH_LEVEL_FLAG_ENABLE_CALC		0x01	//Enable calculation with level
#define MVD_CALC_WITH_LEVEL_FLAG_ENABLE_EXTRA_OP	0x02	//Add one sixth of the level storage candidate
#define MVD_CALC_WITH_LEVEL_FLAG_EXTRA_STORAGE_2 0x04
#define MVD_CALC_WITH_LEVEL_FLAG_UNK				0x04	//Same storage increment as bit 1

#define MVD_H264_LEVEL_1_0		0x00	//H.264 level 1.0
#define MVD_H264_LEVEL_1_0B		0x01	//H.264 level 1.0b
#define MVD_H264_LEVEL_1_1		0x02	//H.264 level 1.1
#define MVD_H264_LEVEL_1_2		0x03	//H.264 level 1.2
#define MVD_H264_LEVEL_1_3		0x04	//H.264 level 1.3
#define MVD_H264_LEVEL_2_0		0x05	//H.264 level 2.0
#define MVD_H264_LEVEL_2_1		0x06	//H.264 level 2.1
#define MVD_H264_LEVEL_2_2		0x07	//H.264 level 2.2
#define MVD_H264_LEVEL_3_0		0x08	//H.264 level 3.0
#define MVD_H264_LEVEL_3_1		0x09	//H.264 level 3.1
#define MVD_H264_LEVEL_3_2		0x0A	//H.264 level 3.2
#define MVD_H264_LEVEL_4_0		0x0B	//H.264 level 4.0
#define MVD_H264_LEVEL_4_1		0x0C	//H.264 level 4.1
#define MVD_H264_LEVEL_4_2		0x0D	//H.264 level 4.2
#define MVD_H264_LEVEL_5_0		0x0E	//H.264 level 5.0
#define MVD_H264_LEVEL_5_1		0x0F	//H.264 level 5.1
#define MVD_H264_LEVEL_5_2		0x10	//H.264 level 5.2

/// Processing mode
typedef enum {
	MVDMODE_COLORFORMATCONV, ///< Converting color formats
	MVDMODE_VIDEOPROCESSING  ///< Processing video
} MVDSTD_Mode;

/// Input format
typedef enum {
	MVD_INPUT_YUYV422 = 0x00010001, ///< YUYV422
	MVD_INPUT_YUV420SP = 0x00020001,
	MVD_INPUT_H264 = 0x00020001     ///< Decoded YCbCr 4:2:0 semiplanar input to PP, not a compressed codec selector
} MVDSTD_InputFormat;

/// Output format
typedef enum {
	MVD_OUTPUT_YUYV422 = 0x00010001, ///< YUYV422
	MVD_OUTPUT_BGR565 = 0x00040002,  ///< Legacy libctru name; Hantro calls this RGB565
	MVD_OUTPUT_RGB565 = 0x00040004, ///< Legacy libctru name; wrapper cache size is undefined
	MVD_OUTPUT_BGR32 = 0x00041002   ///< Hantro BGR32, a wrapper-supported cache size
} MVDSTD_OutputFormat;

/// One PP output mask / optional alpha-blend source, 0x2C bytes
 typedef struct {
 u32 enable;
 s32 origin_x, origin_y;
 u32 height, width;
 u32 alpha_blend_enable, blend_bus_address;
 s32 blend_origin_x, blend_origin_y;
 u32 blend_width, blend_height;
} MVDSTD_OutputMask;

/** Hantro PPConfig, exactly 0x11C bytes
 * Addresses are bus addresses; input_type describes decoded pixels
 * Legacy unknown names alias their recovered fields without changing the layout
 * The service cache-length calculation is defined only for output 0x10001, 0x40002, 0x41002
 * See RE/postprocessor.md for format, geometry and capability restrictions
 */
typedef struct {
 MVDSTD_InputFormat input_type;
 union { u32 input_picture_structure; u32 unk_x04; };
 union { u32 input_video_range; u32 unk_x08; };
 u32 inwidth, inheight;
 u32 physaddr_colorconv_indata;
 union { u32 input_cb_bus_address; u32 physaddr_colorconv_unk0; };
 union { u32 input_cr_bus_address; u32 physaddr_colorconv_unk1; };
 union { u32 input_bottom_bus_address; u32 physaddr_colorconv_unk2; };
 union { u32 input_bottom_chroma_bus_address; u32 physaddr_colorconv_unk3; };
 union {
  u32 unk_x28[6];
  struct {
   u32 vc1_multires_enable, vc1_range_reduction;
   u32 vc1_range_map_y_enable, vc1_range_map_y_coefficient;
   u32 vc1_range_map_c_enable, vc1_range_map_c_coefficient;
  };
 };
 u32 enable_cropping, input_crop_x_pos, input_crop_y_pos;
 u32 input_crop_height, input_crop_width;
 union { u32 rotation; u32 unk_x54; };
 MVDSTD_OutputFormat output_type;
 u32 outwidth, outheight, physaddr_outdata0, physaddr_outdata1;
 union {
  u32 unk_x6c[38];
  struct {
   u32 rgb_transform;
   s32 contrast, brightness, saturation;
   u32 alpha, transparency;
   u32 coefficient_a, coefficient_b, coefficient_c, coefficient_d, coefficient_e;
   u32 mask_r, mask_g, mask_b, mask_alpha;
   u32 dithering_enable;
   MVDSTD_OutputMask output_mask1, output_mask2;
  };
 };
 union { u32 framebuffer_enable; u32 flag_x104; };
 s32 output_x_pos, output_y_pos;
 u32 output_width_override, output_height_override;
 union { u32 deinterlace_enable; u32 unk_x118; };
} MVDSTD_Config;

typedef struct {
	u32 end_vaddr;///< End address in freed module scratch memory; never dereference in the client
	u32 end_physaddr;///< Input bus address plus consumed bytes
	u32 remaining_size;///< Unconsumed bytes; advance the original client input by input_size - remaining_size
} MVDSTD_ProcessNALUnitOut;

typedef struct {
	void* outdata0;//Linearmem vaddr equivalent to config *_outdata0
	void* outdata1;//Linearmem vaddr equivalent to config *_outdata1
} MVDSTD_OutputBuffersEntry;

typedef struct {
	u32 total_entries;//Total actual used entries below
	MVDSTD_OutputBuffersEntry entries[17];
} MVDSTD_OutputBuffersEntryList;

/// This can be used to override the default input values for MVDSTD commands during initialization with video-processing. The default for these fields are all-zero, except for cmd1b_inval which is 1. See also here: https://www.3dbrew.org/wiki/MVD_Services
typedef struct {
	union { s8 no_output_reordering; s8 cmd5_inval0; };
	union { s8 freeze_concealment; s8 cmd5_inval1; };
	union { s8 display_smoothing; s8 cmd5_inval2; };
	union { u32 reference_frame_format; u32 cmd5_inval3; };

	union { u8 pp_decoder_type; u8 cmd1b_inval; };
} MVDSTD_InitStruct;

typedef struct
{
	u8 enable;			//Whether use this calculation method
	u8 flag;			//Flag for calculation (MVD_CALC_WITH_LEVEL_FLAG_XXXXXX)
	u8 double_size;		//If set, size calculation result is doubled
	u8 level;			//H.264 (AVC) level (MVD_H264_LEVEL_1_0 ~ MVD_H264_LEVEL_5_2)
} MVDSTD_WithLevel;

typedef struct
{
	u8 enable;			//Whether use this calculation method
	u8 ref_frames;		//Number of reference frames
} MVDSTD_WithNumOfRefFrames;

/// H.264 buffer calculation configuration
/// See here for detailed explanations : https://www.3dbrew.org/wiki/MVDSTD:CalculateWorkBufSize
typedef struct {
	u8 unused_0x00;								//Unused
	MVDSTD_WithLevel level;						//Calc buffer size with H.264 level
	MVDSTD_WithNumOfRefFrames ref_frames_a;		//Calc buffer size with num of reference frames and resolution
	MVDSTD_WithNumOfRefFrames ref_frames_b;		//Calc buffer size with num of reference frames and resolution
	u8 unused_0x09[3];							//Unused
	union { u32 ignored_0x0c; u32 unk_0x0c; };								//Ignored by this build
	union { u32 ignored_0x10; u32 unk_0x10; };								//Ignored by this build
	union { u32 ignored_0x14; u32 unk_0x14; };								//Ignored by this build
	union { u32 ignored_0x18; u32 unk_0x18; };								//Ignored by this build
	union { u32 ignored_0x1c; u32 unk_0x1c; };								//Ignored by this build
	union { u32 ignored_0x20; u32 unk_0x20; };								//Ignored by this build
	union { u32 ignored_0x24; u32 unk_0x24; };								//Ignored by this build
	u32 width;									//Video width
	u32 height;									//Video height
} MVDSTD_CalculateWorkBufSizeConfig;

/**
 * @brief Initializes MVDSTD using standalone PP or H.264 plus PP
 * Raw VP6/VP8/WebP callers should use mvdstdOpen and explicit codec lifecycle calls
 * Repeated initialization retains the first configuration and increments the reference count
 * @param mode Mode to initialize MVDSTD to
 * @param input_type Type of input to process
 * @param output_type Type of output to produce
 * @param size Size of the work buffer, MVD_DEFAULT_WORKBUF_SIZE can be used for this. Only used when type == MVDMODE_VIDEOPROCESSING
 * @param initstruct Optional MVDSTD_InitStruct, this should be NULL normally
 */
Result mvdstdInit(MVDSTD_Mode mode, MVDSTD_InputFormat input_type, MVDSTD_OutputFormat output_type, u32 size, MVDSTD_InitStruct *initstruct);

/// Shuts down MVDSTD
void mvdstdExit(void);

/**
 * @brief Calculate working buffer size for H.264 decoding
 * @param config Calculation config, config->level.level must NOT exceed MVD_H264_LEVEL_5_2. See here for more explanations : https://www.3dbrew.org/wiki/MVDSTD:CalculateWorkBufSize
 * @param size_out Calculated buffer size in bytes
 */
Result mvdstdCalculateBufferSize(const MVDSTD_CalculateWorkBufSizeConfig* config, u32* size_out);

/**
 * @brief Generates a default MVDSTD configuration
 * @param config Pointer to output the generated config to
 * @param input_width Input width
 * @param input_height Input height
 * @param output_width Output width
 * @param output_height Output height
 * @param vaddr_colorconv_indata Virtual address of the color conversion input data
 * @param vaddr_outdata0 Virtual address of the output data
 * @param vaddr_outdata1 Additional virtual address for output data, only used when the output format type is value 0x00020001
 */
void mvdstdGenerateDefaultConfig(MVDSTD_Config*config, u32 input_width, u32 input_height, u32 output_width, u32 output_height, u32 *vaddr_colorconv_indata, u32 *vaddr_outdata0, u32 *vaddr_outdata1);

/**
 * @brief Run color-format-conversion
 * @param config Pointer to the configuration to use
 */
Result mvdstdConvertImage(MVDSTD_Config* config);

/**
 * @brief Processes a video frame(specifically a NAL-unit)
 * @param inbuf_vaddr Input NAL-unit starting with the 3-byte "00 00 01" prefix. Must be located in linearmem
 * @param size Size of the input buffer
 * @param flag See here regarding this input flag: https://www.3dbrew.org/wiki/MVDSTD:ProcessNALUnit
 * @param out Optional output MVDSTD_ProcessNALUnitOut structure
 */
Result mvdstdProcessVideoFrame(void* inbuf_vaddr, size_t size, u32 flag, MVDSTD_ProcessNALUnitOut *out);

/**
 * @brief Renders the video frame
 * @param config Optional pointer to the configuration to use. When NULL, MVDSTD_SetConfig() should have been used previously for this video
 * @param wait When true, dequeue until no picture is ready; this is not a hardware-busy poll. False dequeues at most one picture
 */
Result mvdstdRenderVideoFrame(MVDSTD_Config* config, bool wait);

/**
 * @brief Sets the current configuration of MVDSTD
 * @param config Pointer to the configuration to set
 */
Result MVDSTD_SetConfig(MVDSTD_Config* config);

/**
 * @brief New3DS Internet Browser doesn't use this. Once done, rendered frames will be written to the output buffers specified by the entrylist instead of the output specified by configuration. See here: https://www.3dbrew.org/wiki/MVDSTD:SetupOutputBuffers
 * @param entrylist Input entrylist
 * @param bufsize Size of each buffer from the entrylist
 */
Result mvdstdSetupOutputBuffers(MVDSTD_OutputBuffersEntryList *entrylist, u32 bufsize);

/**
 * @brief New3DS Internet Browser doesn't use this. This replaces the current display-index buffer pair, subject to address matching; only the first extra replacement mapping is recorded. See also here: https://www.3dbrew.org/wiki/MVDSTD:OverrideOutputBuffers
 * @param cur_outdata0 Linearmem vaddr. The current outdata0 for this entry must match this value
 * @param cur_outdata1 Linearmem vaddr. The current outdata1 for this entry must match this value
 * @param new_outdata0 Linearmem vaddr. This is the new address to use for outaddr0
 * @param new_outdata1 Linearmem vaddr. This is the new address to use for outaddr1
 */
Result mvdstdOverrideOutputBuffers(void* cur_outdata0, void* cur_outdata1, void* new_outdata0, void* new_outdata1);

/// Raw service access without allocating a work buffer or selecting a codec
/// Pair Open/Close separately from Init/Exit; do not mix the two lifecycles
Result mvdstdOpen(void);
/// Release codec/PP and call MVDSTD_Shutdown before closing a raw session
void mvdstdClose(void);

/// Reference storage flags for codec initialization, not PP pixel formats
#define MVD_REFERENCE_TILED 1u
#define MVD_REFERENCE_FIELD_DPB (1u << 30)

typedef enum {
 MVD_VP7 = 1, MVD_VP8 = 2, MVD_WEBP = 3
} MVDSTD_Vp8Format;

typedef enum {
 MVD_PP_H264 = 1, MVD_PP_VP6 = 6, MVD_PP_VP8 = 9, MVD_PP_WEBP = 10
} MVDSTD_PpDecoder;

/// Hantro format names; values are not an all-modes support matrix
/// Existing MVD_OUTPUT_RGB565/BGR565 names use the opposite naming convention
 typedef enum {
 MVD_PIX_YUYV = 0x10001, MVD_PIX_YVYU = 0x10005,
 MVD_PIX_UYVY = 0x10006, MVD_PIX_VYUY = 0x10007,
 MVD_PIX_YUV422SP = 0x10002, MVD_PIX_YUV440 = 0x10004,
 MVD_PIX_YUYV_TILED4 = 0x10008, MVD_PIX_YVYU_TILED4 = 0x10009,
 MVD_PIX_UYVY_TILED4 = 0x1000A, MVD_PIX_VYUY_TILED4 = 0x1000B,
 MVD_PIX_YUV420P = 0x20000, MVD_PIX_YUV420SP = 0x20001,
 MVD_PIX_YUV420_TILED = 0x20002, MVD_PIX_MONOCHROME = 0x80000,
 MVD_PIX_YUV411SP = 0x100001, MVD_PIX_YUV444SP = 0x200001,
 MVD_PIX_CUSTOM_RGB16 = 0x40000, MVD_PIX_RGB555 = 0x40001,
 MVD_PIX_RGB565 = 0x40002, MVD_PIX_BGR555 = 0x40003,
 MVD_PIX_BGR565 = 0x40004, MVD_PIX_CUSTOM_RGB32 = 0x41000,
 MVD_PIX_RGB32 = 0x41001, MVD_PIX_BGR32 = 0x41002
} MVDSTD_PixelFormat;

typedef enum {
 MVD_ROTATION_NONE = 0, MVD_ROTATION_RIGHT_90 = 1, MVD_ROTATION_LEFT_90 = 2,
 MVD_ROTATION_FLIP_HORIZONTAL = 3, MVD_ROTATION_FLIP_VERTICAL = 4, MVD_ROTATION_180 = 5
} MVDSTD_Rotation;

typedef enum {
 MVD_PICTURE_FRAME_OR_TOP = 0, MVD_PICTURE_BOTTOM = 1,
 MVD_PICTURE_SEPARATE_FIELDS = 2, MVD_PICTURE_FIELDS_IN_FRAME = 3,
 MVD_PICTURE_TOP_IN_FRAME = 4, MVD_PICTURE_BOTTOM_IN_FRAME = 5
} MVDSTD_PictureStructure;

typedef enum {
 MVD_RGB_CUSTOM = 0, MVD_RGB_BT601 = 1, MVD_RGB_BT709 = 2
} MVDSTD_RgbTransform;

#define MVD_LAYOUT_RASTER 0
#define MVD_LAYOUT_TILED_8X4 1
#define MVD_LAYOUT_UNAVAILABLE 0xFF

/// Fixed-width IPC records: *_vaddr fields are 32-bit addresses, not host pointers
/// Decoder picture addresses belong to the module; use GetNextOutput for client PP addresses
 typedef struct {
 u32 width, height, video_range, matrix_coefficients;
 u32 crop_left, crop_width, crop_top, crop_height;
 u32 output_format, sar_width, sar_height, monochrome, interlaced_sequence;
 s32 dpb_mode;
 u32 picture_buffer_count, multibuffer_pp_count;
} MVDSTD_H264Info;

typedef struct {
 u32 width, height, crop_left, crop_width, crop_top, crop_height;
 u32 output_vaddr, output_bus_address, picture_id, is_idr, concealed_macroblocks;
 u32 interlaced, field_picture, top_field, view_id;
 u8 output_layout;
 u8 reserved[3];
} MVDSTD_H264Picture;

typedef struct {
 u32 stream_vaddr, stream_bus_address, size;
 u32 slice_height; ///< WebP slice height in macroblock rows; zero disables slice output
 u32 luma_vaddr, luma_bus_address, chroma_vaddr, chroma_bus_address;
} MVDSTD_Vp8Input;

typedef struct {
 u32 version, profile, coded_width, coded_height, frame_width, frame_height;
 u32 scaled_width, scaled_height;
 u8 constant_zero; ///< Zero on this build; original field meaning remains unknown
 u8 reserved[3];
 u32 output_format;
} MVDSTD_Vp8Info;

typedef struct {
 u32 coded_width, coded_height, frame_width, frame_height, luma_stride, chroma_stride;
 u32 luma_vaddr, luma_bus_address, chroma_vaddr, chroma_bus_address;
 u32 picture_id, intra, golden, concealed_macroblocks, slice_rows;
 u8 output_layout;
 u8 reserved[3];
} MVDSTD_Vp8Picture;

typedef struct {
 u32 version, profile, frame_width, frame_height, scaled_width, scaled_height, scaling_mode;
 u8 constant_zero; ///< Zero on this build; original field meaning remains unknown
 u8 reserved[3];
 u32 output_format;
} MVDSTD_Vp6Info;

typedef struct {
 u32 frame_width, frame_height, output_vaddr, output_bus_address;
 u32 picture_id, intra, golden, concealed_macroblocks;
 u8 output_layout;
 u8 reserved[3];
} MVDSTD_Vp6Picture;

/** Low-level commands, in service order
 * Call mvdstdOpen first for explicit lifecycle management
 * Only one decoder family can exist in the session at a time
 * Keep the LINEAR work buffer alive through Shutdown and use its new LINEAR alias
 * Init/size/release commands can return zero; codec events use MVD_STATUS_*
 * Output pointers may be NULL to discard replies; input records may not be NULL
 * Info is copied only for MVD_STATUS_OK; pictures only for MVD_STATUS_PICTURE_READY
 * Output records are cleared before the request, including server padding
 * H264Peek clears its unwritten view_id; VP6/VP8Peek mark layout UNAVAILABLE
 * VP8Peek clears its unwritten slice_rows; zero here is a client placeholder
 */
Result MVDSTD_Initialize(u32* work_buffer, u32 size);
Result MVDSTD_Shutdown(void);
Result MVDSTD_CalculateWorkBufSize(const MVDSTD_CalculateWorkBufSizeConfig* config, u32* size);
/// Only YUYV, Hantro RGB565 and Hantro BGR32 are supported by this helper
Result MVDSTD_CalculateImageSize(u32 width, u32 height, MVDSTD_PixelFormat format, u32* size);
Result MVDSTD_H264Initialize(s8 no_output_reordering, s8 freeze_concealment, s8 display_smoothing, u32 reference_format);
/// Present in the ABI but returns unsupported format on this console
Result MVDSTD_H264EnableMvc(void);
Result MVDSTD_H264Release(void);
/// Stream is staged in module memory; end_vaddr is not a usable client pointer
Result MVDSTD_ProcessNALUnit(u32 stream_vaddr, u32 stream_bus_address, u32 size, u32 picture_id, u32 skip_nonreference, MVDSTD_ProcessNALUnitOut* out);
Result MVDSTD_H264NextPicture(s8 end_of_stream, MVDSTD_H264Picture* picture);
Result MVDSTD_H264GetInfo(MVDSTD_H264Info* info);
Result MVDSTD_H264Peek(MVDSTD_H264Picture* picture);
Result MVDSTD_Vp8Initialize(MVDSTD_Vp8Format format, s8 freeze_concealment, u32 buffer_count, u32 reference_format);
Result MVDSTD_Vp8Release(void);
/// The additional native output word is unused and intentionally not exposed
/// Supply compressed VP7/VP8 frame payloads, not an IVF or RIFF/WebP container
/// Optional WebP picture pointers are forwarded, not mapped into module VA space
Result MVDSTD_Vp8Decode(const MVDSTD_Vp8Input* input);
Result MVDSTD_Vp8NextPicture(s8 end_of_stream, MVDSTD_Vp8Picture* picture);
Result MVDSTD_Vp8GetInfo(MVDSTD_Vp8Info* info);
Result MVDSTD_Vp8Peek(MVDSTD_Vp8Picture* picture);
Result MVDSTD_Vp6Initialize(s8 freeze_concealment, u32 buffer_count, u32 reference_format);
Result MVDSTD_Vp6Release(void);
/// Supply a compressed VP6 frame payload; the extra reply word is unused
Result MVDSTD_Vp6Decode(u32 stream_vaddr, u32 stream_bus_address, u32 size);
Result MVDSTD_Vp6NextPicture(s8 end_of_stream, MVDSTD_Vp6Picture* picture);
Result MVDSTD_Vp6GetInfo(MVDSTD_Vp6Info* info);
Result MVDSTD_Vp6Peek(MVDSTD_Vp6Picture* picture);
Result MVDSTD_PpInitialize(void);
Result MVDSTD_PpRelease(void);
/// Runs standalone PP synchronously, or returns the stored combined-mode result
Result MVDSTD_PpGetResult(void);
Result MVDSTD_PpEnableCombinedMode(u8 decoder_type);
Result MVDSTD_PpDisableCombinedMode(void);
Result MVDSTD_GetConfig(MVDSTD_Config* config);
/// Get the selected PP output's saved client VA pair, after SetupOutputBuffers
Result MVDSTD_GetNextOutput(MVDSTD_OutputBuffersEntry* output);

#ifdef __cplusplus
}
#endif
