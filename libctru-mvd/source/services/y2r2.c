/* Altered libctru copy: recovered MVD y2r2:u interface; see libctru-mvd/README.md */
#include <stdlib.h>
#include <string.h>
#include <3ds/services/y2r2.h>
#include <3ds/srv.h>
#include <3ds/svc.h>
#include <3ds/types.h>
#include <3ds/result.h>
#include <3ds/ipc.h>
#include <3ds/synchronization.h>

static Handle y2r2Handle;
static int y2r2RefCount;

Result y2r2Init(void)
{
	Result ret = 0;

	if (AtomicPostIncrement(&y2r2RefCount)) return 0;

	ret = srvGetServiceHandle(&y2r2Handle, "y2r2:u");
	if (R_SUCCEEDED(ret))
	{
		ret = Y2R2U_DriverInitialize();
		if (R_FAILED(ret)) svcCloseHandle(y2r2Handle);
	}
	if (R_FAILED(ret)) { y2r2Handle = 0; AtomicDecrement(&y2r2RefCount); }
	return ret;
}

void y2r2Exit(void)
{
	if (!y2r2RefCount || AtomicDecrement(&y2r2RefCount)) return;
	Y2R2U_DriverFinalize();
	svcCloseHandle(y2r2Handle);
 y2r2Handle = 0;
}

Result Y2R2U_SetInputFormat(Y2R2U_InputFormat format)
{
 if ((u32)format > 4) return Y2R2_ERROR_INVALID_ARGUMENT;
	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x1,1,0); // 0x10040
	cmdbuf[1] = format;

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
	return cmdbuf[1];
}

Result Y2R2U_GetInputFormat(Y2R2U_InputFormat* format)
{
 if (!format) return Y2R2_ERROR_INVALID_ARGUMENT;
 memset(format, 0, sizeof(*format));
	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x2,0,0); // 0x20000

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
 if (R_FAILED((Result)cmdbuf[1])) return cmdbuf[1];
	*format = cmdbuf[2] & 0xFF;
	return cmdbuf[1];
}

Result Y2R2U_SetOutputFormat(Y2R2U_OutputFormat format)
{
 if ((u32)format > 3) return Y2R2_ERROR_INVALID_ARGUMENT;
	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x3,1,0); // 0x30040
	cmdbuf[1] = format;

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
	return cmdbuf[1];
}

Result Y2R2U_GetOutputFormat(Y2R2U_OutputFormat* format)
{
 if (!format) return Y2R2_ERROR_INVALID_ARGUMENT;
 memset(format, 0, sizeof(*format));
	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x4,0,0); // 0x40000

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
 if (R_FAILED((Result)cmdbuf[1])) return cmdbuf[1];
	*format = cmdbuf[2] & 0xFF;
	return cmdbuf[1];
}

Result Y2R2U_SetRotation(Y2R2U_Rotation rotation)
{
 if ((u32)rotation > 3) return Y2R2_ERROR_INVALID_ARGUMENT;
	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x5,1,0); // 0x50040
	cmdbuf[1] = rotation;

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
	return cmdbuf[1];
}

Result Y2R2U_GetRotation(Y2R2U_Rotation* rotation)
{
 if (!rotation) return Y2R2_ERROR_INVALID_ARGUMENT;
 memset(rotation, 0, sizeof(*rotation));
	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x6,0,0); // 0x60000

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
 if (R_FAILED((Result)cmdbuf[1])) return cmdbuf[1];
	*rotation = cmdbuf[2] & 0xFF;
	return cmdbuf[1];
}

Result Y2R2U_SetBlockAlignment(Y2R2U_BlockAlignment alignment)
{
 if ((u32)alignment > 1) return Y2R2_ERROR_INVALID_ARGUMENT;
	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x7,1,0); // 0x70040
	cmdbuf[1] = alignment;

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
	return cmdbuf[1];
}

Result Y2R2U_GetBlockAlignment(Y2R2U_BlockAlignment* alignment)
{
 if (!alignment) return Y2R2_ERROR_INVALID_ARGUMENT;
 memset(alignment, 0, sizeof(*alignment));
	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x8,0,0); // 0x80000

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
 if (R_FAILED((Result)cmdbuf[1])) return cmdbuf[1];
	*alignment = cmdbuf[2] & 0xFF;
	return cmdbuf[1];
}

