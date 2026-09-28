# Internal layouts applied in IDA

These are recovered 32-bit layouts for this database. Public configuration and IPC output layouts are documented in [postprocessor.md](postprocessor.md) and [mvd-std.md](mvd-std.md). Reserved or unresolved names deliberately retain uncertainty. `PPConfig` is 284 bytes; `PPOutputBuffers` is a count plus 17 address pairs, 140 bytes. `MvdPpBufferData` is five u32 words: top luma/chroma bus addresses, bottom luma/chroma bus addresses, setup ID.

## MvdPpContainer (1404 bytes)

| Offset | Member | Type | Bytes |
|---|---|---|---:|
| `0x0` | `ppRegs` | `u32[41]` | 164 |
| `0xa4` | `ppCfg` | `PPConfig` | 284 |
| `0x1c0` | `prevCfg` | `PPConfig` | 284 |
| `0x2dc` | `combinedModeBuffers` | `PPOutputBuffers` | 140 |
| `0x368` | `bufferData` | `MvdPpBufferData[17]` | 340 |
| `0x4bc` | `displayIndex` | `u32` | 4 |
| `0x4c0` | `currentSetupID` | `u32` | 4 |
| `0x4c4` | `prevOutSetupID` | `u32` | 4 |
| `0x4c8` | `combinedResult` | `s16` | 2 |
| `0x4ca` | `resultPadding` | `u16` | 2 |
| `0x4cc` | `status` | `u32` | 4 |
| `0x4d0` | `pipeline` | `u32` | 4 |
| `0x4d4` | `multiBuffer` | `u32` | 4 |
| `0x4d8` | `dwl` | `void *` | 4 |
| `0x4dc` | `decInst` | `void *` | 4 |
| `0x4e0` | `decType` | `u32` | 4 |
| `0x4e4` | `frmBufferLumaOrRgbOffset` | `s32` | 4 |
| `0x4e8` | `frmBufferChromaOffset` | `s32` | 4 |
| `0x4ec` | `outFormat` | `u32` | 4 |
| `0x4f0` | `outStartCh` | `u32` | 4 |
| `0x4f4` | `outCrFirst` | `u32` | 4 |
| `0x4f8` | `outTiled4x4` | `u32` | 4 |
| `0x4fc` | `inFormat` | `u32` | 4 |
| `0x500` | `inStartCh` | `u32` | 4 |
| `0x504` | `inCrFirst` | `u32` | 4 |
| `0x508` | `rgbDepth` | `u32` | 4 |
| `0x50c` | `inWidth` | `u32` | 4 |
| `0x510` | `inHeight` | `u32` | 4 |
| `0x514` | `altRegs` | `u32` | 4 |
| `0x518` | `maxOutWidth` | `u32` | 4 |
| `0x51c` | `maxOutHeight` | `u32` | 4 |
| `0x520` | `blendEna` | `u32` | 4 |
| `0x524` | `deintEna` | `u32` | 4 |
| `0x528` | `ditherEna` | `u32` | 4 |
| `0x52c` | `scalingEna` | `u32` | 4 |
| `0x530` | `tiledEna` | `u32` | 4 |
| `0x534` | `pixAccSupport` | `u32` | 4 |
| `0x538` | `blendCropSupport` | `u32` | 4 |
| `0x53c` | `fastScalingSupport` | `u32` | 4 |
| `0x540` | `fastVerticalDownscale` | `u32` | 4 |
| `0x544` | `fastHorizontalDownscale` | `u32` | 4 |
| `0x548` | `fastVerticalDownscaleDisable` | `u32` | 4 |
| `0x54c` | `fastHorizontalDownscaleDisable` | `u32` | 4 |
| `0x550` | `cVnorm` | `u32` | 4 |
| `0x554` | `cVfast` | `u32` | 4 |
| `0x558` | `cHnorm` | `u32` | 4 |
| `0x55c` | `cHfast` | `u32` | 4 |
| `0x560` | `cHfast4x` | `u32` | 4 |
| `0x564` | `horizontalRoundingWorkaround` | `u32` | 4 |
| `0x568` | `fastScaleMode` | `u32` | 4 |
| `0x56c` | `hwId` | `u32` | 4 |
| `0x570` | `hwEndianVer` | `u32` | 4 |
| `0x574` | `tiledModeSupport` | `u32` | 4 |
| `0x578` | `webpSupport` | `u32` | 4 |
## MvdDwlHwConfig (100 bytes)

