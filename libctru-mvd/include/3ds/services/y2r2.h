/* Altered libctru copy: recovered MVD y2r2:u interface; see libctru-mvd/README.md */
/**
 * @file y2r2.h
 * @brief MVD Y2R2 service for hardware YUV->RGB conversions
 */
#pragma once
#include <3ds/types.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief YUV input layouts supported by Y2R2
 *
 * Sixteen-bit containers store the eight-bit component in bits 7..0; bits 15..8 are padding
 */
typedef enum
{
	/// @brief 8-bit per component, planar YUV 4:2:2, 16bpp, (1 Cr & Cb sample per 2x1 Y samples).\n Usually named
	/// YUV422P
	Y2R2_INPUT_YUV422_INDIV_8 = 0x0,
	/// @brief 8-bit per component, planar YUV 4:2:0, 12bpp, (1 Cr & Cb sample per 2x2 Y samples).\n Usually named
	/// YUV420P
	Y2R2_INPUT_YUV420_INDIV_8 = 0x1,
	/// @brief 16-bit per component, planar YUV 4:2:2, 32bpp, (1 Cr & Cb sample per 2x1 Y samples).\n Usually named
	/// YUV422P16
	Y2R2_INPUT_YUV422_INDIV_16 = 0x2,
	/// @brief 16-bit per component, planar YUV 4:2:0, 24bpp, (1 Cr & Cb sample per 2x2 Y samples).\n Usually named
	/// YUV420P16
	Y2R2_INPUT_YUV420_INDIV_16 = 0x3,
	/// @brief 8-bit per component, packed YUV 4:2:2, 16bpp, (Y0 Cb Y1 Cr).\n Usually named YUYV422
	Y2R2_INPUT_YUV422_BATCH = 0x4,
} Y2R2U_InputFormat;

/**
 * @brief Y2R2 packed RGB output formats
 */
typedef enum
{
	/// @brief 32-bit RGBA8888; memory bytes AA BB GG RR. The alpha component is the 8-bit value set by @ref
	/// Y2R2U_SetAlpha
	Y2R2_OUTPUT_RGB_32 = 0x0,
	/// @brief 24-bit RGB888
	Y2R2_OUTPUT_RGB_24 = 0x1,
	/// @brief 16-bit RGBA5551. The alpha bit is the 7th bit of the alpha value set by @ref Y2R2U_SetAlpha
	Y2R2_OUTPUT_RGB_16_555 = 0x2,
	/// @brief 16-bit RGB565
	Y2R2_OUTPUT_RGB_16_565 = 0x3,
} Y2R2U_OutputFormat;

/**
 * @brief Rotation applied to output in groups of eight input lines
 */
typedef enum
{
	/// @brief No rotation
	Y2R2_ROTATION_NONE = 0x0,
	/// @brief Clockwise 90 degrees
	Y2R2_ROTATION_CLOCKWISE_90 = 0x1,
	/// @brief Clockwise 180 degrees
	Y2R2_ROTATION_CLOCKWISE_180 = 0x2,
	/// @brief Clockwise 270 degrees
	Y2R2_ROTATION_CLOCKWISE_270 = 0x3,
} Y2R2U_Rotation;

/**
 * @brief Linear or 8x8 tiled pixel addressing
 */
typedef enum
{
	/// @brief The result buffer will be laid out in linear format, the usual way
	Y2R2_BLOCK_LINE = 0x0,
	/// @brief The result will be stored as 8x8 blocks in Z-order.\n Useful for textures since it is the format used by
	/// the PICA200
	Y2R2_BLOCK_8_BY_8 = 0x1,
} Y2R2U_BlockAlignment;

/**
 * @brief Eight fixed-point YUV-to-RGB conversion coefficients
 *
 * The first five values are unsigned 2.8 coefficients and the last three encode
 * signed 11.5 offsets; the inherited reference formula includes a 0.75 bias
 * @code
 * R = trunc(rgb_Y * Y + r_V * V + 0.75 + r_offset)
 * G = trunc(rgb_Y * Y - g_U * U - g_V * V + 0.75 + g_offset)
 * B = trunc(rgb_Y * Y + b_U * U + 0.75 + b_offset)
 * @endcode
 */