Result Y2R2U_SetSpacialDithering(bool enable)
{

	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x9,1,0); // 0x90040
	cmdbuf[1] = enable;

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
	return cmdbuf[1];
}

Result Y2R2U_GetSpacialDithering(bool* enabled)
{
 if (!enabled) return Y2R2_ERROR_INVALID_ARGUMENT;
 memset(enabled, 0, sizeof(*enabled));
	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0xA,0,0); // 0xA0000

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
 if (R_FAILED((Result)cmdbuf[1])) return cmdbuf[1];
	*enabled = cmdbuf[2] & 0xFF;
	return cmdbuf[1];
}

Result Y2R2U_SetTemporalDithering(bool enable)
{

	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0xB,1,0); // 0xB0040
	cmdbuf[1] = enable;

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
	return cmdbuf[1];
}

Result Y2R2U_GetTemporalDithering(bool* enabled)
{
 if (!enabled) return Y2R2_ERROR_INVALID_ARGUMENT;
 memset(enabled, 0, sizeof(*enabled));
	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0xC,0,0); // 0xC0000

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
 if (R_FAILED((Result)cmdbuf[1])) return cmdbuf[1];
	*enabled = cmdbuf[2] & 0xFF;
	return cmdbuf[1];
}

Result Y2R2U_SetTransferEndInterrupt(bool should_interrupt)
{

	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0xD,1,0); // 0xD0040
	cmdbuf[1] = should_interrupt;

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
	return cmdbuf[1];
}

Result Y2R2U_GetTransferEndInterrupt(bool* should_interrupt)
{
 if (!should_interrupt) return Y2R2_ERROR_INVALID_ARGUMENT;
 memset(should_interrupt, 0, sizeof(*should_interrupt));
	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0xE,0,0); // 0xE0000

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
 if (R_FAILED((Result)cmdbuf[1])) return cmdbuf[1];
	*should_interrupt = cmdbuf[2] & 0xFF;
	return cmdbuf[1];
}

Result Y2R2U_GetTransferEndEvent(Handle* end_event)
{
 if (!end_event) return Y2R2_ERROR_INVALID_ARGUMENT;
 memset(end_event, 0, sizeof(*end_event));

	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0xF,0,0); // 0xF0000

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
 if (R_FAILED((Result)cmdbuf[1])) return cmdbuf[1];

	*end_event = cmdbuf[3];
	return cmdbuf[1];
}

Result Y2R2U_SetSendingY(const void* src_buf, u32 image_size, s16 transfer_unit, s16 transfer_gap)
{
 if (!src_buf) return Y2R2_ERROR_INVALID_ARGUMENT;
 if (!image_size || transfer_unit <= 0) return Y2R2_ERROR_INVALID_ARGUMENT;
	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x10,4,2); // 0x100102
	cmdbuf[1] = (u32)(uintptr_t)src_buf;
	cmdbuf[2] = image_size;
	cmdbuf[3] = transfer_unit;
	cmdbuf[4] = transfer_gap;
	cmdbuf[5] = IPC_Desc_SharedHandles(1);
	cmdbuf[6] = CUR_PROCESS_HANDLE;

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
	return cmdbuf[1];
}

Result Y2R2U_SetSendingU(const void* src_buf, u32 image_size, s16 transfer_unit, s16 transfer_gap)
{
 if (!src_buf) return Y2R2_ERROR_INVALID_ARGUMENT;
 if (!image_size || transfer_unit <= 0) return Y2R2_ERROR_INVALID_ARGUMENT;
	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x11,4,2); // 0x110102
	cmdbuf[1] = (u32)(uintptr_t)src_buf;
	cmdbuf[2] = image_size;
	cmdbuf[3] = transfer_unit;
	cmdbuf[4] = transfer_gap;
	cmdbuf[5] = IPC_Desc_SharedHandles(1);
	cmdbuf[6] = CUR_PROCESS_HANDLE;

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
	return cmdbuf[1];
}