| Offset | Member | Type | Bytes |
|---|---|---|---:|
| `0x0` | `mpeg4Support` | `u32` | 4 |
| `0x4` | `customMpeg4Support` | `u32` | 4 |
| `0x8` | `h264Support` | `u32` | 4 |
| `0xc` | `vc1Support` | `u32` | 4 |
| `0x10` | `mpeg2Support` | `u32` | 4 |
| `0x14` | `jpegSupport` | `u32` | 4 |
| `0x18` | `unresolvedWord6` | `u32` | 4 |
| `0x1c` | `maxDecPicWidth` | `u32` | 4 |
| `0x20` | `ppSupport` | `u32` | 4 |
| `0x24` | `ppConfig` | `u32` | 4 |
| `0x28` | `maxPpOutPicWidth` | `u32` | 4 |
| `0x2c` | `sorensonSparkSupport` | `u32` | 4 |
| `0x30` | `refBufSupport` | `u32` | 4 |
| `0x34` | `tiledModeSupport` | `u32` | 4 |
| `0x38` | `vp6Support` | `u32` | 4 |
| `0x3c` | `vp7Support` | `u32` | 4 |
| `0x40` | `vp8Support` | `u32` | 4 |
| `0x44` | `avsSupport` | `u32` | 4 |
| `0x48` | `jpegESupport` | `u32` | 4 |
| `0x4c` | `rvSupport` | `u32` | 4 |
| `0x50` | `mvcSupport` | `u32` | 4 |
| `0x54` | `webpSupport` | `u32` | 4 |
| `0x58` | `hardwareEcSupport` | `u32` | 4 |
| `0x5c` | `strideSupport` | `u32` | 4 |
| `0x60` | `fieldDpbSupport` | `u32` | 4 |
## MvdSession (8 bytes)

| Offset | Member | Type | Bytes |
|---|---|---|---:|
| `0x0` | `decoder` | `void *` | 4 |
| `0x4` | `postprocessor` | `PPInst` | 4 |

## MvdWorkSizeParams (16 bytes)

| Offset | Member | Type | Bytes |
|---|---|---|---:|
| `0x0` | `width` | `u32` | 4 |
| `0x4` | `height` | `u32` | 4 |
| `0x8` | `levelEnable` | `u8` | 1 |
| `0x9` | `levelFlags` | `u8` | 1 |
| `0xa` | `doubleSize` | `u8` | 1 |
| `0xb` | `level` | `u8` | 1 |
| `0xc` | `refAEnable` | `u8` | 1 |
| `0xd` | `refACount` | `u8` | 1 |
| `0xe` | `refBEnable` | `u8` | 1 |
| `0xf` | `refBCount` | `u8` | 1 |

## MvdOutputMapping (16 bytes)

| Offset | Member | Type | Bytes |
|---|---|---|---:|
| `0x0` | `lumaBus` | `u32` | 4 |
| `0x4` | `chromaBus` | `u32` | 4 |
| `0x8` | `lumaClient` | `u32 *` | 4 |
| `0xc` | `chromaClient` | `u32 *` | 4 |




The continuation layouts below are described in [decoder-internals.md](decoder-internals.md). Opaque arrays preserve unresolved regions; see that document for limitations.

## MvdDwlFuseStatus (76 bytes)

| Offset | Member | Type | Bytes |
|---|---|---|---:|
| `0x0` | `h264Support` | `u32` | 4 |
| `0x4` | `mpeg4Support` | `u32` | 4 |
| `0x8` | `mpeg2Support` | `u32` | 4 |
| `0xc` | `sorensonSparkSupport` | `u32` | 4 |
| `0x10` | `jpegSupport` | `u32` | 4 |
| `0x14` | `vp6Support` | `u32` | 4 |
| `0x18` | `vp7Support` | `u32` | 4 |
| `0x1c` | `vp8Support` | `u32` | 4 |
| `0x20` | `vc1Support` | `u32` | 4 |
| `0x24` | `progressiveJpegSupport` | `u32` | 4 |
| `0x28` | `ppSupport` | `u32` | 4 |
| `0x2c` | `ppConfig` | `u32` | 4 |
| `0x30` | `maxDecPicWidth` | `u32` | 4 |
| `0x34` | `maxPpOutPicWidth` | `u32` | 4 |
| `0x38` | `refBufSupport` | `u32` | 4 |
| `0x3c` | `avsSupport` | `u32` | 4 |
| `0x40` | `rvSupport` | `u32` | 4 |
| `0x44` | `mvcSupport` | `u32` | 4 |
| `0x48` | `customMpeg4Support` | `u32` | 4 |

