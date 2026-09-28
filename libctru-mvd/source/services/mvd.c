/* Altered libctru copy: complete MVD STD IPC; see libctru-mvd/README.md */
/*
  mvd.c - code for using this: http://3dbrew.org/wiki/MVD_Services
*/

#include <stdlib.h>
#include <string.h>
#include <3ds/types.h>
#include <3ds/result.h>
#include <3ds/svc.h>
#include <3ds/srv.h>
#include <3ds/os.h>
#include <3ds/allocator/linear.h>
#include <3ds/synchronization.h>
#include <3ds/services/mvd.h>
#include <3ds/ipc.h>

#define CALC_BUF_SIZE_SAFETY_MARGIN			(u16)(4096)

Handle mvdstdHandle = 0;
static int mvdstdRefCount;
static bool mvdstdRawSession;
static MVDSTD_Mode mvdstd_mode;
static MVDSTD_InputFormat mvdstd_input_type;
static MVDSTD_OutputFormat mvdstd_output_type;
static u32 *mvdstd_workbuf;
static size_t mvdstd_workbufsize;

static u32 mvdstd_videoproc_frameid;

Result MVDSTD_Initialize(u32* buf, u32 bufsize)
{
 if (!buf || !bufsize) return MVD_ERROR_PARAMETER;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x1,2,2); // 0x10082
	cmdbuf[1] = (u32)(uintptr_t)buf;
	cmdbuf[2] = bufsize;
	cmdbuf[3] = IPC_Desc_SharedHandles(1);
	cmdbuf[4] = CUR_PROCESS_HANDLE;

	Result ret=0;
	if(R_FAILED(ret=svcSendSyncRequest(mvdstdHandle)))return ret;

	return cmdbuf[1];
}

Result MVDSTD_Shutdown(void)
{
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x2,0,0); // 0x20000

	Result ret=0;
	if((ret = svcSendSyncRequest(mvdstdHandle)))return ret;

	return cmdbuf[1];
}

Result MVDSTD_CalculateWorkBufSize(const MVDSTD_CalculateWorkBufSizeConfig* config, u32* size_out)
{
 if (size_out) *size_out = 0;
 if (!config || config->level.level > MVD_H264_LEVEL_5_2) return MVD_ERROR_PARAMETER;
	Result ret=0;
	u32* cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(0x3,12,0); // 0x30300
	memcpy(&cmdbuf[1], config, sizeof(MVDSTD_CalculateWorkBufSizeConfig));

	if(R_FAILED(ret=svcSendSyncRequest(mvdstdHandle)))return ret;
	if (size_out && cmdbuf[1] == 0) *size_out = cmdbuf[2];

	return cmdbuf[1];
}

Result MVDSTD_H264Initialize(s8 no_output_reordering, s8 freeze_concealment, s8 display_smoothing, u32 reference_format)
{
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x5,4,0); // 0x50100
	cmdbuf[1] = no_output_reordering;
	cmdbuf[2] = freeze_concealment;
	cmdbuf[3] = display_smoothing;
	cmdbuf[4] = reference_format;

	Result ret=0;
	if(R_FAILED(ret=svcSendSyncRequest(mvdstdHandle)))return ret;

	return cmdbuf[1];
}

Result MVDSTD_H264Release(void)
{
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x7,0,0); // 0x70000

	Result ret=0;
	if(R_FAILED(ret=svcSendSyncRequest(mvdstdHandle)))return ret;

	return cmdbuf[1];
}

Result MVDSTD_ProcessNALUnit(u32 vaddr_buf, u32 physaddr_buf, u32 size, u32 frameid, u32 flag, MVDSTD_ProcessNALUnitOut *out)
{
 if (out) memset(out, 0, sizeof(*out));
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x8,5,2); // 0x80142
	cmdbuf[1] = vaddr_buf;
	cmdbuf[2] = physaddr_buf;
	cmdbuf[3] = size;
	cmdbuf[4] = frameid;
	cmdbuf[5] = flag;
	cmdbuf[6] = IPC_Desc_SharedHandles(1);
	cmdbuf[7] = CUR_PROCESS_HANDLE;

	Result ret=0;
	if(R_FAILED(ret=svcSendSyncRequest(mvdstdHandle)))return ret;

	if (out && cmdbuf[1] >= MVD_STATUS_OK && cmdbuf[1] <= MVD_STATUS_NONREF_SKIPPED)
  memcpy(out, &cmdbuf[2], sizeof(*out));

	return cmdbuf[1];
}