typedef struct
{
	u16 rgb_Y;    ///< @brief Unsigned 2.8 fixed-point coefficient applied to Y in all three channels
	u16 r_V;      ///< @brief Unsigned 2.8 fixed-point Cr-to-red coefficient
	u16 g_V;      ///< @brief Unsigned 2.8 fixed-point Cr coefficient subtracted from green
	u16 g_U;      ///< @brief Unsigned 2.8 fixed-point Cb coefficient subtracted from green
	u16 b_U;      ///< @brief Unsigned 2.8 fixed-point Cb-to-blue coefficient
	u16 r_offset; ///< @brief Raw halfword encoding of the signed 11.5 fixed-point red offset
	u16 g_offset; ///< @brief Raw halfword encoding of the signed 11.5 fixed-point green offset
	u16 b_offset; ///< @brief Raw halfword encoding of the signed 11.5 fixed-point blue offset
} Y2R2U_ColorCoefficients;

/**
 * @brief Standard YUV-to-RGB conversion coefficient presets
 */
typedef enum
{
	/// @brief Coefficients from the ITU-R BT.601 standard with PC ranges
	Y2R2_COEFFICIENT_ITU_R_BT_601 = 0x0,
	/// @brief Coefficients from the ITU-R BT.709 standard with PC ranges
	Y2R2_COEFFICIENT_ITU_R_BT_709 = 0x1,
	/// @brief Coefficients from the ITU-R BT.601 standard with TV ranges
	Y2R2_COEFFICIENT_ITU_R_BT_601_SCALING = 0x2,
	/// @brief Coefficients from the ITU-R BT.709 standard with TV ranges
	Y2R2_COEFFICIENT_ITU_R_BT_709_SCALING = 0x3,
} Y2R2U_StandardCoefficient;

/**
 * @brief Twelve-byte Y2R2 conversion-parameter package
 */
typedef struct
{
	u8 input_format;         ///< @brief Y2R2U_InputFormat value encoded in one byte
	u8 output_format;        ///< @brief Y2R2U_OutputFormat value encoded in one byte
	u8 rotation;             ///< @brief Y2R2U_Rotation value encoded in one byte
	u8 block_alignment;      ///< @brief Y2R2U_BlockAlignment value encoded in one byte
	s16 input_line_width;    ///< @brief Positive input width in pixels, a multiple of eight and at most 1024
	s16 input_lines;         ///< @brief Line count 1..1023; setting 1024 succeeds without changing the register
	u8 standard_coefficient; ///< @brief Preset index 0..3; getters return 4 for a custom matrix, which setters reject
	u8 unused;               ///< @brief Unused wire byte; cleared by the client
	u16 alpha;               ///< @brief Low-byte output alpha; RGBA5551 uses bit 7
} Y2R2U_ConversionParams;

/// @brief Client-side argument validation failed; not a native service result
#define Y2R2_ERROR_INVALID_ARGUMENT ((Result) - 1)
/// @brief Native driver or session is already initialized
#define Y2R2_ERROR_ALREADY_INITIALIZED ((Result)0xD82053F9)
/// @brief Native decoder or service state is not initialized
#define Y2R2_ERROR_NOT_INITIALIZED ((Result)0xD82053F8)
/// @brief Native driver rejected the engine selector
#define Y2R2_ERROR_INVALID_ENGINE ((Result)0xE0E05002)
/// @brief Native driver rejected the requested dimensions
#define Y2R2_ERROR_INVALID_DIMENSIONS ((Result)0xE0E053FD)
/// @brief Native driver rejected the standard-coefficient index
#define Y2R2_ERROR_INVALID_COEFFICIENT ((Result)0xE0E053ED)
/// @brief Conversion is blocked by the native driver state
#define Y2R2_ERROR_CONVERSION_BLOCKED ((Result)0xC9405001)

/// Pointer arguments are required; getters clear outputs and copy only on service success
/// Open/close and conversion sequences must be serialized by the caller

/**
 * @brief Sixteen two-bit dithering weights, stored as 32 wire bytes
 */