## MvdDecPpInterface (104 bytes)

| Offset | Member | Type | Bytes |
|---|---|---|---:|
| `0x0` | `ppStatus` | `u8` | 1 |
| `0x1` | `multiBufStat` | `u8` | 1 |
| `0x2` | `padding` | `u8[2]` | 2 |
| `0x4` | `inputBusLuma` | `u32` | 4 |
| `0x8` | `inputBusChroma` | `u32` | 4 |
| `0xc` | `bottomBusLuma` | `u32` | 4 |
| `0x10` | `bottomBusChroma` | `u32` | 4 |
| `0x14` | `picStruct` | `u32` | 4 |
| `0x18` | `topField` | `u32` | 4 |
| `0x1c` | `inwidth` | `u32` | 4 |
| `0x20` | `inheight` | `u32` | 4 |
| `0x24` | `usePipeline` | `u32` | 4 |
| `0x28` | `littleEndian` | `u32` | 4 |
| `0x2c` | `wordSwap` | `u32` | 4 |
| `0x30` | `croppedW` | `u32` | 4 |
| `0x34` | `croppedH` | `u32` | 4 |
| `0x38` | `bufferIndex` | `u32` | 4 |
| `0x3c` | `displayIndex` | `u32` | 4 |
| `0x40` | `prevAnchorDisplayIndex` | `u32` | 4 |
| `0x44` | `rangeRed` | `u32` | 4 |
| `0x48` | `rangeMapYEnable` | `u32` | 4 |
| `0x4c` | `rangeMapYCoeff` | `u32` | 4 |
| `0x50` | `rangeMapCEnable` | `u32` | 4 |
| `0x54` | `rangeMapCCoeff` | `u32` | 4 |
| `0x58` | `tiledInputMode` | `u32` | 4 |
| `0x5c` | `progressiveSequence` | `u32` | 4 |
| `0x60` | `lumaStride` | `u32` | 4 |
| `0x64` | `chromaStride` | `u32` | 4 |

## MvdH264Container (15860 bytes)

| Offset | Member | Type | Bytes |
|---|---|---|---:|
| `0x0` | `checksum` | `void *` | 4 |
| `0x4` | `decStat` | `u32` | 4 |
| `0x8` | `picNumber` | `u32` | 4 |
| `0xc` | `asicRunning` | `u32` | 4 |
| `0x10` | `rlcMode` | `u32` | 4 |
| `0x14` | `tryVlc` | `u32` | 4 |
| `0x18` | `reallocate` | `u32` | 4 |
| `0x1c` | `pHwStreamStart` | `const u8 *` | 4 |
| `0x20` | `hwStreamStartBus` | `u32` | 4 |
| `0x24` | `hwBitPos` | `u32` | 4 |
| `0x28` | `hwLength` | `u32` | 4 |
| `0x2c` | `streamPosUpdated` | `u32` | 4 |
| `0x30` | `nalStartCode` | `u32` | 4 |
| `0x34` | `modeChange` | `u32` | 4 |
| `0x38` | `gapsCheckedForThis` | `u32` | 4 |
| `0x3c` | `packetDecoded` | `u32` | 4 |
| `0x40` | `forceRlcMode` | `u32` | 4 |
| `0x44` | `h264Regs` | `u32[60]` | 240 |
| `0x134` | `storage` | `MvdH264Storage` | 14864 |
| `0x3b44` | `asicBuff` | `MvdH264AsicBuffers` | 176 |
| `0x3bf4` | `dwl` | `const void *` | 4 |
| `0x3bf8` | `refBufSupport` | `u32` | 4 |
| `0x3bfc` | `tiledModeSupport` | `u32` | 4 |
| `0x3c00` | `tiledReferenceEnable` | `u32` | 4 |
| `0x3c04` | `h264ProfileSupport` | `u32` | 4 |
| `0x3c08` | `is8190` | `u32` | 4 |
| `0x3c0c` | `maxDecPicWidth` | `u32` | 4 |
| `0x3c10` | `allowDpbFieldOrdering` | `u32` | 4 |
| `0x3c14` | `dpbMode` | `u32` | 4 |
| `0x3c18` | `refBufferCtrl` | `MvdRefBuffer` | 228 |
| `0x3cfc` | `keepHwReserved` | `u32` | 4 |
| `0x3d00` | `skipNonReference` | `u32` | 4 |
| `0x3d04` | `workaroundWords` | `u32[4]` | 16 |
| `0x3d14` | `pp` | `MvdH264Pp` | 224 |