static Result MVDSTD_ControlFrameRendering(s8 type)
{
 return MVDSTD_H264NextPicture(type, NULL);
}

Result MVDSTD_PpInitialize(void)
{
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x18,0,0); // 0x180000

	Result ret=0;
	if(R_FAILED(ret=svcSendSyncRequest(mvdstdHandle)))return ret;

	return cmdbuf[1];
}

Result MVDSTD_PpRelease(void)
{
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x19,0,0); // 0x190000

	Result ret=0;
	if(R_FAILED(ret=svcSendSyncRequest(mvdstdHandle)))return ret;

	return cmdbuf[1];
}

Result MVDSTD_PpGetResult(void)
{
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x1A,0,0); // 0x1A0000

	Result ret=0;
	if(R_FAILED(ret=svcSendSyncRequest(mvdstdHandle)))return ret;

	return cmdbuf[1];
}

Result MVDSTD_PpEnableCombinedMode(u8 decoder_type)
{
 if (decoder_type != MVD_PP_H264 && decoder_type != MVD_PP_VP6 &&
     decoder_type != MVD_PP_VP8 && decoder_type != MVD_PP_WEBP) return MVD_ERROR_PARAMETER;
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x1B,1,0); // 0x1B0040
	cmdbuf[1] = decoder_type;

	Result ret=0;
	if(R_FAILED(ret=svcSendSyncRequest(mvdstdHandle)))return ret;

	return cmdbuf[1];
}

Result MVDSTD_PpDisableCombinedMode(void)
{
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = IPC_MakeHeader(0x1C,0,0); // 0x1C0000

	Result ret=0;
	if(R_FAILED(ret=svcSendSyncRequest(mvdstdHandle)))return ret;

	return cmdbuf[1];
}

Result MVDSTD_SetConfig(MVDSTD_Config* config)
{
 if (!config) return MVD_ERROR_PARAMETER;
	Result ret=0;
	u32* cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(0x1E,1,4); // 0x1E0044
	cmdbuf[1] = sizeof(MVDSTD_Config);
	cmdbuf[2] = IPC_Desc_SharedHandles(1);
	cmdbuf[3] = CUR_PROCESS_HANDLE;
	cmdbuf[4] = IPC_Desc_Buffer(sizeof(MVDSTD_Config),IPC_BUFFER_R);
	cmdbuf[5] = (u32)(uintptr_t)config;

	if(R_FAILED(ret=svcSendSyncRequest(mvdstdHandle)))return ret;

	return cmdbuf[1];
}

Result mvdstdSetupOutputBuffers(MVDSTD_OutputBuffersEntryList *entrylist, u32 bufsize)
{
 if (!entrylist || entrylist->total_entries < 1 || entrylist->total_entries > 17)
  return MVD_ERROR_PARAMETER;
	Result ret=0;
	u32* cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(0x1F,36,2); // 0x1F0902
	cmdbuf[1] = entrylist->total_entries;
 for (u32 i = 0; i < 17; ++i) {
  cmdbuf[2 + 2*i] = i < entrylist->total_entries ? (u32)(uintptr_t)entrylist->entries[i].outdata0 : 0;
  cmdbuf[3 + 2*i] = i < entrylist->total_entries ? (u32)(uintptr_t)entrylist->entries[i].outdata1 : 0;
 }
	cmdbuf[36] = bufsize;
	cmdbuf[37] = IPC_Desc_SharedHandles(1);
	cmdbuf[38] = CUR_PROCESS_HANDLE;

	if(R_FAILED(ret=svcSendSyncRequest(mvdstdHandle)))return ret;

	return cmdbuf[1];
}

Result mvdstdOverrideOutputBuffers(void* cur_outdata0, void* cur_outdata1, void* new_outdata0, void* new_outdata1)
{
	Result ret=0;
	u32* cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(0x21,4,0); // 0x210100
	cmdbuf[1] = (u32)(uintptr_t)cur_outdata0;
	cmdbuf[2] = (u32)(uintptr_t)cur_outdata1;
	cmdbuf[3] = (u32)(uintptr_t)new_outdata0;
	cmdbuf[4] = (u32)(uintptr_t)new_outdata1;

	if(R_FAILED(ret=svcSendSyncRequest(mvdstdHandle)))return ret;

	return cmdbuf[1];
}