typedef struct
{
	u16 w0_xEven_yEven; ///< @brief Two-bit dithering weight for phase 0, even X and even Y
	u16 w0_xOdd_yEven;  ///< @brief Two-bit dithering weight for phase 0, odd X and even Y
	u16 w0_xEven_yOdd;  ///< @brief Two-bit dithering weight for phase 0, even X and odd Y
	u16 w0_xOdd_yOdd;   ///< @brief Two-bit dithering weight for phase 0, odd X and odd Y
	u16 w1_xEven_yEven; ///< @brief Two-bit dithering weight for phase 1, even X and even Y
	u16 w1_xOdd_yEven;  ///< @brief Two-bit dithering weight for phase 1, odd X and even Y
	u16 w1_xEven_yOdd;  ///< @brief Two-bit dithering weight for phase 1, even X and odd Y
	u16 w1_xOdd_yOdd;   ///< @brief Two-bit dithering weight for phase 1, odd X and odd Y
	u16 w2_xEven_yEven; ///< @brief Two-bit dithering weight for phase 2, even X and even Y
	u16 w2_xOdd_yEven;  ///< @brief Two-bit dithering weight for phase 2, odd X and even Y
	u16 w2_xEven_yOdd;  ///< @brief Two-bit dithering weight for phase 2, even X and odd Y
	u16 w2_xOdd_yOdd;   ///< @brief Two-bit dithering weight for phase 2, odd X and odd Y
	u16 w3_xEven_yEven; ///< @brief Two-bit dithering weight for phase 3, even X and even Y
	u16 w3_xOdd_yEven;  ///< @brief Two-bit dithering weight for phase 3, odd X and even Y
	u16 w3_xEven_yOdd;  ///< @brief Two-bit dithering weight for phase 3, even X and odd Y
	u16 w3_xOdd_yOdd;   ///< @brief Two-bit dithering weight for phase 3, odd X and odd Y
} Y2R2U_DitheringWeightParams;

/**
 * @brief Initializes the y2r2 service
 *
 * This will internally get the handle of the service, and on success call Y2R2U_DriverInitialize
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result y2r2Init(void);

/**
 * @brief Closes the y2r2 service
 *
 * This will internally call Y2R2U_DriverFinalize and close the handle of the service
 */
void y2r2Exit(void);

