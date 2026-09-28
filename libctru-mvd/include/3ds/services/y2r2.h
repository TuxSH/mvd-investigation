/* Altered libctru copy: recovered MVD y2r2:u interface; see libctru-mvd/README.md */
/**
 * @file y2r2.h
 * @brief MVD Y2R2 service for hardware YUV->RGB conversions
 */
#pragma once
#include <3ds/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Input color formats
 *
 * For the 16-bit per component formats, bits 15-8 are padding and 7-0 contains the value
 */
typedef enum
{
	Y2R2_INPUT_YUV422_INDIV_8  = 0x0, ///<  8-bit per component, planar YUV 4:2:2, 16bpp, (1 Cr & Cb sample per 2x1 Y samples).\n Usually named YUV422P
	Y2R2_INPUT_YUV420_INDIV_8  = 0x1, ///<  8-bit per component, planar YUV 4:2:0, 12bpp, (1 Cr & Cb sample per 2x2 Y samples).\n Usually named YUV420P
	Y2R2_INPUT_YUV422_INDIV_16 = 0x2, ///< 16-bit per component, planar YUV 4:2:2, 32bpp, (1 Cr & Cb sample per 2x1 Y samples).\n Usually named YUV422P16
	Y2R2_INPUT_YUV420_INDIV_16 = 0x3, ///< 16-bit per component, planar YUV 4:2:0, 24bpp, (1 Cr & Cb sample per 2x2 Y samples).\n Usually named YUV420P16
	Y2R2_INPUT_YUV422_BATCH    = 0x4, ///<  8-bit per component, packed YUV 4:2:2, 16bpp, (Y0 Cb Y1 Cr).\n Usually named YUYV422
} Y2R2U_InputFormat;

/**
 * @brief Output color formats
 *
 * Format 0 stores AA BB GG RR bytes, format 1 stores BB GG RR bytes
 * Component packing is independent of linear versus tiled pixel addressing
 */
typedef enum
{
	Y2R2_OUTPUT_RGB_32     = 0x0, ///< 32-bit RGBA8888; memory bytes AA BB GG RR. The alpha component is the 8-bit value set by @ref Y2R2U_SetAlpha
	Y2R2_OUTPUT_RGB_24     = 0x1, ///< 24-bit RGB888
	Y2R2_OUTPUT_RGB_16_555 = 0x2, ///< 16-bit RGBA5551. The alpha bit is the 7th bit of the alpha value set by @ref Y2R2U_SetAlpha
	Y2R2_OUTPUT_RGB_16_565 = 0x3, ///< 16-bit RGB565
} Y2R2U_OutputFormat;

/// Rotation to be applied to the output
typedef enum
{
	Y2R2_ROTATION_NONE          = 0x0, ///< No rotation
	Y2R2_ROTATION_CLOCKWISE_90  = 0x1, ///< Clockwise 90 degrees
	Y2R2_ROTATION_CLOCKWISE_180 = 0x2, ///< Clockwise 180 degrees
	Y2R2_ROTATION_CLOCKWISE_270 = 0x3, ///< Clockwise 270 degrees
} Y2R2U_Rotation;

/**
 * @brief Block alignment of output
 *
 * Defines the way the output will be laid out in memory
 */
typedef enum
{
	Y2R2_BLOCK_LINE   = 0x0, ///< The result buffer will be laid out in linear format, the usual way
	Y2R2_BLOCK_8_BY_8 = 0x1, ///< The result will be stored as 8x8 blocks in Z-order.\n Useful for textures since it is the format used by the PICA200
} Y2R2U_BlockAlignment;

/**
 * @brief Coefficients of the YUV->RGB conversion formula
 *
 * A set of coefficients configuring the YUV to RGB conversion. Coefficients 0-4 are unsigned 2.8
 * fixed point numbers representing entries on the conversion matrix, while coefficient 5-7 are
 * signed 11.5 fixed point numbers added as offsets to the RGB result
 *
 * The overall conversion process formula is:
 * @code
 * R = trunc((rgb_Y * Y           + r_V * V) + 0.75 + r_offset)
 * G = trunc((rgb_Y * Y - g_U * U - g_V * V) + 0.75 + g_offset)
 * B = trunc((rgb_Y * Y + b_U * U          ) + 0.75 + b_offset)
 * @endcode
 */