Result mvdstdOpen(void)
{
 if (mvdstdHandle || mvdstdRefCount) return MVD_ERROR_INITIALIZATION;
 Result ret = srvGetServiceHandle(&mvdstdHandle, "mvd:STD");
 if (R_SUCCEEDED(ret)) mvdstdRawSession = true;
 else mvdstdHandle = 0;
 return ret;
}

void mvdstdClose(void)
{
 if (!mvdstdRawSession) return;
 svcCloseHandle(mvdstdHandle);
 mvdstdHandle = 0;
 mvdstdRawSession = false;
}

Result mvdstdInit(MVDSTD_Mode mode, MVDSTD_InputFormat input_type, MVDSTD_OutputFormat output_type, u32 size, MVDSTD_InitStruct *initstruct)
{
 Result ret;
 bool decoder_ready = false, pp_ready = false;
 MVDSTD_InitStruct settings = {0};
 settings.pp_decoder_type = MVD_PP_H264;
 if (initstruct) settings = *initstruct;
 if (mvdstdRawSession) return MVD_ERROR_INITIALIZATION;
 if (mode != MVDMODE_COLORFORMATCONV && mode != MVDMODE_VIDEOPROCESSING)
  return MVD_ERROR_PARAMETER;
 if (mode == MVDMODE_VIDEOPROCESSING &&
     (size > UINT32_MAX - CALC_BUF_SIZE_SAFETY_MARGIN || settings.pp_decoder_type != MVD_PP_H264))
  return MVD_ERROR_PARAMETER;
 if (AtomicPostIncrement(&mvdstdRefCount)) return 0;

 mvdstd_mode = mode;
 mvdstd_input_type = input_type;
 mvdstd_output_type = output_type;
 mvdstd_videoproc_frameid = 0;
 // Preserve libctru's allocation margin for the service work-size underestimate
 mvdstd_workbufsize = mode == MVDMODE_COLORFORMATCONV ? 1 : size + CALC_BUF_SIZE_SAFETY_MARGIN;
 if (R_FAILED(ret = srvGetServiceHandle(&mvdstdHandle, "mvd:STD"))) goto fail_open;
 mvdstd_workbuf = linearAlloc(mvdstd_workbufsize);
 if (!mvdstd_workbuf) { ret = MVD_ERROR_MEMORY; goto fail_alloc; }
 memset(mvdstd_workbuf, 0, mvdstd_workbufsize);
 ret = MVDSTD_Initialize((u32*)osConvertOldLINEARMemToNew(mvdstd_workbuf), mvdstd_workbufsize);
 if (R_FAILED(ret)) goto fail_initialize;
 if (mode == MVDMODE_VIDEOPROCESSING) {
  ret = MVDSTD_H264Initialize(settings.no_output_reordering, settings.freeze_concealment,
                             settings.display_smoothing, settings.reference_frame_format);
  if (ret != MVD_STATUS_OK) {
   if (R_SUCCEEDED(ret)) ret = MVD_ERROR_INITIALIZATION;
   goto fail_components;
  }
  decoder_ready = true;
 }
 ret = MVDSTD_PpInitialize();
 if (ret != MVD_STATUS_OK) {
   if (R_SUCCEEDED(ret)) ret = MVD_ERROR_INITIALIZATION;
   goto fail_components;
  }
 pp_ready = true;
 if (mode == MVDMODE_VIDEOPROCESSING) {
  ret = MVDSTD_PpEnableCombinedMode(settings.pp_decoder_type);
  if (ret != MVD_STATUS_OK) {
   if (R_SUCCEEDED(ret)) ret = MVD_ERROR_INITIALIZATION;
   goto fail_components;
  }
 }
 return 0;

fail_components:
 if (pp_ready) MVDSTD_PpRelease();
 if (decoder_ready) MVDSTD_H264Release();
 MVDSTD_Shutdown();
fail_initialize:
 linearFree(mvdstd_workbuf);
 mvdstd_workbuf = NULL;
fail_alloc:
 svcCloseHandle(mvdstdHandle);
fail_open:
 mvdstdHandle = 0;
 AtomicDecrement(&mvdstdRefCount);
 return ret;
}