/**
 * @brief Used to configure the input format
 *
 * @note Prefer using @ref Y2R2U_SetConversionParams if you have to set multiple parameters
 *
 * @param[in] format Input YUV format from Y2R2U_InputFormat
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_SetInputFormat(Y2R2U_InputFormat format);

/**
 * @brief Gets the configured input format
 *
 * @param[out] format Required destination for input YUV format from Y2R2U_InputFormat
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_GetInputFormat(Y2R2U_InputFormat* format);

/**
 * @brief Used to configure the output format
 *
 * @note Prefer using @ref Y2R2U_SetConversionParams if you have to set multiple parameters
 *
 * @param[in] format Output RGB format from Y2R2U_OutputFormat
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_SetOutputFormat(Y2R2U_OutputFormat format);

/**
 * @brief Gets the configured output format
 *
 * @param[out] format Required destination for output RGB format from Y2R2U_OutputFormat
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_GetOutputFormat(Y2R2U_OutputFormat* format);

/**
 * @brief Used to configure the rotation of the output
 *
 * The inherited Y2R layout rotates groups of eight input lines; arrange the destination accordingly
 * @note Prefer using @ref Y2R2U_SetConversionParams if you have to set multiple parameters
 *
 * @param[in] rotation Rotation selector from Y2R2U_Rotation
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_SetRotation(Y2R2U_Rotation rotation);

/**
 * @brief Gets the configured rotation
 *
 * @param[out] rotation Required destination for rotation selector from Y2R2U_Rotation
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_GetRotation(Y2R2U_Rotation* rotation);

/**
 * @brief Used to configure the alignment of the output buffer
 *
 * @note Prefer using @ref Y2R2U_SetConversionParams if you have to set multiple parameters
 *
 * @param[in] alignment Linear or 8x8 tiled output addressing
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_SetBlockAlignment(Y2R2U_BlockAlignment alignment);

/**
 * @brief Gets the configured alignment
 *
 * @param[out] alignment Required destination for linear or 8x8 tiled output addressing
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_GetBlockAlignment(Y2R2U_BlockAlignment* alignment);

/**
 * @brief Sets whether to use spacial dithering
 *
 * @param[in] enable True to enable dithering, false to disable it
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_SetSpacialDithering(bool enable);

/**
 * @brief Gets whether to use spacial dithering
 *
 * @param[out] enabled Required destination for current dithering enable state
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_GetSpacialDithering(bool* enabled);

/**
 * @brief Sets whether to use temporal dithering
 *
 * @param[in] enable True to enable dithering, false to disable it
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_SetTemporalDithering(bool enable);

/**
 * @brief Gets whether to use temporal dithering
 *
 * @param[out] enabled Required destination for current dithering enable state
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_GetTemporalDithering(bool* enabled);

/**
 * @brief Used to configure the width of the image
 *
 * @note Prefer using @ref Y2R2U_SetConversionParams if you have to set multiple parameters
 *
 * @param[in] line_width Positive width in pixels, divisible by eight and at most 1024
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_SetInputLineWidth(u16 line_width);

/**
 * @brief Gets the raw input-width register (zero is the encoding written for 1024)
 *
 * @param[out] line_width Required destination for raw width register; zero is the encoding written for 1024
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_GetInputLineWidth(u16* line_width);

/**
 * @brief Used to configure the height of the image
 *
 * Use complete groups of eight lines for tiled output
 * Values 1..1023 write the register; exactly 1024 succeeds without changing it
 * This client rejects zero and values above 1024
 * @note Prefer using @ref Y2R2U_SetConversionParams if you have to set multiple parameters
 *
 * @param[in] num_lines Line count 1..1023; 1024 succeeds without changing the register
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_SetInputLines(u16 num_lines);

/**
 * @brief Gets the configured number of input lines
 *
 * @param[out] num_lines Required destination for current raw line-count register
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_GetInputLines(u16* num_lines);

/**
 * @brief Used to configure the color conversion formula
 *
 * See @ref Y2R2U_ColorCoefficients for more information about the coefficients
 * @note Prefer using @ref Y2R2U_SetConversionParams if you have to set multiple parameters
 *
 * @param[in] coefficients Eight fixed-point conversion coefficients
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_SetCoefficients(const Y2R2U_ColorCoefficients* coefficients);

/**
 * @brief Gets the configured color coefficients
 *
 * @param[out] coefficients Required destination for eight fixed-point conversion coefficients
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_GetCoefficients(Y2R2U_ColorCoefficients* coefficients);

/**
 * @brief Used to configure the color conversion formula with ITU stantards coefficients
 *
 * See @ref Y2R2U_ColorCoefficients for more information about the coefficients
 * @note Prefer using @ref Y2R2U_SetConversionParams if you have to set multiple parameters
 *
 * @param[in] coefficient Preset coefficient index 0..3
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_SetStandardCoefficient(Y2R2U_StandardCoefficient coefficient);

/**
 * @brief Gets the color coefficient parameters of a standard coefficient
 *
 * @param[out] coefficients Required destination for eight fixed-point conversion coefficients
 * @param[in] standardCoeff Preset coefficient index 0..3 to retrieve
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_GetStandardCoefficient(Y2R2U_ColorCoefficients* coefficients, Y2R2U_StandardCoefficient standardCoeff);

/**
 * @brief Used to configure the alpha value of the output
 *
 * @note Prefer using @ref Y2R2U_SetConversionParams if you have to set multiple parameters
 *
 * @param[in] alpha Alpha value; only the low byte is stored and RGBA5551 uses bit 7
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_SetAlpha(u16 alpha);

/**
 * @brief Gets the configured output alpha value
 *
 * @param[out] alpha Required destination for alpha value; only the low byte is stored and RGBA5551 uses bit 7
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_GetAlpha(u16* alpha);

/**
 * @brief Used to enable the end of conversion interrupt
 *
 * The event follows the conversion interrupt, not a completed destination DMA
 * Enable explicitly, and check Y2R2U_IsDoneReceiving before using the output
 *
 * @param[in] should_interrupt Conversion interrupt enable state
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_SetTransferEndInterrupt(bool should_interrupt);

/**
 * @brief Gets whether the transfer end interrupt is enabled
 *
 * @param[out] should_interrupt Required destination for conversion interrupt enable state
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_GetTransferEndInterrupt(bool* should_interrupt);

/**
 * @brief Gets an handle to the end of conversion event
 *
 * The caller owns the new handle; no preexisting handle in the output variable is closed
 * Use a bounded event wait and separately query Y2R2U_IsDoneReceiving
 *
 * @param[out] end_event Required destination for newly shared event handle owned by the caller; close it with
 * svcCloseHandle
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_GetTransferEndEvent(Handle* end_event);

/**
 * @brief Configures the Y plane buffer
 *
 * @warning transfer_unit+transfer_gap must be less than 32768 (0x8000)
 * This specifies the Y data buffer for the planar input formats (INPUT_YUV42*_INDIV_*)
 * The actual transfer will only happen after calling @ref Y2R2U_StartConversion
 *
 * @param[in] src_buf Client source VA of the Y plane
 * @param[in] image_size Nonzero total transfer length in bytes
 * @param[in] transfer_unit Positive bytes per DMA unit; choose a divisor of image_size
 * @param[in] transfer_gap Signed byte offset added after each DMA unit
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_SetSendingY(const void* src_buf, u32 image_size, s16 transfer_unit, s16 transfer_gap);

/**
 * @brief Configures the U plane buffer
 *
 * @warning transfer_unit+transfer_gap must be less than 32768 (0x8000)
 * This specifies the U data buffer for the planar input formats (INPUT_YUV42*_INDIV_*)
 * The actual transfer will only happen after calling @ref Y2R2U_StartConversion
 *
 * @param[in] src_buf Client source VA of the U plane
 * @param[in] image_size Nonzero total transfer length in bytes
 * @param[in] transfer_unit Positive bytes per DMA unit; choose a divisor of image_size
 * @param[in] transfer_gap Signed byte offset added after each DMA unit
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_SetSendingU(const void* src_buf, u32 image_size, s16 transfer_unit, s16 transfer_gap);

/**
 * @brief Configures the V plane buffer
 *
 * @warning transfer_unit+transfer_gap must be less than 32768 (0x8000)
 * This specifies the V data buffer for the planar input formats (INPUT_YUV42*_INDIV_*)
 * The actual transfer will only happen after calling @ref Y2R2U_StartConversion
 *
 * @param[in] src_buf Client source VA of the V plane
 * @param[in] image_size Nonzero total transfer length in bytes
 * @param[in] transfer_unit Positive bytes per DMA unit; choose a divisor of image_size
 * @param[in] transfer_gap Signed byte offset added after each DMA unit
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_SetSendingV(const void* src_buf, u32 image_size, s16 transfer_unit, s16 transfer_gap);

/**
 * @brief Configures the YUYV source buffer
 *
 * @warning transfer_unit+transfer_gap must be less than 32768 (0x8000)
 * This specifies the YUYV data buffer for the packed input format @ref Y2R2_INPUT_YUV422_BATCH
 * The actual transfer will only happen after calling @ref Y2R2U_StartConversion
 *
 * @param[in] src_buf Client source VA of the packed YUYV image
 * @param[in] image_size Nonzero total transfer length in bytes
 * @param[in] transfer_unit Positive bytes per DMA unit; choose a divisor of image_size
 * @param[in] transfer_gap Signed byte offset added after each DMA unit
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_SetSendingYUYV(const void* src_buf, u32 image_size, s16 transfer_unit, s16 transfer_gap);

/**
 * @brief Configures the destination buffer
 *
 * This specifies the destination buffer of the conversion
 * The actual transfer will only happen after calling @ref Y2R2U_StartConversion
 * The buffer does NOT need to be allocated in the linear heap
 * @warning transfer_unit+transfer_gap must be less than 32768 (0x8000)
 * @note The upstream Y2R client reports premature events for some 400x240
 * transfers with a one-line transfer unit, and uses larger units as a workaround
 * That empirical advice has not been independently tested on this Y2R2 service
 * Check IsDoneReceiving, even after an event or an idle conversion query
 *
 * @param[out] dst_buf Client destination VA in FCRAM; retain through DMA completion
 * @param[in] image_size Nonzero total transfer length in bytes
 * @param[in] transfer_unit Positive bytes per DMA unit; choose a divisor of image_size
 * @param[in] transfer_gap Signed byte offset added after each DMA unit
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_SetReceiving(void* dst_buf, u32 image_size, s16 transfer_unit, s16 transfer_gap);

/**
 * @brief Checks if the DMA has finished sending the Y buffer
 *
 * A missing DMA handle also reports done; this does not prove that a transfer took place
 *
 * @param[out] is_done Required destination for true when the DMA handle is signaled or absent
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_IsDoneSendingY(bool* is_done);

/**
 * @brief Checks if the DMA has finished sending the U buffer
 *
 * A missing DMA handle also reports done; this does not prove that a transfer took place
 *
 * @param[out] is_done Required destination for true when the DMA handle is signaled or absent
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_IsDoneSendingU(bool* is_done);

/**
 * @brief Checks if the DMA has finished sending the V buffer
 *
 * A missing DMA handle also reports done; this does not prove that a transfer took place
 *
 * @param[out] is_done Required destination for true when the DMA handle is signaled or absent
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_IsDoneSendingV(bool* is_done);

/**
 * @brief Checks if the DMA has finished sending the YUYV buffer
 *
 * A missing DMA handle also reports done; this does not prove that a transfer took place
 *
 * @param[out] is_done Required destination for true when the DMA handle is signaled or absent
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_IsDoneSendingYUYV(bool* is_done);

/**
 * @brief Checks if the DMA has finished sending the converted result
 *
 * A missing DMA handle also reports done; this does not prove that a transfer took place
 *
 * @param[out] is_done Required destination for true when the DMA handle is signaled or absent
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_IsDoneReceiving(bool* is_done);

/**
 * @brief Configures the dithering weight parameters
 *
 * @param[in] params Sixteen dithering weights
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_SetDitheringWeightParams(const Y2R2U_DitheringWeightParams* params);

/**
 * @brief Gets the configured dithering weight parameters
 *
 * @param[out] params Required destination for sixteen dithering weights
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_GetDitheringWeightParams(Y2R2U_DitheringWeightParams* params);

/**
 * @brief Sets all of the parameters of Y2R2U_ConversionParams at once
 *
 * The wire request contains three normal words; the unused byte is cleared
 * Fields are applied sequentially without rollback on a service error
 * Preset index 4 is not accepted; use Y2R2U_SetCoefficients for a custom matrix
 *
 * @param[in] params Twelve-byte conversion parameter record
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_SetConversionParams(const Y2R2U_ConversionParams* params);

/**
 * @brief Starts conversion with the current settings and DMA buffers
 *
 * Starts the conversion process
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_StartConversion(void);

/**
 * @brief Stops conversion without closing the DMA handles
 *
 * Cancels the conversion
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_StopConversion(void);

/**
 * @brief Checks conversion/DRQ state; separately query IsDoneReceiving for DMA completion
 *
 * This does not substitute for IsDoneReceiving; see Y2R2U_SetTransferEndInterrupt
 *
 * @param[out] is_busy Required destination for conversion/DRQ busy state; not a substitute for destination DMA
 * completion
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_IsBusyConversion(bool* is_busy);

/**
 * @brief Gets the session-count byte (normally one while connected)
 *
 * @param[out] ping Required destination for service session-count byte, normally one while connected
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_PingProcess(u8* ping);

/**
 * @brief Initializes the Y2R2 register driver
 *
 * Initializes the Y2R driver
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_DriverInitialize(void);

/**
 * @brief Stops and closes DMA handles and finalizes the Y2R2 driver
 *
 * Terminates the Y2R driver
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_DriverFinalize(void);

/**
 * @brief Retrieves the twelve-byte current conversion parameter block
 *
 * The returned standard_coefficient is 0..3 for a preset or 4 for a custom matrix
 * Padding byte 9 is cleared and the four unwritten advertised reply words are ignored
 *
 * @param[out] params Required destination for twelve-byte conversion parameter record
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result Y2R2U_GetConversionParams(Y2R2U_ConversionParams* params);

/// @brief Correctly spelled alias for Y2R2U_SetSpacialDithering
#define Y2R2U_SetSpatialDithering Y2R2U_SetSpacialDithering
/// @brief Correctly spelled alias for Y2R2U_GetSpacialDithering
#define Y2R2U_GetSpatialDithering Y2R2U_GetSpacialDithering

#ifdef __cplusplus
}
#endif