typedef struct
{
	u16 rgb_Y;    ///< RGB per unit Y
	u16 r_V;      ///< Red per unit V
	u16 g_V;      ///< Green per unit V
	u16 g_U;      ///< Green per unit U
	u16 b_U;      ///< Blue per unit U
	u16 r_offset; ///< Red offset
	u16 g_offset; ///< Green offset
	u16 b_offset; ///< Blue offset
} Y2R2U_ColorCoefficients;

/**
 * @brief Preset conversion coefficients based on ITU standards for the YUV->RGB formula
 *
 * For more details refer to @ref Y2R2U_ColorCoefficients
 */
typedef enum
{
	Y2R2_COEFFICIENT_ITU_R_BT_601         = 0x0, ///< Coefficients from the ITU-R BT.601 standard with PC ranges
	Y2R2_COEFFICIENT_ITU_R_BT_709         = 0x1, ///< Coefficients from the ITU-R BT.709 standard with PC ranges
	Y2R2_COEFFICIENT_ITU_R_BT_601_SCALING = 0x2, ///< Coefficients from the ITU-R BT.601 standard with TV ranges
	Y2R2_COEFFICIENT_ITU_R_BT_709_SCALING = 0x3, ///< Coefficients from the ITU-R BT.709 standard with TV ranges
} Y2R2U_StandardCoefficient;

/**
 * @brief Structure used to configure all parameters at once
 *
 * You can send a batch of configuration parameters using this structure and @ref Y2R2U_SetConversionParams
 */
typedef struct
{
	u8 input_format;           ///< Value passed to @ref Y2R2U_SetInputFormat
	u8 output_format;           ///< Value passed to @ref Y2R2U_SetOutputFormat
	u8 rotation;           ///< Value passed to @ref Y2R2U_SetRotation
	u8 block_alignment;           ///< Value passed to @ref Y2R2U_SetBlockAlignment
	s16 input_line_width;                              ///< Value passed to @ref Y2R2U_SetInputLineWidth
	s16 input_lines;                                   ///< Value passed to @ref Y2R2U_SetInputLines
	u8 standard_coefficient; ///< Value passed to @ref Y2R2U_SetStandardCoefficient
	u8 unused;                                         ///< Unused
	u16 alpha;                                         ///< Value passed to @ref Y2R2U_SetAlpha
} Y2R2U_ConversionParams;


/// Client validation failure, distinct from the service result codes below
#define Y2R2_ERROR_INVALID_ARGUMENT ((Result)-1)
#define Y2R2_ERROR_ALREADY_INITIALIZED ((Result)0xD82053F9)
#define Y2R2_ERROR_NOT_INITIALIZED ((Result)0xD82053F8)
#define Y2R2_ERROR_INVALID_ENGINE ((Result)0xE0E05002)
#define Y2R2_ERROR_INVALID_DIMENSIONS ((Result)0xE0E053FD)
#define Y2R2_ERROR_INVALID_COEFFICIENT ((Result)0xE0E053ED)
#define Y2R2_ERROR_CONVERSION_BLOCKED ((Result)0xC9405001)

/// Pointer arguments are required; getters clear outputs and copy only on service success
/// Open/close and conversion sequences must be serialized by the caller

