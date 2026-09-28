/**
 * @file wire.c
 * @brief Host IPC, reply-validity and lifecycle regression checks with mocked OS calls
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds/services/mvd.h>
#include <3ds/services/l2b.h>
#include <3ds/services/y2r2.h>
#include <3ds/svc.h>

static u32 command[128], sent[128], reply[32];
static Result reply_result = MVD_STATUS_OK, transport_result;
static u32 history[256], history_count, sends;
static Handle sent_handle;
static bool lifecycle, fail_allocation;
static u32 fail_command;
static unsigned allocations, frees, closes;

/**
 * @brief Returns the mock thread command buffer
 * @return Pointer to the shared mock command/reply storage
 */
u32* getThreadCommandBuffer(void)
{
	return command;
}
/**
 * @brief Captures the request and synthesizes a configured service reply
 * @param[in] handle Mock destination service handle
 * @return Configured transport result
 */
Result svcSendSyncRequest(Handle handle)
{
	sent_handle = handle;
	memcpy(sent, command, sizeof(sent));
	++sends;
	assert(history_count < 256);
	history[history_count++] = command[0] >> 16;
	if (transport_result)
		return transport_result;
	u32 id = command[0] >> 16;
	memset(command, 0xCD, sizeof(command));
	command[1] = reply_result;
	if (lifecycle)
	{
		command[1] = id == 1 || id == 2 || id == 7 || id == 0x19 ? 0 : MVD_STATUS_OK;
		if (id == fail_command)
			command[1] = MVD_ERROR_MEMORY;
	}
	memcpy(command + 2, reply, sizeof(reply));
	return 0;
}
/**
 * @brief Records closure of a nonzero mock handle
 * @param[in] handle Mock handle to close
 * @return Zero after recording the closure
 */
Result svcCloseHandle(Handle handle)
{
	assert(handle);
	++closes;
	return 0;
}
/**
 * @brief Resolves one of the four supported service names to a mock handle
 * @param[out] handle Destination for the mock service handle
 * @param[in] name Service name to resolve
 * @return Zero for a recognized service
 */
Result srvGetServiceHandle(Handle* handle, const char* name)
{
	if (!strcmp(name, "mvd:STD"))
		*handle = 10;
	else if (!strcmp(name, "l2b:u"))
		*handle = 20;
	else if (!strcmp(name, "l2b2:u"))
		*handle = 21;
	else if (!strcmp(name, "y2r2:u"))
		*handle = 22;
	else
		assert(false);
	return 0;
}
/**
 * @brief Applies a deterministic mock client-VA to bus-address mapping
 * @param[in] p Mock client virtual address
 * @return Synthetic 32-bit bus address
 */
u32 osConvertVirtToPhys(const void* p)
{
	return (u32)(uintptr_t)p + 0x20000000;
}
/**
 * @brief Applies a deterministic mock LINEAR alias conversion
 * @param[in] p Mock old-LINEAR virtual address
 * @return Synthetic new-LINEAR alias
 */
void* osConvertOldLINEARMemToNew(const void* p)
{
	return (void*)((uintptr_t)p + 0x10000000);
}
/**
 * @brief Allocates host memory or injects an allocation failure
 * @param[in] size Requested allocation bytes
 * @return Host allocation or NULL for the configured failure
 */
void* linearAlloc(size_t size)
{
	if (fail_allocation)
		return NULL;
	++allocations;
	return malloc(size);
}
/**
 * @brief Records and frees a mock LINEAR allocation
 * @param[in] p Non-NULL allocation previously returned by linearAlloc
 */
void linearFree(void* p)
{
	assert(p);
	++frees;
	free(p);
}

/**
 * @brief Resets mock IPC state and installs recognizable reply data
 */
static void reset(void)
{
	memset(command, 0xCD, sizeof(command));
	for (u32 i = 0; i < 32; ++i)
		reply[i] = 0xABC00000 + i;
	reply_result = MVD_STATUS_OK;
	transport_result = 0;
	lifecycle = false;
	fail_command = 0;
	history_count = 0;
}

/**
 * @brief Checks one mock service call and its encoded request header
 * @param[in] expression API call to evaluate exactly once
 * @param[in] header Expected encoded IPC header
 */
#define CALL(expression, header)                                                                                       \
	do                                                                                                                 \
	{                                                                                                                  \
		Result result_ = (expression);                                                                                 \
		assert(result_ == reply_result);                                                                               \
		assert(sent[0] == (header));                                                                                   \
	} while (0)

