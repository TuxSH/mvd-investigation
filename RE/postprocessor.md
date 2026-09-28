# Postprocessor configuration

## Identity and complete field map

`MVDSTD_Config` is Hantro `PPConfig`, exactly `0x11C` bytes. Evidence: field accesses in `PPCheckConfig` (`0x10635C`), copy size in `PPGetConfig` (`0x1074FC`), default values in `PPInitDataStructures` (`0x107720`), and register setup in `PPSetupHW` (`0x108540`). This also explains matching pixel-format constants in `source/inc/ppapi.h`.

All fields are four bytes; signed fields matter for brightness, contrast, saturation, mask coordinates and framebuffer placement. Bus addresses are not ordinary process pointers.

| Offset | Hantro field | Type |
|---|---|---|
| `0x000` | `ppInImg.pixFormat` | `u32` |
| `0x004` | `ppInImg.picStruct` | `u32` |
| `0x008` | `ppInImg.videoRange` | `u32` |
| `0x00C` | `ppInImg.width` | `u32` |
| `0x010` | `ppInImg.height` | `u32` |
| `0x014` | `ppInImg.bufferBusAddr` | `u32` |
| `0x018` | `ppInImg.bufferCbBusAddr` | `u32` |
| `0x01C` | `ppInImg.bufferCrBusAddr` | `u32` |
| `0x020` | `ppInImg.bufferBusAddrBot` | `u32` |
| `0x024` | `ppInImg.bufferBusAddrChBot` | `u32` |
| `0x028` | `ppInImg.vc1MultiResEnable` | `u32` |
| `0x02C` | `ppInImg.vc1RangeRedFrm` | `u32` |
| `0x030` | `ppInImg.vc1RangeMapYEnable` | `u32` |
| `0x034` | `ppInImg.vc1RangeMapYCoeff` | `u32` |
| `0x038` | `ppInImg.vc1RangeMapCEnable` | `u32` |
| `0x03C` | `ppInImg.vc1RangeMapCCoeff` | `u32` |
| `0x040` | `ppInCrop.enable` | `u32` |
| `0x044` | `ppInCrop.originX` | `u32` |
| `0x048` | `ppInCrop.originY` | `u32` |
| `0x04C` | `ppInCrop.height` | `u32` |
| `0x050` | `ppInCrop.width` | `u32` |
| `0x054` | `ppInRotation.rotation` | `u32` |
| `0x058` | `ppOutImg.pixFormat` | `u32` |
| `0x05C` | `ppOutImg.width` | `u32` |
| `0x060` | `ppOutImg.height` | `u32` |
| `0x064` | `ppOutImg.bufferBusAddr` | `u32` |
| `0x068` | `ppOutImg.bufferChromaBusAddr` | `u32` |
| `0x06C` | `ppOutRgb.rgbTransform` | `u32` |
| `0x070` | `ppOutRgb.contrast` | `i32` |
| `0x074` | `ppOutRgb.brightness` | `i32` |
| `0x078` | `ppOutRgb.saturation` | `i32` |
| `0x07C` | `ppOutRgb.alpha` | `u32` |
| `0x080` | `ppOutRgb.transparency` | `u32` |
| `0x084` | `ppOutRgb.rgbTransformCoeffs.a` | `u32` |
| `0x088` | `ppOutRgb.rgbTransformCoeffs.b` | `u32` |
| `0x08C` | `ppOutRgb.rgbTransformCoeffs.c` | `u32` |
| `0x090` | `ppOutRgb.rgbTransformCoeffs.d` | `u32` |
| `0x094` | `ppOutRgb.rgbTransformCoeffs.e` | `u32` |
| `0x098` | `ppOutRgb.rgbBitmask.maskR` | `u32` |
| `0x09C` | `ppOutRgb.rgbBitmask.maskG` | `u32` |
| `0x0A0` | `ppOutRgb.rgbBitmask.maskB` | `u32` |
| `0x0A4` | `ppOutRgb.rgbBitmask.maskAlpha` | `u32` |
| `0x0A8` | `ppOutRgb.ditheringEnable` | `u32` |
| `0x0AC` | `ppOutMask1.enable` | `u32` |
| `0x0B0` | `ppOutMask1.originX` | `i32` |
| `0x0B4` | `ppOutMask1.originY` | `i32` |
| `0x0B8` | `ppOutMask1.height` | `u32` |
| `0x0BC` | `ppOutMask1.width` | `u32` |
| `0x0C0` | `ppOutMask1.alphaBlendEna` | `u32` |
| `0x0C4` | `ppOutMask1.blendComponentBase` | `u32` |
| `0x0C8` | `ppOutMask1.blendOriginX` | `i32` |
| `0x0CC` | `ppOutMask1.blendOriginY` | `i32` |
| `0x0D0` | `ppOutMask1.blendWidth` | `u32` |
| `0x0D4` | `ppOutMask1.blendHeight` | `u32` |
| `0x0D8` | `ppOutMask2.enable` | `u32` |
| `0x0DC` | `ppOutMask2.originX` | `i32` |
| `0x0E0` | `ppOutMask2.originY` | `i32` |
| `0x0E4` | `ppOutMask2.height` | `u32` |
| `0x0E8` | `ppOutMask2.width` | `u32` |
| `0x0EC` | `ppOutMask2.alphaBlendEna` | `u32` |
| `0x0F0` | `ppOutMask2.blendComponentBase` | `u32` |
| `0x0F4` | `ppOutMask2.blendOriginX` | `i32` |
| `0x0F8` | `ppOutMask2.blendOriginY` | `i32` |
| `0x0FC` | `ppOutMask2.blendWidth` | `u32` |
| `0x100` | `ppOutMask2.blendHeight` | `u32` |
| `0x104` | `ppOutFrmBuffer.enable` | `u32` |
| `0x108` | `ppOutFrmBuffer.writeOriginX` | `i32` |
| `0x10C` | `ppOutFrmBuffer.writeOriginY` | `i32` |
| `0x110` | `ppOutFrmBuffer.frameBufferWidth` | `u32` |
| `0x114` | `ppOutFrmBuffer.frameBufferHeight` | `u32` |
| `0x118` | `ppOutDeinterlace.enable` | `u32` |