/// Dithering weights
typedef struct
{
	u16 w0_xEven_yEven; ///< Weight 0 for even X, even Y
	u16 w0_xOdd_yEven;  ///< Weight 0 for odd X, even Y
	u16 w0_xEven_yOdd;  ///< Weight 0 for even X, odd Y
	u16 w0_xOdd_yOdd;   ///< Weight 0 for odd X, odd Y
	u16 w1_xEven_yEven; ///< Weight 1 for even X, even Y
	u16 w1_xOdd_yEven;  ///< Weight 1 for odd X, even Y
	u16 w1_xEven_yOdd;  ///< Weight 1 for even X, odd Y
	u16 w1_xOdd_yOdd;   ///< Weight 1 for odd X, odd Y
	u16 w2_xEven_yEven; ///< Weight 2 for even X, even Y
	u16 w2_xOdd_yEven;  ///< Weight 2 for odd X, even Y
	u16 w2_xEven_yOdd;  ///< Weight 2 for even X, odd Y
	u16 w2_xOdd_yOdd;   ///< Weight 2 for odd X, odd Y
	u16 w3_xEven_yEven; ///< Weight 3 for even X, even Y
	u16 w3_xOdd_yEven;  ///< Weight 3 for odd X, even Y
	u16 w3_xEven_yOdd;  ///< Weight 3 for even X, odd Y
	u16 w3_xOdd_yOdd;   ///< Weight 3 for odd X, odd Y
} Y2R2U_DitheringWeightParams;

/**
 * @brief Initializes the y2r2 service
 *
 * This will internally get the handle of the service, and on success call Y2R2U_DriverInitialize
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
 * @param format Input format to use
 *
 * @note Prefer using @ref Y2R2U_SetConversionParams if you have to set multiple parameters
 */
Result Y2R2U_SetInputFormat(Y2R2U_InputFormat format);

/**
 * @brief Gets the configured input format
 * @param format Pointer to output the input format to
 */
Result Y2R2U_GetInputFormat(Y2R2U_InputFormat* format);

/**
 * @brief Used to configure the output format
 * @param format Output format to use
 *
 * @note Prefer using @ref Y2R2U_SetConversionParams if you have to set multiple parameters
 */
Result Y2R2U_SetOutputFormat(Y2R2U_OutputFormat format);

/**
 * @brief Gets the configured output format
 * @param format Pointer to output the output format to
 */
Result Y2R2U_GetOutputFormat(Y2R2U_OutputFormat* format);

/**
 * @brief Used to configure the rotation of the output
 * @param rotation Rotation to use
 *
 * The inherited Y2R layout rotates groups of eight input lines; arrange the destination accordingly
 *
 * @note Prefer using @ref Y2R2U_SetConversionParams if you have to set multiple parameters
 */
Result Y2R2U_SetRotation(Y2R2U_Rotation rotation);

/**
 * @brief Gets the configured rotation
 * @param rotation Pointer to output the rotation to
 */
Result Y2R2U_GetRotation(Y2R2U_Rotation* rotation);

/**
 * @brief Used to configure the alignment of the output buffer
 * @param alignment Alignment to use
 *
 * @note Prefer using @ref Y2R2U_SetConversionParams if you have to set multiple parameters
 */
Result Y2R2U_SetBlockAlignment(Y2R2U_BlockAlignment alignment);

/**
 * @brief Gets the configured alignment
 * @param alignment Pointer to output the alignment to
 */
Result Y2R2U_GetBlockAlignment(Y2R2U_BlockAlignment* alignment);

/**
 * @brief Sets whether to use spacial dithering
 * @param enable Whether to use spacial dithering
 */
Result Y2R2U_SetSpacialDithering(bool enable);

/**
 * @brief Gets whether to use spacial dithering
 * @param enable Pointer to output the spacial dithering state to
 */
Result Y2R2U_GetSpacialDithering(bool* enabled);

/**
 * @brief Sets whether to use temporal dithering
 * @param enable Whether to use temporal dithering
 */
Result Y2R2U_SetTemporalDithering(bool enable);

/**
 * @brief Gets whether to use temporal dithering
 * @param enable Pointer to output the temporal dithering state to
 */
Result Y2R2U_GetTemporalDithering(bool* enabled);


/**
 * @brief Used to configure the width of the image
 * @param line_width Width of the image in pixels. Must be a multiple of 8, up to 1024
 *
 * @note Prefer using @ref Y2R2U_SetConversionParams if you have to set multiple parameters
 */
Result Y2R2U_SetInputLineWidth(u16 line_width);

