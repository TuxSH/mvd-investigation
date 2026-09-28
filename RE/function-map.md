# Applied function names and prototypes

Addresses are IDA virtual addresses. This inventory contains 598 applied functions. `MVDSTD_`, `MVDL2B_`, `MVDY2R_` and `Mvd` names describe recovered service, platform or branch-specific codec behavior. Hantro names identify source counterparts, except the explicitly suffixed `PPChangeOutputBuffer_MVD`. DWL API names identify the abstraction boundary; their implementations use Nintendo memory/interrupt services. SVC names identify verified syscall veneers. `ceilf`/`floorf` are runtime semantic identifications with recovered VFP calling conventions.

The existing `MVDSTD_HandleCommands` (`0x1124E8`), `L2BU_HandleCommands` (`0x111E64`) and `Y2RU_HandleCommands` (`0x11328C`) names were preserved, with context prototypes refined. Source-family confidence and ABI differences are documented in [external-code.md](external-code.md).

The database currently contains 796 function entries. `0x115CDE`, formerly `sub_115CDE`, is now an internal epilogue label belonging to `h264bsdDecode`, not an additional function. See [control-flow-corrections.md](control-flow-corrections.md). The correction does not add an inventory row.

The auxiliary-driver continuation adds 95 names and refines service/driver ABIs; the two previously named auxiliary dispatchers now also have explicit inventory rows. See [auxiliary-drivers.md](auxiliary-drivers.md).