## Input meaning

`picStruct`: 0 = frame or top field, 1 = bottom field, 2 = separate top and bottom fields, 3 = top/bottom fields stored as a frame, 4 = top field in a frame, 5 = bottom field in a frame. The extra addresses at `0x18..0x24` are chroma planes and bottom-field planes; they are not unexplained extra images.

`videoRange`: 0 = limited/studio range; 1 = full range. Values above one fail validation. The six words at `0x28..0x3C` are VC-1 range/multiresolution fields inherited from the common PP API; their presence does not mean a VC-1 decoder is compiled into MVD.

The input pixel format describes the already-decoded image entering PP. In combined mode, the H.264 decoder's natural format is semiplanar 4:2:0 or tiled 4:2:0, or monochrome for a monochrome sequence.

## Pixel-format values

| Value | Hantro name / layout |
|---|---|
| `0x010001` | YCbCr 4:2:2 interleaved: Y Cb Y Cr (YUYV) |
| `0x010005` | Y Cr Y Cb (YVYU) |
| `0x010006` | Cb Y Cr Y (UYVY) |
| `0x010007` | Cr Y Cb Y (VYUY) |
| `0x010002` | YCbCr 4:2:2 semiplanar |
| `0x010004` | YCbCr 4:4:0 |
| `0x010008..0x01000B` | The corresponding four 4:2:2 interleaved orders, tiled 4×4 |
| `0x020000` | YCbCr 4:2:0 planar |
| `0x020001` | YCbCr 4:2:0 semiplanar: Y plane plus interleaved CbCr |
| `0x020002` | YCbCr 4:2:0 tiled |
| `0x080000` | YCbCr 4:0:0 (monochrome) |
| `0x100001` | YCbCr 4:1:1 semiplanar |
| `0x200001` | YCbCr 4:4:4 semiplanar |
| `0x040000` | Custom 16-bit RGB masks |
| `0x040001` | RGB 5:5:5 |
| `0x040002` | RGB 5:6:5 |
| `0x040003` | BGR 5:5:5 |
| `0x040004` | BGR 5:6:5 |
| `0x041000` | Custom 32-bit RGB masks |
| `0x041001` | RGB32 |
| `0x041002` | BGR32 |

These are **library layout names**, not an assertion about byte order seen by a 3DS display framebuffer. libctru names `0x040002` BGR565 and `0x040004` RGB565, opposite the Hantro bit-layout names. Preserve that distinction when interoperating with libctru. Output-endian and swap register settings also affect memory interpretation.

The format list is not an all-modes support matrix. The input/output predicates and hardware feature bits restrict legal choices. In particular `0x020001` uses two output planes at `0x64` and `0x68`; these are luma and chroma, not two independent output pictures. Custom RGB formats need valid channel masks. Tiled output depends on PP capability and product checks.

The matched `PPIsInPixFmtOk` at `0x107860` makes the canonical-format restrictions more precise:

* 4:2:0 semiplanar is accepted in both standalone and linked modes; tiled 4:2:0 excludes the JPEG-linked case
* 4:2:0 planar and YUYV are standalone-only inputs; the other three interleaved 4:2:2 orders additionally exclude product `0x8170`
* Monochrome is linked H.264/JPEG only, and H.264 monochrome excludes `0x8170`
* 4:2:2 semiplanar, 4:4:0, 4:1:1 and 4:4:4 are JPEG-linked only in the common predicate. Since this build has no JPEG attachment case, those are not ordinary reachable combined configurations through `1B`

`PPIsOutPixFmtOk` at `0x107918` accepts semiplanar 4:2:0, YUYV and the listed custom/fixed RGB formats without a product restriction in this predicate. Other interleaved 4:2:2 orders exclude `0x8170`; tiled 4×4 output additionally requires the tiled-output capability. Geometry, addresses and other validation still apply. The source's additional YCrCb semiplanar cases are absent from these observed branches.

