# Applied function names and prototypes

Addresses are IDA virtual addresses. This inventory contains 208 applied functions. `MVDSTD_`, `MVDL2B_`, `MVDY2R_` and `Mvd` names describe recovered service/platform behavior. Hantro names identify source counterparts, except the explicitly suffixed `PPChangeOutputBuffer_MVD`. DWL API names identify the abstraction boundary; their implementations use Nintendo memory/interrupt services. `ceilf`/`floorf` are runtime semantic identifications with recovered VFP calling conventions.

The existing `MVDSTD_HandleCommands` (`0x1124E8`), `L2BU_HandleCommands` (`0x111E64`) and `Y2RU_HandleCommands` (`0x11328C`) names were preserved, with context prototypes refined. Source-family confidence and ABI differences are documented in [external-code.md](external-code.md).

| Address | Function | Applied declaration |
|---|---|---|
| `0x101788` | `DWLReadAsicFuseStatus` | `void DWLReadAsicFuseStatus(u32 *fuses)` |
| `0x102D08` | `MvdCalculateImageSize` | `u32 MvdCalculateImageSize(u32 width, u32 height, u32 format)` |
| `0x102E84` | `MvdCalculateWorkBufferSize` | `u32 MvdCalculateWorkBufferSize(const MvdWorkSizeParams *params)` |
| `0x102F2C` | `H264DecDecode` | `H264DecRet H264DecDecode(H264DecInst instance, const H264DecInput *input, H264DecOutput *output)` |
| `0x103AD4` | `H264DecGetInfo` | `H264DecRet H264DecGetInfo(H264DecInst instance, MvdH264Info *info)` |
| `0x103BE0` | `H264DecInit` | `H264DecRet H264DecInit(H264DecInst *instance, u32 noOutputReordering, u32 useVideoFreezeConcealment, u32 useDisplaySmoothing, u32 referenceFrameFormat)` |
| `0x103D4C` | `H264DecNextPicture` | `H264DecRet H264DecNextPicture(H264DecInst instance, MvdH264Picture *picture, u32 endOfStream)` |
| `0x104328` | `H264DecPeek` | `H264DecRet H264DecPeek(H264DecInst instance, MvdH264Picture *picture)` |
| `0x104428` | `H264DecRelease` | `void H264DecRelease(H264DecInst instance)` |
| `0x1044E0` | `H264DecSetMvc` | `H264DecRet H264DecSetMvc(H264DecInst instance)` |
| `0x10635C` | `PPCheckConfig` | `i32 PPCheckConfig(MvdPpContainer *pp, PPConfig *config, u32 decoderLinked, u32 decoderType)` |
| `0x106918` | `PPCheckSetupChanges` | `u32 PPCheckSetupChanges(PPConfig *previous, PPConfig *current)` |
| `0x106AF0` | `PPDecCombinedModeEnable` | `PPResult PPDecCombinedModeEnable(MvdPpContainer *pp, const void *decoder, u32 decoderType)` |
| `0x106B98` | `PPDecConfigQueryFromDec` | `void PPDecConfigQueryFromDec(MvdPpContainer *pp, u32 *query)` |
| `0x106C98` | `PPDecDisplayIndex` | `void PPDecDisplayIndex(MvdPpContainer *pp, u32 index)` |
| `0x106CA2` | `PPDecEndCallback` | `void PPDecEndCallback(MvdPpContainer *pp)` |
| `0x106CF0` | `PPDecSetMultipleOutput` | `PPResult PPDecSetMultipleOutput(MvdPpContainer *pp, const PPOutputBuffers *buffers)` |
| `0x106EB0` | `PPDecStartPp` | `void PPDecStartPp(MvdPpContainer *pp, void *interfaceData)` |
| `0x10741C` | `PPChangeOutputBuffer_MVD` | `PPResult PPChangeOutputBuffer_MVD(MvdPpContainer *pp, const PPOutput *current, const PPOutput *replacement)` |
| `0x1074D8` | `PPFlushRegs` | `void PPFlushRegs(MvdPpContainer *pp)` |
| `0x1074FC` | `PPGetConfig` | `PPResult PPGetConfig(MvdPpContainer *pp, PPConfig *config)` |
| `0x107520` | `PPGetNextOutput` | `PPResult PPGetNextOutput(MvdPpContainer *pp, PPOutput *output)` |
| `0x107628` | `PPGetResult` | `PPResult PPGetResult(MvdPpContainer *pp)` |
| `0x107670` | `PPInit` | `PPResult PPInit(PPInst *instance)` |
| `0x107720` | `PPInitDataStructures` | `void PPInitDataStructures(MvdPpContainer *pp)` |
| `0x107798` | `PPInitHW` | `void PPInitHW(MvdPpContainer *pp)` |
| `0x107860` | `PPIsInPixFmtOk` | `u32 PPIsInPixFmtOk(u32 pixelFormat, const MvdPpContainer *pp);` |
| `0x107918` | `PPIsOutPixFmtOk` | `u32 PPIsOutPixFmtOk(u32 pixelFormat, const MvdPpContainer *pp);` |
| `0x1079A8` | `PPRelease` | `void PPRelease(MvdPpContainer *pp)` |
| `0x1079D0` | `PPSelectOutputSize` | `i32 PPSelectOutputSize(MvdPpContainer *pp)` |
| `0x107A44` | `PPSetConfig` | `PPResult PPSetConfig(MvdPpContainer *pp, PPConfig *config)` |
| `0x108540` | `PPSetupHW` | `void PPSetupHW(MvdPpContainer *pp)` |
| `0x109330` | `VP6DecDecode` | `VP6DecRet VP6DecDecode(VP6DecInst instance, const VP6DecInput *input, VP6DecOutput *output)` |
| `0x1095BC` | `VP6DecGetInfo` | `VP6DecRet VP6DecGetInfo(VP6DecInst instance, MvdVp6Info *info)` |
| `0x109628` | `VP6DecInit` | `VP6DecRet VP6DecInit(VP6DecInst *instance, u32 freezeConcealment, u32 numFrameBuffers, u32 referenceFrameFormat)` |
| `0x10972C` | `VP6DecNextPicture` | `VP6DecRet VP6DecNextPicture(VP6DecInst instance, MvdVp6Picture *picture, u32 endOfStream)` |
| `0x109874` | `VP6DecPeek` | `VP6DecRet VP6DecPeek(VP6DecInst instance, MvdVp6Picture *picture)` |
| `0x1098D2` | `VP6DecRelease` | `void VP6DecRelease(VP6DecInst instance)` |
| `0x10B3F0` | `MvdCalculateMacroblocks` | `u32 MvdCalculateMacroblocks(u32 height, u32 width)` |
| `0x10B43C` | `MvdY2rReadStandardCoefficients` | `Result MvdY2rReadStandardCoefficients(void *registerContext, MvdY2rCoefficients *output, u32 index);` |
| `0x10B5D8` | `MvdClientVirtualToBus` | `u32 MvdClientVirtualToBus(u32 address, u32 size)` |
| `0x10B608` | `MvdConvertLibraryResult` | `Result MvdConvertLibraryResult(s32 status, u32 codecClass, u32 postprocessor)` |
| `0x10B9BE` | `MvdTranslateLinearRange` | `u32 MvdTranslateLinearRange(u32 address, u32 size, u32 rangeBegin, u32 rangeEnd, u32 busBase)` |
| `0x10CE70` | `PPDecCombinedModeDisable` | `PPResult PPDecCombinedModeDisable(MvdPpContainer *pp, const void *decoder)` |
| `0x10CEF0` | `WaitForPp` | `PPResult WaitForPp(MvdPpContainer *pp)` |
| `0x10CF84` | `PPRun` | `PPResult PPRun(MvdPpContainer *pp)` |
| `0x10D2C4` | `PPSetStatus` | `void PPSetStatus(MvdPpContainer *pp, u32 status)` |
| `0x10D2CE` | `PPRefreshRegs` | `void PPRefreshRegs(MvdPpContainer *pp)` |
| `0x10D2F0` | `PPGetStatus` | `u32 PPGetStatus(MvdPpContainer *pp)` |
| `0x10DCCE` | `h264bsdNextOutputPicture` | `void *h264bsdNextOutputPicture(void *storage)` |
| `0x10DD26` | `DWLRelease` | `int DWLRelease(void *dwl)` |
| `0x10DE04` | `DWLInit` | `void *DWLInit(const u32 *params)` |
| `0x10DE0C` | `DWLReadAsicConfig` | `void DWLReadAsicConfig(MvdDwlHwConfig *config);` |
| `0x10E03C` | `DWLmemset` | `void *DWLmemset(void *dest, int value, u32 size)` |
| `0x10E044` | `DWLReadAsicID` | `u32 DWLReadAsicID(void)` |
| `0x10E050` | `h264bsdCroppingParams` | `void h264bsdCroppingParams(void *storage, u32 *croppingFlag, u32 *left, u32 *width, u32 *top, u32 *height)` |
| `0x10E0BE` | `h264bsdPicHeight` | `u32 h264bsdPicHeight(void *storage)` |
| `0x10E0C8` | `h264bsdPicWidth` | `u32 h264bsdPicWidth(void *storage)` |
| `0x10E0FC` | `GetDecRegister` | `u32 GetDecRegister(const u32 *registers, MvdHwIf field)` |
| `0x10E9D4` | `MvdYuv420ReferenceBufferBytes` | `u32 MvdYuv420ReferenceBufferBytes(u32 height, u32 width, u32 count)` |
| `0x10F298` | `h264bsdDecodeExpGolombSigned` | `u32 h264bsdDecodeExpGolombSigned(strmData_t *stream, s32 *value);` |
| `0x10F324` | `SetDecRegister` | `void SetDecRegister(u32 *registers, MvdHwIf field, u32 value)` |
| `0x10F354` | `h264bsdDecodeExpGolombUnsigned` | `u32 h264bsdDecodeExpGolombUnsigned(strmData_t *stream, u32 *value);` |
| `0x10F448` | `h264bsdGetBits` | `u32 h264bsdGetBits(strmData_t *stream, u32 numBits);` |
| `0x10F474` | `MvdCopyMemory` | `void *MvdCopyMemory(void *dest, const void *source, u32 size)` |
| `0x10F500` | `MvdHeapFree` | `void MvdHeapFree(void *ptr)` |
| `0x10F694` | `DWLfree` | `void DWLfree(void *ptr)` |
| `0x10F6A0` | `DWLmalloc` | `void *DWLmalloc(u32 size)` |
| `0x10F780` | `DWLmemcpy` | `void *DWLmemcpy(void *dest, const void *source, u32 size)` |
| `0x10F82C` | `MvdHeapAlloc` | `void *MvdHeapAlloc(u32 size)` |
| `0x10FDB4` | `MvdL2bWriteOutputFormat` | `Result MvdL2bWriteOutputFormat(u32 *registerOffset, u32 shiftedFormat);` |
| `0x10FDD0` | `MvdL2bWriteInputFormat` | `Result MvdL2bWriteInputFormat(u32 *registerOffset, u32 format);` |
| `0x10FEF0` | `MVDY2R_DriverFinalize` | `Result MVDY2R_DriverFinalize(void);` |
| `0x110138` | `MvdIpcWriteResponseHeader` | `u32 MvdIpcWriteResponseHeader(u32 **buffer, u32 command, u32 normalWords, u32 translatedWords, u32 extra);` |
| `0x1107CC` | `VP8DecDecode` | `VP8DecRet VP8DecDecode(VP8DecInst instance, const VP8DecInput *input, VP8DecOutput *output)` |
| `0x110E20` | `VP8DecGetInfo` | `VP8DecRet VP8DecGetInfo(VP8DecInst instance, MvdVp8Info *info)` |
| `0x110EA0` | `VP8DecInit` | `VP8DecRet VP8DecInit(VP8DecInst *instance, VP8DecFormat format, u32 freezeConcealment, u32 numFrameBuffers, u32 referenceFrameFormat)` |
| `0x11102C` | `VP8DecNextPicture` | `VP8DecRet VP8DecNextPicture(VP8DecInst instance, MvdVp8Picture *picture, u32 endOfStream)` |
| `0x1113F0` | `VP8DecPeek` | `VP8DecRet VP8DecPeek(VP8DecInst instance, MvdVp8Picture *picture)` |
| `0x1114BC` | `VP8DecRelease` | `void VP8DecRelease(VP8DecInst instance)` |
| `0x11216C` | `MVDL2B_SetSending` | `Result MVDL2B_SetSending(void *context, Handle process, void *address, u32 size, s16 unit, s16 gap);` |
| `0x112238` | `MVDL2B_PingProcess` | `Result MVDL2B_PingProcess(void *context, u8 *sessions);` |
| `0x112242` | `MVDL2B_SetReceiving` | `Result MVDL2B_SetReceiving(void *context, Handle process, void *address, u32 size, s16 unit, s16 gap);` |
| `0x112268` | `MVDL2B_GetInputLines` | `Result MVDL2B_GetInputLines(void *context, u16 *lines);` |
| `0x112272` | `MVDL2B_SetInputLines` | `Result MVDL2B_SetInputLines(void *context, u16 lines);` |
| `0x11227C` | `MVDL2B_GetInputFormat` | `Result MVDL2B_GetInputFormat(void *context, u8 *format);` |
| `0x112290` | `MVDL2B_SetInputFormat` | `Result MVDL2B_SetInputFormat(void *context, u8 format);` |
| `0x11229A` | `MVDL2B_StopConversion` | `Result MVDL2B_StopConversion(void *context);` |
| `0x1122A4` | `MVDL2B_GetOutputFormat` | `Result MVDL2B_GetOutputFormat(void *context, u8 *format);` |
| `0x1122BA` | `MVDL2B_SetOutputFormat` | `Result MVDL2B_SetOutputFormat(void *context, u8 format);` |
| `0x1122C8` | `MVDL2B_StartConversion` | `Result MVDL2B_StartConversion(void *context);` |
| `0x11230C` | `MVDL2B_IsBusyConversion` | `Result MVDL2B_IsBusyConversion(void *context, u8 *busy);` |
| `0x112318` | `MvdL2bConfigureReceivingDma` | `Result MvdL2bConfigureReceivingDma(void *context, Handle process, void *address, u32 size, s16 unit, s16 gap);` |
| `0x1123D4` | `MVDL2B_GetInputLineWidth` | `Result MVDL2B_GetInputLineWidth(void *context, u16 *width);` |
| `0x1123DE` | `MVDL2B_IsDoneSending` | `Result MVDL2B_IsDoneSending(void *context, u8 *done);` |
| `0x1123EE` | `MVDL2B_SetInputLineWidth` | `Result MVDL2B_SetInputLineWidth(void *context, u16 width);` |
| `0x1123F8` | `MVDL2B_GetPackageParameter` | `Result MVDL2B_GetPackageParameter(void *context, MvdL2bParams *params);` |
| `0x112444` | `MVDL2B_GetTransferEndEvent` | `Result MVDL2B_GetTransferEndEvent(void *context, Handle *event);` |
| `0x11244C` | `MVDL2B_IsDoneReceiving` | `Result MVDL2B_IsDoneReceiving(void *context, u8 *done);` |
| `0x11245C` | `MVDL2B_SetPackageParameter` | `Result MVDL2B_SetPackageParameter(void *context, MvdL2bParams *params);` |
| `0x1124A4` | `MVDL2B_GetTransferEndInterrupt` | `Result MVDL2B_GetTransferEndInterrupt(void *context, u8 *enable);` |
| `0x1124AE` | `MVDL2B_SetTransferEndInterrupt` | `Result MVDL2B_SetTransferEndInterrupt(void *context, s8 enable);` |
| `0x1124B8` | `MVDL2B_GetAlpha` | `Result MVDL2B_GetAlpha(void *context, u16 *alpha);` |
| `0x1124C2` | `MVDL2B_SetAlpha` | `Result MVDL2B_SetAlpha(void *context, u16 alpha);` |
| `0x112BC4` | `MvdNormalizeLibraryStatus` | `u16 MvdNormalizeLibraryStatus(s32 status, u32 codecClass, u32 postprocessor)` |
| `0x112C04` | `MVDSTD_H264Decode` | `Result MVDSTD_H264Decode(MvdSession *session, Handle process, H264DecInput input, H264DecOutput *output)` |
| `0x112C6C` | `MVDSTD_Vp6GetInfo` | `Result MVDSTD_Vp6GetInfo(MvdSession *session, MvdVp6Info *info)` |
| `0x112C7E` | `MVDSTD_Vp8GetInfo` | `Result MVDSTD_Vp8GetInfo(MvdSession *session, MvdVp8Info *info)` |
| `0x112C90` | `MVDSTD_Vp6Release` | `Result MVDSTD_Vp6Release(MvdSession *session)` |
| `0x112C9C` | `MVDSTD_Vp8Release` | `Result MVDSTD_Vp8Release(MvdSession *session)` |
| `0x112CA8` | `MVDSTD_H264EnableMvc` | `Result MVDSTD_H264EnableMvc(MvdSession *session)` |
| `0x112CBC` | `MVDSTD_Shutdown` | `Result MVDSTD_Shutdown(MvdSession *session)` |
| `0x112CE0` | `MVDSTD_H264GetInfo` | `Result MVDSTD_H264GetInfo(MvdSession *session, MvdH264Info *info)` |
| `0x112CF2` | `MVDSTD_PpGetConfig` | `Result MVDSTD_PpGetConfig(MvdSession *session, PPConfig *config, u32 mappedSize)` |
| `0x112D04` | `MVDSTD_PpGetResult` | `Result MVDSTD_PpGetResult(MvdSession *session)` |
| `0x112D16` | `MVDSTD_H264Release` | `Result MVDSTD_H264Release(MvdSession *session)` |
| `0x112D24` | `MVDSTD_PpSetConfig` | `Result MVDSTD_PpSetConfig(MvdSession *session, Handle process, PPConfig *config)` |
| `0x112D88` | `MVDSTD_Initialize` | `Result MVDSTD_Initialize(MvdSession *session, Handle process, u32 workAddress, u32 workSize)` |
| `0x112DE8` | `MVDSTD_GetNextOutput` | `Result MVDSTD_GetNextOutput(MvdSession *session, Handle process, MvdClientBufferPair *output)` |
| `0x112E58` | `MVDSTD_Vp6NextPicture` | `Result MVDSTD_Vp6NextPicture(MvdSession *session, Handle process, MvdVp6Picture *picture, s32 endOfStream)` |
| `0x112E8C` | `MVDSTD_Vp8NextPicture` | `Result MVDSTD_Vp8NextPicture(MvdSession *session, Handle process, MvdVp8Picture *picture, s32 endOfStream)` |
| `0x112EC0` | `MVDSTD_H264NextPicture` | `Result MVDSTD_H264NextPicture(MvdSession *session, Handle process, MvdH264Picture *picture, s32 endOfStream)` |
| `0x112EF4` | `MVDSTD_SetupOutputBuffers` | `Result MVDSTD_SetupOutputBuffers(MvdSession *session, Handle process, MvdClientOutputBuffers *buffers, u32 bufferSize)` |
| `0x112FB4` | `MVDSTD_PpEnableCombinedMode` | `Result MVDSTD_PpEnableCombinedMode(MvdSession *session, u32 decoderType)` |
| `0x112FCA` | `MVDSTD_PpDisableCombinedMode` | `Result MVDSTD_PpDisableCombinedMode(MvdSession *session)` |
| `0x112FE0` | `MVDSTD_OverrideOutputBuffers` | `Result MVDSTD_OverrideOutputBuffers(MvdSession *session, u32 currentLuma, u32 currentChroma, u32 newLuma, u32 newChroma)` |
| `0x11307C` | `MVDSTD_CalculateWorkBufSize` | `Result MVDSTD_CalculateWorkBufSize(MvdSession *session, u32 *sizeOut, u32 packedFlags0, u32 packedFlags1, u32 packedFlags2, u32 width, u32 height)` |
| `0x1130B2` | `MVDSTD_CalculateImageSize` | `Result MVDSTD_CalculateImageSize(MvdSession *session, u32 *sizeOut, u32 width, u32 height, u32 format)` |
| `0x1130C8` | `MVDSTD_PpInitialize` | `Result MVDSTD_PpInitialize(MvdSession *session)` |
| `0x113108` | `MVDSTD_Vp6Initialize` | `Result MVDSTD_Vp6Initialize(MvdSession *session, s32 freezeConcealment, u32 frameBuffers, u32 referenceFormat)` |
| `0x113118` | `MVDSTD_Vp8Initialize` | `Result MVDSTD_Vp8Initialize(MvdSession *session, u32 format, s32 freezeConcealment, u32 frameBuffers, u32 referenceFormat)` |
| `0x11312C` | `MVDSTD_Vp6Peek` | `Result MVDSTD_Vp6Peek(MvdSession *session, MvdVp6Picture *picture)` |
| `0x11313E` | `MVDSTD_Vp8Peek` | `Result MVDSTD_Vp8Peek(MvdSession *session, MvdVp8Picture *picture)` |
| `0x113150` | `MVDSTD_H264Initialize` | `Result MVDSTD_H264Initialize(MvdSession *session, s32 noReordering, s32 freezeConcealment, s32 displaySmoothing, u32 referenceFormat)` |
| `0x113164` | `MVDSTD_H264Peek` | `Result MVDSTD_H264Peek(MvdSession *session, MvdH264Picture *picture)` |
| `0x113178` | `MVDSTD_Vp6Decode` | `Result MVDSTD_Vp6Decode(MvdSession *session, Handle process, VP6DecInput input, VP6DecOutput *output)` |
| `0x1131E0` | `MVDSTD_Vp8Decode` | `Result MVDSTD_Vp8Decode(MvdSession *session, Handle process, VP8DecInput input, VP8DecOutput *output)` |
| `0x113248` | `MVDSTD_PpRelease` | `Result MVDSTD_PpRelease(MvdSession *session)` |
| `0x1138B0` | `MVDY2R_GetRotation` | `Result MVDY2R_GetRotation(void *context, u8 *rotation);` |
| `0x1138CC` | `MVDY2R_PingProcess` | `Result MVDY2R_PingProcess(void *context, u8 *sessions);` |
| `0x1138DC` | `MVDY2R_SetRotation` | `Result MVDY2R_SetRotation(void *context, u8 rotation);` |
| `0x1138F0` | `MVDY2R_SetSendingU` | `Result MVDY2R_SetSendingU(void *context, Handle process, void *address, u32 size, s16 unit, s16 gap);` |
| `0x1139B0` | `MVDY2R_SetSendingV` | `Result MVDY2R_SetSendingV(void *context, Handle process, void *address, u32 size, s16 unit, s16 gap);` |
| `0x113A70` | `MVDY2R_SetSendingY` | `Result MVDY2R_SetSendingY(void *context, Handle process, void *address, u32 size, s16 unit, s16 gap);` |
| `0x113B30` | `MVDY2R_SetReceiving` | `Result MVDY2R_SetReceiving(void *context, Handle process, void *address, u32 size, s16 unit, s16 gap);` |
| `0x113B58` | `MVDY2R_GetInputLines` | `Result MVDY2R_GetInputLines(void *context, u16 *lines);` |
| `0x113B68` | `MVDY2R_SetInputLines` | `Result MVDY2R_SetInputLines(void *context, u16 lines);` |
| `0x113B78` | `MVDY2R_SetSendingYUYV` | `Result MVDY2R_SetSendingYUYV(void *context, Handle process, void *address, u32 size, s16 unit, s16 gap);` |
| `0x113BA0` | `MVDY2R_GetInputFormat` | `Result MVDY2R_GetInputFormat(void *context, u8 *format);` |
| `0x113BB8` | `MVDY2R_SetInputFormat` | `Result MVDY2R_SetInputFormat(void *context, u8 format);` |
| `0x113BC8` | `MVDY2R_StopConversion` | `Result MVDY2R_StopConversion(void);` |
| `0x113BD8` | `MVDY2R_GetOutputFormat` | `Result MVDY2R_GetOutputFormat(void *context, u8 *format);` |
| `0x113BF4` | `MVDY2R_SetOutputFormat` | `Result MVDY2R_SetOutputFormat(void *context, u8 format);` |
| `0x113C04` | `MVDY2R_StartConversion` | `Result MVDY2R_StartConversion(void);` |
| `0x113C68` | `MVDY2R_DriverInitialize` | `Result MVDY2R_DriverInitialize(void);` |
| `0x113C8C` | `MVDY2R_IsBusyConversion` | `Result MVDY2R_IsBusyConversion(u8 *busy);` |
| `0x113D48` | `MVDY2R_GetBlockAlignment` | `Result MVDY2R_GetBlockAlignment(void *context, u8 *alignment);` |
| `0x113D64` | `MVDY2R_GetInputLineWidth` | `Result MVDY2R_GetInputLineWidth(void *context, u16 *width);` |
| `0x113D74` | `MVDY2R_SetBlockAlignment` | `Result MVDY2R_SetBlockAlignment(void *context, u8 alignment);` |
| `0x113D88` | `MVDY2R_SetInputLineWidth` | `Result MVDY2R_SetInputLineWidth(void *context, u16 width);` |
| `0x113E64` | `MVDY2R_IsDoneSendingU` | `Result MVDY2R_IsDoneSendingU(void *context, u8 *done);` |
| `0x113E78` | `MVDY2R_IsDoneSendingV` | `Result MVDY2R_IsDoneSendingV(void *context, u8 *done);` |
| `0x113E8C` | `MVDY2R_IsDoneSendingY` | `Result MVDY2R_IsDoneSendingY(void *context, u8 *done);` |
| `0x113EA0` | `MVDY2R_GetConversionParams` | `Result MVDY2R_GetConversionParams(void *context, MvdY2rParams *params);` |
| `0x113F98` | `MVDY2R_GetSpacialDithering` | `Result MVDY2R_GetSpacialDithering(void *context, u8 *enable);` |
| `0x113FA8` | `MVDY2R_GetTransferEndEvent` | `Result MVDY2R_GetTransferEndEvent(void *context, Handle *event);` |
| `0x113FB8` | `MVDY2R_IsDoneReceiving` | `Result MVDY2R_IsDoneReceiving(void *context, u8 *done);` |
| `0x113FCC` | `MVDY2R_SetConversionParams` | `Result MVDY2R_SetConversionParams(void *context, MvdY2rParams *params);` |
| `0x114044` | `MVDY2R_SetSpacialDithering` | `Result MVDY2R_SetSpacialDithering(void *context, s8 enable);` |
| `0x114054` | `MVDY2R_GetCoefficients` | `Result MVDY2R_GetCoefficients(void *context, MvdY2rCoefficients *coefficients);` |
| `0x114064` | `MVDY2R_GetTemporalDithering` | `Result MVDY2R_GetTemporalDithering(void *context, u8 *enable);` |
| `0x114074` | `MVDY2R_IsDoneSendingYUYV` | `Result MVDY2R_IsDoneSendingYUYV(void *context, u8 *done);` |
| `0x114088` | `MVDY2R_SetCoefficients` | `Result MVDY2R_SetCoefficients(void *context, MvdY2rCoefficients coefficients);` |
| `0x1140D4` | `MVDY2R_SetTemporalDithering` | `Result MVDY2R_SetTemporalDithering(void *context, s8 enable);` |
| `0x1140E4` | `MVDY2R_SetStandardCoefficient` | `Result MVDY2R_SetStandardCoefficient(void *context, u8 index);` |
| `0x1140F4` | `MVDY2R_GetTransferEndInterrupt` | `Result MVDY2R_GetTransferEndInterrupt(void *context, u8 *enable);` |
| `0x114104` | `MVDY2R_SetTransferEndInterrupt` | `Result MVDY2R_SetTransferEndInterrupt(void *context, s8 enable);` |
| `0x114114` | `MVDY2R_GetDitheringWeightParams` | `Result MVDY2R_GetDitheringWeightParams(void *context, MvdY2rDitherWeights *weights);` |
| `0x11418C` | `MVDY2R_SetDitheringWeightParams` | `Result MVDY2R_SetDitheringWeightParams(void *context, MvdY2rDitherWeights weights);` |
| `0x11422C` | `MVDY2R_GetStandardCoefficient` | `Result MVDY2R_GetStandardCoefficient(void *context, MvdY2rCoefficients *coefficients, u8 index);` |
| `0x11423C` | `MVDY2R_GetAlpha` | `Result MVDY2R_GetAlpha(void *context, u16 *alpha);` |
| `0x11424C` | `MVDY2R_SetAlpha` | `Result MVDY2R_SetAlpha(void *context, u16 alpha);` |
| `0x114264` | `MvdBindDecoderInterrupt` | `int MvdBindDecoderInterrupt(void)` |
| `0x1142F8` | `MvdCalculateLevelWorkBufferSize` | `u32 MvdCalculateLevelWorkBufferSize(const MvdWorkSizeParams *params)` |
| `0x114350` | `MvdAttachClientWorkBuffer` | `int MvdAttachClientWorkBuffer(u32 address, u32 size)` |
| `0x114B8E` | `h264bsdSarSize` | `void h264bsdSarSize(const void *storage, u32 *width, u32 *height)` |
| `0x114E50` | `h264RegisterPP` | `i32 h264RegisterPP(void *decoder, const void *pp, void (*start)(const void *, const void *), void (*end)(const void *), void (*query)(const void *, void *), void (*display)(const void *, u32))` |
| `0x114EBC` | `h264UnregisterPP` | `i32 h264UnregisterPP(void *decoder, const void *pp)` |
| `0x115C28` | `h264bsdDecode` | `u32 h264bsdDecode(void *decoder, const u8 *stream, u32 length, u32 pictureId, u32 *readBytes);` |
| `0x116F78` | `h264bsdDecodeSeqParamSet` | `u32 h264bsdDecodeSeqParamSet(strmData_t *stream, MvdH264Sps *sps, u32 mvcFlag);` |
| `0x118030` | `h264bsdFlushBuffer` | `void h264bsdFlushBuffer(void *storage)` |
| `0x11804C` | `h264bsdInit` | `void h264bsdInit(void *storage, u32 noReordering, u32 displaySmoothing)` |
| `0x11833E` | `h264bsdIsMonoChrome` | `u32 h264bsdIsMonoChrome(void *storage)` |
| `0x118750` | `h264bsdMatrixCoefficients` | `u32 h264bsdMatrixCoefficients(const void *storage)` |
| `0x1189EE` | `h264bsdShutdown` | `void h264bsdShutdown(void *storage)` |
| `0x118C6C` | `h264bsdVideoRange` | `u32 h264bsdVideoRange(const void *storage)` |
| `0x118CB0` | `MvdDwlCreate` | `void *MvdDwlCreate(const u32 *params)` |
| `0x118D04` | `MvdFillMemory` | `void *MvdFillMemory(void *dest, int value, u32 size)` |
| `0x118E18` | `vp6RegisterPP` | `i32 vp6RegisterPP(void *decoder, const void *pp, void (*start)(const void *, const void *), void (*end)(const void *), void (*query)(const void *, void *))` |
| `0x118E58` | `vp6UnregisterPP` | `i32 vp6UnregisterPP(void *decoder, const void *pp)` |
| `0x118E82` | `vp8RegisterPP` | `i32 vp8RegisterPP(void *decoder, const void *pp, void (*start)(const void *, const void *), void (*end)(const void *), void (*query)(const void *, void *))` |
| `0x118ECC` | `vp8UnregisterPP` | `i32 vp8UnregisterPP(void *decoder, const void *pp)` |
| `0x1197F0` | `MvdIpcWriteVp6Picture` | `void MvdIpcWriteVp6Picture(u32 **commandBuffer, u32 wordIndex, const MvdVp6Picture *picture)` |
| `0x119804` | `MvdIpcWriteVp8Picture` | `void MvdIpcWriteVp8Picture(u32 **commandBuffer, u32 wordIndex, const MvdVp8Picture *picture)` |
| `0x119818` | `MvdIpcWriteH264Picture` | `void MvdIpcWriteH264Picture(u32 **commandBuffer, u32 wordIndex, const MvdH264Picture *picture)` |
| `0x119D78` | `MvdMaxDpbFramesForLevel` | `u32 MvdMaxDpbFramesForLevel(u32 height, u32 width, u32 levelIndex)` |
| `0x119DDC` | `ceilf` | `float __usercall ceilf@<s0>(float value@<s0>)` |
| `0x119E6C` | `floorf` | `float __usercall floorf@<s0>(float value@<s0>)` |