Result Y2R2U_SetSendingV(const void* src_buf, u32 image_size, s16 transfer_unit, s16 transfer_gap)
{
 if (!src_buf) return Y2R2_ERROR_INVALID_ARGUMENT;
 if (!image_size || transfer_unit <= 0) return Y2R2_ERROR_INVALID_ARGUMENT;
	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x12,4,2); // 0x120102
	cmdbuf[1] = (u32)(uintptr_t)src_buf;
	cmdbuf[2] = image_size;
	cmdbuf[3] = transfer_unit;
	cmdbuf[4] = transfer_gap;
	cmdbuf[5] = IPC_Desc_SharedHandles(1);
	cmdbuf[6] = CUR_PROCESS_HANDLE;

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
	return cmdbuf[1];
}

Result Y2R2U_SetSendingYUYV(const void* src_buf, u32 image_size, s16 transfer_unit, s16 transfer_gap)
{
 if (!src_buf) return Y2R2_ERROR_INVALID_ARGUMENT;
 if (!image_size || transfer_unit <= 0) return Y2R2_ERROR_INVALID_ARGUMENT;
	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x13,4,2); // 0x130102
	cmdbuf[1] = (u32)(uintptr_t)src_buf;
	cmdbuf[2] = image_size;
	cmdbuf[3] = transfer_unit;
	cmdbuf[4] = transfer_gap;
	cmdbuf[5] = IPC_Desc_SharedHandles(1);
	cmdbuf[6] = CUR_PROCESS_HANDLE;

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
	return cmdbuf[1];
}

Result Y2R2U_IsDoneSendingYUYV(bool* is_done)
{
 if (!is_done) return Y2R2_ERROR_INVALID_ARGUMENT;
 memset(is_done, 0, sizeof(*is_done));
	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x14,0,0); // 0x140000

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
 if (R_FAILED((Result)cmdbuf[1])) return cmdbuf[1];
	*is_done = cmdbuf[2] & 0xFF;
	return cmdbuf[1];
}

Result Y2R2U_IsDoneSendingY(bool* is_done)
{
 if (!is_done) return Y2R2_ERROR_INVALID_ARGUMENT;
 memset(is_done, 0, sizeof(*is_done));
	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x15,0,0); // 0x150000

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
 if (R_FAILED((Result)cmdbuf[1])) return cmdbuf[1];
	*is_done = cmdbuf[2] & 0xFF;
	return cmdbuf[1];
}

Result Y2R2U_IsDoneSendingU(bool* is_done)
{
 if (!is_done) return Y2R2_ERROR_INVALID_ARGUMENT;
 memset(is_done, 0, sizeof(*is_done));
	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x16,0,0); // 0x160000

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
 if (R_FAILED((Result)cmdbuf[1])) return cmdbuf[1];
	*is_done = cmdbuf[2] & 0xFF;
	return cmdbuf[1];
}

Result Y2R2U_IsDoneSendingV(bool* is_done)
{
 if (!is_done) return Y2R2_ERROR_INVALID_ARGUMENT;
 memset(is_done, 0, sizeof(*is_done));
	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x17,0,0); // 0x170000

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
 if (R_FAILED((Result)cmdbuf[1])) return cmdbuf[1];
	*is_done = cmdbuf[2] & 0xFF;
	return cmdbuf[1];
}

Result Y2R2U_SetReceiving(void* dst_buf, u32 image_size, s16 transfer_unit, s16 transfer_gap)
{
 if (!dst_buf) return Y2R2_ERROR_INVALID_ARGUMENT;
 if (!image_size || transfer_unit <= 0) return Y2R2_ERROR_INVALID_ARGUMENT;
	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x18,4,2); // 0x180102
	cmdbuf[1] = (u32)(uintptr_t)dst_buf;
	cmdbuf[2] = image_size;
	cmdbuf[3] = transfer_unit;
	cmdbuf[4] = transfer_gap;
	cmdbuf[5] = IPC_Desc_SharedHandles(1);
	cmdbuf[6] = CUR_PROCESS_HANDLE;

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
	return cmdbuf[1];
}