## MvdVp6Container (2424 bytes)

| Offset | Member | Type | Bytes |
|---|---|---|---:|
| `0x0` | `checksum` | `void *` | 4 |
| `0x4` | `decStat` | `u32` | 4 |
| `0x8` | `picNumber` | `u32` | 4 |
| `0xc` | `asicRunning` | `u32` | 4 |
| `0x10` | `width` | `u32` | 4 |
| `0x14` | `height` | `u32` | 4 |
| `0x18` | `vp6Regs` | `u32[60]` | 240 |
| `0x108` | `asicBuff` | `MvdVp6AsicBuffers` | 260 |
| `0x20c` | `dwl` | `const void *` | 4 |
| `0x210` | `refBufSupport` | `u32` | 4 |
| `0x214` | `tiledModeSupport` | `u32` | 4 |
| `0x218` | `tiledReferenceEnable` | `u32` | 4 |
| `0x21c` | `refBufferCtrl` | `MvdRefBuffer` | 228 |
| `0x300` | `pb` | `MvdVp6Pb` | 1480 |
| `0x8c8` | `pp` | `MvdCodecPp` | 140 |
| `0x954` | `pictureBroken` | `u32` | 4 |
| `0x958` | `intraFreeze` | `u32` | 4 |
| `0x95c` | `refToOut` | `u32` | 4 |
| `0x960` | `outCount` | `u32` | 4 |
| `0x964` | `numBuffers` | `u32` | 4 |
| `0x968` | `bq` | `MvdBufferQueue` | 16 |

## MvdVp8Container (4224 bytes)

| Offset | Member | Type | Bytes |
|---|---|---|---:|
| `0x0` | `checksum` | `void *` | 4 |
| `0x4` | `decMode` | `u32` | 4 |
| `0x8` | `decStat` | `u32` | 4 |
| `0xc` | `picNumber` | `u32` | 4 |
| `0x10` | `asicRunning` | `u32` | 4 |
| `0x14` | `width` | `u32` | 4 |
| `0x18` | `height` | `u32` | 4 |
| `0x1c` | `vp8Regs` | `u32[60]` | 240 |
| `0x10c` | `asicBuff` | `MvdVp8AsicBuffers` | 812 |
| `0x438` | `dwl` | `const void *` | 4 |
| `0x43c` | `refBufSupport` | `u32` | 4 |
| `0x440` | `refBufferCtrl` | `MvdRefBuffer` | 228 |
| `0x524` | `decoder` | `MvdVp8Decoder` | 2612 |
| `0xf58` | `bc` | `MvdBoolCoder` | 36 |
| `0xf7c` | `pp` | `MvdCodecPp` | 140 |
| `0x1008` | `pictureBroken` | `u32` | 4 |
| `0x100c` | `intraFreeze` | `u32` | 4 |
| `0x1010` | `outCount` | `u32` | 4 |
| `0x1014` | `refToOut` | `u32` | 4 |
| `0x1018` | `pendingPicToPp` | `u32` | 4 |
| `0x101c` | `bq` | `MvdBufferQueue` | 16 |
| `0x102c` | `numBuffers` | `u32` | 4 |
| `0x1030` | `intraOnly` | `u32` | 4 |
| `0x1034` | `sliceConcealment` | `u32` | 4 |
| `0x1038` | `userMem` | `u32` | 4 |
| `0x103c` | `sliceHeight` | `u32` | 4 |
| `0x1040` | `totDecodedRows` | `u32` | 4 |
| `0x1044` | `outputRows` | `u32` | 4 |
| `0x1048` | `tiledModeSupport` | `u32` | 4 |
| `0x104c` | `tiledReferenceEnable` | `u32` | 4 |
| `0x1050` | `hardwareEcSupport` | `u32` | 4 |
| `0x1054` | `strideSupport` | `u32` | 4 |
| `0x1058` | `unresolvedTail` | `u8[40]` | 40 |

## MvdH264Dpb (1680 bytes)