/**
 * @brief Gets the raw input-width register (zero is the encoding written for 1024)
 * @param line_width Pointer to output the line width to
 */
Result Y2R2U_GetInputLineWidth(u16* line_width);

/**
 * @brief Used to configure the height of the image
 * @param num_lines Number of lines to be converted
 *
 * Use complete groups of eight lines for tiled output
 * Values 1..1023 write the register; exactly 1024 succeeds without changing it
 * This client rejects zero and values above 1024
 *
 * @note Prefer using @ref Y2R2U_SetConversionParams if you have to set multiple parameters
 */
Result Y2R2U_SetInputLines(u16 num_lines);

/**
 * @brief Gets the configured number of input lines
 * @param num_lines Pointer to output the input lines to
 */
Result Y2R2U_GetInputLines(u16* num_lines);

/**
 * @brief Used to configure the color conversion formula
 * @param coefficients Coefficients to use
 *
 * See @ref Y2R2U_ColorCoefficients for more information about the coefficients
 *
 * @note Prefer using @ref Y2R2U_SetConversionParams if you have to set multiple parameters
 */
Result Y2R2U_SetCoefficients(const Y2R2U_ColorCoefficients* coefficients);

/**
 * @brief Gets the configured color coefficients
 * @param num_lines Pointer to output the coefficients to
 */
Result Y2R2U_GetCoefficients(Y2R2U_ColorCoefficients* coefficients);

/**
 * @brief Used to configure the color conversion formula with ITU stantards coefficients
 * @param coefficient Standard coefficient to use
 *
 * See @ref Y2R2U_ColorCoefficients for more information about the coefficients
 *
 * @note Prefer using @ref Y2R2U_SetConversionParams if you have to set multiple parameters
 */
Result Y2R2U_SetStandardCoefficient(Y2R2U_StandardCoefficient coefficient);

/**
 * @brief Gets the color coefficient parameters of a standard coefficient
 * @param coefficients Pointer to output the coefficients to
 * @param standardCoeff Standard coefficient to check
 */
Result Y2R2U_GetStandardCoefficient(Y2R2U_ColorCoefficients* coefficients, Y2R2U_StandardCoefficient standardCoeff);

/**
 * @brief Used to configure the alpha value of the output
 * @param alpha 8-bit value to be used for the output when the format requires it
 *
 * @note Prefer using @ref Y2R2U_SetConversionParams if you have to set multiple parameters
 */
Result Y2R2U_SetAlpha(u16 alpha);

/**
 * @brief Gets the configured output alpha value
 * @param alpha Pointer to output the alpha value to
 */
Result Y2R2U_GetAlpha(u16* alpha);

/**
 * @brief Used to enable the end of conversion interrupt
 * @param should_interrupt Enables the interrupt if true, disable it if false
 *
 * The event follows the conversion interrupt, not a completed destination DMA
 * Enable explicitly, and check Y2R2U_IsDoneReceiving before using the output
 */
Result Y2R2U_SetTransferEndInterrupt(bool should_interrupt);

/**
 * @brief Gets whether the transfer end interrupt is enabled
 * @param should_interrupt Pointer to output the interrupt state to
 */
Result Y2R2U_GetTransferEndInterrupt(bool* should_interrupt);

/**
 * @brief Gets an handle to the end of conversion event
 * @param end_event Pointer receiving a new shared event handle; no existing handle is closed. The caller owns the returned handle and must close it with svcCloseHandle
 *
 * To enable this event you have to use @code{C} Y2R2U_SetTransferEndInterrupt(true);@endcode
 * The event will be triggered when the corresponding interrupt is fired
 *
 * @note Use a bounded wait and independently query destination DMA completion
 */
Result Y2R2U_GetTransferEndEvent(Handle* end_event);