Result Y2R2U_IsDoneReceiving(bool* is_done)
{
 if (!is_done) return Y2R2_ERROR_INVALID_ARGUMENT;
 memset(is_done, 0, sizeof(*is_done));
	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x19,0,0); // 0x190000

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
 if (R_FAILED((Result)cmdbuf[1])) return cmdbuf[1];
	*is_done = cmdbuf[2] & 0xFF;
	return cmdbuf[1];
}

Result Y2R2U_SetInputLineWidth(u16 line_width)
{
 if (!line_width || line_width > 1024 || (line_width & 7)) return Y2R2_ERROR_INVALID_ARGUMENT;
	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x1A,1,0); // 0x1A0040
	cmdbuf[1] = line_width;

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
	return cmdbuf[1];
}

Result Y2R2U_GetInputLineWidth(u16* line_width)
{
 if (!line_width) return Y2R2_ERROR_INVALID_ARGUMENT;
 memset(line_width, 0, sizeof(*line_width));
	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x1B,0,0); // 0x1B0000

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
 if (R_FAILED((Result)cmdbuf[1])) return cmdbuf[1];
	*line_width = cmdbuf[2] & 0xFFFF;
	return cmdbuf[1];
}

Result Y2R2U_SetInputLines(u16 num_lines)
{
 if (!num_lines || num_lines > 1024) return Y2R2_ERROR_INVALID_ARGUMENT;
	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x1C,1,0); // 0x1C0040
	cmdbuf[1] = num_lines;

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
	return cmdbuf[1];
}

Result Y2R2U_GetInputLines(u16* num_lines)
{
 if (!num_lines) return Y2R2_ERROR_INVALID_ARGUMENT;
 memset(num_lines, 0, sizeof(*num_lines));
	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x1D,0,0); // 0x1D0000

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
 if (R_FAILED((Result)cmdbuf[1])) return cmdbuf[1];
	*num_lines = cmdbuf[2] & 0xFFFF;
	return cmdbuf[1];
}

Result Y2R2U_SetCoefficients(const Y2R2U_ColorCoefficients* coefficients)
{
 if (!coefficients) return Y2R2_ERROR_INVALID_ARGUMENT;
	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x1E,4,0); // 0x1E0100
	memcpy(&cmdbuf[1], coefficients, sizeof(Y2R2U_ColorCoefficients));

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
	return cmdbuf[1];
}

Result Y2R2U_GetCoefficients(Y2R2U_ColorCoefficients* coefficients)
{
 if (!coefficients) return Y2R2_ERROR_INVALID_ARGUMENT;
 memset(coefficients, 0, sizeof(*coefficients));
	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x1F,0,0); // 0x1F0000

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
 if (R_FAILED((Result)cmdbuf[1])) return cmdbuf[1];
	memcpy(coefficients,cmdbuf + 2, sizeof(Y2R2U_ColorCoefficients));
	return cmdbuf[1];
}

Result Y2R2U_SetStandardCoefficient(Y2R2U_StandardCoefficient coefficient)
{
 if ((u32)coefficient > 3) return Y2R2_ERROR_INVALID_ARGUMENT;
	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x20,1,0); // 0x200040
	cmdbuf[1] = coefficient;

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
	return cmdbuf[1];
}

Result Y2R2U_GetStandardCoefficient(Y2R2U_ColorCoefficients* coefficients, Y2R2U_StandardCoefficient standardCoeff)
{
 if (!coefficients) return Y2R2_ERROR_INVALID_ARGUMENT;
 memset(coefficients, 0, sizeof(*coefficients));
 if ((u32)standardCoeff > 3) return Y2R2_ERROR_INVALID_ARGUMENT;
	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x21,1,0); // 0x210040
	cmdbuf[1] = standardCoeff;

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
 if (R_FAILED((Result)cmdbuf[1])) return cmdbuf[1];
	memcpy(coefficients,cmdbuf + 2, sizeof(Y2R2U_ColorCoefficients));
	return cmdbuf[1];
}

Result Y2R2U_SetAlpha(u16 alpha)
{

	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x22,1,0); // 0x220040
	cmdbuf[1] = alpha;

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
	return cmdbuf[1];
}