/**
 * @brief Checks STD opcodes, descriptors, output validity and buffer-table bounds
 */
static void codec_wire(void)
{
	reset();
	assert(mvdstdOpen() == 0);
	assert(mvdstdOpen() == MVD_ERROR_INITIALIZATION);
	CALL(MVDSTD_Initialize((u32*)(uintptr_t)0x30001000, 0x123456), 0x10082);
	assert(sent[1] == 0x30001000 && sent[2] == 0x123456 && sent[3] == 0 && sent[4] == CUR_PROCESS_HANDLE);
	CALL(MVDSTD_Shutdown(), 0x20000);
	MVDSTD_CalculateWorkBufSizeConfig calc = {0};
	calc.width = 640;
	calc.height = 480;
	u32 size;
	reply_result = 0;
	CALL(MVDSTD_CalculateWorkBufSize(&calc, &size), 0x30300);
	assert(sent[11] == 640 && sent[12] == 480 && size == reply[0]);
	CALL(MVDSTD_CalculateImageSize(640, 480, MVD_PIX_BGR32, &size), 0x400C0);
	assert(sent[1] == 640 && sent[2] == 480 && sent[3] == 0x41002);
	unsigned before = sends;
	assert(MVDSTD_CalculateImageSize(32, 32, MVD_PIX_BGR565, &size) == MVD_ERROR_PARAMETER);
	assert(MVDSTD_CalculateImageSize(UINT32_MAX, UINT32_MAX, MVD_PIX_BGR32, &size) == MVD_ERROR_PARAMETER);
	assert(MVDSTD_CalculateImageSize(0x80000000, 0x80000000, MVD_PIX_BGR32, &size) == MVD_ERROR_PARAMETER);
	assert(sends == before && size == 0);
	reply_result = MVD_STATUS_OK;
	CALL(MVDSTD_H264Initialize(-1, 1, 0, MVD_REFERENCE_FIELD_DPB), 0x50100);
	assert(sent[1] == UINT32_MAX && sent[2] == 1 && sent[4] == (1u << 30));
	CALL(MVDSTD_H264EnableMvc(), 0x60000);
	CALL(MVDSTD_H264Release(), 0x70000);
	MVDSTD_ProcessNALUnitOut progress;
	reply_result = MVD_STATUS_HEADERS_READY;
	CALL(MVDSTD_ProcessNALUnit(0x30001000, 0x20001000, 321, 17, 1, &progress), 0x80142);
	assert(sent[1] == 0x30001000 && sent[3] == 321 && sent[4] == 17 && sent[5] == 1);
	assert(sent[6] == 0 && sent[7] == CUR_PROCESS_HANDLE && progress.remaining_size == reply[2]);
	reply_result = MVD_ERROR_MEMORY;
	CALL(MVDSTD_ProcessNALUnit(1, 2, 3, 4, 0, &progress), 0x80142);
	assert(progress.end_vaddr == 0 && progress.remaining_size == 0);
	MVDSTD_H264Picture h264;
	reply_result = MVD_STATUS_PICTURE_READY;
	CALL(MVDSTD_H264NextPicture(-1, &h264), 0x90042);
	assert(sent[1] == UINT32_MAX && sent[2] == 0 && sent[3] == CUR_PROCESS_HANDLE);
	assert(h264.width == reply[0] && h264.reserved[0] == 0);
	CALL(MVDSTD_H264Peek(&h264), 0xB0000);
	assert(h264.view_id == 0);
	reply_result = MVD_STATUS_OK;
	CALL(MVDSTD_H264NextPicture(0, &h264), 0x90042);
	assert(h264.width == 0 && h264.output_vaddr == 0);
	MVDSTD_H264Info hi;
	CALL(MVDSTD_H264GetInfo(&hi), 0xA0000);
	assert(hi.width == reply[0]);
	reply_result = MVD_ERROR_HEADERS;
	CALL(MVDSTD_H264GetInfo(&hi), 0xA0000);
	assert(hi.width == 0);
	reply_result = MVD_STATUS_OK;
	CALL(MVDSTD_Vp8Initialize(MVD_WEBP, -1, 4, 1), 0xC0100);
	assert(sent[1] == 3 && sent[2] == UINT32_MAX && sent[3] == 4 && sent[4] == 1);
	CALL(MVDSTD_Vp8Release(), 0xD0000);
	MVDSTD_Vp8Input input = {0x30000000, 0x20000000, 300, 4, 1, 2, 3, 4};
	CALL(MVDSTD_Vp8Decode(&input), 0xE0202);
	assert(!memcmp(sent + 1, &input, 32) && sent[9] == 0 && sent[10] == CUR_PROCESS_HANDLE);
	MVDSTD_Vp8Picture vp8;
	reply_result = MVD_STATUS_PICTURE_READY;
	CALL(MVDSTD_Vp8NextPicture(1, &vp8), 0xF0042);
	CALL(MVDSTD_Vp8Peek(&vp8), 0x110000);
	assert(vp8.output_layout == MVD_LAYOUT_UNAVAILABLE && vp8.slice_rows == 0);
	reply_result = MVD_STATUS_OK;
	MVDSTD_Vp8Info vi;
	CALL(MVDSTD_Vp8GetInfo(&vi), 0x100000);
	assert(vi.output_format == reply[9] && vi.reserved[0] == 0);
	CALL(MVDSTD_Vp6Initialize(-1, 3, 0), 0x1200C0);
	assert(sent[1] == UINT32_MAX);
	CALL(MVDSTD_Vp6Release(), 0x130000);
	CALL(MVDSTD_Vp6Decode(0x30000000, 0x20000000, 128), 0x1400C2);
	assert(sent[4] == 0 && sent[5] == CUR_PROCESS_HANDLE);
	MVDSTD_Vp6Picture vp6;
	reply_result = MVD_STATUS_PICTURE_READY;
	CALL(MVDSTD_Vp6NextPicture(0, &vp6), 0x150042);
	CALL(MVDSTD_Vp6Peek(&vp6), 0x170000);
	assert(vp6.output_layout == MVD_LAYOUT_UNAVAILABLE && vp6.reserved[2] == 0);
	reply_result = MVD_STATUS_OK;
	MVDSTD_Vp6Info v6i;
	CALL(MVDSTD_Vp6GetInfo(&v6i), 0x160000);
	assert(v6i.output_format == reply[8] && v6i.reserved[1] == 0);
	CALL(MVDSTD_PpInitialize(), 0x180000);
	CALL(MVDSTD_PpRelease(), 0x190000);
	CALL(MVDSTD_PpGetResult(), 0x1A0000);
	CALL(MVDSTD_PpEnableCombinedMode(MVD_PP_VP8), 0x1B0040);
	assert(sent[1] == 9);
	CALL(MVDSTD_PpDisableCombinedMode(), 0x1C0000);
	MVDSTD_Config config = {0};
	CALL(MVDSTD_GetConfig(&config), 0x1D0042);
	assert(sent[1] == 284 && sent[2] == 0x11CC);
	CALL(MVDSTD_SetConfig(&config), 0x1E0044);
	assert(sent[1] == 284 && sent[2] == 0 && sent[3] == CUR_PROCESS_HANDLE && sent[4] == 0x11CA);
	MVDSTD_OutputBuffersEntryList list = {0};
	list.total_entries = 17;
	list.entries[16].outdata0 = (void*)(uintptr_t)0x30000100;
	list.entries[16].outdata1 = (void*)(uintptr_t)0x30000200;
	CALL(mvdstdSetupOutputBuffers(&list, 4096), 0x1F0902);
	assert(sent[34] == 0x30000100 && sent[35] == 0x30000200 && sent[36] == 4096);
	assert(sent[37] == 0 && sent[38] == CUR_PROCESS_HANDLE);
	before = sends;
	list.total_entries = 18;
	assert(mvdstdSetupOutputBuffers(&list, 4096) == MVD_ERROR_PARAMETER && sends == before);
	list.total_entries = 0;
	assert(mvdstdSetupOutputBuffers(&list, 4096) == MVD_ERROR_PARAMETER && sends == before);
	MVDSTD_OutputBuffersEntry output;
	CALL(MVDSTD_GetNextOutput(&output), 0x200002);
	assert((uintptr_t)output.outdata0 == reply[0] && sent[1] == 0 && sent[2] == CUR_PROCESS_HANDLE);
	CALL(mvdstdOverrideOutputBuffers((void*)1, (void*)2, (void*)3, (void*)4), 0x210100);
	assert(sent[1] == 1 && sent[4] == 4);
	transport_result = (Result)0xD9000001;
	memset(&hi, 0xAB, sizeof(hi));
	assert(MVDSTD_H264GetInfo(&hi) == transport_result && hi.width == 0);
	mvdstdClose();
}

