/* New libctru-style clients derived from the recovered L2B IPC ABI */
#include <string.h>
#include <3ds/services/l2b.h>
#include <3ds/srv.h>
#include <3ds/svc.h>
#include <3ds/result.h>
#include <3ds/ipc.h>
#include <3ds/synchronization.h>

static Handle l2bHandles[2];
static int l2bRefCounts[2];
#define L2B_BAD_ARGUMENT L2B_ERROR_INVALID_ARGUMENT

Result l2bInit(L2B_Engine engine)
{
 if ((u32)engine > 1) return L2B_BAD_ARGUMENT;
 if (AtomicPostIncrement(&l2bRefCounts[engine])) return 0;
 Result ret = srvGetServiceHandle(&l2bHandles[engine], engine ? "l2b2:u" : "l2b:u");
 if (R_FAILED(ret)) {
  l2bHandles[engine] = 0;
  AtomicDecrement(&l2bRefCounts[engine]);
 }
 return ret;
}

void l2bExit(L2B_Engine engine)
{
 if ((u32)engine > 1 || !l2bRefCounts[engine] || AtomicDecrement(&l2bRefCounts[engine])) return;
 svcCloseHandle(l2bHandles[engine]);
 l2bHandles[engine] = 0;
}

static Result l2bRequest(L2B_Engine engine, u16 command, const u32* words, u32 count, bool process)
{
 if ((u32)engine > 1) return L2B_BAD_ARGUMENT;
 if (!l2bHandles[engine]) return L2B_ERROR_NOT_INITIALIZED;
 u32* cmdbuf = getThreadCommandBuffer();
 cmdbuf[0] = IPC_MakeHeader(command, count, process ? 2 : 0);
 if (count) memcpy(cmdbuf + 1, words, count * sizeof(u32));
 if (process) {
  cmdbuf[count + 1] = IPC_Desc_SharedHandles(1);
  cmdbuf[count + 2] = CUR_PROCESS_HANDLE;
 }
 Result ret = svcSendSyncRequest(l2bHandles[engine]);
 return R_FAILED(ret) ? ret : (Result)cmdbuf[1];
}

Result L2BU_SetInputFormat(L2B_Engine engine, L2BU_Format value)
{
 if ((u32)value > 3) return L2B_BAD_ARGUMENT;
 u32 word = value;
 return l2bRequest(engine, 0x01, &word, 1, false);
}

Result L2BU_GetInputFormat(L2B_Engine engine, L2BU_Format* value)
{
 if (!value) return L2B_BAD_ARGUMENT;
 *value = 0;
 Result ret = l2bRequest(engine, 0x02, NULL, 0, false);
 if (R_SUCCEEDED(ret)) *value = getThreadCommandBuffer()[2] & 0xFF;
 return ret;
}

Result L2BU_SetOutputFormat(L2B_Engine engine, L2BU_Format value)
{
 if ((u32)value > 3) return L2B_BAD_ARGUMENT;
 u32 word = value;
 return l2bRequest(engine, 0x03, &word, 1, false);
}

Result L2BU_GetOutputFormat(L2B_Engine engine, L2BU_Format* value)
{
 if (!value) return L2B_BAD_ARGUMENT;
 *value = 0;
 Result ret = l2bRequest(engine, 0x04, NULL, 0, false);
 if (R_SUCCEEDED(ret)) *value = getThreadCommandBuffer()[2] & 0xFF;
 return ret;
}

Result L2BU_SetTransferEndInterrupt(L2B_Engine engine, bool value)
{
 u32 word = value;
 return l2bRequest(engine, 0x05, &word, 1, false);
}

Result L2BU_GetTransferEndInterrupt(L2B_Engine engine, bool* value)
{
 if (!value) return L2B_BAD_ARGUMENT;
 *value = 0;
 Result ret = l2bRequest(engine, 0x06, NULL, 0, false);
 if (R_SUCCEEDED(ret)) *value = getThreadCommandBuffer()[2] & 0xFF;
 return ret;
}

Result L2BU_SetInputLineWidth(L2B_Engine engine, u16 value)
{
 if (!value || value > 1024 || (value & 7)) return L2B_BAD_ARGUMENT;
 u32 word = value;
 return l2bRequest(engine, 0x0C, &word, 1, false);
}

Result L2BU_GetInputLineWidth(L2B_Engine engine, u16* value)
{
 if (!value) return L2B_BAD_ARGUMENT;
 *value = 0;
 Result ret = l2bRequest(engine, 0x0D, NULL, 0, false);
 if (R_SUCCEEDED(ret)) *value = getThreadCommandBuffer()[2] & 0xFFFF;
 return ret;
}

Result L2BU_SetInputLines(L2B_Engine engine, u16 value)
{
 if (!value || value > 1024 || (value & 7)) return L2B_BAD_ARGUMENT;
 u32 word = value;
 return l2bRequest(engine, 0x0E, &word, 1, false);
}