| Address | Applied name | Prototype |
|---|---|---|
| `0x100724` | `MvdY2rInitializeInterrupt` | `Result MvdY2rInitializeInterrupt(void)` |
| `0x100790` | `MvdY2rFinalizeInterrupt` | `Result MvdY2rFinalizeInterrupt(void)` |
| `0x100B54` | `MvdL2bSetControlBit29` | `Result MvdL2bSetControlBit29(MvdL2bRegisterContext *registers, s32 enable)` |
| `0x100B74` | `MvdY2rSetControlBit29` | `Result MvdY2rSetControlBit29(MvdY2rRegisterContext *registers, s32 enable)` |
| `0x1012B0` | `AllocateAsicBuffers` | `u32 AllocateAsicBuffers(MvdH264Container *decoder,MvdH264AsicBuffers *buffers,u32 picSizeInMbs)` |
| `0x101374` | `MvdFlushPpOutputBeforeStart` | `void MvdFlushPpOutputBeforeStart(const MvdDwlInstance *dwl)` |
| `0x1013E4` | `CheckIntraChromaPrediction` | `u32 CheckIntraChromaPrediction(u32 predictionMode,u32 availableA,u32 availableB,u32 availableD)` |
| `0x101416` | `MvdIsWritableG1Register` | `u32 __spoils<r0,r1> MvdIsWritableG1Register(u32 byteOffset)` |
| `0x10143C` | `CompareFields` | `s32 CompareFields(const MvdH264DpbPicture *first,const MvdH264DpbPicture *second)` |
| `0x1014BC` | `CompareFieldsB` | `s32 CompareFieldsB(const MvdH264DpbPicture *first,const MvdH264DpbPicture *second,s32 currentPoc)` |
| `0x101598` | `ComparePictures` | `s32 ComparePictures(const MvdH264DpbPicture *first,const MvdH264DpbPicture *second)` |
| `0x101678` | `ComparePicturesB` | `s32 ComparePicturesB(const MvdH264DpbPicture *first,const MvdH264DpbPicture *second,s32 currentPoc)` |
| `0x101768` | `MvdBusToClientVirtualForCache` | `u32 __spoils<r0,r1,r2> MvdBusToClientVirtualForCache(u32 address,u32 size)` |
| `0x101788` | `DWLReadAsicFuseStatus` | `void DWLReadAsicFuseStatus(MvdDwlFuseStatus *fuses)` |
| `0x1018A4` | `DecRefPicMarking` | `u32 DecRefPicMarking(strmData_t *stream,MvdH264RefMarking *marking,u32 isIdr,u32 refFrameCount)` |
| `0x101A10` | `DecideParityMode` | `u32 DecideParityMode(MvdRefBuffer *refBuffer,u32 isBframe)` |
| `0x101A3C` | `DecodeBoxOutMap` | `void DecodeBoxOutMap(u32 *map,u32 sliceGroupChangeDirectionFlag,u32 unitsInSliceGroup0,u32 picWidth,u32 picHeight)` |
| `0x101B50` | `DecodeCoeffToken` | `u32 DecodeCoeffToken(u32 bits,u32 nC)` |
| `0x101C20` | `DecodeForegroundLeftOverMap` | `void DecodeForegroundLeftOverMap(u32 *map,u32 numSliceGroups,const u32 *topLeft,const u32 *bottomRight,u32 picWidth,u32 picHeight)` |
| `0x101C8A` | `DecodeLevelPrefix` | `u32 DecodeLevelPrefix(u32 bits)` |
| `0x101D2C` | `DecodeMbLfAdjustments` | `u32 DecodeMbLfAdjustments(MvdBoolCoder *bc,MvdVp8Decoder *decoder)` |
| `0x101DCA` | `DecodeMbPred` | `u32 DecodeMbPred(strmData_t *stream,MvdH264MbPred *prediction,u8 mbType,u32 activeRefs,MvdH264MbStorage *mb)` |
| `0x101EFA` | `DecodeMvcExtension` | `u32 DecodeMvcExtension(strmData_t *stream,MvdH264Sps *sps)` |
| `0x102134` | `DecodeResidual` | `u32 DecodeResidual(strmData_t *stream,MvdH264MbLayer *layer,MvdH264MbStorage *mb)` |
| `0x102258` | `DecodeRunBefore` | `u32 DecodeRunBefore(u32 bits,u32 zerosLeft)` |
| `0x1022F8` | `DecodeSegmentationData` | `u32 DecodeSegmentationData(MvdBoolCoder *bc,MvdVp8Decoder *decoder)` |
| `0x102404` | `DecodeSubMbPred` | `u32 DecodeSubMbPred(strmData_t *stream,u8 *subMbTypes,u8 mbType,u32 activeRefs,MvdH264MbStorage *mb)` |
| `0x1024F8` | `DecodeTotalZeros` | `u32 DecodeTotalZeros(u32 bits,u32 totalCoeff,u32 isChromaDc)` |
| `0x1025C4` | `DecodeVp7FrameHeader` | `u32 DecodeVp7FrameHeader(MvdBoolCoder *bc,MvdVp8Decoder *decoder)` |
| `0x1028C0` | `DecodeVp8FrameHeader` | `u32 DecodeVp8FrameHeader(const u8 *stream,u32 length,MvdBoolCoder *bc,MvdVp8Decoder *decoder)` |
| `0x102B44` | `DetermineIntra4x4PredMode` | `u32 DetermineIntra4x4PredMode(const MvdH264MbLayer *layer,u32 available,const MvdH264Neighbour *neighborA,const MvdH264Neighbour *neighborB,u32 blockIndex,const MvdH264MbStorage *mbA,const MvdH264MbStorage *mbB)` |
| `0x102BA8` | `FallbackScaling` | `void FallbackScaling(u8 (*scalingList)[64],u32 index)` |
| `0x102C34` | `GetDpbSize` | `u32 GetDpbSize(u32 picSizeInMbs,u32 levelIdc)` |
| `0x102D08` | `MvdCalculateImageSize` | `u32 MvdCalculateImageSize(u32 width, u32 height, u32 format)` |
| `0x102D54` | `GetSettings` | `u32 GetSettings(MvdRefBuffer *refBuffer,i32 *offsetX,i32 *offsetY,u32 isBpic,u32 isFieldPic)` |
| `0x102E84` | `MvdCalculateWorkBufferSize` | `u32 MvdCalculateWorkBufferSize(const MvdWorkSizeParams *params)` |
| `0x102F2C` | `H264DecDecode` | `H264DecRet H264DecDecode(H264DecInst instance, const H264DecInput *input, H264DecOutput *output)` |
| `0x103AD4` | `H264DecGetInfo` | `H264DecRet H264DecGetInfo(H264DecInst instance, MvdH264Info *info)` |
| `0x103BE0` | `H264DecInit` | `H264DecRet H264DecInit(H264DecInst *instance, u32 noOutputReordering, u32 useVideoFreezeConcealment, u32 useDisplaySmoothing, u32 referenceFrameFormat)` |
| `0x103D4C` | `H264DecNextPicture` | `H264DecRet H264DecNextPicture(H264DecInst instance, MvdH264Picture *picture, u32 endOfStream)` |
| `0x104328` | `H264DecPeek` | `H264DecRet H264DecPeek(H264DecInst instance, MvdH264Picture *picture)` |
| `0x104428` | `H264DecRelease` | `void H264DecRelease(H264DecInst instance)` |
| `0x1044E0` | `H264DecSetMvc` | `H264DecRet H264DecSetMvc(H264DecInst instance)` |
| `0x10452C` | `H264InitRefPicList` | `void H264InitRefPicList(MvdH264Container *decoder)` |
| `0x1047E4` | `H264InitRefPicList1` | `void H264InitRefPicList1(MvdH264Container *decoder,const u32 *list0,u32 *list1)` |
| `0x1048D4` | `H264InitRefPicList1F` | `void H264InitRefPicList1F(MvdH264Container *decoder,const u32 *list0,u32 *list1)` |
| `0x1049B0` | `H264RunAsic` | `u32 H264RunAsic(MvdH264Container *decoder,MvdH264AsicBuffers *buffers)` |
| `0x105448` | `H264SetupVlcRegs` | `void H264SetupVlcRegs(MvdH264Container *decoder)` |
| `0x1057DC` | `InitMemAccess` | `void InitMemAccess(MvdRefBuffer *refBuffer,u32 decoderMode,u32 busWidth)` |
| `0x105824` | `InitWorkarounds` | `void InitWorkarounds(u32 decoderMode,MvdDecoderWorkarounds *workarounds)` |
| `0x1058F0` | `InsertSorted` | `void InsertSorted(MvdVp6SortNode *nodes,s32 node,s32 *head)` |
| `0x105936` | `Intra16x16Prediction` | `u32 Intra16x16Prediction(MvdH264MbStorage *mb,MvdH264MbLayer *layer,u32 constrainedIntraPred,MvdH264AsicBuffers *buffers)` |
| `0x105A9C` | `Intra4x4Prediction` | `u32 Intra4x4Prediction(MvdH264MbStorage *mb,MvdH264MbLayer *layer,u32 constrainedIntraPred,MvdH264AsicBuffers *buffers)` |
| `0x105D1C` | `Mmcop1` | `u32 Mmcop1(MvdH264Dpb *dpb,u32 currentPicNum,u32 differenceOfPicNums,u32 picStruct)` |
| `0x105D98` | `Mmcop3` | `u32 Mmcop3(MvdH264Dpb *dpb,u32 currentPicNum,u32 differenceOfPicNums,u32 longTermFrameIdx,u32 picStruct)` |
| `0x105E78` | `Mmcop4` | `u32 Mmcop4(MvdH264Dpb *dpb,u32 maxLongTermFrameIdx)` |
| `0x105F14` | `Mmcop6` | `u32 Mmcop6(MvdH264Dpb *dpb,u32 frameNum,const s32 *picOrderCnt,u32 longTermFrameIdx,u32 picStruct)` |
| `0x106008` | `PPCheckAllHeightParams` | `i32 PPCheckAllHeightParams(PPConfig *cfg, u32 pixAcc)` |
| `0x106088` | `PPCheckAllWidthParams` | `i32 PPCheckAllWidthParams(PPConfig *cfg, u32 blendEna, u32 pixAcc, u32 blendCropSupport)` |
| `0x10635C` | `PPCheckConfig` | `i32 PPCheckConfig(MvdPpContainer *pp, PPConfig *config, u32 decoderLinked, u32 decoderType)` |
| `0x106918` | `PPCheckSetupChanges` | `u32 PPCheckSetupChanges(PPConfig *previous, PPConfig *current)` |
| `0x106A2C` | `PPCheckTiledOutput` | `i32 PPCheckTiledOutput(MvdPpContainer *pp,PPConfig *config)` |
| `0x106AF0` | `PPDecCombinedModeEnable` | `PPResult PPDecCombinedModeEnable(MvdPpContainer *pp, const void *decoder, u32 decoderType)` |
| `0x106B98` | `PPDecConfigQueryFromDec` | `void PPDecConfigQueryFromDec(MvdPpContainer *pp, DecPpQuery *query)` |
| `0x106C98` | `PPDecDisplayIndex` | `void PPDecDisplayIndex(MvdPpContainer *pp, u32 index)` |
| `0x106CA2` | `PPDecEndCallback` | `void PPDecEndCallback(MvdPpContainer *pp)` |
| `0x106CF0` | `PPDecSetMultipleOutput` | `PPResult PPDecSetMultipleOutput(MvdPpContainer *pp, const PPOutputBuffers *buffers)` |
| `0x106D88` | `PPDecSetOutBuffer` | `void PPDecSetOutBuffer(MvdPpContainer *pp,const MvdDecPpInterface *decpp)` |
| `0x106E3C` | `MvdPpSetDecoderInput` | `void MvdPpSetDecoderInput(MvdPpContainer *pp,const MvdDecPpInterface *decpp)` |
| `0x106EB0` | `PPDecStartPp` | `void PPDecStartPp(MvdPpContainer *pp, const MvdDecPpInterface *decpp)` |
| `0x10741C` | `PPChangeOutputBuffer_MVD` | `PPResult PPChangeOutputBuffer_MVD(MvdPpContainer *pp, const PPOutput *current, const PPOutput *replacement)` |
| `0x1074A0` | `PPDecWaitResult` | `PPResult PPDecWaitResult(MvdPpContainer *pp)` |
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
| `0x108C24` | `PredWeightTable` | `u32 PredWeightTable(strmData_t *stream,MvdH264SliceHeader *slice,u32 monochrome)` |
| `0x108CFC` | `PrepareInterPrediction` | `u32 PrepareInterPrediction(MvdH264MbStorage *mb,MvdH264MbLayer *layer,MvdH264Dpb *dpb,MvdH264AsicBuffers *asic)` |
| `0x108E56` | `PrepareIntra4x4ModeData` | `void PrepareIntra4x4ModeData(MvdH264Storage *storage,MvdH264AsicBuffers *buffers)` |
| `0x108EB2` | `PrepareIntraPrediction` | `u32 PrepareIntraPrediction(MvdH264MbStorage *mb,MvdH264MbLayer *layer,u32 constrainedIntraPred,MvdH264AsicBuffers *asic)` |
| `0x108EDC` | `PrepareMvData` | `void PrepareMvData(MvdH264Storage *storage,MvdH264AsicBuffers *buffers)` |
| `0x10908E` | `PrepareRlcCount` | `void PrepareRlcCount(MvdH264Storage *storage,MvdH264AsicBuffers *buffers)` |
| `0x1090B8` | `RefPicListReordering` | `u32 RefPicListReordering(strmData_t *stream,MvdH264RefReordering *order,u32 activeRefCount,u32 maxPicNum,u32 isMvc)` |
| `0x109170` | `RefbuMvStatisticsB` | `void RefbuMvStatisticsB(MvdRefBuffer *refbu,u32 *registers)` |
| `0x1091E0` | `RefbuVpxGetPrevFrameStats` | `u32 RefbuVpxGetPrevFrameStats(MvdRefBuffer *refbu)` |
| `0x1091FC` | `ScalingList` | `void ScalingList(u8 (*scalingList)[64],strmData_t *stream,u32 index)` |
| `0x109298` | `UpdateMemModel` | `void UpdateMemModel(MvdRefBuffer *refBuffer)` |
| `0x109330` | `VP6DecDecode` | `VP6DecRet VP6DecDecode(VP6DecInst instance, const VP6DecInput *input, VP6DecOutput *output)` |
| `0x1095BC` | `VP6DecGetInfo` | `VP6DecRet VP6DecGetInfo(VP6DecInst instance, MvdVp6Info *info)` |
| `0x109628` | `VP6DecInit` | `VP6DecRet VP6DecInit(VP6DecInst *instance, u32 freezeConcealment, u32 numFrameBuffers, u32 referenceFrameFormat)` |
| `0x10972C` | `VP6DecNextPicture` | `VP6DecRet VP6DecNextPicture(VP6DecInst instance, MvdVp6Picture *picture, u32 endOfStream)` |
| `0x109874` | `VP6DecPeek` | `VP6DecRet VP6DecPeek(VP6DecInst instance, MvdVp6Picture *picture)` |
| `0x1098D2` | `VP6DecRelease` | `void VP6DecRelease(VP6DecInst instance)` |
| `0x109924` | `VP6HWAllocateHuffman` | `s32 VP6HWAllocateHuffman(MvdVp6Pb *pb)` |
| `0x10994E` | `VP6HWBuildModeTree` | `void VP6HWBuildModeTree(MvdVp6Pb *pb)` |
| `0x109AAC` | `VP6HWBuildScanOrder` | `void VP6HWBuildScanOrder(MvdVp6Pb *pb,const u8 *scanBands)` |
| `0x109B30` | `VP6HWConfigureContexts` | `void VP6HWConfigureContexts(MvdVp6Pb *pb)` |
| `0x109BAC` | `VP6HWConfigureEntropyDecoder` | `void VP6HWConfigureEntropyDecoder(MvdVp6Pb *pb,u8 frameType)` |
| `0x109DD8` | `VP6HWConfigureMvEntropyDecoder` | `void VP6HWConfigureMvEntropyDecoder(MvdVp6Pb *pb)` |
| `0x109EFC` | `VP6HWDecodeBool128` | `u32 VP6HWDecodeBool128(MvdBoolCoder *bc)` |
| `0x109F4C` | `VP6HWDecodeModeProbs` | `void VP6HWDecodeModeProbs(MvdVp6Pb *pb)` |
| `0x10A01C` | `VP6HWDecodeProbUpdates` | `i32 VP6HWDecodeProbUpdates(MvdVp6Pb *pb)` |
| `0x10A0DC` | `VP6HWLoadFrameHeader` | `i32 VP6HWLoadFrameHeader(MvdVp6Pb *pb)` |
| `0x10A328` | `VP6HW_BoolTreeToHuffCodes` | `void VP6HW_BoolTreeToHuffCodes(const u8 *probs,u32 *frequencies)` |
| `0x10A3DC` | `VP6HW_ConvertDecodeBoolTrees` | `void VP6HW_ConvertDecodeBoolTrees(MvdVp6Pb *pb)` |
| `0x10A55C` | `VP6HW_ZerosBoolTreeToHuffCodes` | `void VP6HW_ZerosBoolTreeToHuffCodes(const u8 *probs,u32 *frequencies)` |
| `0x10A5EC` | `VP6HWdecodeModeDiff` | `s32 VP6HWdecodeModeDiff(MvdVp6Pb *pb)` |
| `0x10A678` | `VP6HwdAsicAllocateMem` | `i32 VP6HwdAsicAllocateMem(MvdVp6Container *decoder)` |
| `0x10A6BC` | `VP6HwdAsicAllocatePictures` | `i32 VP6HwdAsicAllocatePictures(MvdVp6Container *decoder)` |
| `0x10A788` | `VP6HwdAsicInit` | `void VP6HwdAsicInit(MvdVp6Container *decoder)` |
| `0x10A7A8` | `VP6HwdAsicInitPicture` | `void VP6HwdAsicInitPicture(MvdVp6Container *decoder)` |
| `0x10A9C8` | `VP6HwdAsicProbUpdate` | `void VP6HwdAsicProbUpdate(MvdVp6Container *decoder)` |
| `0x10AE14` | `MvdVp8AccumulateConcealmentVector` | `void MvdVp8AccumulateConcealmentVector(MvdVp8EcState *state,s32 blockX,s32 blockY,s32 mvX,s32 mvY,u32 referenceIndex,u32 weight)` |
| `0x10AE80` | `Mmcop5` | `u32 Mmcop5(MvdH264Dpb *dpb)` |
| `0x10AED0` | `OutBufFree` | `void OutBufFree(MvdH264Dpb *dpb,u32 outputIndex)` |
| `0x10AF1A` | `h264bsdNextMbAddress` | `u32 h264bsdNextMbAddress(const u32 *sliceGroupMap,u32 picSizeInMbs,u32 currentMb)` |
| `0x10AF3C` | `h264bsdRbspTrailingBits` | `u32 h264bsdRbspTrailingBits(strmData_t *stream)` |
| `0x10AF54` | `h264bsdMoreRbspData` | `u32 h264bsdMoreRbspData(strmData_t *stream)` |
| `0x10AFB0` | `h264bsdIsStartOfPicture` | `u32 h264bsdIsStartOfPicture(MvdH264Storage *storage)` |
| `0x10AFC4` | `h264bsdInitRefPicList` | `void h264bsdInitRefPicList(MvdH264Dpb *dpb)` |
| `0x10AFF6` | `SlidingWindowRefPicMarking` | `u32 SlidingWindowRefPicMarking(MvdH264Dpb *dpb)` |
| `0x10B066` | `SetPicNums` | `void SetPicNums(MvdH264Dpb *dpb,u32 currentFrameNum)` |
| `0x10B0A0` | `h264bsdAllocateDpbImage` | `MvdLinearMem *h264bsdAllocateDpbImage(MvdH264Dpb *dpb)` |
| `0x10B17C` | `CheckPps` | `u32 CheckPps(MvdH264Pps *pps,MvdH264Sps *sps)` |
| `0x10B20A` | `h264DpbUpdateOutputList` | `void h264DpbUpdateOutputList(MvdH264Dpb *dpb)` |
| `0x10B37E` | `MvdL2bConstructContext` | `MvdL2bContext * MvdL2bConstructContext(MvdL2bContext *context)` |
| `0x10B3C0` | `MvdY2rInvalidCoefficientResult` | `Result MvdY2rInvalidCoefficientResult(void)` |
| `0x10B3C8` | `MvdIsClientLinearRange` | `u32 MvdIsClientLinearRange(u32 address,u32 size)` |
| `0x10B3F0` | `MvdCalculateMacroblocks` | `u32 MvdCalculateMacroblocks(u32 height, u32 width)` |
| `0x10B43C` | `MvdY2rReadStandardCoefficients` | `Result MvdY2rReadStandardCoefficients(MvdY2rRegisterContext *registers, MvdY2rCoefficients *output, u32 index)` |
| `0x10B48C` | `MvdY2rWriteStandardCoefficients` | `Result MvdY2rWriteStandardCoefficients(MvdY2rRegisterContext *registers, u32 index)` |
| `0x10B4C0` | `MvdY2rReadAlpha` | `Result MvdY2rReadAlpha(MvdY2rRegisterContext *registers, u16 *alpha)` |
| `0x10B4D4` | `MvdY2rReadCoefficients` | `Result MvdY2rReadCoefficients(MvdY2rRegisterContext *registers, MvdY2rCoefficients *coefficients)` |
| `0x10B53C` | `MvdY2rReadLineWidth` | `Result MvdY2rReadLineWidth(MvdY2rRegisterContext *registers, u16 *width)` |
| `0x10B550` | `MvdY2rReadBlockAlignmentBits` | `Result MvdY2rReadBlockAlignmentBits(MvdY2rRegisterContext *registers, u16 *bits)` |
| `0x10B568` | `MvdY2rReadBusy` | `Result MvdY2rReadBusy(MvdY2rRegisterContext *registers, u8 *busy)` |
| `0x10B57C` | `MvdY2rReadOutputFormatBits` | `Result MvdY2rReadOutputFormatBits(MvdY2rRegisterContext *registers, u16 *bits)` |
| `0x10B594` | `MvdY2rReadInputFormat` | `Result MvdY2rReadInputFormat(MvdY2rRegisterContext *registers, u8 *format)` |
| `0x10B5AC` | `MvdY2rReadLines` | `Result MvdY2rReadLines(MvdY2rRegisterContext *registers, u16 *lines)` |
| `0x10B5C0` | `MvdY2rReadRotationBits` | `Result MvdY2rReadRotationBits(MvdY2rRegisterContext *registers, u16 *bits)` |
| `0x10B5D8` | `MvdClientVirtualToBus` | `u32 MvdClientVirtualToBus(u32 address, u32 size)` |
| `0x10B608` | `MvdConvertLibraryResult` | `Result MvdConvertLibraryResult(s32 status, u32 codecClass, u32 postprocessor)` |
| `0x10B84E` | `MvdDmaDestroyHandle` | `Handle * MvdDmaDestroyHandle(Handle *dma)` |
| `0x10B860` | `MvdL2bReadAlpha` | `Result MvdL2bReadAlpha(MvdL2bRegisterContext *registers, u16 *alpha)` |
| `0x10B874` | `MvdDmaIsDone` | `u32 MvdDmaIsDone(const Handle *dma)` |
| `0x10B8A0` | `MvdL2bReadLineWidth` | `Result MvdL2bReadLineWidth(MvdL2bRegisterContext *registers, u16 *width)` |
| `0x10B8BC` | `MvdL2bReadBusy` | `Result MvdL2bReadBusy(MvdL2bRegisterContext *registers, u8 *busy)` |
| `0x10B8D0` | `MvdL2bReadOutputFormatBits` | `Result MvdL2bReadOutputFormatBits(MvdL2bRegisterContext *registers, u16 *bits)` |
| `0x10B8E8` | `MvdL2bReadInputFormat` | `Result MvdL2bReadInputFormat(MvdL2bRegisterContext *registers, MvdRgbFormat *format)` |
| `0x10B900` | `MvdL2bReadLines` | `Result MvdL2bReadLines(MvdL2bRegisterContext *registers, u16 *lines)` |
| `0x10B914` | `MvdDmaGetState` | `u8 MvdDmaGetState(const Handle *dma)` |
| `0x10B936` | `MvdDmaTryStart` | `u32 MvdDmaTryStart(Handle *dma, Handle dstProcess, u32 dstAddress, Handle srcProcess, u32 srcAddress, u32 size, const MvdDmaConfig *config)` |
| `0x10B97C` | `svcStoreProcessDataCache` | `Result svcStoreProcessDataCache(Handle process, u32 address, u32 size)` |
| `0x10B984` | `MvdIpcHasSharedHandleDescriptor` | `u32 MvdIpcHasSharedHandleDescriptor(u32 **buffer, u32 wordIndex, u32 handleCount)` |
| `0x10B99E` | `MvdIpcHasExactHeader` | `u32 MvdIpcHasExactHeader(u32 **buffer, u32 commandId, u32 normalWords, u32 translatedWords, u32 extra)` |
| `0x10B9BE` | `MvdTranslateLinearRange` | `u32 MvdTranslateLinearRange(u32 address, u32 size, u32 rangeBegin, u32 rangeEnd, u32 busBase)` |
| `0x10B9DE` | `MvdWaitEventRaw` | `Result MvdWaitEventRaw(const Handle *event,s64 timeoutNs)` |
| `0x10B9E8` | `WriteBlock` | `void WriteBlock(const unsigned short *rlc,u32 *pendingWord,u32 **output,u32 *halfwordCount)` |
| `0x10BA0C` | `WriteSubBlock` | `void WriteSubBlock(const unsigned short *rlc,u32 *pendingWord,u32 **output,u32 *halfwordCount)` |
| `0x10BA64` | `MvdWaitEventUntilTimeout` | `u32 MvdWaitEventUntilTimeout(const Handle *event,s64 timeoutNs)` |
| `0x10BA88` | `MvdTimeSpanFromMilliseconds` | `void MvdTimeSpanFromMilliseconds(s64 *out,s64 milliseconds)` |
| `0x10BAA0` | `MvdClearEventChecked` | `void MvdClearEventChecked(const Handle *event)` |
| `0x10BAB2` | `VP8HwdAsicReleaseMem` | `void VP8HwdAsicReleaseMem(MvdVp8Container *decoder)` |
| `0x10BADE` | `VP8HwdUpdateRefs` | `void VP8HwdUpdateRefs(MvdVp8Container *decoder,u32 corrupted)` |
| `0x10BB6C` | `VP8HwdAsicRun` | `u32 VP8HwdAsicRun(MvdVp8Container *decoder)` |
| `0x10BE30` | `MvdVp8RunConcealment` | `u32 MvdVp8RunConcealment(MvdVp8Container *decoder,u32 streamBusAddress,u32 extrapolate)` |
| `0x10BEB0` | `VP8HwdAsicStrmPosUpdate` | `void VP8HwdAsicStrmPosUpdate(MvdVp8Container *decoder,u32 streamBusAddress)` |
| `0x10BFF0` | `VP8HwdAsicInitPicture` | `void VP8HwdAsicInitPicture(MvdVp8Container *decoder)` |
| `0x10C860` | `VP8HwdAsicReleasePictures` | `void VP8HwdAsicReleasePictures(MvdVp8Container *decoder)` |
| `0x10C918` | `DWLFreeRefFrm` | `void DWLFreeRefFrm(const MvdDwlInstance *dwl,MvdLinearMem *buffer)` |
| `0x10C920` | `BqueueRelease` | `void BqueueRelease(MvdBufferQueue *queue)` |
| `0x10C938` | `DecSetupTiledReference` | `u32 DecSetupTiledReference(u32 *registers,u32 tiledSupport,u32 dpbMode,u32 interlaced)` |
| `0x10C980` | `DWLMallocRefFrm` | `i32 DWLMallocRefFrm(MvdDwlInstance *dwl,u32 size,MvdLinearMem *buffer)` |
| `0x10C988` | `BqueueInit` | `u32 BqueueInit(MvdBufferQueue *queue,u32 numBuffers)` |
| `0x10C9BE` | `VP6HW_CreateHuffmanLUT` | `void VP6HW_CreateHuffmanLUT(const MvdVp6HuffNode *nodes,unsigned short *table,s32 values)` |
| `0x10C9E0` | `VP6HW_BuildHuffTree` | `void VP6HW_BuildHuffTree(MvdVp6HuffNode *nodes,u32 *counts,s32 values)` |
| `0x10CAD0` | `VP6HWStartDecode` | `void VP6HWStartDecode(MvdBoolCoder *bc,const u8 *stream,u32 length)` |
| `0x10CB0C` | `Vp6StrmGetBits` | `u32 Vp6StrmGetBits(MvdVp6Stream *stream,u32 bits)` |
| `0x10CB5C` | `VP6HWbitread` | `u32 VP6HWbitread(MvdBoolCoder *bc,s32 bits)` |
| `0x10CB78` | `VP6HWDecodeBool` | `u32 VP6HWDecodeBool(MvdBoolCoder *bc,s32 probability)` |
| `0x10CBCC` | `VP6HWDeleteHuffman` | `void VP6HWDeleteHuffman(MvdVp6Pb *pb)` |
| `0x10CBE0` | `VP6HwdAsicReleaseMem` | `void VP6HwdAsicReleaseMem(MvdVp6Container *decoder)` |
| `0x10CC06` | `BqueueDiscard` | `void BqueueDiscard(MvdBufferQueue *queue,u32 buffer)` |
| `0x10CC10` | `BqueueNext` | `u32 BqueueNext(MvdBufferQueue *queue,u32 ref0,u32 ref1,u32 ref2,u32 bPic)` |
| `0x10CC88` | `vp6PreparePpRun` | `void vp6PreparePpRun(MvdVp6Container *decoder)` |
| `0x10CCD0` | `VP6HwdAsicReleasePictures` | `void VP6HwdAsicReleasePictures(MvdVp6Container *decoder)` |
| `0x10CD24` | `DWLFreeLinear` | `void DWLFreeLinear(const MvdDwlInstance *dwl,MvdLinearMem *buffer)` |
| `0x10CD2C` | `RefbuGetHitThreshold` | `i32 RefbuGetHitThreshold(MvdRefBuffer *refbu)` |
| `0x10CD8E` | `ProcessTreeNode` | `void ProcessTreeNode(const MvdVp6HuffNode *nodes,MvdVp6TokenOrPtr node,u32 code,u32 length,unsigned short *table)` |
| `0x10CDE2` | `GetInterNeighbour` | `u32 GetInterNeighbour(u32 sliceId,const MvdH264MbStorage *neighbor)` |
| `0x10CDF4` | `h264bsdGetRefPicData` | `s32 h264bsdGetRefPicData(const MvdH264Dpb *dpb,u32 index)` |
| `0x10CE2C` | `PPFindFirstNonZeroBit` | `u32 PPFindFirstNonZeroBit(u32 mask)` |
| `0x10CE42` | `PPSelectDitheringValue` | `u32 PPSelectDitheringValue(u32 mask)` |
| `0x10CE70` | `PPDecCombinedModeDisable` | `PPResult PPDecCombinedModeDisable(MvdPpContainer *pp, const void *decoder)` |
| `0x10CEF0` | `WaitForPp` | `PPResult WaitForPp(MvdPpContainer *pp)` |
| `0x10CF84` | `PPRun` | `PPResult PPRun(MvdPpContainer *pp)` |
| `0x10CFEC` | `PPSetupScaling` | `void PPSetupScaling(MvdPpContainer *pp, const PPOutImage *out)` |
| `0x10D2C4` | `PPSetStatus` | `void PPSetStatus(MvdPpContainer *pp, u32 status)` |
| `0x10D2CE` | `PPRefreshRegs` | `void PPRefreshRegs(MvdPpContainer *pp)` |
| `0x10D2F0` | `PPGetStatus` | `u32 PPGetStatus(MvdPpContainer *pp)` |
| `0x10D2FA` | `PPContinuousCheck` | `i32 PPContinuousCheck(u32 mask)` |
| `0x10D328` | `PPIsOutPixFmtBlendOk` | `u32 PPIsOutPixFmtBlendOk(u32 format)` |
| `0x10D380` | `GetPoc` | `s32 GetPoc(const MvdH264DpbPicture *picture)` |
| `0x10D3A4` | `SetPoc` | `void SetPoc(MvdH264DpbPicture *picture,const s32 *picOrderCnt,u32 field)` |
| `0x10D3BC` | `OutputPicture` | `u32 OutputPicture(MvdH264Dpb *dpb)` |
| `0x10D4C4` | `IsExisting` | `u32 IsExisting(const MvdH264DpbPicture *picture,u32 field)` |
| `0x10D4EA` | `IsLongTermField` | `u32 IsLongTermField(const MvdH264DpbPicture *picture)` |
| `0x10D4FE` | `DpbBufFree` | `void DpbBufFree(MvdH264Dpb *dpb,u32 index)` |
| `0x10D520` | `IsUnused` | `u32 IsUnused(const MvdH264DpbPicture *picture,u32 field)` |
| `0x10D53E` | `SetStatus` | `void SetStatus(MvdH264DpbPicture *picture,u8 status,u32 field)` |
| `0x10D54C` | `FindDpbPic` | `s32 FindDpbPic(MvdH264Dpb *dpb,s32 picNum,u32 isShortTerm,u32 field)` |
| `0x10D5CE` | `h264bsdGetNeighbourMb` | `MvdH264MbStorage *h264bsdGetNeighbourMb(MvdH264MbStorage *mb,u8 neighbor)` |
| `0x10D5FA` | `GetIntraNeighbour` | `u32 GetIntraNeighbour(u32 sliceId,const MvdH264MbStorage *neighbor)` |
| `0x10D60C` | `h264bsdGetRefPicDataVlcMode` | `u8 *h264bsdGetRefPicDataVlcMode(const MvdH264Dpb *dpb,u32 index,u32 fieldMode)` |
| `0x10D650` | `RefbuMvStatistics` | `void RefbuMvStatistics(MvdRefBuffer *refbu,u32 *registers,u32 *mvs,u32 directMvsAvailable,u32 isIntra)` |
| `0x10D7C4` | `DWLReadReg` | `u32 DWLReadReg(const MvdDwlInstance *dwl,u32 byteOffset)` |
| `0x10D7CC` | `DWLWaitHwReady` | `i32 DWLWaitHwReady(const MvdDwlInstance *dwl,u32 timeout)` |
| `0x10D7D4` | `DWLEnableHW` | `void DWLEnableHW(const MvdDwlInstance *dwl,u32 byteOffset,u32 value)` |
| `0x10D7DC` | `DWLReserveHw` | `i32 DWLReserveHw(const MvdDwlInstance *dwl)` |
| `0x10D7E4` | `RefbuSetup` | `void RefbuSetup(MvdRefBuffer *refbu,u32 *registers,u32 mode,u32 isIntra,u32 isB,u32 refPicId0,u32 refPicId1,u32 flags)` |
| `0x10DA50` | `DWLWriteReg` | `void DWLWriteReg(const MvdDwlInstance *dwl,u32 byteOffset,u32 value)` |
| `0x10DA58` | `h264StreamPosUpdate` | `void h264StreamPosUpdate(MvdH264Container *decoder)` |
| `0x10DAD0` | `ShellSort` | `void ShellSort(MvdH264Dpb *dpb,u32 *list,u32 type,s32 currentPoc)` |
| `0x10DB60` | `ShellSortF` | `void ShellSortF(MvdH264Dpb *dpb,u32 *list,u32 type,s32 currentPoc)` |
| `0x10DBF0` | `ReleaseAsicBuffers` | `void ReleaseAsicBuffers(const void *dwl,MvdH264AsicBuffers *buffers)` |
| `0x10DC52` | `h264bsdFreeDpb` | `void h264bsdFreeDpb(const MvdDwlInstance *dwl,MvdH264Dpb *dpb)` |
| `0x10DC9A` | `h264PpMultiAddPic` | `u32 h264PpMultiAddPic(MvdH264Container *decoder,const MvdLinearMem *data)` |
| `0x10DCCE` | `h264bsdNextOutputPicture` | `MvdH264DpbOutPicture *h264bsdNextOutputPicture(MvdH264Storage *storage)` |
| `0x10DD1E` | `DWLDisableHW` | `void DWLDisableHW(const MvdDwlInstance *dwl,u32 byteOffset,u32 value)` |
| `0x10DD26` | `DWLRelease` | `int DWLRelease(void *dwl)` |
| `0x10DD30` | `MvdInitDecoderRegisters` | `void MvdInitDecoderRegisters(u32 *registers)` |
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
| `0x10E7EC` | `h264bsdFlushDpb` | `void h264bsdFlushDpb(MvdH264Dpb *dpb)` |
| `0x10E828` | `h264CheckReleasePpAndHw` | `void h264CheckReleasePpAndHw(MvdH264Container *decoder)` |
| `0x10E874` | `h264PpMultiMvc` | `void h264PpMultiMvc(MvdH264Container *decoder,u32 maxBuffId)` |
| `0x10E87E` | `h264PpMultiInit` | `void h264PpMultiInit(MvdH264Container *decoder,u32 maxBuffId)` |
| `0x10E8A4` | `h264bsdResetStorage` | `void h264bsdResetStorage(MvdH264Storage *storage)` |
| `0x10E8E0` | `h264AllocateResources` | `u32 h264AllocateResources(MvdH264Container *decoder)` |
| `0x10E9D4` | `MvdYuv420ReferenceBufferBytes` | `u32 MvdYuv420ReferenceBufferBytes(u32 height, u32 width, u32 count)` |
| `0x10E9E8` | `MvdSignedDivideWithRemainder` | `MvdDivModResult __usercall MvdSignedDivideWithRemainder@<R1:R0>(s32 dividend@<R0>, s32 divisor@<R1>)` |
| `0x10EB64` | `IsLongTerm` | `u32 IsLongTerm(const MvdH264DpbPicture *picture,u32 field)` |
| `0x10EB82` | `h264bsdIsNeighbourAvailable` | `u32 h264bsdIsNeighbourAvailable(const MvdH264MbStorage *mb,const MvdH264MbStorage *neighbor)` |
| `0x10EB98` | `h264bsdNeighbour4x4BlockB` | `const MvdH264Neighbour *h264bsdNeighbour4x4BlockB(u32 blockIndex)` |
| `0x10EBA4` | `h264bsdNeighbour4x4BlockA` | `const MvdH264Neighbour *h264bsdNeighbour4x4BlockA(u32 blockIndex)` |
| `0x10EBB0` | `DecodeQuantizerDelta` | `s32 DecodeQuantizerDelta(MvdBoolCoder *bc)` |
| `0x10EBDA` | `ScaleDimension` | `u32 ScaleDimension(u32 original,u32 scale)` |
| `0x10EC20` | `h264bsdDecodeResidualBlockCavlc` | `u32 h264bsdDecodeResidualBlockCavlc(strmData_t *stream,unsigned short *rlc,s32 nC,u32 maxCoefficients)` |
| `0x10EEFA` | `DetermineNc` | `u32 DetermineNc(MvdH264MbStorage *mb,u32 blockIndex,const u8 *totalCoeff)` |
| `0x10EFA0` | `h264bsdDecodeHrdParameters` | `u32 h264bsdDecodeHrdParameters(strmData_t *stream,hrdParameters_t *hrd)` |
| `0x10F098` | `h264bsdFlushBits` | `u32 h264bsdFlushBits(strmData_t *stream,u32 numBits)` |
| `0x10F1BA` | `h264bsdShowBits` | `u32 h264bsdShowBits(strmData_t *stream,u32 numBits)` |
| `0x10F298` | `h264bsdDecodeExpGolombSigned` | `u32 h264bsdDecodeExpGolombSigned(strmData_t *stream, s32 *value)` |
| `0x10F2D6` | `h264bsdNumMbPart` | `u32 h264bsdNumMbPart(u8 mbType)` |
| `0x10F2F2` | `h264bsdMbPartPredMode` | `u8 h264bsdMbPartPredMode(u8 mbType)` |
| `0x10F306` | `vp8hwdReadBits` | `u32 vp8hwdReadBits(MvdBoolCoder *bc,s32 bits)` |
| `0x10F324` | `SetDecRegister` | `void SetDecRegister(u32 *registers, MvdHwIf field, u32 value)` |
| `0x10F354` | `h264bsdDecodeExpGolombUnsigned` | `u32 h264bsdDecodeExpGolombUnsigned(strmData_t *stream, u32 *value)` |
| `0x10F448` | `h264bsdGetBits` | `u32 h264bsdGetBits(strmData_t *stream, u32 numBits)` |
| `0x10F474` | `MvdCopyMemory` | `void *MvdCopyMemory(void *dest, const void *source, u32 size)` |
| `0x10F500` | `MvdHeapFree` | `void MvdHeapFree(void *ptr)` |
| `0x10F56C` | `MvdDwlAllocateLinear` | `i32 MvdDwlAllocateLinear(MvdDwlInstance *dwl,u32 size,MvdLinearMem *buffer)` |
| `0x10F5D0` | `IsReference` | `u32 IsReference(MvdH264DpbPicture picture,u32 field)` |
| `0x10F608` | `IsShortTerm` | `u32 IsShortTerm(const MvdH264DpbPicture *picture,u32 field)` |
| `0x10F630` | `IsShortTermField` | `u32 IsShortTermField(const MvdH264DpbPicture *picture)` |
| `0x10F64C` | `IsReferenceField` | `u32 IsReferenceField(const MvdH264DpbPicture *picture)` |
| `0x10F68C` | `svcFlushProcessDataCache` | `Result svcFlushProcessDataCache(Handle process,u32 address,u32 size)` |
| `0x10F694` | `DWLfree` | `void DWLfree(void *ptr)` |
| `0x10F6A0` | `DWLmalloc` | `void *DWLmalloc(u32 size)` |
| `0x10F6A8` | `RefbuInit` | `void RefbuInit(MvdRefBuffer *refbu,u32 decMode,u32 widthInMbs,u32 heightInMbs,u32 supportFlags)` |
| `0x10F780` | `DWLmemcpy` | `void *DWLmemcpy(void *dest, const void *source, u32 size)` |
| `0x10F788` | `DWLMallocLinear` | `i32 DWLMallocLinear(MvdDwlInstance *dwl,u32 size,MvdLinearMem *buffer)` |
| `0x10F82C` | `MvdHeapAlloc` | `void *MvdHeapAlloc(u32 size)` |
| `0x10F85C` | `MvdIpcBuildHeader` | `u32 MvdIpcBuildHeader(u32 commandId, u32 normalWords, u32 translatedWords, u32 extra)` |
| `0x10F874` | `MvdY2rInvalidDimensionsResult` | `Result MvdY2rInvalidDimensionsResult(void)` |
| `0x10F87C` | `MvdL2bInvalidDimensionsResult` | `Result MvdL2bInvalidDimensionsResult(void)` |
| `0x10F898` | `MvdY2rSetOutputDmaEnable` | `Result MvdY2rSetOutputDmaEnable(MvdY2rRegisterContext *registers, s32 enable)` |
| `0x10F8B8` | `MvdY2rSetInputDmaEnable` | `Result MvdY2rSetInputDmaEnable(MvdY2rRegisterContext *registers, s32 enable)` |
| `0x10F8D8` | `MvdY2rSetTransferEndInterrupt` | `Result MvdY2rSetTransferEndInterrupt(MvdY2rRegisterContext *registers, s32 enable)` |
| `0x10F8F8` | `MvdY2rWriteBackOutputDrq` | `Result MvdY2rWriteBackOutputDrq(MvdY2rRegisterContext *registers)` |
| `0x10F914` | `MvdY2rWriteBackYuyvInputDrq` | `Result MvdY2rWriteBackYuyvInputDrq(MvdY2rRegisterContext *registers)` |
| `0x10F930` | `MvdY2rWriteBackControlBit26` | `Result MvdY2rWriteBackControlBit26(MvdY2rRegisterContext *registers)` |
| `0x10F94C` | `MvdY2rWriteBackControlBit25` | `Result MvdY2rWriteBackControlBit25(MvdY2rRegisterContext *registers)` |
| `0x10F968` | `MvdY2rWriteBackControlBit24` | `Result MvdY2rWriteBackControlBit24(MvdY2rRegisterContext *registers)` |
| `0x10F984` | `MvdY2rWriteAlpha` | `Result MvdY2rWriteAlpha(MvdY2rRegisterContext *registers, u8 alpha)` |
| `0x10F998` | `MvdY2rWriteLines` | `Result MvdY2rWriteLines(MvdY2rRegisterContext *registers, s32 lines)` |
| `0x10F9C4` | `MvdY2rWriteLineWidth` | `Result MvdY2rWriteLineWidth(MvdY2rRegisterContext *registers, s32 width)` |
| `0x10F9FC` | `MvdY2rWriteDitherWeights` | `Result MvdY2rWriteDitherWeights(MvdY2rRegisterContext *registers, MvdY2rDitherWeights weights)` |
| `0x10FB30` | `MvdY2rSetTemporalDithering` | `Result MvdY2rSetTemporalDithering(MvdY2rRegisterContext *registers, s32 enable)` |
| `0x10FB50` | `MvdY2rSetSpatialDithering` | `Result MvdY2rSetSpatialDithering(MvdY2rRegisterContext *registers, s32 enable)` |
| `0x10FB70` | `MvdY2rWriteCoefficients` | `Result MvdY2rWriteCoefficients(MvdY2rRegisterContext *registers, MvdY2rCoefficients coefficients)` |
| `0x10FC00` | `MvdY2rWriteBlockAlignmentBits` | `Result MvdY2rWriteBlockAlignmentBits(MvdY2rRegisterContext *registers, u32 bits)` |
| `0x10FC1C` | `MvdY2rWriteRotationBits` | `Result MvdY2rWriteRotationBits(MvdY2rRegisterContext *registers, u32 bits)` |
| `0x10FC38` | `MvdY2rWriteOutputFormatBits` | `Result MvdY2rWriteOutputFormatBits(MvdY2rRegisterContext *registers, u32 bits)` |
| `0x10FC54` | `MvdY2rWriteInputFormat` | `Result MvdY2rWriteInputFormat(MvdY2rRegisterContext *registers, u32 format)` |
| `0x10FC6C` | `MvdY2rWriteReset` | `Result MvdY2rWriteReset(MvdY2rRegisterContext *registers)` |
| `0x10FC80` | `MvdY2rStop` | `Result MvdY2rStop(MvdY2rRegisterContext *registers)` |
| `0x10FC98` | `MvdL2bSetOutputDmaEnable` | `Result MvdL2bSetOutputDmaEnable(MvdL2bRegisterContext *registers, s32 enable)` |
| `0x10FCB8` | `MvdL2bSetInputDmaEnable` | `Result MvdL2bSetInputDmaEnable(MvdL2bRegisterContext *registers, s32 enable)` |
| `0x10FCD8` | `MvdL2bSetTransferEndInterrupt` | `Result MvdL2bSetTransferEndInterrupt(MvdL2bRegisterContext *registers, s32 enable)` |
| `0x10FCF8` | `MvdL2bWriteBackOutputDrq` | `Result MvdL2bWriteBackOutputDrq(MvdL2bRegisterContext *registers)` |
| `0x10FD14` | `MvdL2bWriteBackInputDrq` | `Result MvdL2bWriteBackInputDrq(MvdL2bRegisterContext *registers)` |
| `0x10FD30` | `MvdL2bWriteAlpha` | `Result MvdL2bWriteAlpha(MvdL2bRegisterContext *registers, u8 alpha)` |
| `0x10FD44` | `MvdL2bWriteLines` | `Result MvdL2bWriteLines(MvdL2bRegisterContext *registers, s32 lines)` |
| `0x10FD7C` | `MvdL2bWriteLineWidth` | `Result MvdL2bWriteLineWidth(MvdL2bRegisterContext *registers, s32 width)` |
| `0x10FDB4` | `MvdL2bWriteOutputFormat` | `Result MvdL2bWriteOutputFormat(MvdL2bRegisterContext *registers, u32 shiftedFormat)` |
| `0x10FDD0` | `MvdL2bWriteInputFormat` | `Result MvdL2bWriteInputFormat(MvdL2bRegisterContext *registers, u32 format)` |
| `0x10FDE8` | `MvdL2bWriteReset` | `Result MvdL2bWriteReset(MvdL2bRegisterContext *registers)` |
| `0x10FDFC` | `MvdL2bStop` | `Result MvdL2bStop(MvdL2bRegisterContext *registers)` |
| `0x10FEF0` | `MVDY2R_DriverFinalize` | `Result MVDY2R_DriverFinalize(void)` |
| `0x10FF54` | `MvdL2bCloseSession` | `Result MvdL2bCloseSession(MvdL2bContext *context)` |
| `0x10FF98` | `MvdY2rRegistersFinalize` | `Result MvdY2rRegistersFinalize(MvdY2rRegisterContext *registers)` |
| `0x10FFF4` | `MvdY2rRegistersInitialize` | `Result MvdY2rRegistersInitialize(MvdY2rRegisterContext *registers, u32 engine)` |
| `0x110114` | `MvdIpcReadWord` | `u32 MvdIpcReadWord(u32 **buffer, u32 wordIndex)` |
| `0x110138` | `MvdIpcWriteResponseHeader` | `u32 MvdIpcWriteResponseHeader(u32 **buffer, u32 command, u32 normalWords, u32 translatedWords, u32 extra)` |
| `0x11019C` | `svcUnbindInterrupt` | `Result svcUnbindInterrupt(u32 interruptId, Handle event)` |
| `0x1101A4` | `MvdDmaStopAndClose` | `Result MvdDmaStopAndClose(Handle *dma)` |
| `0x1101B4` | `MvdDmaStop` | `Result MvdDmaStop(const Handle *dma)` |
| `0x1101CC` | `MvdL2bRegistersFinalize` | `Result MvdL2bRegistersFinalize(MvdL2bRegisterContext *registers)` |
| `0x110218` | `svcBindInterrupt` | `Result svcBindInterrupt(u32 interruptId, Handle event, s32 priority, u8 manualClear)` |
| `0x11026C` | `svcCreateEvent` | `Result svcCreateEvent(Handle *event, u32 resetType)` |
| `0x110284` | `MvdL2bRegistersInitialize` | `Result MvdL2bRegistersInitialize(MvdL2bRegisterContext *registers, u32 engine)` |
| `0x110314` | `MvdCloseOwnedHandle` | `Result MvdCloseOwnedHandle(Handle *handle)` |
| `0x110328` | `MvdL2bFinalizeInterrupt` | `Result MvdL2bFinalizeInterrupt(MvdL2bContext *context)` |
| `0x110398` | `MvdL2bOpenSession` | `Result MvdL2bOpenSession(MvdL2bContext *context)` |
| `0x1103D8` | `MvdL2bInitializeInterrupt` | `Result MvdL2bInitializeInterrupt(MvdL2bContext *context, u32 engine)` |
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
| `0x111AF8` | `WriteRlcToAsic` | `void WriteRlcToAsic(u8 mbType,u32 codedBlockPattern,MvdH264Residual *residual,MvdH264AsicBuffers *asic)` |
| `0x111E64` | `L2BU_HandleCommands` | `void L2BU_HandleCommands(MvdL2bContext *context)` |
| `0x11216C` | `MVDL2B_SetSending` | `Result MVDL2B_SetSending(MvdL2bContext *context, Handle process, void *address, u32 size, s16 unit, s16 gap)` |
| `0x112238` | `MVDL2B_PingProcess` | `Result MVDL2B_PingProcess(MvdL2bContext *context, u8 *sessions)` |
| `0x112242` | `MVDL2B_SetReceiving` | `Result MVDL2B_SetReceiving(MvdL2bContext *context, Handle process, void *address, u32 size, s16 unit, s16 gap)` |
| `0x112268` | `MVDL2B_GetInputLines` | `Result MVDL2B_GetInputLines(MvdL2bContext *context, u16 *lines)` |
| `0x112272` | `MVDL2B_SetInputLines` | `Result MVDL2B_SetInputLines(MvdL2bContext *context, s16 lines)` |
| `0x11227C` | `MVDL2B_GetInputFormat` | `Result MVDL2B_GetInputFormat(MvdL2bContext *context, MvdRgbFormat *format)` |
| `0x112290` | `MVDL2B_SetInputFormat` | `Result MVDL2B_SetInputFormat(MvdL2bContext *context, MvdRgbFormat format)` |
| `0x11229A` | `MVDL2B_StopConversion` | `Result MVDL2B_StopConversion(MvdL2bContext *context)` |
| `0x1122A4` | `MVDL2B_GetOutputFormat` | `Result MVDL2B_GetOutputFormat(MvdL2bContext *context, MvdRgbFormat *format)` |
| `0x1122BA` | `MVDL2B_SetOutputFormat` | `Result MVDL2B_SetOutputFormat(MvdL2bContext *context, MvdRgbFormat format)` |
| `0x1122C8` | `MVDL2B_StartConversion` | `Result MVDL2B_StartConversion(MvdL2bContext *context)` |
| `0x11230C` | `MVDL2B_IsBusyConversion` | `Result MVDL2B_IsBusyConversion(MvdL2bContext *context, u8 *busy)` |
| `0x112318` | `MvdL2bConfigureReceivingDma` | `Result MvdL2bConfigureReceivingDma(MvdL2bContext *context, Handle process, void *address, u32 size, s16 unit, s16 gap)` |
| `0x1123D4` | `MVDL2B_GetInputLineWidth` | `Result MVDL2B_GetInputLineWidth(MvdL2bContext *context, u16 *width)` |
| `0x1123DE` | `MVDL2B_IsDoneSending` | `Result MVDL2B_IsDoneSending(MvdL2bContext *context, u8 *done)` |
| `0x1123EE` | `MVDL2B_SetInputLineWidth` | `Result MVDL2B_SetInputLineWidth(MvdL2bContext *context, s16 width)` |
| `0x1123F8` | `MVDL2B_GetPackageParameter` | `Result MVDL2B_GetPackageParameter(MvdL2bContext *context, MvdL2bParams *params)` |
| `0x112444` | `MVDL2B_GetTransferEndEvent` | `Result MVDL2B_GetTransferEndEvent(MvdL2bContext *context, Handle *event)` |
| `0x11244C` | `MVDL2B_IsDoneReceiving` | `Result MVDL2B_IsDoneReceiving(MvdL2bContext *context, u8 *done)` |
| `0x11245C` | `MVDL2B_SetPackageParameter` | `Result MVDL2B_SetPackageParameter(MvdL2bContext *context, MvdL2bParams *params)` |
| `0x1124A4` | `MVDL2B_GetTransferEndInterrupt` | `Result MVDL2B_GetTransferEndInterrupt(MvdL2bContext *context, u8 *enable)` |
| `0x1124AE` | `MVDL2B_SetTransferEndInterrupt` | `Result MVDL2B_SetTransferEndInterrupt(MvdL2bContext *context, s8 enable)` |
| `0x1124B8` | `MVDL2B_GetAlpha` | `Result MVDL2B_GetAlpha(MvdL2bContext *context, u16 *alpha)` |
| `0x1124C2` | `MVDL2B_SetAlpha` | `Result MVDL2B_SetAlpha(MvdL2bContext *context, u16 alpha)` |
| `0x1124CC` | `MvdL2bDestroyContext` | `MvdL2bContext * MvdL2bDestroyContext(MvdL2bContext *context)` |
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
| `0x11328C` | `Y2RU_HandleCommands` | `void Y2RU_HandleCommands(MvdY2rContext *context)` |
| `0x1138B0` | `MVDY2R_GetRotation` | `Result MVDY2R_GetRotation(MvdY2rContext *context, u8 *rotation)` |
| `0x1138CC` | `MVDY2R_PingProcess` | `Result MVDY2R_PingProcess(MvdY2rContext *context, u8 *sessions)` |
| `0x1138DC` | `MVDY2R_SetRotation` | `Result MVDY2R_SetRotation(MvdY2rContext *context, u8 rotation)` |
| `0x1138F0` | `MVDY2R_SetSendingU` | `Result MVDY2R_SetSendingU(MvdY2rContext *context, Handle process, void *address, u32 size, s16 unit, s16 gap)` |
| `0x1139B0` | `MVDY2R_SetSendingV` | `Result MVDY2R_SetSendingV(MvdY2rContext *context, Handle process, void *address, u32 size, s16 unit, s16 gap)` |
| `0x113A70` | `MVDY2R_SetSendingY` | `Result MVDY2R_SetSendingY(MvdY2rContext *context, Handle process, void *address, u32 size, s16 unit, s16 gap)` |
| `0x113B30` | `MVDY2R_SetReceiving` | `Result MVDY2R_SetReceiving(MvdY2rContext *context, Handle process, void *address, u32 size, s16 unit, s16 gap)` |
| `0x113B58` | `MVDY2R_GetInputLines` | `Result MVDY2R_GetInputLines(MvdY2rContext *context, u16 *lines)` |
| `0x113B68` | `MVDY2R_SetInputLines` | `Result MVDY2R_SetInputLines(MvdY2rContext *context, s16 lines)` |
| `0x113B78` | `MVDY2R_SetSendingYUYV` | `Result MVDY2R_SetSendingYUYV(MvdY2rContext *context, Handle process, void *address, u32 size, s16 unit, s16 gap)` |
| `0x113BA0` | `MVDY2R_GetInputFormat` | `Result MVDY2R_GetInputFormat(MvdY2rContext *context, u8 *format)` |
| `0x113BB8` | `MVDY2R_SetInputFormat` | `Result MVDY2R_SetInputFormat(MvdY2rContext *context, u8 format)` |
| `0x113BC8` | `MVDY2R_StopConversion` | `Result MVDY2R_StopConversion(void)` |
| `0x113BD8` | `MVDY2R_GetOutputFormat` | `Result MVDY2R_GetOutputFormat(MvdY2rContext *context, MvdRgbFormat *format)` |
| `0x113BF4` | `MVDY2R_SetOutputFormat` | `Result MVDY2R_SetOutputFormat(MvdY2rContext *context, MvdRgbFormat format)` |
| `0x113C04` | `MVDY2R_StartConversion` | `Result MVDY2R_StartConversion(void)` |
| `0x113C68` | `MVDY2R_DriverInitialize` | `Result MVDY2R_DriverInitialize(void)` |
| `0x113C8C` | `MVDY2R_IsBusyConversion` | `Result MVDY2R_IsBusyConversion(u8 *busy)` |
| `0x113C9C` | `MvdY2rConfigureReceivingDma` | `Result MvdY2rConfigureReceivingDma(MvdY2rContext *context, Handle process, void *address, u32 size, s16 unit, s16 gap)` |
| `0x113D48` | `MVDY2R_GetBlockAlignment` | `Result MVDY2R_GetBlockAlignment(MvdY2rContext *context, u8 *alignment)` |
| `0x113D64` | `MVDY2R_GetInputLineWidth` | `Result MVDY2R_GetInputLineWidth(MvdY2rContext *context, u16 *width)` |
| `0x113D74` | `MVDY2R_SetBlockAlignment` | `Result MVDY2R_SetBlockAlignment(MvdY2rContext *context, u8 alignment)` |
| `0x113D88` | `MVDY2R_SetInputLineWidth` | `Result MVDY2R_SetInputLineWidth(MvdY2rContext *context, s16 width)` |
| `0x113D98` | `MvdY2rConfigureSendingYuyvDma` | `Result MvdY2rConfigureSendingYuyvDma(MvdY2rContext *context, Handle process, void *address, u32 size, s16 unit, s16 gap)` |
| `0x113E64` | `MVDY2R_IsDoneSendingU` | `Result MVDY2R_IsDoneSendingU(MvdY2rContext *context, u8 *done)` |
| `0x113E78` | `MVDY2R_IsDoneSendingV` | `Result MVDY2R_IsDoneSendingV(MvdY2rContext *context, u8 *done)` |
| `0x113E8C` | `MVDY2R_IsDoneSendingY` | `Result MVDY2R_IsDoneSendingY(MvdY2rContext *context, u8 *done)` |
| `0x113EA0` | `MVDY2R_GetConversionParams` | `Result MVDY2R_GetConversionParams(MvdY2rContext *context, MvdY2rParams *params)` |
| `0x113F98` | `MVDY2R_GetSpacialDithering` | `Result MVDY2R_GetSpacialDithering(MvdY2rContext *context, u8 *enable)` |
| `0x113FA8` | `MVDY2R_GetTransferEndEvent` | `Result MVDY2R_GetTransferEndEvent(MvdY2rContext *context, Handle *event)` |
| `0x113FB8` | `MVDY2R_IsDoneReceiving` | `Result MVDY2R_IsDoneReceiving(MvdY2rContext *context, u8 *done)` |
| `0x113FCC` | `MVDY2R_SetConversionParams` | `Result MVDY2R_SetConversionParams(MvdY2rContext *context, MvdY2rParams *params)` |
| `0x114044` | `MVDY2R_SetSpacialDithering` | `Result MVDY2R_SetSpacialDithering(MvdY2rContext *context, s8 enable)` |
| `0x114054` | `MVDY2R_GetCoefficients` | `Result MVDY2R_GetCoefficients(MvdY2rContext *context, MvdY2rCoefficients *coefficients)` |
| `0x114064` | `MVDY2R_GetTemporalDithering` | `Result MVDY2R_GetTemporalDithering(MvdY2rContext *context, u8 *enable)` |
| `0x114074` | `MVDY2R_IsDoneSendingYUYV` | `Result MVDY2R_IsDoneSendingYUYV(MvdY2rContext *context, u8 *done)` |
| `0x114088` | `MVDY2R_SetCoefficients` | `Result MVDY2R_SetCoefficients(MvdY2rContext *context, MvdY2rCoefficients coefficients)` |
| `0x1140D4` | `MVDY2R_SetTemporalDithering` | `Result MVDY2R_SetTemporalDithering(MvdY2rContext *context, s8 enable)` |
| `0x1140E4` | `MVDY2R_SetStandardCoefficient` | `Result MVDY2R_SetStandardCoefficient(MvdY2rContext *context, u8 index)` |
| `0x1140F4` | `MVDY2R_GetTransferEndInterrupt` | `Result MVDY2R_GetTransferEndInterrupt(MvdY2rContext *context, u8 *enable)` |
| `0x114104` | `MVDY2R_SetTransferEndInterrupt` | `Result MVDY2R_SetTransferEndInterrupt(MvdY2rContext *context, s8 enable)` |
| `0x114114` | `MVDY2R_GetDitheringWeightParams` | `Result MVDY2R_GetDitheringWeightParams(MvdY2rContext *context, MvdY2rDitherWeights *weights)` |
| `0x11418C` | `MVDY2R_SetDitheringWeightParams` | `Result MVDY2R_SetDitheringWeightParams(MvdY2rContext *context, MvdY2rDitherWeights weights)` |
| `0x11422C` | `MVDY2R_GetStandardCoefficient` | `Result MVDY2R_GetStandardCoefficient(MvdY2rContext *context, MvdY2rCoefficients *coefficients, u8 index)` |
| `0x11423C` | `MVDY2R_GetAlpha` | `Result MVDY2R_GetAlpha(MvdY2rContext *context, u16 *alpha)` |
| `0x11424C` | `MVDY2R_SetAlpha` | `Result MVDY2R_SetAlpha(MvdY2rContext *context, u16 alpha)` |
| `0x114264` | `MvdBindDecoderInterrupt` | `int MvdBindDecoderInterrupt(void)` |
| `0x1142F8` | `MvdCalculateLevelWorkBufferSize` | `u32 MvdCalculateLevelWorkBufferSize(const MvdWorkSizeParams *params)` |
| `0x114350` | `MvdAttachClientWorkBuffer` | `int MvdAttachClientWorkBuffer(u32 address, u32 size)` |
| `0x114378` | `MvdL2bStart` | `Result MvdL2bStart(MvdL2bRegisterContext *registers)` |
| `0x114390` | `MvdL2bReadTransferEndInterrupt` | `Result MvdL2bReadTransferEndInterrupt(MvdL2bRegisterContext *registers, u8 *enabled)` |
| `0x1143A8` | `MvdY2rStart` | `Result MvdY2rStart(MvdY2rRegisterContext *registers)` |
| `0x1143C0` | `MvdY2rReadSpatialDithering` | `Result MvdY2rReadSpatialDithering(MvdY2rRegisterContext *registers, u8 *enabled)` |
| `0x1143D8` | `MvdY2rReadTemporalDithering` | `Result MvdY2rReadTemporalDithering(MvdY2rRegisterContext *registers, u8 *enabled)` |
| `0x1143F0` | `MvdY2rReadTransferEndInterrupt` | `Result MvdY2rReadTransferEndInterrupt(MvdY2rRegisterContext *registers, u8 *enabled)` |
| `0x114408` | `MvdY2rReadDitherWeights` | `Result MvdY2rReadDitherWeights(MvdY2rRegisterContext *registers, MvdY2rDitherWeights *weights)` |
| `0x11475C` | `MvdY2rInitializeStaticState` | `Handle *MvdY2rInitializeStaticState(void)` |
| `0x114904` | `MvdVp8CollectNeighborVectors` | `u32 MvdVp8CollectNeighborVectors(const u32 *currentMb,MvdVp8Mv *vectors,u32 *referenceIds,u32 mbY,u32 mbX,u32 validRows,u32 widthInMbs)` |
| `0x114B58` | `h264DpbAdjStereoOutput` | `void h264DpbAdjStereoOutput(MvdH264Dpb *dpb,u32 targetCount)` |
| `0x114B8E` | `h264GetSarInfo` | `void h264GetSarInfo(const MvdH264Storage *storage, u32 *width, u32 *height)` |
| `0x114C30` | `h264PpMultiFindPic` | `u32 h264PpMultiFindPic(MvdH264Container *decoder,const MvdLinearMem *picture)` |
| `0x114C58` | `h264PreparePpRun` | `void h264PreparePpRun(MvdH264Container *decoder)` |
| `0x114E50` | `h264RegisterPP` | `i32 h264RegisterPP(MvdH264Container *decoder, const void *pp, void (*start)(const void *, const MvdDecPpInterface *), void (*end)(const void *), void (*query)(const void *, DecPpQuery *), void (*display)(const void *, u32))` |
| `0x114EBC` | `h264UnregisterPP` | `i32 h264UnregisterPP(MvdH264Container *decoder, const void *pp)` |
| `0x114EE6` | `h264UseDisplaySmoothing` | `u32 h264UseDisplaySmoothing(const MvdH264Container *decoder)` |
| `0x114EF8` | `h264bsdActivateParamSets` | `u32 h264bsdActivateParamSets(MvdH264Storage *storage,u32 ppsId,u32 isIdr)` |
| `0x115004` | `h264bsdAllocateSwResources` | `u32 h264bsdAllocateSwResources(const void *dwl,MvdH264Storage *storage,u32 highSupported)` |
| `0x1150C4` | `h264bsdAspectRatioIdc` | `u32 h264bsdAspectRatioIdc(const MvdH264Storage *storage)` |
| `0x1150E0` | `h264bsdCheckAccessUnitBoundary` | `u32 h264bsdCheckAccessUnitBoundary(strmData_t *stream,MvdH264Nal *nextNal,MvdH264Storage *storage,u32 *boundaryFlag)` |
| `0x115338` | `h264bsdCheckBottomFieldFlag` | `u32 h264bsdCheckBottomFieldFlag(const strmData_t *stream,u32 maxFrameNum,u8 nalUnitType,u32 fieldPicFlagPresent,u32 *bottomFieldFlag)` |
| `0x1153B4` | `h264bsdCheckDeltaPicOrderCnt` | `u32 h264bsdCheckDeltaPicOrderCnt(const strmData_t *stream,const MvdH264Sps *sps,u8 nalUnitType,u32 picOrderPresent,s32 *delta)` |
| `0x11546A` | `h264bsdCheckDeltaPicOrderCntBottom` | `u32 h264bsdCheckDeltaPicOrderCntBottom(const strmData_t *stream,const MvdH264Sps *sps,u8 nalUnitType,s32 *delta)` |
| `0x11551A` | `h264bsdCheckFieldPicFlag` | `u32 h264bsdCheckFieldPicFlag(const strmData_t *stream,u32 maxFrameNum,u8 nalUnitType,u32 fieldPicFlagPresent,u32 *fieldPicFlag)` |
| `0x115586` | `h264bsdCheckFrameNum` | `u32 h264bsdCheckFrameNum(const strmData_t *stream,u32 maxFrameNum,u32 *frameNum)` |
| `0x1155E0` | `h264bsdCheckGapsInFrameNum` | `u32 h264bsdCheckGapsInFrameNum(MvdH264Dpb *dpb,u32 frameNum,u32 isReference,u32 gapsAllowed)` |
| `0x11573C` | `h264bsdCheckIdrPicId` | `u32 h264bsdCheckIdrPicId(const strmData_t *stream,u32 maxFrameNum,u8 nalUnitType,u32 fieldPicFlagPresent,u32 *idrPicId)` |
| `0x1157C2` | `h264bsdCheckPicOrderCntLsb` | `u32 h264bsdCheckPicOrderCntLsb(const strmData_t *stream,const MvdH264Sps *sps,u8 nalUnitType,u32 *pocLsb)` |
| `0x11586C` | `h264bsdCheckPpsId` | `u32 h264bsdCheckPpsId(const strmData_t *stream,u32 *ppsId)` |
| `0x1158B2` | `h264bsdCheckPriorPicsFlag` | `u32 h264bsdCheckPriorPicsFlag(u32 *noOutputOfPriorPicsFlag,const strmData_t *stream,const MvdH264Sps *sps,const MvdH264Pps *pps)` |
| `0x1159C2` | `h264bsdCheckValidParamSets` | `u32 h264bsdCheckValidParamSets(MvdH264Storage *storage)` |
| `0x1159D4` | `h264bsdCompareSeqParamSets` | `u32 h264bsdCompareSeqParamSets(const MvdH264Sps *newSps,MvdH264Sps *storedSps)` |
| `0x115AF6` | `h264bsdComputeSliceGroupMap` | `void h264bsdComputeSliceGroupMap(MvdH264Storage *storage,u32 changeCycle)` |
| `0x115B14` | `h264bsdConceal` | `void h264bsdConceal(MvdH264Storage *storage,MvdH264AsicBuffers *asic,u32 sliceType)` |
| `0x115C0C` | `h264bsdCountLeadingZeros` | `u32 h264bsdCountLeadingZeros(u32 bits,u32 length)` |
| `0x115C28` | `h264bsdDecode` | `u32 h264bsdDecode(MvdH264Container *decoder,const u8 *stream,u32 length,u32 pictureId,u32 *readBytes)` |
| `0x116664` | `h264bsdDecodeExpGolombMapped` | `u32 h264bsdDecodeExpGolombMapped(strmData_t *stream,u32 *value,u32 isIntra)` |
| `0x116698` | `h264bsdDecodeExpGolombTruncated` | `u32 h264bsdDecodeExpGolombTruncated(strmData_t *stream,u32 *value,u32 rangeGreaterThanOne)` |
| `0x1166C0` | `h264bsdDecodeMacroblock` | `u32 h264bsdDecodeMacroblock(MvdH264Storage *storage,u32 mbIndex,s32 *qpY,MvdH264AsicBuffers *asic)` |
| `0x11686C` | `h264bsdDecodeMacroblockLayerCavlc` | `u32 h264bsdDecodeMacroblockLayerCavlc(strmData_t *stream,MvdH264MbLayer *layer,MvdH264MbStorage *mb,const MvdH264SliceHeader *slice)` |
| `0x116994` | `h264bsdDecodeNalUnit` | `u32 h264bsdDecodeNalUnit(strmData_t *stream,MvdH264Nal *nal)` |
| `0x116A5E` | `h264bsdDecodePicOrderCnt` | `void h264bsdDecodePicOrderCnt(MvdH264Poc *poc,const MvdH264Sps *sps,const MvdH264SliceHeader *sliceHeader,const MvdH264Nal *nalUnit)` |
| `0x116C68` | `h264bsdDecodePicParamSet` | `u32 h264bsdDecodePicParamSet(strmData_t *stream,MvdH264Pps *pps)` |
| `0x116F78` | `h264bsdDecodeSeqParamSet` | `u32 h264bsdDecodeSeqParamSet(strmData_t *stream, MvdH264Sps *sps, u32 mvcFlag)` |
| `0x117338` | `h264bsdDecodeSliceData` | `u32 h264bsdDecodeSliceData(MvdH264Container *decoder,strmData_t *stream,MvdH264SliceHeader *slice)` |
| `0x1174B0` | `h264bsdDecodeSliceGroupMap` | `void h264bsdDecodeSliceGroupMap(u32 *map,MvdH264Pps *pps,u32 changeCycle,u32 widthInMbs,u32 heightInMbs)` |
| `0x117610` | `h264bsdDecodeSliceHeader` | `u32 h264bsdDecodeSliceHeader(strmData_t *stream,MvdH264SliceHeader *slice,MvdH264Sps *sps,MvdH264Pps *pps,MvdH264Nal *nal)` |
| `0x117A94` | `h264bsdDecodeVuiParameters` | `u32 h264bsdDecodeVuiParameters(strmData_t *stream,vuiParameters_t *vui)` |
| `0x117DC4` | `h264bsdDpbOutputPicture` | `MvdH264DpbOutPicture *h264bsdDpbOutputPicture(MvdH264Dpb *dpb)` |
| `0x117E0E` | `h264bsdExtractNalUnit` | `u32 h264bsdExtractNalUnit(u8 *byteStream,u32 length,strmData_t *stream,u32 *readBytes,u32 rlcMode)` |
| `0x117F4E` | `MvdH264PatchFrameNumBit12` | `u32 MvdH264PatchFrameNumBit12(u8 *stream,u32 length,u32 frameNum,u32 maxFrameNum,u32 *initialStartCodeBytes)` |
| `0x118030` | `h264bsdFlushBuffer` | `void h264bsdFlushBuffer(MvdH264Storage *storage)` |
| `0x11804C` | `h264bsdInit` | `void h264bsdInit(MvdH264Storage *storage, u32 noReordering, u32 displaySmoothing)` |
| `0x118084` | `h264bsdInitDpb` | `u32 h264bsdInitDpb(const MvdDwlInstance *dwl,MvdH264Dpb *dpb,u32 picSizeInMbs,u32 dpbSize,u32 maxRefFrames,u32 maxFrameNum,u32 noReordering,u32 displaySmoothing,u32 monochrome,u32 highSupported,u32 enableSecondChroma,u32 multiBufferPp)` |
| `0x118204` | `h264bsdInitMbNeighbours` | `void h264bsdInitMbNeighbours(MvdH264MbStorage *macroblocks,u32 picWidth,u32 picSize)` |
| `0x1182A8` | `h264bsdInitStorage` | `void h264bsdInitStorage(MvdH264Storage *storage)` |
| `0x1182E0` | `h264bsdIsByteAligned` | `u32 h264bsdIsByteAligned(const strmData_t *stream)` |
| `0x1182EE` | `h264bsdIsEndOfPicture` | `u32 h264bsdIsEndOfPicture(MvdH264Storage *storage)` |
| `0x11833E` | `h264bsdIsMonoChrome` | `u32 h264bsdIsMonoChrome(MvdH264Storage *storage)` |
| `0x118348` | `h264bsdIsOppositeFieldPic` | `u32 h264bsdIsOppositeFieldPic(MvdH264SliceHeader *current,MvdH264SliceHeader *previous,u32 *secondField,u32 prevRefFrameNum,u32 newPicture)` |
| `0x118394` | `h264bsdMarkDecRefPic` | `u32 h264bsdMarkDecRefPic(MvdH264Dpb *dpb,const MvdH264RefMarking *mark,const MvdH264Image *image,u32 frameNum,const s32 *picOrderCnt,u32 isIdr,u32 currentPicId,u32 numErrMbs,u32 tiledMode)` |
| `0x1186DC` | `h264bsdMarkSliceCorrupted` | `void h264bsdMarkSliceCorrupted(MvdH264Storage *storage,u32 firstMbInSlice)` |
| `0x118750` | `h264bsdMatrixCoefficients` | `u32 h264bsdMatrixCoefficients(const MvdH264Storage *storage)` |
| `0x118772` | `h264bsdModifyScalingLists` | `void h264bsdModifyScalingLists(MvdH264Storage *storage,MvdH264Pps *pps)` |
| `0x118838` | `h264bsdNeighbour4x4BlockD` | `const MvdH264Neighbour *h264bsdNeighbour4x4BlockD(u32 blockIndex)` |
| `0x118844` | `h264bsdNumSubMbPart` | `u32 h264bsdNumSubMbPart(u8 subMbType)` |
| `0x11885C` | `h264bsdPredModeIntra16x16` | `u32 h264bsdPredModeIntra16x16(u8 mbType)` |
| `0x118864` | `h264bsdReorderRefPicList` | `u32 h264bsdReorderRefPicList(MvdH264Dpb *dpb,MvdH264RefReordering *order,u32 currentFrameNum,u32 activeRefCount)` |
| `0x11894C` | `h264bsdResetDpb` | `u32 h264bsdResetDpb(const MvdDwlInstance *dwl,MvdH264Dpb *dpb,u32 picSizeInMbs,u32 dpbSize,u32 maxRefFrames,u32 maxFrameNum,u32 noReordering,u32 displaySmoothing,u32 monochrome,u32 highSupported,u32 enableSecondChroma,u32 multiBufferPp)` |
| `0x1189C0` | `h264bsdSarSize` | `void h264bsdSarSize(const MvdH264Storage *storage,u32 *sarWidth,u32 *sarHeight)` |
| `0x1189EE` | `h264bsdShutdown` | `void h264bsdShutdown(MvdH264Storage *storage)` |
| `0x118AB4` | `h264bsdStorePicParamSet` | `u32 h264bsdStorePicParamSet(MvdH264Storage *storage,MvdH264Pps *pps)` |
| `0x118B50` | `h264bsdStoreSeqParamSet` | `u32 h264bsdStoreSeqParamSet(MvdH264Storage *storage,MvdH264Sps *sps)` |
| `0x118C3C` | `h264bsdValidParamSets` | `u32 h264bsdValidParamSets(MvdH264Storage *storage)` |
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
| `0x118EFE` | `vp8hwdBoolStart` | `void vp8hwdBoolStart(MvdBoolCoder *bc,const u8 *stream,u32 length)` |
| `0x118F3A` | `vp8hwdDecodeBool` | `u32 vp8hwdDecodeBool(MvdBoolCoder *bc,s32 probability)` |
| `0x118F8E` | `vp8hwdDecodeBool128` | `u32 vp8hwdDecodeBool128(MvdBoolCoder *bc)` |
| `0x118FE0` | `vp8hwdDecodeCoeffUpdate` | `u32 vp8hwdDecodeCoeffUpdate(MvdBoolCoder *bc,MvdVp8Decoder *decoder)` |
| `0x11906C` | `vp8hwdDecodeFrameHeader` | `u32 vp8hwdDecodeFrameHeader(const u8 *stream,u32 streamLen,MvdBoolCoder *bc,MvdVp8Decoder *decoder)` |
| `0x119094` | `vp8hwdDecodeFrameTag` | `void vp8hwdDecodeFrameTag(const u8 *stream,MvdVp8Decoder *decoder)` |
| `0x1190E8` | `vp8hwdDecodeMvUpdate` | `u32 vp8hwdDecodeMvUpdate(MvdBoolCoder *bc,MvdVp8Decoder *decoder)` |
| `0x119150` | `MvdVp8ConcealMotionVectors` | `void MvdVp8ConcealMotionVectors(MvdVp8EcState *state,const u32 *previousMvs,u32 *currentMvs,u32 startMb,u32 extrapolate)` |
| `0x1193E4` | `vp8hwdFreeze` | `void vp8hwdFreeze(MvdVp8Container *decoder)` |
| `0x1194A0` | `MvdVp8InitConcealment` | `u32 MvdVp8InitConcealment(MvdVp8EcState *state,u32 width,u32 height,u32 vectorsPerMb)` |
| `0x1194C8` | `vp8hwdPreparePpRun` | `void vp8hwdPreparePpRun(MvdVp8Container *decoder)` |
| `0x119530` | `vp8hwdPrepareVp7Scan` | `void vp8hwdPrepareVp7Scan(MvdVp8Decoder *decoder,const u32 *newOrder)` |
| `0x119568` | `MvdVp8ReleaseConcealment` | `void MvdVp8ReleaseConcealment(MvdVp8EcState *state)` |
| `0x119578` | `vp8hwdResetDecoder` | `void vp8hwdResetDecoder(MvdVp8Decoder *decoder,MvdVp8AsicBuffers *buffers)` |
| `0x1195C0` | `vp8hwdResetProbs` | `void vp8hwdResetProbs(MvdVp8Decoder *decoder)` |
| `0x1196B0` | `vp8hwdSetPartitionOffsets` | `u32 vp8hwdSetPartitionOffsets(const u8 *stream,u32 length,MvdVp8Decoder *decoder)` |
| `0x119742` | `vp8hwdUpdateOutBase` | `void vp8hwdUpdateOutBase(MvdVp8Container *decoder)` |
| `0x1197B8` | `MvdBeginAuxiliaryServiceTermination` | `u8 *MvdBeginAuxiliaryServiceTermination(void)` |
| `0x1197F0` | `MvdIpcWriteVp6Picture` | `void MvdIpcWriteVp6Picture(u32 **commandBuffer, u32 wordIndex, const MvdVp6Picture *picture)` |
| `0x119804` | `MvdIpcWriteVp8Picture` | `void MvdIpcWriteVp8Picture(u32 **commandBuffer, u32 wordIndex, const MvdVp8Picture *picture)` |
| `0x119818` | `MvdIpcWriteH264Picture` | `void MvdIpcWriteH264Picture(u32 **commandBuffer, u32 wordIndex, const MvdH264Picture *picture)` |
| `0x11982C` | `MvdIpcWrite16Bytes` | `void * MvdIpcWrite16Bytes(u32 **buffer, u32 wordIndex, const void *source)` |
| `0x119AAC` | `svcStopDma` | `Result svcStopDma(Handle dma)` |
| `0x119CC0` | `svcClearEvent` | `Result svcClearEvent(Handle event)` |
| `0x119CE0` | `svcGetDmaState` | `Result svcGetDmaState(u32 *state, Handle dma)` |
| `0x119CF8` | `svcStartInterProcessDma` | `Result svcStartInterProcessDma(Handle *dma, Handle dstProcess, u32 dstAddress, Handle srcProcess, u32 srcAddress, u32 size, const MvdDmaConfig *config)` |
| `0x119D20` | `svcWaitSynchronization` | `Result svcWaitSynchronization(Handle handle,s64 timeoutNs)` |
| `0x119D78` | `MvdMaxDpbFramesForLevel` | `u32 MvdMaxDpbFramesForLevel(u32 height, u32 width, u32 levelIndex)` |
| `0x119DDC` | `ceilf` | `float __usercall ceilf@<s0>(float value@<s0>)` |
| `0x119E6C` | `floorf` | `float __usercall floorf@<s0>(float value@<s0>)` |