/**
 * @brief Configures the Y plane buffer
 * @param src_buf A pointer to the beginning of your Y data buffer
 * @param image_size The total size of the data buffer
 * @param transfer_unit Specifies the size of 1 DMA transfer. Usually set to 1 line. This has to be a divisor of image_size
 * @param transfer_gap Specifies the gap (offset) to be added after each transfer. Can be used to convert images with stride or only a part of it
 *
 * @warning transfer_unit+transfer_gap must be less than 32768 (0x8000)
 *
 * This specifies the Y data buffer for the planar input formats (INPUT_YUV42*_INDIV_*)
 * The actual transfer will only happen after calling @ref Y2R2U_StartConversion
 */
Result Y2R2U_SetSendingY(const void* src_buf, u32 image_size, s16 transfer_unit, s16 transfer_gap);

/**
 * @brief Configures the U plane buffer
 * @param src_buf A pointer to the beginning of your Y data buffer
 * @param image_size The total size of the data buffer
 * @param transfer_unit Specifies the size of 1 DMA transfer. Usually set to 1 line. This has to be a divisor of image_size
 * @param transfer_gap Specifies the gap (offset) to be added after each transfer. Can be used to convert images with stride or only a part of it
 *
 * @warning transfer_unit+transfer_gap must be less than 32768 (0x8000)
 *
 * This specifies the U data buffer for the planar input formats (INPUT_YUV42*_INDIV_*)
 * The actual transfer will only happen after calling @ref Y2R2U_StartConversion
 */
Result Y2R2U_SetSendingU(const void* src_buf, u32 image_size, s16 transfer_unit, s16 transfer_gap);

/**
 * @brief Configures the V plane buffer
 * @param src_buf A pointer to the beginning of your Y data buffer
 * @param image_size The total size of the data buffer
 * @param transfer_unit Specifies the size of 1 DMA transfer. Usually set to 1 line. This has to be a divisor of image_size
 * @param transfer_gap Specifies the gap (offset) to be added after each transfer. Can be used to convert images with stride or only a part of it
 *
 * @warning transfer_unit+transfer_gap must be less than 32768 (0x8000)
 *
 * This specifies the V data buffer for the planar input formats (INPUT_YUV42*_INDIV_*)
 * The actual transfer will only happen after calling @ref Y2R2U_StartConversion
 */
Result Y2R2U_SetSendingV(const void* src_buf, u32 image_size, s16 transfer_unit, s16 transfer_gap);

/**
 * @brief Configures the YUYV source buffer
 * @param src_buf A pointer to the beginning of your Y data buffer
 * @param image_size The total size of the data buffer
 * @param transfer_unit Specifies the size of 1 DMA transfer. Usually set to 1 line. This has to be a divisor of image_size
 * @param transfer_gap Specifies the gap (offset) to be added after each transfer. Can be used to convert images with stride or only a part of it
 *
 * @warning transfer_unit+transfer_gap must be less than 32768 (0x8000)
 *
 * This specifies the YUYV data buffer for the packed input format @ref Y2R2_INPUT_YUV422_BATCH
 * The actual transfer will only happen after calling @ref Y2R2U_StartConversion
 */
Result Y2R2U_SetSendingYUYV(const void* src_buf, u32 image_size, s16 transfer_unit, s16 transfer_gap);

/**
 * @brief Configures the destination buffer
 * @param src_buf A pointer to the beginning of your destination buffer in FCRAM
 * @param image_size The total size of the data buffer
 * @param transfer_unit Specifies the size of 1 DMA transfer. Usually set to 1 line. This has to be a divisor of image_size
 * @param transfer_gap Specifies the gap (offset) to be added after each transfer. Can be used to convert images with stride or only a part of it
 *
 * This specifies the destination buffer of the conversion
 * The actual transfer will only happen after calling @ref Y2R2U_StartConversion
 * The buffer does NOT need to be allocated in the linear heap
 *
 * @warning transfer_unit+transfer_gap must be less than 32768 (0x8000)
 *
 * @note The upstream Y2R client reports premature events for some 400x240
 * transfers with a one-line transfer unit, and uses larger units as a workaround
 * That empirical advice has not been independently tested on this Y2R2 service
 * Check IsDoneReceiving, even after an event or an idle conversion query
 */