| Offset | Member | Type | Bytes |
|---|---|---|---:|
| `0x0` | `buffer` | `MvdH264DpbPicture[17]` | 884 |
| `0x374` | `list` | `u32[17]` | 68 |
| `0x3b8` | `currentOut` | `MvdH264DpbPicture *` | 4 |
| `0x3bc` | `currentOutPos` | `u32` | 4 |
| `0x3c0` | `outBuf` | `MvdH264DpbOutPicture *` | 4 |
| `0x3c4` | `numOut` | `u32` | 4 |
| `0x3c8` | `outIndexW` | `u32` | 4 |
| `0x3cc` | `outIndexR` | `u32` | 4 |
| `0x3d0` | `maxRefFrames` | `u32` | 4 |
| `0x3d4` | `dpbSize` | `u32` | 4 |
| `0x3d8` | `maxFrameNum` | `u32` | 4 |
| `0x3dc` | `maxLongTermFrameIdx` | `u32` | 4 |
| `0x3e0` | `numRefFrames` | `u32` | 4 |
| `0x3e4` | `fullness` | `u32` | 4 |
| `0x3e8` | `prevRefFrameNum` | `u32` | 4 |
| `0x3ec` | `lastContainsMmco5` | `u32` | 4 |
| `0x3f0` | `noReordering` | `u32` | 4 |
| `0x3f4` | `flushed` | `u32` | 4 |
| `0x3f8` | `picSizeInMbs` | `u32` | 4 |
| `0x3fc` | `dirMvOffset` | `u32` | 4 |
| `0x400` | `poc` | `MvdLinearMem` | 12 |
| `0x40c` | `delayedOut` | `u32` | 4 |
| `0x410` | `delayedId` | `u32` | 4 |
| `0x414` | `interlaced` | `u32` | 4 |
| `0x418` | `ch2Offset` | `u32` | 4 |
| `0x41c` | `numFreeBuffers` | `u32` | 4 |
| `0x420` | `freeBuffers` | `u32[17]` | 68 |
| `0x464` | `memStat` | `u32[34]` | 136 |
| `0x4ec` | `totBuffers` | `u32` | 4 |
| `0x4f0` | `picBuffers` | `MvdLinearMem[34]` | 408 |
| `0x688` | `noOutput` | `u32` | 4 |
| `0x68c` | `prevOutIdx` | `u32` | 4 |

## MvdVp6Pb (1480 bytes)

| Offset | Member | Type | Bytes |
|---|---|---|---:|
| `0x0` | `strm` | `MvdVp6Stream` | 24 |
| `0x18` | `br` | `MvdBoolCoder` | 36 |
| `0x3c` | `br2` | `MvdBoolCoder` | 36 |
| `0x60` | `huff` | `void *` | 4 |
| `0x64` | `Vp3VersionNo` | `u8` | 1 |
| `0x65` | `VpProfile` | `u8` | 1 |
| `0x66` | `FrameType` | `u8` | 1 |
| `0x67` | `padding103` | `u8` | 1 |
| `0x68` | `VFragments` | `u32` | 4 |
| `0x6c` | `HFragments` | `u32` | 4 |
| `0x70` | `OutputWidth` | `u32` | 4 |
| `0x74` | `OutputHeight` | `u32` | 4 |
| `0x78` | `ScalingMode` | `u32` | 4 |
| `0x7c` | `PredictionFilterMode` | `u8` | 1 |
| `0x7d` | `PredictionFilterMvSizeThresh` | `u8` | 1 |
| `0x7e` | `padding126` | `u8[2]` | 2 |
| `0x80` | `PredictionFilterVarThresh` | `u32` | 4 |
| `0x84` | `PredictionFilterAlpha` | `u8` | 1 |
| `0x85` | `padding133` | `u8[3]` | 3 |
| `0x88` | `RefreshGoldenFrame` | `u32` | 4 |
| `0x8c` | `MultiStream` | `u32` | 4 |
| `0x90` | `Buff2Offset` | `u32` | 4 |
| `0x94` | `UseHuffman` | `u32` | 4 |
| `0x98` | `UseLoopFilter` | `u8` | 1 |
| `0x99` | `padding153` | `u8[3]` | 3 |
| `0x9c` | `DctQMask` | `u32` | 4 |
| `0xa0` | `MvSignProbs` | `u8[2]` | 2 |
| `0xa2` | `IsMvShortProb` | `u8[2]` | 2 |
| `0xa4` | `MvShortProbs` | `u8[2][7]` | 14 |
| `0xb2` | `MvSizeProbs` | `u8[2][8]` | 16 |
| `0xc2` | `probXmitted` | `u8[4][2][10]` | 80 |
| `0x112` | `probModeSame` | `u8[4][10]` | 40 |
| `0x13a` | `probMode` | `u8[4][10][9]` | 360 |
| `0x2a2` | `DcProbs` | `u8[22]` | 22 |
| `0x2b8` | `AcProbs` | `u8[396]` | 396 |
| `0x444` | `DcNodeContexts` | `u8[30]` | 30 |
| `0x462` | `ZeroRunProbs` | `u8[2][14]` | 28 |
| `0x47e` | `ModifiedScanOrder` | `u8[64]` | 64 |
| `0x4be` | `MergedScanOrder` | `u8[129]` | 129 |
| `0x53f` | `EobOffsetTable` | `u8[64]` | 64 |
| `0x57f` | `ScanBands` | `u8[64]` | 64 |
| `0x5bf` | `probModeUpdate` | `u8` | 1 |
| `0x5c0` | `probMvUpdate` | `u8` | 1 |
| `0x5c1` | `scanUpdate` | `u8` | 1 |
| `0x5c2` | `probDcUpdate` | `u8` | 1 |
| `0x5c3` | `probAcUpdate` | `u8` | 1 |
| `0x5c4` | `probZrlUpdate` | `u8` | 1 |
| `0x5c5` | `padding1477` | `u8[3]` | 3 |