Result Y2R2U_GetAlpha(u16* alpha)
{
 if (!alpha) return Y2R2_ERROR_INVALID_ARGUMENT;
 memset(alpha, 0, sizeof(*alpha));
	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x23,0,0); // 0x230000

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
 if (R_FAILED((Result)cmdbuf[1])) return cmdbuf[1];
	*alpha = cmdbuf[2] & 0xFFFF;
	return cmdbuf[1];
}


Result Y2R2U_SetDitheringWeightParams(const Y2R2U_DitheringWeightParams* params)
{
 if (!params) return Y2R2_ERROR_INVALID_ARGUMENT;
	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x24,8,0); // 0x240200
	memcpy(&cmdbuf[1], params, sizeof(Y2R2U_DitheringWeightParams));

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
	return cmdbuf[1];
}

Result Y2R2U_GetDitheringWeightParams(Y2R2U_DitheringWeightParams* params)
{
 if (!params) return Y2R2_ERROR_INVALID_ARGUMENT;
 memset(params, 0, sizeof(*params));
	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x25,0,0); // 0x250000

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
 if (R_FAILED((Result)cmdbuf[1])) return cmdbuf[1];
	memcpy(params,cmdbuf+2, sizeof(Y2R2U_DitheringWeightParams));
	return cmdbuf[1];
}

Result Y2R2U_StartConversion(void)
{

	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x26,0,0); // 0x260000

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
	return cmdbuf[1];
}

Result Y2R2U_StopConversion(void)
{

	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x27,0,0); // 0x270000

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
	return cmdbuf[1];
}

Result Y2R2U_IsBusyConversion(bool* is_busy)
{
 if (!is_busy) return Y2R2_ERROR_INVALID_ARGUMENT;
 memset(is_busy, 0, sizeof(*is_busy));
	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x28,0,0); // 0x280000

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
 if (R_FAILED((Result)cmdbuf[1])) return cmdbuf[1];
	*is_busy = cmdbuf[2] & 0xFF;
	return cmdbuf[1];
}

Result Y2R2U_SetConversionParams(const Y2R2U_ConversionParams* params)
{
 if (!params) return Y2R2_ERROR_INVALID_ARGUMENT;
 if (params->input_format > 4 || params->output_format > 3 || params->rotation > 3 ||
     params->block_alignment > 1 || params->standard_coefficient > 3 ||
     params->input_line_width <= 0 || params->input_line_width > 1024 || (params->input_line_width & 7) ||
     params->input_lines <= 0 || params->input_lines > 1024) return Y2R2_ERROR_INVALID_ARGUMENT;
	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x29,3,0); // 0x2900C0
	Y2R2U_ConversionParams wire = *params;
 wire.unused = 0;
 memcpy(&cmdbuf[1], &wire, sizeof(wire));

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
	return cmdbuf[1];
}

Result Y2R2U_PingProcess(u8* ping)
{
 if (!ping) return Y2R2_ERROR_INVALID_ARGUMENT;
 memset(ping, 0, sizeof(*ping));
	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x2A,0,0); // 0x2A0000

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
 if (R_FAILED((Result)cmdbuf[1])) return cmdbuf[1];
	*ping = (u8)cmdbuf[2];
	return cmdbuf[1];
}

Result Y2R2U_DriverInitialize(void)
{

	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x2B,0,0); // 0x2B0000

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
	return cmdbuf[1];
}

Result Y2R2U_DriverFinalize(void)
{

	Result ret = 0;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x2C,0,0); // 0x2C0000

	if (R_FAILED(ret = svcSendSyncRequest(y2r2Handle))) return ret;
	return cmdbuf[1];
}

Result Y2R2U_GetConversionParams(Y2R2U_ConversionParams* params)
{
 if (!params) return Y2R2_ERROR_INVALID_ARGUMENT;
 memset(params, 0, sizeof(*params));
 u32* cmdbuf = getThreadCommandBuffer();
 cmdbuf[0] = IPC_MakeHeader(0x2D, 0, 0);
 Result ret = svcSendSyncRequest(y2r2Handle);
 if (R_FAILED(ret)) return ret;
 if (R_SUCCEEDED((Result)cmdbuf[1])) {
  memcpy(params, cmdbuf + 2, sizeof(*params));
  params->unused = 0;
 }
 return cmdbuf[1];
}