Result Y2R2U_SetReceiving(void* dst_buf, u32 image_size, s16 transfer_unit, s16 transfer_gap);

/**
 * @brief Checks if the DMA has finished sending the Y buffer
 * @param is_done Pointer to the boolean that will hold the result
 *
 * True if the DMA has finished transferring the Y plane, false otherwise. To be used with @ref Y2R2U_SetSendingY
 */
Result Y2R2U_IsDoneSendingY(bool* is_done);

/**
 * @brief Checks if the DMA has finished sending the U buffer
 * @param is_done Pointer to the boolean that will hold the result
 *
 * True if the DMA has finished transferring the U plane, false otherwise. To be used with @ref Y2R2U_SetSendingU
 */
Result Y2R2U_IsDoneSendingU(bool* is_done);

/**
 * @brief Checks if the DMA has finished sending the V buffer
 * @param is_done Pointer to the boolean that will hold the result
 *
 * True if the DMA has finished transferring the V plane, false otherwise. To be used with @ref Y2R2U_SetSendingV
 */
Result Y2R2U_IsDoneSendingV(bool* is_done);

/**
 * @brief Checks if the DMA has finished sending the YUYV buffer
 * @param is_done Pointer to the boolean that will hold the result
 *
 * True if the DMA has finished transferring the YUYV buffer, false otherwise. To be used with @ref Y2R2U_SetSendingYUYV
 */
Result Y2R2U_IsDoneSendingYUYV(bool* is_done);

/**
 * @brief Checks if the DMA has finished sending the converted result
 * @param is_done Pointer to the boolean that will hold the result
 *
 * True if the DMA wait reports completion or no DMA handle exists
 * A true result before a transfer was started does not establish valid output
 */
Result Y2R2U_IsDoneReceiving(bool* is_done);

/**
 * @brief Configures the dithering weight parameters
 * @param params Dithering weight parameters to use
 */
Result Y2R2U_SetDitheringWeightParams(const Y2R2U_DitheringWeightParams* params);

/**
 * @brief Gets the configured dithering weight parameters
 * @param params Pointer to output the dithering weight parameters to
 */
Result Y2R2U_GetDitheringWeightParams(Y2R2U_DitheringWeightParams* params);

/**
 * @brief Sets all of the parameters of Y2R2U_ConversionParams at once
 * @param params Conversion parameters to set
 *
 * Faster than calling the individual value through Y2R_Set* because only one system call is made
 */
Result Y2R2U_SetConversionParams(const Y2R2U_ConversionParams* params);

/// Starts the conversion process
Result Y2R2U_StartConversion(void);

/// Cancels the conversion
Result Y2R2U_StopConversion(void);

/**
 * @brief Checks conversion/DRQ state; separately query IsDoneReceiving for DMA completion
 * @param is_busy Pointer to output the busy state to
 *
 * This does not substitute for IsDoneReceiving; see Y2R2U_SetTransferEndInterrupt
 */
Result Y2R2U_IsBusyConversion(bool* is_busy);

/**
 * @brief Gets the session-count byte (normally one while connected)
 * @param ping Pointer to output the ready status to
 */
Result Y2R2U_PingProcess(u8* ping);

/// Initializes the Y2R driver
Result Y2R2U_DriverInitialize(void);

/// Terminates the Y2R driver
Result Y2R2U_DriverFinalize(void);


/** Read the 12-byte current parameter block (MVD command 0x2D)
 * standard_coefficient is 0..3 for a matching preset, or 4 for custom coefficients
 * The unused byte is cleared; unwritten excess IPC reply words are ignored
 * SetConversionParams rejects index 4; use SetCoefficients for custom coefficients
 */
Result Y2R2U_GetConversionParams(Y2R2U_ConversionParams* params);

/// Correctly spelled aliases for libctru's historical Spacial names
#define Y2R2U_SetSpatialDithering Y2R2U_SetSpacialDithering
#define Y2R2U_GetSpatialDithering Y2R2U_GetSpacialDithering

#ifdef __cplusplus
}
#endif
