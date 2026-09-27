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
| `0x53c` | `unresolvedScalingFields` | `u32[12]` | 48 |
| `0x56c` | `hwId` | `u32` | 4 |
| `0x570` | `hwEndianVer` | `u32` | 4 |
| `0x574` | `tiledModeSupport` | `u32` | 4 |
| `0x578` | `remainingLast` | `u32` | 4 |

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
| `0x58` | `unresolvedSynth2Bits12_13` | `u32` | 4 |
| `0x5c` | `unresolvedSynth2Bit11` | `u32` | 4 |
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