## MvdVp8AsicBuffers (812 bytes)

| Offset | Member | Type | Bytes |
|---|---|---|---:|
| `0x0` | `width` | `u32` | 4 |
| `0x4` | `height` | `u32` | 4 |
| `0x8` | `strideEnable` | `u32` | 4 |
| `0xc` | `userMem` | `u32` | 4 |
| `0x10` | `lumaStride` | `u32` | 4 |
| `0x14` | `chromaStride` | `u32` | 4 |
| `0x18` | `encodedLumaStride` | `u32` | 4 |
| `0x1c` | `encodedChromaStride` | `u32` | 4 |
| `0x20` | `lumaPlaneSize` | `u32` | 4 |
| `0x24` | `probTbl` | `MvdLinearMem` | 12 |
| `0x30` | `segmentMap` | `MvdLinearMem` | 12 |
| `0x3c` | `outBuffer` | `MvdLinearMem *` | 4 |
| `0x40` | `prevOutBuffer` | `MvdLinearMem *` | 4 |
| `0x44` | `refBuffer` | `MvdLinearMem *` | 4 |
| `0x48` | `goldenBuffer` | `MvdLinearMem *` | 4 |
| `0x4c` | `alternateBuffer` | `MvdLinearMem *` | 4 |
| `0x50` | `pictures` | `MvdLinearMem[16]` | 192 |
| `0x110` | `chromaPictures` | `MvdLinearMem[16]` | 192 |
| `0x1d0` | `mvs` | `MvdLinearMem[2]` | 24 |
| `0x1e8` | `currMvIndex` | `u32` | 4 |
| `0x1ec` | `prevMvIndex` | `u32` | 4 |
| `0x1f0` | `outBufferI` | `u32` | 4 |
| `0x1f4` | `refBufferI` | `u32` | 4 |
| `0x1f8` | `goldenBufferI` | `u32` | 4 |
| `0x1fc` | `alternateBufferI` | `u32` | 4 |
| `0x200` | `prevOutBufferI` | `u32` | 4 |
| `0x204` | `wholePicConcealed` | `u32` | 4 |
| `0x208` | `disableOutWriting` | `u32` | 4 |
| `0x20c` | `segmentMapSize` | `u32` | 4 |
| `0x210` | `partition1Base` | `u32` | 4 |
| `0x214` | `partition1BitOffset` | `u32` | 4 |
| `0x218` | `partition2Base` | `u32` | 4 |
| `0x21c` | `dcPred` | `s32[2]` | 8 |
| `0x224` | `dcMatch` | `s32[2]` | 8 |
| `0x22c` | `userLuma` | `u32 *[16]` | 64 |
| `0x26c` | `userLumaBus` | `u32[16]` | 64 |
| `0x2ac` | `userChroma` | `u32 *[16]` | 64 |
| `0x2ec` | `userChromaBus` | `u32[16]` | 64 |