## Crop, orientation, RGB and masks

Cropping uses input-space X/Y, height and width. X/Y must be multiples of 16; crop dimensions must be multiples of eight. Minimum width is 16 standalone or 48 combined; minimum height is 16. The observed validation checks origins and dimensions individually against the input dimensions; it does not simply enforce `origin + extent <= input extent` in this initial crop block.

Rotation at `0x54`: 0 none, 1 right 90°, 2 left 90°, 3 horizontal flip, 4 vertical flip, 5 180°. Combined mode rejects nonzero rotation for certain subsampled input layouts (`0x010004`, `0x010002`, `0x100001`, `0x200001`); additional JPEG-specific compatibility checks survive in common PP code despite the missing JPEG decoder attachment case.

RGB transform at `0x6C`: 0 custom coefficients, 1 BT.601, 2 BT.709. The signed adjustments are contrast (`-64..64`), brightness (`-128..127`) and saturation (`-64..128`). Alpha is at `0x7C`, transparency at `0x80`; their applicable constraints depend on 16/32-bit output. Dithering is `0xA8`, gated by hardware support.

| Transform / input range | a | b | c | d | e |
|---|---:|---:|---:|---:|---:|
| BT.601 limited | 298 | 409 | 208 | 100 | 516 |
| BT.601 full | 256 | 350 | 179 | 86 | 443 |
| BT.709 limited | 298 | 459 | 137 | 55 | 544 |
| BT.709 full | 256 | 403 | 120 | 48 | 475 |

`PPSetConfig` installs these values into its internal configuration for transforms 1/2. The four channel masks at `0x98..0xA4` are used for custom RGB. Overlapping masks, unsuitable runs of bits, and masks exceeding 16 bits for custom RGB16 fail validation.

Mask 1 (`0xAC..0xD4`) and mask 2 (`0xD8..0x100`) each describe enable, signed output-space X/Y, height, width, alpha-blend enable, blend-source bus address, and optional blend-source crop X/Y/width/height. An enabled alpha-blend source must have a nonzero eight-byte-aligned address. Mask geometry and blending/cropping support have additional checks in `0x106088` and `0x106008`.

The framebuffer block at `0x104..0x114` supplies signed placement coordinates and framebuffer dimensions. Negative coordinates are possible: validation checks whether any output overlaps the framebuffer. Semiplanar output imposes even vertical placement and framebuffer height. Framebuffer width is limited to 4096 in this code. Deinterlacing is `0x118`, requires capability support and an accepted 4:2:0 or monochrome input format.

## Dimensions and scaling

The often quoted limits of 2048 standalone / 4672 combined belong to the **legacy `hwId == 0x8170` branch**, not all G1-compatible products.

For other products, the standalone input maximum is 4096 per dimension. The usual combined maximum is `511 * 16`; WebP and a special JPEG path use `1024 * 16`. Minimum input width is 16 standalone / 48 combined; minimum input height is 16 for either. Alignment is 16 pixels standalone and eight combined. H.264 decoding has its own tighter capability/SPS checks; passing PP validation does not establish decoder support for that size.

Output dimensions must be at least 16 and fit the initialized PP output maxima. Output width comes from the capability reader; the output-height limit is 4096. Scaling must be supported. After crop and rotation, ordinary upscaling is bounded by three times input width and `3 * inputHeight - 2`; downscaling is bounded by a factor of 70. Mixed horizontal upscaling with vertical downscaling, or vice versa, fails this validation path. VC-1 multiresolution and tiled-output paths add restrictions.

Required image-plane addresses are nonzero and eight-byte aligned. In combined mode PP takes decoder input addresses through callbacks, so standalone input-address checks are skipped. Output-address validation still applies.

## Service-layer caveat

Although PP knows these layouts, the wrapper at `0x112D24` only assigns its cache-maintenance length for output formats `0x010001`, `0x040002`, and `0x041002`. Other accepted PP formats reach that call with an uninitialized size register. This is a static implementation defect, not evidence that every format listed above was tested on hardware. See [memory-and-results.md](memory-and-results.md).

## Internal scaling and capability continuation

The 12 previously opaque scaling words and the final WebP-support word in `MvdPpContainer` are now named; see [database-layouts.md](database-layouts.md). `PPSetupScaling` is at `0x10CFEC`, framebuffer writing/clipping at `0x107D5C`, RGB transform coefficients at `0x108234`, RGB masks at `0x107F3C`/`0x108194`, and dithering at `0x107CB8`. The supplied register reference selects fast-scaling support mode 1 and reports a 1920-pixel PP width limit, with the software height limit of 4096; see [hardware.md](hardware.md) for the conditional feature matrix and chip-revision workaround. Shared decoder-to-PP fields include the additional luma/chroma strides documented in [decoder-internals.md](decoder-internals.md).