/**
 * @brief Checks auxiliary engine routing, DMA packing, reply bounds and handle ownership
 */
static void auxiliary_wire(void)
{
	reset();
	reply_result = 0;
	assert(l2bInit(L2B_ENGINE_0) == 0 && l2bInit(L2B_ENGINE_1) == 0);
	CALL(L2BU_SetInputFormat(L2B_ENGINE_0, L2B_RGBA8888), 0x10040);
	assert(sent_handle == 20);
	CALL(L2BU_SetOutputFormat(L2B_ENGINE_1, L2B_RGB565), 0x30040);
	assert(sent_handle == 21);
	L2BU_Format format;
	reply[0] = 0xFFFFFF02;
	CALL(L2BU_GetInputFormat(L2B_ENGINE_0, &format), 0x20000);
	assert(format == 2);
	u16 width;
	reply[0] = 0xAAAA0120;
	CALL(L2BU_GetInputLineWidth(L2B_ENGINE_0, &width), 0xD0000);
	assert(width == 0x120);
	CALL(L2BU_SetSending(L2B_ENGINE_1, (void*)0x12340000, 320, 64, -16), 0x80102);
	assert(sent[3] == 64 && sent[4] == 0xFFFFFFF0 && sent[5] == 0 && sent[6] == CUR_PROCESS_HANDLE);
	CALL(L2BU_SetReceiving(L2B_ENGINE_0, (void*)0x12340000, 320, 64, 16), 0xA0102);
	L2BU_ConversionParams params = {0, 3, 320, 240, 0x80};
	CALL(L2BU_SetPackageParameter(L2B_ENGINE_0, &params), 0x150080);
	assert(sent[1] == 0x01400300 && sent[2] == 0x008000F0);
	struct
	{
		L2BU_ConversionParams params;
		u32 canary;
	} lout = {{0}, 0xFEEDBEEF};
	CALL(L2BU_GetPackageParameter(L2B_ENGINE_1, &lout.params), 0x160000);
	assert(lout.canary == 0xFEEDBEEF);
	Handle event;
	reply[1] = 0x123;
	unsigned old_closes = closes;
	CALL(L2BU_GetTransferEndEvent(L2B_ENGINE_0, &event), 0x70000);
	assert(event == 0x123 && closes == old_closes);
	assert(y2r2Init() == 0);
	assert(sent_handle == 22 && sent[0] == 0x2B0000);
	Y2R2U_ConversionParams yparams = {1, 0, 2, 1, 320, 240, 2, 0xAB, 0x80};
	CALL(Y2R2U_SetConversionParams(&yparams), 0x2900C0);
	assert(sent[1] == 0x01020001 && sent[2] == 0x00F00140 && sent[3] == 0x00800002);
	assert(yparams.unused == 0xAB);
	struct
	{
		Y2R2U_ConversionParams params;
		u32 canary;
	} yout = {{0}, 0xFEEDBEEF};
	memset(reply, 0xFF, sizeof(reply));
	CALL(Y2R2U_GetConversionParams(&yout.params), 0x2D0000);
	assert(yout.canary == 0xFEEDBEEF && yout.params.unused == 0 && yout.params.alpha == 0xFFFF);
	CALL(Y2R2U_SetReceiving((void*)0x12340000, 1024, 128, -8), 0x180102);
	assert(sent[3] == 128 && sent[4] == 0xFFFFFFF8 && sent[5] == 0 && sent[6] == CUR_PROCESS_HANDLE);
	Y2R2U_ColorCoefficients coefficients;
	reply_result = (Result)0xE0E053ED;
	CALL(Y2R2U_GetCoefficients(&coefficients), 0x1F0000);
	assert(coefficients.rgb_Y == 0 && coefficients.b_offset == 0);
	reply_result = 0;
	unsigned before = sends;
	assert(Y2R2U_GetStandardCoefficient(&coefficients, (Y2R2U_StandardCoefficient)4) < 0 && sends == before);
	assert(Y2R2U_SetInputFormat((Y2R2U_InputFormat)256) < 0 && sends == before);
	assert(Y2R2U_SetReceiving((void*)1, 12, 0, 0) < 0 && sends == before);
	assert(L2BU_SetInputLineWidth(L2B_ENGINE_0, 7) < 0 && sends == before);
	assert(L2BU_SetInputLines(L2B_ENGINE_0, 1025) < 0 && sends == before);
	transport_result = (Result)0xD9000001;
	event = 0x777;
	assert(Y2R2U_GetTransferEndEvent(&event) == transport_result && event == 0 && closes == old_closes);
	transport_result = 0;
	l2bExit(L2B_ENGINE_0);
	l2bExit(L2B_ENGINE_1);
	y2r2Exit();
}