void mvdstdExit(void)
{
 if (!mvdstdRefCount || AtomicDecrement(&mvdstdRefCount)) return;
 if (mvdstd_mode == MVDMODE_VIDEOPROCESSING) {
  while (MVDSTD_ControlFrameRendering(1) == MVD_STATUS_PICTURE_READY) {}
  MVDSTD_PpDisableCombinedMode();
 }
 MVDSTD_PpRelease();
 if (mvdstd_mode == MVDMODE_VIDEOPROCESSING) MVDSTD_H264Release();
 MVDSTD_Shutdown();
 svcCloseHandle(mvdstdHandle);
 mvdstdHandle = 0;
 linearFree(mvdstd_workbuf);
 mvdstd_workbuf = NULL;
}

Result mvdstdCalculateBufferSize(const MVDSTD_CalculateWorkBufSizeConfig* config, u32* size_out)
{
	if (size_out) *size_out = 0;
	bool must_close_handle = false;
	Result ret = 0;

	if(!config || !size_out || config->level.level > 0x10)
		return -1;

	//If we don't have mvdstd handle, get it
	if(mvdstdHandle == 0)
	{
		if(R_FAILED(ret=srvGetServiceHandle(&mvdstdHandle, "mvd:STD")))
			return ret;

		must_close_handle = true;
	}

	ret = MVDSTD_CalculateWorkBufSize(config, size_out);

	//Release handle if we must do so
	if(must_close_handle)
	{
		svcCloseHandle(mvdstdHandle);
		mvdstdHandle = 0;
	}

	return ret;
}

void mvdstdGenerateDefaultConfig(MVDSTD_Config*config, u32 input_width, u32 input_height, u32 output_width, u32 output_height, u32 *vaddr_colorconv_indata, u32 *vaddr_outdata0, u32 *vaddr_outdata1)
{
	memset(config, 0, sizeof(MVDSTD_Config));

	config->input_type = mvdstd_input_type;

	config->inwidth = input_width;
	config->inheight = input_height;

	if(mvdstd_mode==MVDMODE_COLORFORMATCONV)config->physaddr_colorconv_indata = osConvertVirtToPhys(vaddr_colorconv_indata);

	config->output_type = mvdstd_output_type;

	config->outwidth = output_width;
	config->outheight = output_height;

	config->physaddr_outdata0 = osConvertVirtToPhys(vaddr_outdata0);
	if(config->output_type==0x00020001)config->physaddr_outdata1 = osConvertVirtToPhys(vaddr_outdata1);

	config->rgb_transform = 0x1;
	config->coefficient_a = 0x12a;
	config->coefficient_b = 0x199;
	config->coefficient_c = 0xd0;
	config->coefficient_d = 0x64;
	config->coefficient_e = 0x204;
	config->dithering_enable = 0x1;
}

Result mvdstdConvertImage(MVDSTD_Config* config)
{
	Result ret;

	if(mvdstdRefCount==0)return -3;
	if(config==NULL)return -1;
	if(mvdstd_mode!=MVDMODE_COLORFORMATCONV)return -2;

	ret = MVDSTD_SetConfig(config);
	if(ret!=MVD_STATUS_OK)return ret;

	return MVDSTD_PpGetResult();
}

Result mvdstdProcessVideoFrame(void* inbuf_vaddr, size_t size, u32 flag, MVDSTD_ProcessNALUnitOut *out)
{
 if (out) memset(out, 0, sizeof(*out));
	Result ret;

	if(mvdstdRefCount==0)return -3;
	if(mvdstd_mode!=MVDMODE_VIDEOPROCESSING)return -2;

	if (!inbuf_vaddr || !size || size > UINT32_MAX) return MVD_ERROR_PARAMETER;
	ret = MVDSTD_ProcessNALUnit((u32)(uintptr_t)osConvertOldLINEARMemToNew(inbuf_vaddr), (u32)osConvertVirtToPhys(inbuf_vaddr), size, mvdstd_videoproc_frameid, flag, out);
	mvdstd_videoproc_frameid++;
	if(mvdstd_videoproc_frameid>=0x12)mvdstd_videoproc_frameid = 0;

	return ret;
}

Result mvdstdRenderVideoFrame(MVDSTD_Config* config, bool wait)
{
	Result ret;

	if(mvdstdRefCount==0)return -3;
	if(mvdstd_mode!=MVDMODE_VIDEOPROCESSING)return -2;

	if (config) {
		ret = MVDSTD_SetConfig(config);
		if (ret != MVD_STATUS_OK) return ret;
	}

	ret = MVD_STATUS_PICTURE_READY;
	while(ret==MVD_STATUS_PICTURE_READY)
	{
		ret = MVDSTD_ControlFrameRendering(0);
		if(!wait)break;
	}

	return ret;
}


