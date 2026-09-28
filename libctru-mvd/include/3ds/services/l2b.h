/** @file l2b.h
 * New clients for the MVD l2b:u and l2b2:u RGB linear-to-tiled converters
 * These services are distinct from the Hantro postprocessor
 */
#pragma once
#include <3ds/types.h>
#ifdef __cplusplus
extern "C" {
#endif

/// Client validation failure, distinct from the service result codes below
#define L2B_ERROR_INVALID_ARGUMENT ((Result)-1)
#define L2B_ERROR_ALREADY_INITIALIZED ((Result)0xD8216FF9)
#define L2B_ERROR_NOT_INITIALIZED ((Result)0xD8216FF8)
#define L2B_ERROR_INVALID_ENGINE ((Result)0xE0E16C02)
#define L2B_ERROR_INVALID_DIMENSIONS ((Result)0xE0E16FFD)
#define L2B_ERROR_CONVERSION_BLOCKED ((Result)0xC9416C01)

typedef enum { L2B_ENGINE_0 = 0, L2B_ENGINE_1 = 1 } L2B_Engine;
typedef enum {
 L2B_RGBA8888 = 0, ///< Bytes AA BB GG RR
 L2B_RGB888 = 1,   ///< Bytes BB GG RR
 L2B_RGBA5551 = 2, ///< R bits 15..11, G 10..6, B 5..1, A bit 0
 L2B_RGB565 = 3    ///< R bits 15..11, G 10..5, B 4..0
} L2BU_Format;

/// Eight-byte wire structure; input alpha is ignored in favor of the alpha register
 typedef struct {
 u8 input_format, output_format;
 s16 input_line_width, input_lines;
 u16 alpha;
} L2BU_ConversionParams;

/// Reference-counted open; engine 0 selects l2b:u and engine 1 selects l2b2:u
Result l2bInit(L2B_Engine engine);
void l2bExit(L2B_Engine engine);

/** All output pointers are required and cleared before a request
 * Enum setters reject invalid values that could corrupt adjacent register fields
 * Width and lines must be positive multiples of eight, at most 1024
 * The getters expose register encoding, including zero for 1024
 * DMA uses the current process, with signed 16-bit transfer unit and gap in bytes
 * Keep buffers alive until DMA completes; event/busy alone do not establish completion
 * Zero DMA handles report done even when no transfer has been started
 */
Result L2BU_SetInputFormat(L2B_Engine engine, L2BU_Format value);
Result L2BU_GetInputFormat(L2B_Engine engine, L2BU_Format* value);
Result L2BU_SetOutputFormat(L2B_Engine engine, L2BU_Format value);
Result L2BU_GetOutputFormat(L2B_Engine engine, L2BU_Format* value);
Result L2BU_SetTransferEndInterrupt(L2B_Engine engine, bool value);
Result L2BU_GetTransferEndInterrupt(L2B_Engine engine, bool* value);
Result L2BU_SetInputLineWidth(L2B_Engine engine, u16 value);
Result L2BU_GetInputLineWidth(L2B_Engine engine, u16* value);
Result L2BU_SetInputLines(L2B_Engine engine, u16 value);
Result L2BU_GetInputLines(L2B_Engine engine, u16* value);
Result L2BU_SetAlpha(L2B_Engine engine, u16 value);
Result L2BU_GetAlpha(L2B_Engine engine, u16* value);
Result L2BU_IsDoneSending(L2B_Engine engine, bool* value);
Result L2BU_IsDoneReceiving(L2B_Engine engine, bool* value);
Result L2BU_IsBusyConversion(L2B_Engine engine, bool* value);
Result L2BU_PingProcess(L2B_Engine engine, u8* value);
Result L2BU_StartConversion(L2B_Engine engine);
Result L2BU_StopConversion(L2B_Engine engine);
Result L2BU_SetSending(L2B_Engine engine, const void* buffer, u32 size, s16 transfer_unit, s16 transfer_gap);
Result L2BU_SetReceiving(L2B_Engine engine, void* buffer, u32 size, s16 transfer_unit, s16 transfer_gap);

/// Returns a new shared handle that the caller must close; does not close any old handle
Result L2BU_GetTransferEndEvent(L2B_Engine engine, Handle* event);
/// Applies sequentially without rollback on a service error
Result L2BU_SetPackageParameter(L2B_Engine engine, const L2BU_ConversionParams* params);
/// Copies only the eight bytes actually initialized by the service
Result L2BU_GetPackageParameter(L2B_Engine engine, L2BU_ConversionParams* params);
#ifdef __cplusplus
}
#endif
