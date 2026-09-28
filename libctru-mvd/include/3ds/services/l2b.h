/** @file l2b.h
 * New clients for the MVD l2b:u and l2b2:u RGB linear-to-tiled converters
 * These services are distinct from the Hantro postprocessor
 */
#pragma once
#include <3ds/types.h>
#ifdef __cplusplus
extern "C"
{
#endif

/// @brief Client-side argument validation failed; not a native service result
#define L2B_ERROR_INVALID_ARGUMENT ((Result) - 1)
/// @brief Native driver or session is already initialized
#define L2B_ERROR_ALREADY_INITIALIZED ((Result)0xD8216FF9)
/// @brief Native decoder or service state is not initialized
#define L2B_ERROR_NOT_INITIALIZED ((Result)0xD8216FF8)
/// @brief Native driver rejected the engine selector
#define L2B_ERROR_INVALID_ENGINE ((Result)0xE0E16C02)
/// @brief Native driver rejected the requested dimensions
#define L2B_ERROR_INVALID_DIMENSIONS ((Result)0xE0E16FFD)
/// @brief Conversion is blocked by the native driver state
#define L2B_ERROR_CONVERSION_BLOCKED ((Result)0xC9416C01)

/**
 * @brief Selects one of the two independent L2B services
 */
typedef enum
{
	/// @brief First L2B engine, exposed as l2b:u
	L2B_ENGINE_0 = 0,
	/// @brief Second L2B engine, exposed as l2b2:u
	L2B_ENGINE_1 = 1,
} L2B_Engine;
/**
 * @brief RGB formats used by L2B, independent of Hantro PP format identifiers
 */
typedef enum
{
	/// @brief Memory bytes AA BB GG RR; incoming alpha is ignored
	L2B_RGBA8888 = 0,
	/// @brief Memory bytes BB GG RR
	L2B_RGB888 = 1,
	/// @brief R bits 15..11, G 10..6, B 5..1 and A bit 0; incoming alpha is ignored
	L2B_RGBA5551 = 2,
	/// @brief R bits 15..11, G 10..5 and B 4..0
	L2B_RGB565 = 3,
} L2BU_Format;

/**
 * @brief Eight-byte conversion-parameter package shared by both L2B engines
 */
typedef struct
{
	u8 input_format;      ///< @brief Input L2BU_Format value; input alpha is ignored
	u8 output_format;     ///< @brief Output L2BU_Format value
	s16 input_line_width; ///< @brief Positive input width in pixels, a multiple of eight and at most 1024
	s16 input_lines;      ///< @brief Positive line count, a multiple of eight and at most 1024
	u16 alpha;            ///< @brief Low-byte output alpha; RGBA5551 uses bit 7
} L2BU_ConversionParams;

/**
 * @brief Acquires a reference to the selected L2B service
 *
 * Engine 0 opens l2b:u; engine 1 opens l2b2:u
 *
 * @param[in] engine L2B_ENGINE_0 for l2b:u or L2B_ENGINE_1 for l2b2:u
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result l2bInit(L2B_Engine engine);

/**
 * @brief Releases a reference and closes the selected service after the last user exits
 *
 * @param[in] engine L2B_ENGINE_0 for l2b:u or L2B_ENGINE_1 for l2b2:u
 */
void l2bExit(L2B_Engine engine);

/**
 * @brief Sets the input RGB format
 *
 * @param[in] engine L2B_ENGINE_0 for l2b:u or L2B_ENGINE_1 for l2b2:u
 * @param[in] value RGB pixel format from L2BU_Format
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result L2BU_SetInputFormat(L2B_Engine engine, L2BU_Format value);

/**
 * @brief Retrieves the input RGB format
 *
 * @param[in] engine L2B_ENGINE_0 for l2b:u or L2B_ENGINE_1 for l2b2:u
 * @param[out] value Required destination for RGB pixel format from L2BU_Format
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result L2BU_GetInputFormat(L2B_Engine engine, L2BU_Format* value);

/**
 * @brief Sets the output RGB format
 *
 * @param[in] engine L2B_ENGINE_0 for l2b:u or L2B_ENGINE_1 for l2b2:u
 * @param[in] value RGB pixel format from L2BU_Format
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result L2BU_SetOutputFormat(L2B_Engine engine, L2BU_Format value);

/**
 * @brief Retrieves the output RGB format
 *
 * @param[in] engine L2B_ENGINE_0 for l2b:u or L2B_ENGINE_1 for l2b2:u
 * @param[out] value Required destination for RGB pixel format from L2BU_Format
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result L2BU_GetOutputFormat(L2B_Engine engine, L2BU_Format* value);

/**
 * @brief Sets the conversion interrupt enable state
 *
 * @param[in] engine L2B_ENGINE_0 for l2b:u or L2B_ENGINE_1 for l2b2:u
 * @param[in] value true to enable conversion interrupts, false to disable them
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result L2BU_SetTransferEndInterrupt(L2B_Engine engine, bool value);

/**
 * @brief Retrieves the conversion interrupt enable state
 *
 * @param[in] engine L2B_ENGINE_0 for l2b:u or L2B_ENGINE_1 for l2b2:u
 * @param[out] value Required destination for the current conversion-interrupt enable state
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result L2BU_GetTransferEndInterrupt(L2B_Engine engine, bool* value);

/**
 * @brief Sets the input line width
 *
 * @param[in] engine L2B_ENGINE_0 for l2b:u or L2B_ENGINE_1 for l2b2:u
 * @param[in] value positive pixel count, divisible by eight and no greater than 1024
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result L2BU_SetInputLineWidth(L2B_Engine engine, u16 value);

/**
 * @brief Retrieves the input line width
 *
 * @param[in] engine L2B_ENGINE_0 for l2b:u or L2B_ENGINE_1 for l2b2:u
 * @param[out] value Required destination for raw dimension register; zero is the encoding written for 1024
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result L2BU_GetInputLineWidth(L2B_Engine engine, u16* value);

/**
 * @brief Sets the input line count
 *
 * @param[in] engine L2B_ENGINE_0 for l2b:u or L2B_ENGINE_1 for l2b2:u
 * @param[in] value positive pixel count, divisible by eight and no greater than 1024
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result L2BU_SetInputLines(L2B_Engine engine, u16 value);

/**
 * @brief Retrieves the input line count
 *
 * @param[in] engine L2B_ENGINE_0 for l2b:u or L2B_ENGINE_1 for l2b2:u
 * @param[out] value Required destination for raw dimension register; zero is the encoding written for 1024
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result L2BU_GetInputLines(L2B_Engine engine, u16* value);

/**
 * @brief Sets the output alpha register
 *
 * @param[in] engine L2B_ENGINE_0 for l2b:u or L2B_ENGINE_1 for l2b2:u
 * @param[in] value alpha value; only the low byte is stored, and RGBA5551 uses its high bit
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result L2BU_SetAlpha(L2B_Engine engine, u16 value);

/**
 * @brief Retrieves the output alpha register
 *
 * @param[in] engine L2B_ENGINE_0 for l2b:u or L2B_ENGINE_1 for l2b2:u
 * @param[out] value Required destination for alpha value; only the low byte is stored, and RGBA5551 uses its high bit
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result L2BU_GetAlpha(L2B_Engine engine, u16* value);

/**
 * @brief Queries input DMA completion
 *
 * A zero DMA handle also reports done; this does not establish that a transfer was started
 *
 * @param[in] engine L2B_ENGINE_0 for l2b:u or L2B_ENGINE_1 for l2b2:u
 * @param[out] value Required destination for true when the DMA handle is signaled or absent
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result L2BU_IsDoneSending(L2B_Engine engine, bool* value);

/**
 * @brief Queries output DMA completion
 *
 * A zero DMA handle also reports done; this does not establish that a transfer was started
 *
 * @param[in] engine L2B_ENGINE_0 for l2b:u or L2B_ENGINE_1 for l2b2:u
 * @param[out] value Required destination for true when the DMA handle is signaled or absent
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result L2BU_IsDoneReceiving(L2B_Engine engine, bool* value);

/**
 * @brief Queries conversion/DRQ busy state
 *
 * An idle conversion state does not prove destination DMA completion; query L2BU_IsDoneReceiving
 *
 * @param[in] engine L2B_ENGINE_0 for l2b:u or L2B_ENGINE_1 for l2b2:u
 * @param[out] value Required destination for true when the conversion/DRQ state reports busy
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result L2BU_IsBusyConversion(L2B_Engine engine, bool* value);

/**
 * @brief Retrieves the service session-count byte
 *
 * @param[in] engine L2B_ENGINE_0 for l2b:u or L2B_ENGINE_1 for l2b2:u
 * @param[out] value Required destination for session-count byte, normally one while the service is open
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result L2BU_PingProcess(L2B_Engine engine, u8* value);

/**
 * @brief Starts conversion using the configured buffers and formats
 *
 * @param[in] engine L2B_ENGINE_0 for l2b:u or L2B_ENGINE_1 for l2b2:u
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result L2BU_StartConversion(L2B_Engine engine);

/**
 * @brief Stops conversion without closing the DMA handles
 *
 * @param[in] engine L2B_ENGINE_0 for l2b:u or L2B_ENGINE_1 for l2b2:u
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result L2BU_StopConversion(L2B_Engine engine);

/**
 * @brief Configures input DMA for the current process
 *
 * @param[in] engine L2B_ENGINE_0 for l2b:u or L2B_ENGINE_1 for l2b2:u
 * @param[in] buffer Client source VA retained through DMA completion
 * @param[in] size Nonzero transfer length in bytes
 * @param[in] transfer_unit Positive bytes per DMA transfer unit; choose a divisor of the transfer length
 * @param[in] transfer_gap Signed byte offset added after each transfer unit
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result L2BU_SetSending(L2B_Engine engine, const void* buffer, u32 size, s16 transfer_unit, s16 transfer_gap);

/**
 * @brief Configures output DMA for the current process
 *
 * @param[in] engine L2B_ENGINE_0 for l2b:u or L2B_ENGINE_1 for l2b2:u
 * @param[out] buffer Client destination VA retained through DMA completion
 * @param[in] size Nonzero transfer length in bytes
 * @param[in] transfer_unit Positive bytes per DMA transfer unit; choose a divisor of the transfer length
 * @param[in] transfer_gap Signed byte offset added after each transfer unit
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result L2BU_SetReceiving(L2B_Engine engine, void* buffer, u32 size, s16 transfer_unit, s16 transfer_gap);

/**
 * @brief Retrieves a new shared conversion-event handle
 *
 * The caller owns the returned handle and must close it; no preexisting handle is closed
 *
 * @param[in] engine L2B_ENGINE_0 for l2b:u or L2B_ENGINE_1 for l2b2:u
 * @param[out] event Required destination for the newly shared handle; cleared before the request
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result L2BU_GetTransferEndEvent(L2B_Engine engine, Handle* event);

/**
 * @brief Applies the eight-byte conversion parameter package
 *
 * Fields are applied sequentially; the server does not roll back on a later failure
 *
 * @param[in] engine L2B_ENGINE_0 for l2b:u or L2B_ENGINE_1 for l2b2:u
 * @param[in] params Required eight-byte conversion parameter record
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result L2BU_SetPackageParameter(L2B_Engine engine, const L2BU_ConversionParams* params);

/**
 * @brief Retrieves the eight-byte conversion parameter package
 *
 * Only the eight written bytes are copied; excess advertised reply words are ignored
 *
 * @param[in] engine L2B_ENGINE_0 for l2b:u or L2B_ENGINE_1 for l2b2:u
 * @param[out] params Required eight-byte conversion parameter record
 * @return Zero on success, otherwise a service, transport or client validation error
 */
Result L2BU_GetPackageParameter(L2B_Engine engine, L2BU_ConversionParams* params);
#ifdef __cplusplus
}
#endif