static Result mvdstdRequest(u16 command, const u32* words, u32 count, bool process)
{
 u32* cmdbuf = getThreadCommandBuffer();
 cmdbuf[0] = IPC_MakeHeader(command, count, process ? 2 : 0);
 if (count) memcpy(cmdbuf + 1, words, count * sizeof(u32));
 if (process) {
  cmdbuf[count + 1] = IPC_Desc_SharedHandles(1);
  cmdbuf[count + 2] = CUR_PROCESS_HANDLE;
 }
 Result ret = svcSendSyncRequest(mvdstdHandle);
 return R_FAILED(ret) ? ret : (Result)cmdbuf[1];
}

Result MVDSTD_CalculateImageSize(u32 width, u32 height, MVDSTD_PixelFormat format, u32* size)
{
 if (size) *size = 0;
 u32 bytes = format == MVD_PIX_BGR32 ? 4 : 2;
 if ((format != MVD_PIX_YUYV && format != MVD_PIX_RGB565 && format != MVD_PIX_BGR32) ||
     !width || !height || (u64)width * height > UINT32_MAX / bytes)
  return MVD_ERROR_PARAMETER;
 u32 words[] = {width, height, format};
 Result ret = mvdstdRequest(0x04, words, 3, false);
 if (ret == 0 && size) *size = getThreadCommandBuffer()[2];
 return ret;
}

Result MVDSTD_H264EnableMvc(void) { return mvdstdRequest(0x06, NULL, 0, false); }

Result MVDSTD_Vp8Initialize(MVDSTD_Vp8Format format, s8 freeze, u32 count, u32 references)
{
 if (format < MVD_VP7 || format > MVD_WEBP) return MVD_ERROR_PARAMETER;
 u32 words[] = {(u8)format, (u32)(s32)freeze, count, references};
 return mvdstdRequest(0x0C, words, 4, false);
}

Result MVDSTD_Vp8Release(void) { return mvdstdRequest(0x0D, NULL, 0, false); }

Result MVDSTD_Vp8Decode(const MVDSTD_Vp8Input* input)
{
 if (!input) return MVD_ERROR_PARAMETER;
 u32 words[8];
 memcpy(words, input, sizeof(words));
 return mvdstdRequest(0x0E, words, 8, true);
}

Result MVDSTD_Vp6Initialize(s8 freeze, u32 count, u32 references)
{
 u32 words[] = {(u32)(s32)freeze, count, references};
 return mvdstdRequest(0x12, words, 3, false);
}

Result MVDSTD_Vp6Release(void) { return mvdstdRequest(0x13, NULL, 0, false); }

Result MVDSTD_Vp6Decode(u32 stream_vaddr, u32 stream_bus_address, u32 size)
{
 u32 words[] = {stream_vaddr, stream_bus_address, size};
 return mvdstdRequest(0x14, words, 3, true);
}

Result MVDSTD_GetConfig(MVDSTD_Config* config)
{
 if (!config) return MVD_ERROR_PARAMETER;
 memset(config, 0, sizeof(*config));
 u32* cmdbuf = getThreadCommandBuffer();
 cmdbuf[0] = IPC_MakeHeader(0x1D, 1, 2);
 cmdbuf[1] = sizeof(*config);
 cmdbuf[2] = IPC_Desc_Buffer(sizeof(*config), IPC_BUFFER_W);
 cmdbuf[3] = (u32)(uintptr_t)config;
 Result ret = svcSendSyncRequest(mvdstdHandle);
 if (R_SUCCEEDED(ret)) ret = cmdbuf[1];
 if (ret != MVD_STATUS_OK) memset(config, 0, sizeof(*config));
 return ret;
}

Result MVDSTD_GetNextOutput(MVDSTD_OutputBuffersEntry* output)
{
 if (output) memset(output, 0, sizeof(*output));
 Result ret = mvdstdRequest(0x20, NULL, 0, true);
 if (ret == MVD_STATUS_OK && output) {
  u32* cmdbuf = getThreadCommandBuffer();
  output->outdata0 = (void*)(uintptr_t)cmdbuf[2];
  output->outdata1 = (void*)(uintptr_t)cmdbuf[3];
 }
 return ret;
}

