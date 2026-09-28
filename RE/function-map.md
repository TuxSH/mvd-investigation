# Applied function names and prototypes

Addresses are IDA virtual addresses. This inventory contains 316 applied functions. `MVDSTD_`, `MVDL2B_`, `MVDY2R_` and `Mvd` names describe recovered service/platform behavior. Hantro names identify source counterparts, except the explicitly suffixed `PPChangeOutputBuffer_MVD`. DWL API names identify the abstraction boundary; their implementations use Nintendo memory/interrupt services. SVC names identify verified syscall veneers. `ceilf`/`floorf` are runtime semantic identifications with recovered VFP calling conventions.

The existing `MVDSTD_HandleCommands` (`0x1124E8`), `L2BU_HandleCommands` (`0x111E64`) and `Y2RU_HandleCommands` (`0x11328C`) names were preserved, with context prototypes refined. Source-family confidence and ABI differences are documented in [external-code.md](external-code.md).

| Address | Function | Applied declaration |
|---|---|---|
| `0x1012B0` | `AllocateAsicBuffers` | `u32 AllocateAsicBuffers(MvdH264Container *decoder,MvdH264AsicBuffers *buffers,u32 picSizeInMbs)` |
| `0x101374` | `MvdFlushPpOutputBeforeStart` | `void MvdFlushPpOutputBeforeStart(const MvdDwlInstance *dwl)` |
| `0x101416` | `MvdIsWritableG1Register` | `u32 __spoils<r0,r1> MvdIsWritableG1Register(u32 byteOffset)` |
| `0x101768` | `MvdBusToClientVirtualForCache` | `u32 __spoils<r0,r1,r2> MvdBusToClientVirtualForCache(u32 address,u32 size)` |
| `0x101788` | `DWLReadAsicFuseStatus` | `void DWLReadAsicFuseStatus(MvdDwlFuseStatus *fuses)` |
| `0x102D08` | `MvdCalculateImageSize` | `u32 MvdCalculateImageSize(u32 width, u32 height, u32 format)` |
| `0x102E84` | `MvdCalculateWorkBufferSize` | `u32 MvdCalculateWorkBufferSize(const MvdWorkSizeParams *params)` |
| `0x102F2C` | `H264DecDecode` | `H264DecRet H264DecDecode(H264DecInst instance, const H264DecInput *input, H264DecOutput *output)` |
| `0x103AD4` | `H264DecGetInfo` | `H264DecRet H264DecGetInfo(H264DecInst instance, MvdH264Info *info)` |
| `0x103BE0` | `H264DecInit` | `H264DecRet H264DecInit(H264DecInst *instance, u32 noOutputReordering, u32 useVideoFreezeConcealment, u32 useDisplaySmoothing, u32 referenceFrameFormat)` |
| `0x103D4C` | `H264DecNextPicture` | `H264DecRet H264DecNextPicture(H264DecInst instance, MvdH264Picture *picture, u32 endOfStream)` |
| `0x104328` | `H264DecPeek` | `H264DecRet H264DecPeek(H264DecInst instance, MvdH264Picture *picture)` |
| `0x104428` | `H264DecRelease` | `void H264DecRelease(H264DecInst instance)` |
| `0x1044E0` | `H264DecSetMvc` | `H264DecRet H264DecSetMvc(H264DecInst instance)` |
| `0x1049B0` | `H264RunAsic` | `u32 H264RunAsic(MvdH264Container *decoder,MvdH264AsicBuffers *buffers)` |
| `0x105448` | `H264SetupVlcRegs` | `void H264SetupVlcRegs(MvdH264Container *decoder)` |
| `0x106008` | `PPCheckAllHeightParams` | `i32 PPCheckAllHeightParams(PPConfig *cfg, u32 pixAcc)` |
| `0x106088` | `PPCheckAllWidthParams` | `i32 PPCheckAllWidthParams(PPConfig *cfg, u32 blendEna, u32 pixAcc, u32 blendCropSupport)` |
| `0x10635C` | `PPCheckConfig` | `i32 PPCheckConfig(MvdPpContainer *pp, PPConfig *config, u32 decoderLinked, u32 decoderType)` |
| `0x106918` | `PPCheckSetupChanges` | `u32 PPCheckSetupChanges(PPConfig *previous, PPConfig *current)` |
| `0x106AF0` | `PPDecCombinedModeEnable` | `PPResult PPDecCombinedModeEnable(MvdPpContainer *pp, const void *decoder, u32 decoderType)` |
| `0x106B98` | `PPDecConfigQueryFromDec` | `void PPDecConfigQueryFromDec(MvdPpContainer *pp, DecPpQuery *query)` |
| `0x106C98` | `PPDecDisplayIndex` | `void PPDecDisplayIndex(MvdPpContainer *pp, u32 index)` |
| `0x106CA2` | `PPDecEndCallback` | `void PPDecEndCallback(MvdPpContainer *pp)` |
| `0x106CF0` | `PPDecSetMultipleOutput` | `PPResult PPDecSetMultipleOutput(MvdPpContainer *pp, const PPOutputBuffers *buffers)` |
| `0x106EB0` | `PPDecStartPp` | `void PPDecStartPp(MvdPpContainer *pp, const MvdDecPpInterface *decpp)` |
| `0x10741C` | `PPChangeOutputBuffer_MVD` | `PPResult PPChangeOutputBuffer_MVD(MvdPpContainer *pp, const PPOutput *current, const PPOutput *replacement)` |
| `0x1074D8` | `PPFlushRegs` | `void PPFlushRegs(MvdPpContainer *pp)` |
| `0x1074FC` | `PPGetConfig` | `PPResult PPGetConfig(MvdPpContainer *pp, PPConfig *config)` |
| `0x107520` | `PPGetNextOutput` | `PPResult PPGetNextOutput(MvdPpContainer *pp, PPOutput *output)` |
| `0x107628` | `PPGetResult` | `PPResult PPGetResult(MvdPpContainer *pp)` |
| `0x107670` | `PPInit` | `PPResult PPInit(PPInst *instance)` |
| `0x107720` | `PPInitDataStructures` | `void PPInitDataStructures(MvdPpContainer *pp)` |
| `0x107798` | `PPInitHW` | `void PPInitHW(MvdPpContainer *pp)` |
| `0x107860` | `PPIsInPixFmtOk` | `u32 PPIsInPixFmtOk(u32 pixelFormat, const MvdPpContainer *pp)` |
| `0x107918` | `PPIsOutPixFmtOk` | `u32 PPIsOutPixFmtOk(u32 pixelFormat, const MvdPpContainer *pp)` |
| `0x1079A8` | `PPRelease` | `void PPRelease(MvdPpContainer *pp)` |
| `0x1079D0` | `PPSelectOutputSize` | `i32 PPSelectOutputSize(MvdPpContainer *pp)` |
| `0x107A44` | `PPSetConfig` | `PPResult PPSetConfig(MvdPpContainer *pp, PPConfig *config)` |
| `0x107CB8` | `PPSetDithering` | `void PPSetDithering(MvdPpContainer *pp)` |
| `0x107D5C` | `PPSetFrmBufferWriting` | `void PPSetFrmBufferWriting(MvdPpContainer *pp)` |
| `0x107F3C` | `PPSetRgbBitmask` | `void PPSetRgbBitmask(MvdPpContainer *pp)` |
| `0x108194` | `PPSetRgbBitmaskCustom` | `void PPSetRgbBitmaskCustom(MvdPpContainer *pp, u32 rgb16)` |
| `0x108234` | `PPSetRgbTransformCoeffs` | `void PPSetRgbTransformCoeffs(MvdPpContainer *pp)` |
| `0x108540` | `PPSetupHW` | `void PPSetupHW(MvdPpContainer *pp)` |
| `0x108E56` | `PrepareIntra4x4ModeData` | `void PrepareIntra4x4ModeData(MvdH264Storage *storage,MvdH264AsicBuffers *buffers)` |
| `0x108EDC` | `PrepareMvData` | `void PrepareMvData(MvdH264Storage *storage,MvdH264AsicBuffers *buffers)` |
| `0x10908E` | `PrepareRlcCount` | `void PrepareRlcCount(MvdH264Storage *storage,MvdH264AsicBuffers *buffers)` |
| `0x109170` | `RefbuMvStatisticsB` | `void RefbuMvStatisticsB(MvdRefBuffer *refbu,u32 *registers)` |
| `0x1091E0` | `RefbuVpxGetPrevFrameStats` | `u32 RefbuVpxGetPrevFrameStats(MvdRefBuffer *refbu)` |
| `0x109330` | `VP6DecDecode` | `VP6DecRet VP6DecDecode(VP6DecInst instance, const VP6DecInput *input, VP6DecOutput *output)` |
| `0x1095BC` | `VP6DecGetInfo` | `VP6DecRet VP6DecGetInfo(VP6DecInst instance, MvdVp6Info *info)` |
| `0x109628` | `VP6DecInit` | `VP6DecRet VP6DecInit(VP6DecInst *instance, u32 freezeConcealment, u32 numFrameBuffers, u32 referenceFrameFormat)` |
| `0x10972C` | `VP6DecNextPicture` | `VP6DecRet VP6DecNextPicture(VP6DecInst instance, MvdVp6Picture *picture, u32 endOfStream)` |
| `0x109874` | `VP6DecPeek` | `VP6DecRet VP6DecPeek(VP6DecInst instance, MvdVp6Picture *picture)` |
| `0x1098D2` | `VP6DecRelease` | `void VP6DecRelease(VP6DecInst instance)` |
| `0x10A01C` | `VP6HWDecodeProbUpdates` | `i32 VP6HWDecodeProbUpdates(MvdVp6Pb *pb)` |
| `0x10A0DC` | `VP6HWLoadFrameHeader` | `i32 VP6HWLoadFrameHeader(MvdVp6Pb *pb)` |
| `0x10A678` | `VP6HwdAsicAllocateMem` | `i32 VP6HwdAsicAllocateMem(MvdVp6Container *decoder)` |
| `0x10A6BC` | `VP6HwdAsicAllocatePictures` | `i32 VP6HwdAsicAllocatePictures(MvdVp6Container *decoder)` |
| `0x10A788` | `VP6HwdAsicInit` | `void VP6HwdAsicInit(MvdVp6Container *decoder)` |
| `0x10A7A8` | `VP6HwdAsicInitPicture` | `void VP6HwdAsicInitPicture(MvdVp6Container *decoder)` |
| `0x10A9C8` | `VP6HwdAsicProbUpdate` | `void VP6HwdAsicProbUpdate(MvdVp6Container *decoder)` |
| `0x10B3C8` | `MvdIsClientLinearRange` | `u32 MvdIsClientLinearRange(u32 address,u32 size)` |
| `0x10B3F0` | `MvdCalculateMacroblocks` | `u32 MvdCalculateMacroblocks(u32 height, u32 width)` |
| `0x10B43C` | `MvdY2rReadStandardCoefficients` | `Result MvdY2rReadStandardCoefficients(void *registerContext, MvdY2rCoefficients *output, u32 index)` |
| `0x10B5D8` | `MvdClientVirtualToBus` | `u32 MvdClientVirtualToBus(u32 address, u32 size)` |
| `0x10B608` | `MvdConvertLibraryResult` | `Result MvdConvertLibraryResult(s32 status, u32 codecClass, u32 postprocessor)` |
| `0x10B9BE` | `MvdTranslateLinearRange` | `u32 MvdTranslateLinearRange(u32 address, u32 size, u32 rangeBegin, u32 rangeEnd, u32 busBase)` |
| `0x10B9DE` | `MvdWaitEventRaw` | `Result MvdWaitEventRaw(const Handle *event,s64 timeoutNs)` |
| `0x10BA64` | `MvdWaitEventUntilTimeout` | `u32 MvdWaitEventUntilTimeout(const Handle *event,s64 timeoutNs)` |
| `0x10BA88` | `MvdTimeSpanFromMilliseconds` | `void MvdTimeSpanFromMilliseconds(s64 *out,s64 milliseconds)` |
| `0x10BAA0` | `MvdClearEventChecked` | `void MvdClearEventChecked(const Handle *event)` |
| `0x10BAB2` | `VP8HwdAsicReleaseMem` | `void VP8HwdAsicReleaseMem(MvdVp8Container *decoder)` |
| `0x10BADE` | `VP8HwdUpdateRefs` | `void VP8HwdUpdateRefs(MvdVp8Container *decoder,u32 corrupted)` |
| `0x10BB6C` | `VP8HwdAsicRun` | `u32 VP8HwdAsicRun(MvdVp8Container *decoder)` |
| `0x10BEB0` | `VP8HwdAsicStrmPosUpdate` | `void VP8HwdAsicStrmPosUpdate(MvdVp8Container *decoder,u32 streamBusAddress)` |
| `0x10BFF0` | `VP8HwdAsicInitPicture` | `void VP8HwdAsicInitPicture(MvdVp8Container *decoder)` |
| `0x10C860` | `VP8HwdAsicReleasePictures` | `void VP8HwdAsicReleasePictures(MvdVp8Container *decoder)` |
| `0x10C918` | `DWLFreeRefFrm` | `void DWLFreeRefFrm(const MvdDwlInstance *dwl,MvdLinearMem *buffer)` |
| `0x10C920` | `BqueueRelease` | `void BqueueRelease(MvdBufferQueue *queue)` |
| `0x10C938` | `DecSetupTiledReference` | `u32 DecSetupTiledReference(u32 *registers,u32 tiledSupport,u32 dpbMode,u32 interlaced)` |
| `0x10C980` | `DWLMallocRefFrm` | `i32 DWLMallocRefFrm(MvdDwlInstance *dwl,u32 size,MvdLinearMem *buffer)` |
| `0x10C988` | `BqueueInit` | `u32 BqueueInit(MvdBufferQueue *queue,u32 numBuffers)` |
| `0x10CC06` | `BqueueDiscard` | `void BqueueDiscard(MvdBufferQueue *queue,u32 buffer)` |
| `0x10CC10` | `BqueueNext` | `u32 BqueueNext(MvdBufferQueue *queue,u32 ref0,u32 ref1,u32 ref2,u32 bPic)` |
| `0x10CC88` | `vp6PreparePpRun` | `void vp6PreparePpRun(MvdVp6Container *decoder)` |
| `0x10CCD0` | `VP6HwdAsicReleasePictures` | `void VP6HwdAsicReleasePictures(MvdVp6Container *decoder)` |
| `0x10CD24` | `DWLFreeLinear` | `void DWLFreeLinear(const MvdDwlInstance *dwl,MvdLinearMem *buffer)` |
| `0x10CD2C` | `RefbuGetHitThreshold` | `i32 RefbuGetHitThreshold(MvdRefBuffer *refbu)` |
| `0x10CE2C` | `PPFindFirstNonZeroBit` | `u32 PPFindFirstNonZeroBit(u32 mask)` |
| `0x10CE42` | `PPSelectDitheringValue` | `u32 PPSelectDitheringValue(u32 mask)` |
| `0x10CE70` | `PPDecCombinedModeDisable` | `PPResult PPDecCombinedModeDisable(MvdPpContainer *pp, const void *decoder)` |
| `0x10CEF0` | `WaitForPp` | `PPResult WaitForPp(MvdPpContainer *pp)` |
| `0x10CF84` | `PPRun` | `PPResult PPRun(MvdPpContainer *pp)` |
| `0x10CFEC` | `PPSetupScaling` | `void PPSetupScaling(MvdPpContainer *pp, const PPOutImage *out)` |
| `0x10D2C4` | `PPSetStatus` | `void PPSetStatus(MvdPpContainer *pp, u32 status)` |
| `0x10D2CE` | `PPRefreshRegs` | `void PPRefreshRegs(MvdPpContainer *pp)` |
| `0x10D2F0` | `PPGetStatus` | `u32 PPGetStatus(MvdPpContainer *pp)` |
| `0x10D328` | `PPIsOutPixFmtBlendOk` | `u32 PPIsOutPixFmtBlendOk(u32 format)` |
| `0x10D60C` | `h264bsdGetRefPicDataVlcMode` | `u8 *h264bsdGetRefPicDataVlcMode(const MvdH264Dpb *dpb,u32 index,u32 fieldMode)` |
| `0x10D650` | `RefbuMvStatistics` | `void RefbuMvStatistics(MvdRefBuffer *refbu,u32 *registers,u32 *mvs,u32 directMvsAvailable,u32 isIntra)` |
| `0x10D7C4` | `DWLReadReg` | `u32 DWLReadReg(const MvdDwlInstance *dwl,u32 byteOffset)` |
| `0x10D7CC` | `DWLWaitHwReady` | `i32 DWLWaitHwReady(const MvdDwlInstance *dwl,u32 timeout)` |
| `0x10D7D4` | `DWLEnableHW` | `void DWLEnableHW(const MvdDwlInstance *dwl,u32 byteOffset,u32 value)` |
| `0x10D7DC` | `DWLReserveHw` | `i32 DWLReserveHw(const MvdDwlInstance *dwl)` |
| `0x10D7E4` | `RefbuSetup` | `void RefbuSetup(MvdRefBuffer *refbu,u32 *registers,u32 mode,u32 isIntra,u32 isB,u32 refPicId0,u32 refPicId1,u32 flags)` |
| `0x10DA50` | `DWLWriteReg` | `void DWLWriteReg(const MvdDwlInstance *dwl,u32 byteOffset,u32 value)` |
| `0x10DA58` | `h264StreamPosUpdate` | `void h264StreamPosUpdate(MvdH264Container *decoder)` |
| `0x10DBF0` | `ReleaseAsicBuffers` | `void ReleaseAsicBuffers(const void *dwl,MvdH264AsicBuffers *buffers)` |
| `0x10DC9A` | `h264PpMultiAddPic` | `u32 h264PpMultiAddPic(MvdH264Container *decoder,const MvdLinearMem *data)` |
| `0x10DCCE` | `h264bsdNextOutputPicture` | `MvdH264DpbOutPicture *h264bsdNextOutputPicture(MvdH264Storage *storage)` |
| `0x10DD1E` | `DWLDisableHW` | `void DWLDisableHW(const MvdDwlInstance *dwl,u32 byteOffset,u32 value)` |
| `0x10DD26` | `DWLRelease` | `int DWLRelease(void *dwl)` |
| `0x10DE04` | `DWLInit` | `MvdDwlInstance *DWLInit(const MvdDwlInitParams *params)` |
| `0x10DE0C` | `DWLReadAsicConfig` | `void DWLReadAsicConfig(MvdDwlHwConfig *config)` |
| `0x10E03C` | `DWLmemset` | `void *DWLmemset(void *dest, int value, u32 size)` |
| `0x10E044` | `DWLReadAsicID` | `u32 DWLReadAsicID(void)` |
| `0x10E050` | `h264bsdCroppingParams` | `void h264bsdCroppingParams(MvdH264Storage *storage, u32 *croppingFlag, u32 *left, u32 *width, u32 *top, u32 *height)` |
| `0x10E0BE` | `h264bsdPicHeight` | `u32 h264bsdPicHeight(MvdH264Storage *storage)` |
| `0x10E0C8` | `h264bsdPicWidth` | `u32 h264bsdPicWidth(MvdH264Storage *storage)` |
| `0x10E0D2` | `h264CheckCabacZeroWords` | `u32 h264CheckCabacZeroWords(strmData_t *stream)` |
| `0x10E0FC` | `GetDecRegister` | `u32 GetDecRegister(const u32 *registers, MvdHwIf field)` |
| `0x10E120` | `h264bsdFindNextStartCode` | `const u8 *h264bsdFindNextStartCode(const u8 *stream,u32 length)` |
| `0x10E15E` | `h264PpMultiRemovePic` | `u32 h264PpMultiRemovePic(MvdH264Container *decoder,const MvdLinearMem *data)` |
| `0x10E194` | `h264InitPicFreezeOutput` | `void h264InitPicFreezeOutput(MvdH264Container *decoder,u32 fromOldDpb)` |
| `0x10E4E8` | `h264UpdateAfterPictureDecode` | `void h264UpdateAfterPictureDecode(MvdH264Container *decoder)` |
| `0x10E7AC` | `h264StreamIsBaseline` | `u32 h264StreamIsBaseline(MvdH264Container *decoder)` |
| `0x10E828` | `h264CheckReleasePpAndHw` | `void h264CheckReleasePpAndHw(MvdH264Container *decoder)` |
| `0x10E874` | `h264PpMultiMvc` | `void h264PpMultiMvc(MvdH264Container *decoder,u32 maxBuffId)` |
| `0x10E87E` | `h264PpMultiInit` | `void h264PpMultiInit(MvdH264Container *decoder,u32 maxBuffId)` |
| `0x10E8A4` | `h264bsdResetStorage` | `void h264bsdResetStorage(MvdH264Storage *storage)` |
| `0x10E8E0` | `h264AllocateResources` | `u32 h264AllocateResources(MvdH264Container *decoder)` |
| `0x10E9D4` | `MvdYuv420ReferenceBufferBytes` | `u32 MvdYuv420ReferenceBufferBytes(u32 height, u32 width, u32 count)` |
| `0x10F098` | `h264bsdFlushBits` | `u32 h264bsdFlushBits(strmData_t *stream,u32 numBits)` |
| `0x10F1BA` | `h264bsdShowBits` | `u32 h264bsdShowBits(strmData_t *stream,u32 numBits)` |
| `0x10F298` | `h264bsdDecodeExpGolombSigned` | `u32 h264bsdDecodeExpGolombSigned(strmData_t *stream, s32 *value)` |
| `0x10F324` | `SetDecRegister` | `void SetDecRegister(u32 *registers, MvdHwIf field, u32 value)` |
| `0x10F354` | `h264bsdDecodeExpGolombUnsigned` | `u32 h264bsdDecodeExpGolombUnsigned(strmData_t *stream, u32 *value)` |
| `0x10F448` | `h264bsdGetBits` | `u32 h264bsdGetBits(strmData_t *stream, u32 numBits)` |
| `0x10F474` | `MvdCopyMemory` | `void *MvdCopyMemory(void *dest, const void *source, u32 size)` |
| `0x10F500` | `MvdHeapFree` | `void MvdHeapFree(void *ptr)` |
| `0x10F56C` | `MvdDwlAllocateLinear` | `i32 MvdDwlAllocateLinear(MvdDwlInstance *dwl,u32 size,MvdLinearMem *buffer)` |
| `0x10F68C` | `svcFlushProcessDataCache` | `Result svcFlushProcessDataCache(Handle process,u32 address,u32 size)` |
| `0x10F694` | `DWLfree` | `void DWLfree(void *ptr)` |
| `0x10F6A0` | `DWLmalloc` | `void *DWLmalloc(u32 size)` |
| `0x10F6A8` | `RefbuInit` | `void RefbuInit(MvdRefBuffer *refbu,u32 decMode,u32 widthInMbs,u32 heightInMbs,u32 supportFlags)` |
| `0x10F780` | `DWLmemcpy` | `void *DWLmemcpy(void *dest, const void *source, u32 size)` |
| `0x10F788` | `DWLMallocLinear` | `i32 DWLMallocLinear(MvdDwlInstance *dwl,u32 size,MvdLinearMem *buffer)` |
| `0x10F82C` | `MvdHeapAlloc` | `void *MvdHeapAlloc(u32 size)` |
| `0x10FDB4` | `MvdL2bWriteOutputFormat` | `Result MvdL2bWriteOutputFormat(u32 *registerOffset, u32 shiftedFormat)` |
| `0x10FDD0` | `MvdL2bWriteInputFormat` | `Result MvdL2bWriteInputFormat(u32 *registerOffset, u32 format)` |
| `0x10FEF0` | `MVDY2R_DriverFinalize` | `Result MVDY2R_DriverFinalize(void)` |
| `0x110138` | `MvdIpcWriteResponseHeader` | `u32 MvdIpcWriteResponseHeader(u32 **buffer, u32 command, u32 normalWords, u32 translatedWords, u32 extra)` |
| `0x1104D8` | `VP6HwdAsicRun` | `u32 VP6HwdAsicRun(MvdVp6Container *decoder)` |
| `0x1106D4` | `VP6HwdAsicStrmPosUpdate` | `void VP6HwdAsicStrmPosUpdate(MvdVp6Container *decoder)` |
| `0x1107CC` | `VP8DecDecode` | `VP8DecRet VP8DecDecode(VP8DecInst instance, const VP8DecInput *input, VP8DecOutput *output)` |
| `0x110E20` | `VP8DecGetInfo` | `VP8DecRet VP8DecGetInfo(VP8DecInst instance, MvdVp8Info *info)` |
| `0x110EA0` | `VP8DecInit` | `VP8DecRet VP8DecInit(VP8DecInst *instance, VP8DecFormat format, u32 freezeConcealment, u32 numFrameBuffers, u32 referenceFrameFormat)` |
| `0x11102C` | `VP8DecNextPicture` | `VP8DecRet VP8DecNextPicture(VP8DecInst instance, MvdVp8Picture *picture, u32 endOfStream)` |
| `0x1113F0` | `VP8DecPeek` | `VP8DecRet VP8DecPeek(VP8DecInst instance, MvdVp8Picture *picture)` |
| `0x1114BC` | `VP8DecRelease` | `void VP8DecRelease(VP8DecInst instance)` |
| `0x111520` | `VP8HwdAsicAllocateMem` | `i32 VP8HwdAsicAllocateMem(MvdVp8Container *decoder)` |
| `0x111564` | `VP8HwdAsicAllocatePictures` | `i32 VP8HwdAsicAllocatePictures(MvdVp8Container *decoder)` |
| `0x1117E0` | `VP8HwdAsicContPicture` | `void VP8HwdAsicContPicture(MvdVp8Container *decoder)` |
| `0x11182E` | `VP8HwdAsicInit` | `void VP8HwdAsicInit(MvdVp8Container *decoder)` |
| `0x11185C` | `VP8HwdAsicProbUpdate` | `void VP8HwdAsicProbUpdate(MvdVp8Container *decoder)` |
| `0x111A24` | `Vp6StrmInit` | `u32 Vp6StrmInit(MvdVp6Stream *stream,const u8 *data,u32 amount)` |
| `0x111A4C` | `MvdWaitDecoderStatus` | `i32 MvdWaitDecoderStatus(void)` |
| `0x111A9C` | `MvdWaitPostprocessorStatus` | `i32 MvdWaitPostprocessorStatus(void)` |
| `0x11216C` | `MVDL2B_SetSending` | `Result MVDL2B_SetSending(void *context, Handle process, void *address, u32 size, s16 unit, s16 gap)` |
| `0x112238` | `MVDL2B_PingProcess` | `Result MVDL2B_PingProcess(void *context, u8 *sessions)` |
| `0x112242` | `MVDL2B_SetReceiving` | `Result MVDL2B_SetReceiving(void *context, Handle process, void *address, u32 size, s16 unit, s16 gap)` |
| `0x112268` | `MVDL2B_GetInputLines` | `Result MVDL2B_GetInputLines(void *context, u16 *lines)` |
| `0x112272` | `MVDL2B_SetInputLines` | `Result MVDL2B_SetInputLines(void *context, u16 lines)` |
| `0x11227C` | `MVDL2B_GetInputFormat` | `Result MVDL2B_GetInputFormat(void *context, u8 *format)` |
| `0x112290` | `MVDL2B_SetInputFormat` | `Result MVDL2B_SetInputFormat(void *context, u8 format)` |
| `0x11229A` | `MVDL2B_StopConversion` | `Result MVDL2B_StopConversion(void *context)` |
| `0x1122A4` | `MVDL2B_GetOutputFormat` | `Result MVDL2B_GetOutputFormat(void *context, u8 *format)` |
| `0x1122BA` | `MVDL2B_SetOutputFormat` | `Result MVDL2B_SetOutputFormat(void *context, u8 format)` |
| `0x1122C8` | `MVDL2B_StartConversion` | `Result MVDL2B_StartConversion(void *context)` |
| `0x11230C` | `MVDL2B_IsBusyConversion` | `Result MVDL2B_IsBusyConversion(void *context, u8 *busy)` |
| `0x112318` | `MvdL2bConfigureReceivingDma` | `Result MvdL2bConfigureReceivingDma(void *context, Handle process, void *address, u32 size, s16 unit, s16 gap)` |
| `0x1123D4` | `MVDL2B_GetInputLineWidth` | `Result MVDL2B_GetInputLineWidth(void *context, u16 *width)` |
| `0x1123DE` | `MVDL2B_IsDoneSending` | `Result MVDL2B_IsDoneSending(void *context, u8 *done)` |
| `0x1123EE` | `MVDL2B_SetInputLineWidth` | `Result MVDL2B_SetInputLineWidth(void *context, u16 width)` |
| `0x1123F8` | `MVDL2B_GetPackageParameter` | `Result MVDL2B_GetPackageParameter(void *context, MvdL2bParams *params)` |
| `0x112444` | `MVDL2B_GetTransferEndEvent` | `Result MVDL2B_GetTransferEndEvent(void *context, Handle *event)` |
| `0x11244C` | `MVDL2B_IsDoneReceiving` | `Result MVDL2B_IsDoneReceiving(void *context, u8 *done)` |
| `0x11245C` | `MVDL2B_SetPackageParameter` | `Result MVDL2B_SetPackageParameter(void *context, MvdL2bParams *params)` |
| `0x1124A4` | `MVDL2B_GetTransferEndInterrupt` | `Result MVDL2B_GetTransferEndInterrupt(void *context, u8 *enable)` |
| `0x1124AE` | `MVDL2B_SetTransferEndInterrupt` | `Result MVDL2B_SetTransferEndInterrupt(void *context, s8 enable)` |
| `0x1124B8` | `MVDL2B_GetAlpha` | `Result MVDL2B_GetAlpha(void *context, u16 *alpha)` |
| `0x1124C2` | `MVDL2B_SetAlpha` | `Result MVDL2B_SetAlpha(void *context, u16 alpha)` |
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
| `0x1138B0` | `MVDY2R_GetRotation` | `Result MVDY2R_GetRotation(void *context, u8 *rotation)` |
| `0x1138CC` | `MVDY2R_PingProcess` | `Result MVDY2R_PingProcess(void *context, u8 *sessions)` |
| `0x1138DC` | `MVDY2R_SetRotation` | `Result MVDY2R_SetRotation(void *context, u8 rotation)` |
| `0x1138F0` | `MVDY2R_SetSendingU` | `Result MVDY2R_SetSendingU(void *context, Handle process, void *address, u32 size, s16 unit, s16 gap)` |
| `0x1139B0` | `MVDY2R_SetSendingV` | `Result MVDY2R_SetSendingV(void *context, Handle process, void *address, u32 size, s16 unit, s16 gap)` |
| `0x113A70` | `MVDY2R_SetSendingY` | `Result MVDY2R_SetSendingY(void *context, Handle process, void *address, u32 size, s16 unit, s16 gap)` |
| `0x113B30` | `MVDY2R_SetReceiving` | `Result MVDY2R_SetReceiving(void *context, Handle process, void *address, u32 size, s16 unit, s16 gap)` |
| `0x113B58` | `MVDY2R_GetInputLines` | `Result MVDY2R_GetInputLines(void *context, u16 *lines)` |
| `0x113B68` | `MVDY2R_SetInputLines` | `Result MVDY2R_SetInputLines(void *context, u16 lines)` |
| `0x113B78` | `MVDY2R_SetSendingYUYV` | `Result MVDY2R_SetSendingYUYV(void *context, Handle process, void *address, u32 size, s16 unit, s16 gap)` |
| `0x113BA0` | `MVDY2R_GetInputFormat` | `Result MVDY2R_GetInputFormat(void *context, u8 *format)` |
| `0x113BB8` | `MVDY2R_SetInputFormat` | `Result MVDY2R_SetInputFormat(void *context, u8 format)` |
| `0x113BC8` | `MVDY2R_StopConversion` | `Result MVDY2R_StopConversion(void)` |
| `0x113BD8` | `MVDY2R_GetOutputFormat` | `Result MVDY2R_GetOutputFormat(void *context, u8 *format)` |
| `0x113BF4` | `MVDY2R_SetOutputFormat` | `Result MVDY2R_SetOutputFormat(void *context, u8 format)` |
| `0x113C04` | `MVDY2R_StartConversion` | `Result MVDY2R_StartConversion(void)` |
| `0x113C68` | `MVDY2R_DriverInitialize` | `Result MVDY2R_DriverInitialize(void)` |
| `0x113C8C` | `MVDY2R_IsBusyConversion` | `Result MVDY2R_IsBusyConversion(u8 *busy)` |
| `0x113D48` | `MVDY2R_GetBlockAlignment` | `Result MVDY2R_GetBlockAlignment(void *context, u8 *alignment)` |
| `0x113D64` | `MVDY2R_GetInputLineWidth` | `Result MVDY2R_GetInputLineWidth(void *context, u16 *width)` |
| `0x113D74` | `MVDY2R_SetBlockAlignment` | `Result MVDY2R_SetBlockAlignment(void *context, u8 alignment)` |
| `0x113D88` | `MVDY2R_SetInputLineWidth` | `Result MVDY2R_SetInputLineWidth(void *context, u16 width)` |
| `0x113E64` | `MVDY2R_IsDoneSendingU` | `Result MVDY2R_IsDoneSendingU(void *context, u8 *done)` |
| `0x113E78` | `MVDY2R_IsDoneSendingV` | `Result MVDY2R_IsDoneSendingV(void *context, u8 *done)` |
| `0x113E8C` | `MVDY2R_IsDoneSendingY` | `Result MVDY2R_IsDoneSendingY(void *context, u8 *done)` |
| `0x113EA0` | `MVDY2R_GetConversionParams` | `Result MVDY2R_GetConversionParams(void *context, MvdY2rParams *params)` |
| `0x113F98` | `MVDY2R_GetSpacialDithering` | `Result MVDY2R_GetSpacialDithering(void *context, u8 *enable)` |
| `0x113FA8` | `MVDY2R_GetTransferEndEvent` | `Result MVDY2R_GetTransferEndEvent(void *context, Handle *event)` |
| `0x113FB8` | `MVDY2R_IsDoneReceiving` | `Result MVDY2R_IsDoneReceiving(void *context, u8 *done)` |
| `0x113FCC` | `MVDY2R_SetConversionParams` | `Result MVDY2R_SetConversionParams(void *context, MvdY2rParams *params)` |
| `0x114044` | `MVDY2R_SetSpacialDithering` | `Result MVDY2R_SetSpacialDithering(void *context, s8 enable)` |
| `0x114054` | `MVDY2R_GetCoefficients` | `Result MVDY2R_GetCoefficients(void *context, MvdY2rCoefficients *coefficients)` |
| `0x114064` | `MVDY2R_GetTemporalDithering` | `Result MVDY2R_GetTemporalDithering(void *context, u8 *enable)` |
| `0x114074` | `MVDY2R_IsDoneSendingYUYV` | `Result MVDY2R_IsDoneSendingYUYV(void *context, u8 *done)` |
| `0x114088` | `MVDY2R_SetCoefficients` | `Result MVDY2R_SetCoefficients(void *context, MvdY2rCoefficients coefficients)` |
| `0x1140D4` | `MVDY2R_SetTemporalDithering` | `Result MVDY2R_SetTemporalDithering(void *context, s8 enable)` |
| `0x1140E4` | `MVDY2R_SetStandardCoefficient` | `Result MVDY2R_SetStandardCoefficient(void *context, u8 index)` |
| `0x1140F4` | `MVDY2R_GetTransferEndInterrupt` | `Result MVDY2R_GetTransferEndInterrupt(void *context, u8 *enable)` |
| `0x114104` | `MVDY2R_SetTransferEndInterrupt` | `Result MVDY2R_SetTransferEndInterrupt(void *context, s8 enable)` |
| `0x114114` | `MVDY2R_GetDitheringWeightParams` | `Result MVDY2R_GetDitheringWeightParams(void *context, MvdY2rDitherWeights *weights)` |
| `0x11418C` | `MVDY2R_SetDitheringWeightParams` | `Result MVDY2R_SetDitheringWeightParams(void *context, MvdY2rDitherWeights weights)` |
| `0x11422C` | `MVDY2R_GetStandardCoefficient` | `Result MVDY2R_GetStandardCoefficient(void *context, MvdY2rCoefficients *coefficients, u8 index)` |
| `0x11423C` | `MVDY2R_GetAlpha` | `Result MVDY2R_GetAlpha(void *context, u16 *alpha)` |
| `0x11424C` | `MVDY2R_SetAlpha` | `Result MVDY2R_SetAlpha(void *context, u16 alpha)` |
| `0x114264` | `MvdBindDecoderInterrupt` | `int MvdBindDecoderInterrupt(void)` |
| `0x1142F8` | `MvdCalculateLevelWorkBufferSize` | `u32 MvdCalculateLevelWorkBufferSize(const MvdWorkSizeParams *params)` |
| `0x114350` | `MvdAttachClientWorkBuffer` | `int MvdAttachClientWorkBuffer(u32 address, u32 size)` |
| `0x114B8E` | `h264bsdSarSize` | `void h264bsdSarSize(const MvdH264Storage *storage, u32 *width, u32 *height)` |
| `0x114C58` | `h264PreparePpRun` | `void h264PreparePpRun(MvdH264Container *decoder)` |
| `0x114E50` | `h264RegisterPP` | `i32 h264RegisterPP(MvdH264Container *decoder, const void *pp, void (*start)(const void *, const MvdDecPpInterface *), void (*end)(const void *), void (*query)(const void *, DecPpQuery *), void (*display)(const void *, u32))` |
| `0x114EBC` | `h264UnregisterPP` | `i32 h264UnregisterPP(MvdH264Container *decoder, const void *pp)` |
| `0x115004` | `h264bsdAllocateSwResources` | `u32 h264bsdAllocateSwResources(const void *dwl,MvdH264Storage *storage,u32 highSupported)` |
| `0x115C0C` | `h264bsdCountLeadingZeros` | `u32 h264bsdCountLeadingZeros(u32 bits,u32 length)` |
| `0x115C28` | `h264bsdDecode` | `u32 h264bsdDecode(MvdH264Container *decoder,const u8 *stream,u32 length,u32 pictureId,u32 *readBytes)` |
| `0x116F78` | `h264bsdDecodeSeqParamSet` | `u32 h264bsdDecodeSeqParamSet(strmData_t *stream, MvdH264Sps *sps, u32 mvcFlag)` |
| `0x118030` | `h264bsdFlushBuffer` | `void h264bsdFlushBuffer(MvdH264Storage *storage)` |
| `0x11804C` | `h264bsdInit` | `void h264bsdInit(MvdH264Storage *storage, u32 noReordering, u32 displaySmoothing)` |
| `0x11833E` | `h264bsdIsMonoChrome` | `u32 h264bsdIsMonoChrome(MvdH264Storage *storage)` |
| `0x118750` | `h264bsdMatrixCoefficients` | `u32 h264bsdMatrixCoefficients(const MvdH264Storage *storage)` |
| `0x1189EE` | `h264bsdShutdown` | `void h264bsdShutdown(MvdH264Storage *storage)` |
| `0x118C6C` | `h264bsdVideoRange` | `u32 h264bsdVideoRange(const MvdH264Storage *storage)` |
| `0x118C8E` | `MvdDwlDisableHardware` | `void MvdDwlDisableHardware(const MvdDwlInstance *dwl,u32 byteOffset,u32 value)` |
| `0x118C96` | `MvdDwlEnableHardware` | `void MvdDwlEnableHardware(const MvdDwlInstance *dwl,u32 byteOffset,u32 value)` |
| `0x118CB0` | `MvdDwlCreate` | `MvdDwlInstance *MvdDwlCreate(const MvdDwlInitParams *params)` |
| `0x118CF0` | `MvdDwlAllocateReference` | `i32 MvdDwlAllocateReference(MvdDwlInstance *dwl,u32 size,MvdLinearMem *buffer)` |
| `0x118D04` | `MvdFillMemory` | `void *MvdFillMemory(void *dest, int value, u32 size)` |
| `0x118D8C` | `MvdDwlReadRegister` | `u32 MvdDwlReadRegister(const MvdDwlInstance *dwl,u32 byteOffset)` |
| `0x118D98` | `MvdDwlDestroy` | `i32 MvdDwlDestroy(MvdDwlInstance *dwl)` |
| `0x118DA4` | `MvdDwlReserveHardware` | `i32 MvdDwlReserveHardware(const MvdDwlInstance *dwl)` |
| `0x118DCC` | `MvdDwlWaitHwReady` | `i32 MvdDwlWaitHwReady(const MvdDwlInstance *dwl,u32 ignoredTimeout)` |
| `0x118DFC` | `MvdDwlWriteRegister` | `void MvdDwlWriteRegister(const MvdDwlInstance *dwl,u32 byteOffset,u32 value)` |
| `0x118E18` | `vp6RegisterPP` | `i32 vp6RegisterPP(MvdVp6Container *decoder, const void *pp, void (*start)(const void *, const MvdDecPpInterface *), void (*end)(const void *), void (*query)(const void *, DecPpQuery *))` |
| `0x118E58` | `vp6UnregisterPP` | `i32 vp6UnregisterPP(MvdVp6Container *decoder, const void *pp)` |
| `0x118E82` | `vp8RegisterPP` | `i32 vp8RegisterPP(MvdVp8Container *decoder, const void *pp, void (*start)(const void *, const MvdDecPpInterface *), void (*end)(const void *), void (*query)(const void *, DecPpQuery *))` |
| `0x118ECC` | `vp8UnregisterPP` | `i32 vp8UnregisterPP(MvdVp8Container *decoder, const void *pp)` |
| `0x11906C` | `vp8hwdDecodeFrameHeader` | `u32 vp8hwdDecodeFrameHeader(const u8 *stream,u32 streamLen,MvdBoolCoder *bc,MvdVp8Decoder *decoder)` |
| `0x119094` | `vp8hwdDecodeFrameTag` | `void vp8hwdDecodeFrameTag(const u8 *stream,MvdVp8Decoder *decoder)` |
| `0x1194C8` | `vp8hwdPreparePpRun` | `void vp8hwdPreparePpRun(MvdVp8Container *decoder)` |
| `0x119578` | `vp8hwdResetDecoder` | `void vp8hwdResetDecoder(MvdVp8Decoder *decoder,MvdVp8AsicBuffers *buffers)` |
| `0x1196B0` | `vp8hwdSetPartitionOffsets` | `u32 vp8hwdSetPartitionOffsets(const u8 *stream,u32 length,MvdVp8Decoder *decoder)` |
| `0x1197F0` | `MvdIpcWriteVp6Picture` | `void MvdIpcWriteVp6Picture(u32 **commandBuffer, u32 wordIndex, const MvdVp6Picture *picture)` |
| `0x119804` | `MvdIpcWriteVp8Picture` | `void MvdIpcWriteVp8Picture(u32 **commandBuffer, u32 wordIndex, const MvdVp8Picture *picture)` |
| `0x119818` | `MvdIpcWriteH264Picture` | `void MvdIpcWriteH264Picture(u32 **commandBuffer, u32 wordIndex, const MvdH264Picture *picture)` |
| `0x119CC0` | `svcClearEvent` | `Result svcClearEvent(Handle event)` |
| `0x119D20` | `svcWaitSynchronization` | `Result svcWaitSynchronization(Handle handle,s64 timeoutNs)` |
| `0x119D78` | `MvdMaxDpbFramesForLevel` | `u32 MvdMaxDpbFramesForLevel(u32 height, u32 width, u32 levelIndex)` |
| `0x119DDC` | `ceilf` | `float __usercall ceilf@<s0>(float value@<s0>)` |
| `0x119E6C` | `floorf` | `float __usercall floorf@<s0>(float value@<s0>)` |