/**
 * @brief Tests whether a command was recorded in the current mock history
 * @param[in] id Command identifier to locate
 * @return True if the command was recorded, otherwise false
 */
static bool seen(u32 id)
{
	for (u32 i = 0; i < history_count; ++i)
		if (history[i] == id)
			return true;
	return false;
}

/**
 * @brief Checks component cleanup and convenience-API state across failures
 */
static void lifecycle_tests(void)
{
	reset();
	lifecycle = true;
	fail_command = 0x18;
	assert(mvdstdInit(MVDMODE_VIDEOPROCESSING, MVD_INPUT_H264, MVD_OUTPUT_BGR565, 4096, NULL) == MVD_ERROR_MEMORY);
	assert(seen(5) && seen(7) && seen(2) && !seen(0x19) && !seen(9));
	assert(allocations == frees);
	reset();
	lifecycle = true;
	fail_command = 0x1B;
	assert(mvdstdInit(MVDMODE_VIDEOPROCESSING, MVD_INPUT_H264, MVD_OUTPUT_BGR565, 4096, NULL) == MVD_ERROR_MEMORY);
	assert(seen(0x19) && seen(7) && seen(2));
	reset();
	lifecycle = true;
	assert(mvdstdInit(MVDMODE_COLORFORMATCONV, MVD_INPUT_YUYV422, MVD_OUTPUT_BGR565, 0, NULL) == 0);
	assert(mvdstdInit(MVDMODE_VIDEOPROCESSING, MVD_INPUT_H264, MVD_OUTPUT_YUYV422, 1024, NULL) == 0);
	MVDSTD_Config config;
	mvdstdGenerateDefaultConfig(&config, 320, 240, 320, 240, (u32*)1, (u32*)2, NULL);
	assert(config.input_type == MVD_INPUT_YUYV422 && config.output_type == MVD_OUTPUT_BGR565);
	assert(mvdstdConvertImage(&config) == MVD_STATUS_OK);
	mvdstdExit();
	assert(!seen(2));
	mvdstdExit();
	assert(seen(2) && !seen(5) && !seen(7) && !seen(9) && !seen(0x1C));
	reset();
	lifecycle = true;
	assert(mvdstdInit(MVDMODE_VIDEOPROCESSING, MVD_INPUT_H264, MVD_OUTPUT_BGR565, 1024, NULL) == 0);
	assert(mvdstdRenderVideoFrame(NULL, false) == MVD_STATUS_OK);
	assert(sent[0] == 0x90042 && sent[1] == 0);
	assert(mvdstdProcessVideoFrame((void*)0x14000000, 123, 0, NULL) == MVD_STATUS_OK);
	assert(sent[1] == 0x24000000 && sent[2] == 0x34000000);
	mvdstdExit();
	assert(allocations == frees);
	reset();
	lifecycle = true;
	unsigned before = sends;
	assert(mvdstdInit(MVDMODE_VIDEOPROCESSING, MVD_INPUT_H264, MVD_OUTPUT_BGR565, UINT32_MAX, NULL) < 0);
	assert(sends == before);
	fail_allocation = true;
	assert(mvdstdInit(MVDMODE_COLORFORMATCONV, MVD_INPUT_YUYV422, MVD_OUTPUT_BGR565, 0, NULL) == MVD_ERROR_MEMORY);
	fail_allocation = false;
	assert(mvdstdOpen() == 0);
	mvdstdClose();
}

/**
 * @brief Runs the host regression checks
 * @return Zero when all assertions pass
 */
int main(void)
{
	codec_wire();
	auxiliary_wire();
	lifecycle_tests();
	puts("IPC encoding, result validity, reply bounds and lifecycle checks passed");
	return 0;
}