Result MVDSTD_H264NextPicture(s8 end_of_stream, MVDSTD_H264Picture* output)
{
 if (output) memset(output, 0, sizeof(*output));
 u32 word = (u32)(s32)end_of_stream;
 Result ret = mvdstdRequest(0x09, &word, 1, true);
 if (ret == MVD_STATUS_PICTURE_READY && output) {
  memcpy(output, getThreadCommandBuffer() + 2, sizeof(*output));
  memset(output->reserved, 0, sizeof(output->reserved));
 }
 return ret;
}

Result MVDSTD_H264GetInfo(MVDSTD_H264Info* output)
{
 if (output) memset(output, 0, sizeof(*output));
 Result ret = mvdstdRequest(0x0A, NULL, 0, false);
 if (ret == MVD_STATUS_OK && output) {
  memcpy(output, getThreadCommandBuffer() + 2, sizeof(*output));
 }
 return ret;
}

Result MVDSTD_H264Peek(MVDSTD_H264Picture* output)
{
 if (output) memset(output, 0, sizeof(*output));
 Result ret = mvdstdRequest(0x0B, NULL, 0, false);
 if (ret == MVD_STATUS_PICTURE_READY && output) {
  memcpy(output, getThreadCommandBuffer() + 2, sizeof(*output));
  output->view_id = 0;
  memset(output->reserved, 0, sizeof(output->reserved));
 }
 return ret;
}

Result MVDSTD_Vp8NextPicture(s8 end_of_stream, MVDSTD_Vp8Picture* output)
{
 if (output) memset(output, 0, sizeof(*output));
 u32 word = (u32)(s32)end_of_stream;
 Result ret = mvdstdRequest(0x0F, &word, 1, true);
 if (ret == MVD_STATUS_PICTURE_READY && output) {
  memcpy(output, getThreadCommandBuffer() + 2, sizeof(*output));
  memset(output->reserved, 0, sizeof(output->reserved));
 }
 return ret;
}

Result MVDSTD_Vp8GetInfo(MVDSTD_Vp8Info* output)
{
 if (output) memset(output, 0, sizeof(*output));
 Result ret = mvdstdRequest(0x10, NULL, 0, false);
 if (ret == MVD_STATUS_OK && output) {
  memcpy(output, getThreadCommandBuffer() + 2, sizeof(*output));
  memset(output->reserved, 0, sizeof(output->reserved));
 }
 return ret;
}

Result MVDSTD_Vp8Peek(MVDSTD_Vp8Picture* output)
{
 if (output) memset(output, 0, sizeof(*output));
 Result ret = mvdstdRequest(0x11, NULL, 0, false);
 if (ret == MVD_STATUS_PICTURE_READY && output) {
  memcpy(output, getThreadCommandBuffer() + 2, sizeof(*output));
  output->slice_rows = 0;
  output->output_layout = MVD_LAYOUT_UNAVAILABLE;
  memset(output->reserved, 0, sizeof(output->reserved));
 }
 return ret;
}

Result MVDSTD_Vp6NextPicture(s8 end_of_stream, MVDSTD_Vp6Picture* output)
{
 if (output) memset(output, 0, sizeof(*output));
 u32 word = (u32)(s32)end_of_stream;
 Result ret = mvdstdRequest(0x15, &word, 1, true);
 if (ret == MVD_STATUS_PICTURE_READY && output) {
  memcpy(output, getThreadCommandBuffer() + 2, sizeof(*output));
  memset(output->reserved, 0, sizeof(output->reserved));
 }
 return ret;
}

Result MVDSTD_Vp6GetInfo(MVDSTD_Vp6Info* output)
{
 if (output) memset(output, 0, sizeof(*output));
 Result ret = mvdstdRequest(0x16, NULL, 0, false);
 if (ret == MVD_STATUS_OK && output) {
  memcpy(output, getThreadCommandBuffer() + 2, sizeof(*output));
  memset(output->reserved, 0, sizeof(output->reserved));
 }
 return ret;
}

Result MVDSTD_Vp6Peek(MVDSTD_Vp6Picture* output)
{
 if (output) memset(output, 0, sizeof(*output));
 Result ret = mvdstdRequest(0x17, NULL, 0, false);
 if (ret == MVD_STATUS_PICTURE_READY && output) {
  memcpy(output, getThreadCommandBuffer() + 2, sizeof(*output));
  output->output_layout = MVD_LAYOUT_UNAVAILABLE;
  memset(output->reserved, 0, sizeof(output->reserved));
 }
 return ret;
}