Result L2BU_GetInputLines(L2B_Engine engine, u16* value)
{
 if (!value) return L2B_BAD_ARGUMENT;
 *value = 0;
 Result ret = l2bRequest(engine, 0x0F, NULL, 0, false);
 if (R_SUCCEEDED(ret)) *value = getThreadCommandBuffer()[2] & 0xFFFF;
 return ret;
}

Result L2BU_SetAlpha(L2B_Engine engine, u16 value)
{
 u32 word = value;
 return l2bRequest(engine, 0x10, &word, 1, false);
}

Result L2BU_GetAlpha(L2B_Engine engine, u16* value)
{
 if (!value) return L2B_BAD_ARGUMENT;
 *value = 0;
 Result ret = l2bRequest(engine, 0x11, NULL, 0, false);
 if (R_SUCCEEDED(ret)) *value = getThreadCommandBuffer()[2] & 0xFFFF;
 return ret;
}

Result L2BU_IsDoneSending(L2B_Engine engine, bool* value)
{
 if (!value) return L2B_BAD_ARGUMENT;
 *value = 0;
 Result ret = l2bRequest(engine, 0x09, NULL, 0, false);
 if (R_SUCCEEDED(ret)) *value = getThreadCommandBuffer()[2] & 0xFF;
 return ret;
}

Result L2BU_IsDoneReceiving(L2B_Engine engine, bool* value)
{
 if (!value) return L2B_BAD_ARGUMENT;
 *value = 0;
 Result ret = l2bRequest(engine, 0x0B, NULL, 0, false);
 if (R_SUCCEEDED(ret)) *value = getThreadCommandBuffer()[2] & 0xFF;
 return ret;
}

Result L2BU_IsBusyConversion(L2B_Engine engine, bool* value)
{
 if (!value) return L2B_BAD_ARGUMENT;
 *value = 0;
 Result ret = l2bRequest(engine, 0x14, NULL, 0, false);
 if (R_SUCCEEDED(ret)) *value = getThreadCommandBuffer()[2] & 0xFF;
 return ret;
}

Result L2BU_PingProcess(L2B_Engine engine, u8* value)
{
 if (!value) return L2B_BAD_ARGUMENT;
 *value = 0;
 Result ret = l2bRequest(engine, 0x17, NULL, 0, false);
 if (R_SUCCEEDED(ret)) *value = getThreadCommandBuffer()[2] & 0xFF;
 return ret;
}

Result L2BU_StartConversion(L2B_Engine engine)
{
 return l2bRequest(engine, 0x12, NULL, 0, false);
}

Result L2BU_StopConversion(L2B_Engine engine)
{
 return l2bRequest(engine, 0x13, NULL, 0, false);
}

Result L2BU_SetSending(L2B_Engine engine, const void* buffer, u32 size, s16 transfer_unit, s16 transfer_gap)
{
 if (!buffer || !size || transfer_unit <= 0) return L2B_BAD_ARGUMENT;
 u32 words[] = {(u32)(uintptr_t)buffer, size, (u32)(s32)transfer_unit, (u32)(s32)transfer_gap};
 return l2bRequest(engine, 0x08, words, 4, true);
}

Result L2BU_SetReceiving(L2B_Engine engine, void* buffer, u32 size, s16 transfer_unit, s16 transfer_gap)
{
 if (!buffer || !size || transfer_unit <= 0) return L2B_BAD_ARGUMENT;
 u32 words[] = {(u32)(uintptr_t)buffer, size, (u32)(s32)transfer_unit, (u32)(s32)transfer_gap};
 return l2bRequest(engine, 0x0A, words, 4, true);
}

Result L2BU_GetTransferEndEvent(L2B_Engine engine, Handle* event)
{
 if (!event) return L2B_BAD_ARGUMENT;
 *event = 0;
 Result ret = l2bRequest(engine, 0x07, NULL, 0, false);
 if (R_SUCCEEDED(ret)) *event = getThreadCommandBuffer()[3];
 return ret;
}

Result L2BU_SetPackageParameter(L2B_Engine engine, const L2BU_ConversionParams* params)
{
 if (!params || params->input_format > 3 || params->output_format > 3 ||
     params->input_line_width <= 0 || params->input_line_width > 1024 || (params->input_line_width & 7) ||
     params->input_lines <= 0 || params->input_lines > 1024 || (params->input_lines & 7))
  return L2B_BAD_ARGUMENT;
 u32 words[2];
 memcpy(words, params, sizeof(words));
 return l2bRequest(engine, 0x15, words, 2, false);
}

Result L2BU_GetPackageParameter(L2B_Engine engine, L2BU_ConversionParams* params)
{
 if (!params) return L2B_BAD_ARGUMENT;
 memset(params, 0, sizeof(*params));
 Result ret = l2bRequest(engine, 0x16, NULL, 0, false);
 if (R_SUCCEEDED(ret)) memcpy(params, getThreadCommandBuffer() + 2, sizeof(*params));
 return ret;
}
