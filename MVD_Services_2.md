# MVD Services

MVD is a New 3DS system module for hardware video decoding and pixel-format conversion. The [New 3DS](https://www.3dbrew.org/wiki/New_3DS) [Internet Browser](https://www.3dbrew.org/wiki/Internet_Browser) (SKATER) uses `mvd:STD` to decode H.264 video and to convert decoded MJPEG frames from YUV to RGB.

MVD exposes four services. Each allows only one open session at a time.

| Service | Hardware | I/O registers (physical) | Interrupt |
|---|---|---|---|
| `mvd:STD` | Hantro G1 video decoder and its postprocessor | `0x10207000` | `0x4F` |
| `l2b:u` | L2B converter, engine 0 | `0x10130000` | `0x45` |
| `l2b2:u` | L2B converter, engine 1 | `0x10131000` | `0x46` |
| `y2r2:u` | Second Y2R converter | `0x10132000` | `0x4E` |

The decoder side of `mvd:STD` is Hantro's G1 decoder library (H.264, VP6, VP8 and postprocessor components), wrapped in Nintendo's IPC layer. As a result, much of the interface uses Hantro's own structures and constants: the configuration structure is Hantro's `PPConfig`, the pixel-format values are Hantro's `PP_PIX_FMT_*`, and the status values behind the result codes are Hantro's `*_OK`/`*_PIC_RDY`/… codes. See [writeup2.md](writeup2.md) for the full analysis.

Unless noted otherwise, the information below comes from static analysis of the MVD module, cross-checked against the Hantro source and against the observations previously documented on this page. Items that come only from observation of SKATER or from browser tests are labeled as such.

## Contents

* [mvd:STD](#mvdstd)
  * [Command list](#command-list)
  * [Usage](#usage)
  * [Command reference](#command-reference)
  * [Configuration structure](#configuration-structure)
  * [Pixel formats](#pixel-formats)
  * [Output structures](#output-structures)
  * [Work buffer size](#work-buffer-size)
* [Result codes](#result-codes)
* [l2b:u and l2b2:u](#l2bu-and-l2b2u)
* [y2r2:u](#y2r2u)
* [Supported codecs, H.264 levels and profiles](#supported-codecs-h264-levels-and-profiles)
* [Corrections to earlier documentation](#corrections-to-earlier-documentation)

## mvd:STD

The G1 register bank is at physical `0x10207000` (MVD maps it at `0x1ED07000`).

Linear-memory virtual addresses passed to this service must be in the `0x30000000` region. MVD's memory-copy code treats any address below `0x30000000` as its own memory, so the old `0x14000000` linear region does not work.

The session holds one decoder and one postprocessor. H.264, VP6 and VP8 share the decoder slot: only one of them can be initialized at a time.

### Command list

"SKATER" indicates whether the Internet Browser uses the command.

| Command header | Available since | Name | SKATER | Description |
|---|---|---|---|---|
| `0x00010082` | [8.1.0-0_New3DS](https://www.3dbrew.org/wiki/8.1.0-0_New3DS) | [Initialize](#initialize) | Yes | Set up the service with a work buffer |
| `0x00020000` | 8.1.0-0_New3DS | [Shutdown](#shutdown) | Yes | Tear down the service |
| `0x00030300` | 8.1.0-0_New3DS | [CalculateWorkBufSize](#calculateworkbufsize) | Yes | Compute a work-buffer size for H.264 |
| `0x000400C0` | 8.1.0-0_New3DS | [CalculateImageSize](#calculateimagesize) | No | Compute an image-buffer size |
| `0x00050100` | 8.1.0-0_New3DS | [H264Initialize](#h264initialize) | Yes | Create the H.264 decoder |
| `0x00060000` | 8.1.0-0_New3DS | [H264EnableMvc](#h264enablemvc) | No | Enable MVC (stereo) decoding; always fails |
| `0x00070000` | 8.1.0-0_New3DS | [H264Release](#h264release) | Yes | Destroy the H.264 decoder |
| `0x00080142` | 8.1.0-0_New3DS | [ProcessNALUnit](#processnalunit) (H264Decode) | Yes | Decode H.264 input |
| `0x00090042` | 8.1.0-0_New3DS | [ControlFrameRendering](#controlframerendering) (H264NextPicture) | Yes | Dequeue the next decoded picture |
| `0x000A0000` | 8.1.0-0_New3DS | [GetStatus](#getstatus) (H264GetInfo) | Yes | Get stream information |
| `0x000B0000` | 8.1.0-0_New3DS | [H264Peek](#h264peek) | No | Look at the next picture without dequeuing it |
| `0x000C0100` | 8.1.0-0_New3DS | [Vp8Initialize](#vp8initialize) | No | Create a VP7, VP8 or WebP decoder |
| `0x000D0000` | 8.1.0-0_New3DS | [Vp8Release](#vp8release) | No | Destroy the VP8-family decoder |
| `0x000E0202` | 8.1.0-0_New3DS | [Vp8Decode](#vp8decode) | No | Decode one VP8 frame |
| `0x000F0042` | 8.1.0-0_New3DS | [Vp8NextPicture](#vp8nextpicture) | No | Dequeue the next VP8 picture |
| `0x00100000` | 8.1.0-0_New3DS | [Vp8GetInfo](#vp8getinfo) | No | Get VP8 stream information |
| `0x00110000` | 8.1.0-0_New3DS | [Vp8Peek](#vp8peek) | No | Look at the next VP8 picture |
| `0x001200C0` | 8.1.0-0_New3DS | [Vp6Initialize](#vp6initialize) | No | Create a VP6 decoder |
| `0x00130000` | 8.1.0-0_New3DS | [Vp6Release](#vp6release) | No | Destroy the VP6 decoder |
| `0x001400C2` | 8.1.0-0_New3DS | [Vp6Decode](#vp6decode) | No | Decode one VP6 frame |
| `0x00150042` | 8.1.0-0_New3DS | [Vp6NextPicture](#vp6nextpicture) | No | Dequeue the next VP6 picture |
| `0x00160000` | 8.1.0-0_New3DS | [Vp6GetInfo](#vp6getinfo) | No | Get VP6 stream information |
| `0x00170000` | 8.1.0-0_New3DS | [Vp6Peek](#vp6peek) | No | Look at the next VP6 picture |
| `0x00180000` | 8.1.0-0_New3DS | [PpInitialize](#ppinitialize) | Yes | Create the postprocessor |
| `0x00190000` | 8.1.0-0_New3DS | [PpRelease](#pprelease) | Yes | Destroy the postprocessor |
| `0x001A0000` | 8.1.0-0_New3DS | [PpGetResult](#ppgetresult) | Yes | Run a standalone color conversion |
| `0x001B0040` | 8.1.0-0_New3DS | [PpEnableCombinedMode](#ppenablecombinedmode) | Yes | Attach the postprocessor to the decoder |
| `0x001C0000` | 8.1.0-0_New3DS | [PpDisableCombinedMode](#ppdisablecombinedmode) | Yes | Detach the postprocessor |
| `0x001D0042` | 8.1.0-0_New3DS | [GetConfig](#getconfig) | Yes | Read the postprocessor configuration |
| `0x001E0044` | 8.1.0-0_New3DS | [SetConfig](#setconfig) | Yes | Write the postprocessor configuration |
| `0x001F0902` | 8.1.0-0_New3DS | [SetupOutputBuffers](#setupoutputbuffers) | No | Register multiple output buffers |
| `0x00200002` | 8.1.0-0_New3DS | [GetNextOutput](#getnextoutput) | No | Get the output buffer holding the next picture |
| `0x00210100` | 8.1.0-0_New3DS | [OverrideOutputBuffers](#overrideoutputbuffers) | No | Replace the current output buffer |

The dispatcher uses only the low byte of the command number. Commands with handles or buffers validate their header and descriptors; most scalar-only commands do not check the header. An invalid descriptor or unknown command gets a reply with header `0x00000040` and an error result.

### Usage

**Color conversion** (SKATER's MJPEG path; no decoder needed):

1. [Initialize](#initialize) with a work-buffer size of 1 (the work buffer is not used).
2. [PpInitialize](#ppinitialize).
3. For each image: [GetConfig](#getconfig), fill in the configuration, [SetConfig](#setconfig), then [PpGetResult](#ppgetresult), which performs the conversion.
4. [PpRelease](#pprelease), then [Shutdown](#shutdown).

**H.264 video:**

1. If needed, [CalculateWorkBufSize](#calculateworkbufsize); then [Initialize](#initialize) with the work buffer.
2. [H264Initialize](#h264initialize), [PpInitialize](#ppinitialize), then [PpEnableCombinedMode](#ppenablecombinedmode) with value 1.
3. Pass the SPS and PPS to [ProcessNALUnit](#processnalunit), then the rest of the stream. When a result of `0x17004` (headers ready) is returned, call [GetStatus](#getstatus) to get the dimensions and set the output configuration with [SetConfig](#setconfig).
4. After each decode, call [ControlFrameRendering](#controlframerendering) with 0; each `0x17002` result means a picture was written to the output buffer. (SKATER, on `0x17002`, repeats GetConfig, SetConfig and ControlFrameRendering with the same configuration.)
5. **Shutdown:** call [ControlFrameRendering](#controlframerendering) with a nonzero value in a loop until the result is no longer `0x17002`, to drain the pictures still queued. Then [PpDisableCombinedMode](#ppdisablecombinedmode), [PpRelease](#pprelease), [H264Release](#h264release) and [Shutdown](#shutdown).

SKATER runs the whole initialization and shutdown sequence each time its video player is opened and closed.

**VP8, WebP and VP6** follow the same pattern with their own commands: [Vp8Initialize](#vp8initialize) (or [Vp6Initialize](#vp6initialize)), attach the postprocessor with the corresponding decoder type, decode frame by frame with [Vp8Decode](#vp8decode), and dequeue with [Vp8NextPicture](#vp8nextpicture). SKATER does not use these commands.

### Command reference

In the tables below, word 0 is always the header. Every response starts with the header and a result code in word 1; only the words after that are listed. A **process handle** is sent as a shared-handle descriptor (`0x00000000`) followed by the caller's process handle (normally `0xFFFF8001`, `CUR_PROCESS_HANDLE`); MVD uses it to access the caller's memory with DMA.

Several commands return a raw `0x00000000` on success instead of `0x00017000`: Initialize, Shutdown, CalculateWorkBufSize, CalculateImageSize and the release commands. Codec and postprocessor commands convert the Hantro status (see [result codes](#result-codes)).

#### Initialize

| Request word | Description |
|---|---|
| 1 | Work-buffer virtual address |
| 2 | Work-buffer size. For H.264, normally from [CalculateWorkBufSize](#calculateworkbufsize); SKATER uses 1 for color conversion, where the buffer is not used |
| 3 | `0x00000000` |
| 4 | Process handle |

The address range must be within the `0x30000000` linear region. All memory the hardware uses (reference pictures, output pictures, hardware tables) is allocated from this buffer by advancing a pointer; individual frees do not return space to it. The picture addresses returned by the NextPicture/Peek commands point inside this buffer, so the client can read decoded pictures directly. The client does not otherwise need to touch the buffer's contents.

#### Shutdown

No parameters.

#### CalculateWorkBufSize

| Request word | Description |
|---|---|
| 1..3 | Configuration bytes 0x00..0x0B (below) |
| 4..10 | Unused |
| 11 | Video width |
| 12 | Video height |

| Response word | Description |
|---|---|
| 2 | Work-buffer size in bytes |

See [work buffer size](#work-buffer-size) for the configuration bytes and the calculation. This command does not touch MVD state or hardware.

#### CalculateImageSize

| Request word | Description |
|---|---|
| 1 | Width |
| 2 | Height |
| 3 | Output pixel format |

| Response word | Description |
|---|---|
| 2 | `width * height * bytes_per_pixel` |

Width and height are each clamped to 1920. The format must be `0x00010001` (YUYV) or `0x00040002` (RGB565), giving 2 bytes per pixel, or `0x00041002` (32-bit), giving 4. Any other format triggers `svcBreak`.

#### H264Initialize

| Request word | Description |
|---|---|
| 1 | `s8` no output reordering: output pictures in decode order |
| 2 | `s8` freeze concealment: on errors, repeat the last good picture instead of showing a partly concealed one |
| 3 | `s8` display smoothing: allocate extra buffers so output pictures stay valid longer |
| 4 | Reference-frame format: bit 0 requests tiled reference pictures, bit 30 requests field-DPB mode |

These are the arguments of Hantro's `H264DecInit`. SKATER passes 0 for all four. Changing them has been observed to have no visible effect on normal video playback.

#### H264EnableMvc

No parameters. Calls Hantro's `H264DecSetMvc` to enable MVC (stereoscopic H.264). MVD always reports the hardware's MVC capability as zero, so this always fails with `0xD9617384` (unsupported format).

#### H264Release

No parameters. Destroys the H.264 decoder.

#### ProcessNALUnit

Also known as H264Decode.

| Request word | Description |
|---|---|
| 1 | Input virtual address |
| 2 | Input physical address (the same buffer) |
| 3 | Input size in bytes |
| 4 | Picture ID, copied into the picture record when this input produces a picture. SKATER cycles it through 0..0x12 |
| 5 | Skip non-reference pictures (`bool`) |
| 6 | `0x00000000` |
| 7 | Process handle |

| Response word | Description |
|---|---|
| 2 | Virtual address where decoding stopped. **This points into MVD's own copy of the input, which has already been freed; do not use it** |
| 3 | Physical address where decoding stopped, relative to the input physical address |
| 4 | Number of input bytes not yet consumed |

The input is an Annex B byte stream (NAL units preceded by `00 00 01` start codes). MVD copies it into its own heap for parsing, while the hardware reads it directly from the physical address. If word 4 is nonzero, the call stopped early (for example at a change of stream headers) and the rest must be passed again.

When word 5 is set, non-reference pictures are skipped and the call returns `0x17007` for them. SKATER passes 0. Setting it gives a decoding speed-up at the cost of missing frames; forcing it to 1 inside SKATER has been observed to break video playback.

Typical results: `0x17001` (input consumed, e.g. after a parameter set), `0x17003` (a picture was decoded), `0x17004` (headers ready: call [GetStatus](#getstatus) and configure output).

#### ControlFrameRendering

Also known as H264NextPicture.

| Request word | Description |
|---|---|
| 1 | `s8` end of stream: 0 during decoding, nonzero to flush the remaining pictures at the end |
| 2 | `0x00000000` |
| 3 | Process handle |

| Response word | Description |
|---|---|
| 2..17 | [H.264 picture](#h264-picture) (0x40 bytes) |

Dequeues the next picture that is ready for display, in display order. When the postprocessor is attached, the picture is converted into the output buffer set in the configuration (or the next [multibuffer](#setupoutputbuffers) output). Returns `0x17002` when a picture was output, and `0x17000` when none is ready. The picture record is only meaningful with `0x17002`.

#### GetStatus

Also known as H264GetInfo. No request parameters.

| Response word | Description |
|---|---|
| 2..17 | [H.264 stream information](#h264-information) (0x40 bytes) |

Valid once ProcessNALUnit has returned `0x17004` (headers ready).

#### H264Peek

No request parameters. Returns the next displayable picture in words 2..17 (same layout as [ControlFrameRendering](#controlframerendering)) without dequeuing it. Not every field is filled in.

#### Vp8Initialize

| Request word | Description |
|---|---|
| 1 | `u8` format: 1 = VP7, 2 = VP8, 3 = WebP (VP8 still image) |
| 2 | `s8` freeze concealment |
| 3 | Number of frame buffers: at most 16; at least 3 for VP7 and 4 for VP8; WebP always uses 1 |
| 4 | Reference-frame format (bit 0: tiled references) |

VP7 is rejected with `0xD9617384` on retail hardware, because the G1 in the New 3DS has VP7 disabled.

#### Vp8Release

No parameters.

#### Vp8Decode

| Request word | Description |
|---|---|
| 1 | Input virtual address |
| 2 | Input physical address |
| 3 | Input size (one complete frame) |
| 4 | WebP only: slice height in pixels, for decoding in horizontal bands (0 = whole picture) |
| 5 | Optional user luma buffer virtual address (0 = use the work buffer) |
| 6 | User luma buffer physical address |
| 7 | Optional user chroma buffer virtual address |
| 8 | User chroma buffer physical address |
| 9 | `0x00000000` |
| 10 | Process handle |

| Response word | Description |
|---|---|
| 2 | Unused (Hantro's `VP8DecOutput`, which has no meaningful field) |

Words 1..8 are Hantro's `VP8DecInput`. In WebP sliced mode, each band returns `0x17038` (slice ready), and the next call continues the picture.

#### Vp8NextPicture

Same request as [ControlFrameRendering](#controlframerendering). Returns a [VP8 picture](#vp8-picture) in words 2..17.

#### Vp8GetInfo

No request parameters. Returns [VP8 information](#vp8-information) (0x28 bytes) in words 2..11.

#### Vp8Peek

No request parameters. Returns a [VP8 picture](#vp8-picture) in words 2..17 without dequeuing it.

#### Vp6Initialize

| Request word | Description |
|---|---|
| 1 | `s8` freeze concealment |
| 2 | Number of frame buffers, clamped to 3..16 |
| 3 | Reference-frame format (bit 0: tiled references) |

#### Vp6Release

No parameters.

#### Vp6Decode

| Request word | Description |
|---|---|
| 1 | Input virtual address |
| 2 | Input physical address |
| 3 | Input size (one frame) |
| 4 | `0x00000000` |
| 5 | Process handle |

| Response word | Description |
|---|---|
| 2 | Unused (Hantro's `VP6DecOutput`) |

#### Vp6NextPicture

Same request as [ControlFrameRendering](#controlframerendering). Returns a [VP6 picture](#vp6-picture) in words 2..10.

#### Vp6GetInfo

No request parameters. Returns [VP6 information](#vp6-information) in words 2..10.

#### Vp6Peek

No request parameters. Returns a [VP6 picture](#vp6-picture) in words 2..10 without dequeuing it.

#### PpInitialize

No parameters. Creates the postprocessor instance and resets the configuration to its defaults. Used for both color conversion and video.

#### PpRelease

No parameters. Destroys the postprocessor. It also tries to clear the table of output buffers registered with [SetupOutputBuffers](#setupoutputbuffers), but because of a bug it only resets the entry count.

#### PpGetResult

No parameters. With no decoder attached, this runs the postprocessor once using the current configuration (standalone color conversion) and waits for it to finish. With a decoder attached, it only returns the result of the last combined decode-and-convert operation.

#### PpEnableCombinedMode

| Request word | Description |
|---|---|
| 1 | `u8` decoder type: 1 = H.264, 6 = VP6, 9 = VP8, 10 = WebP |

Attaches the postprocessor to the current decoder, so that each output picture is converted automatically. Values 0 and above 11 are rejected immediately, and the other unlisted values fall through to a parameter error. The type must match the decoder actually initialized: SKATER uses 1, and passing 6, 9 or 10 with an H.264 decoder results in no output without an error. There is no VP7 type.

#### PpDisableCombinedMode

No parameters. Detaches the postprocessor.

#### GetConfig

| Request word | Description |
|---|---|
| 1 | Size, normally `0x11C` |
| 2 | `(size << 4) \| 0xC` (write buffer descriptor) |
| 3 | Output buffer pointer |

Copies the current [configuration](#configuration-structure) (always `0x11C` bytes, regardless of word 1) to the buffer.

#### SetConfig

| Request word | Description |
|---|---|
| 1 | Size, normally `0x11C` |
| 2 | `0x00000000` |
| 3 | Process handle |
| 4 | `(size << 4) \| 0xA` (read buffer descriptor) |
| 5 | Input buffer pointer |

Validates and installs a new [configuration](#configuration-structure) (always `0x11C` bytes). Validation failures return one of the `0xD9617108..0xD961711B` codes. SetConfig also flushes the output buffer from the data cache, but only computes the length correctly for output formats `0x00010001`, `0x00040002` and `0x00041002`; for other formats the length is uninitialized.

#### SetupOutputBuffers

| Request word | Description |
|---|---|
| 1 | Number of entries used (1..17) |
| 2..35 | 17 entries: output virtual address (configuration offset `0x64`, must be nonzero), second output virtual address (offset `0x68`, 0 if unused) |
| 36 | Size of each buffer |
| 37 | `0x00000000` |
| 38 | Process handle |

Enables **multibuffer** mode (Hantro's `PPDecSetMultipleOutput`): instead of always writing to the configured output buffer, the postprocessor writes each picture to the next of these buffers, so the client can keep several converted frames. Requires the postprocessor to be attached to a decoder. Calling it again replaces the list.

The service translates and stores every entry *before* checking the count, so a count above 17 overflows its internal tables.

#### GetNextOutput

| Request word | Description |
|---|---|
| 1 | `0x00000000` |
| 2 | Process handle |

| Response word | Description |
|---|---|
| 2 | Virtual address of the output buffer holding the next picture |
| 3 | Virtual address of its second (chroma) buffer |

Asks the postprocessor which multibuffer entry holds the next picture to display, and returns that entry's addresses by looking up its physical address in the table saved by [SetupOutputBuffers](#setupoutputbuffers). If the picture was converted with an older configuration, it is converted again first. If no entry matches, words 2..3 are not written.

#### OverrideOutputBuffers

| Request word | Description |
|---|---|
| 1 | Current output virtual address |
| 2 | Current second output virtual address |
| 3 | Replacement output virtual address |
| 4 | Replacement second output virtual address |

Only valid after [SetupOutputBuffers](#setupoutputbuffers), with the postprocessor attached and idle. If the current addresses (converted to physical) match the buffer at the postprocessor's current display position, that buffer is replaced by the new one. The first time this command is used, it also appends the replacement to the table used by [GetNextOutput](#getnextoutput); later overrides do not update that table.

### Configuration structure

The configuration read by [GetConfig](#getconfig) and written by [SetConfig](#setconfig) is Hantro's `PPConfig` (from `ppapi.h`), exactly `0x11C` bytes. It controls the postprocessor, both for standalone color conversion and when attached to a decoder.

All fields are 32-bit. Addresses in this structure are **physical** addresses; the client converts its linear-memory virtual addresses itself (for example with `osConvertVirtToPhys`) before calling SetConfig. Every address that is used must be nonzero and 8-byte aligned. In combined (video) mode the input addresses are supplied by the decoder and the input address fields are ignored.

| Offset | Hantro name | Description |
|---|---|---|
| `0x00` | `ppInImg.pixFormat` | Input [pixel format](#pixel-formats) |
| `0x04` | `ppInImg.picStruct` | Input picture structure (0..5): 0 = frame or top field, 1 = bottom field, 2 = top and bottom fields in separate buffers, 3 = both fields interleaved in a frame, 4 = top field of a frame, 5 = bottom field of a frame. Non-frame values need an interlaced-capable input format (`0x00020001` or one of the interleaved 4:2:2 formats). Default 0 |
| `0x08` | `ppInImg.videoRange` | 0 = limited (16..235) range, 1 = full range. Values above 1 are rejected. Called "H264 range" in SKATER; setting 1 has been observed to make the output brighter. Default 0 |
| `0x0C` | `ppInImg.width` | Input width. See [limits](#dimension-limits) |
| `0x10` | `ppInImg.height` | Input height. See [limits](#dimension-limits) |
| `0x14` | `ppInImg.bufferBusAddr` | Input luma (or packed image) address. Standalone only. Unused for picStruct 1 and 5 |
| `0x18` | `ppInImg.bufferCbBusAddr` | Input chroma (semiplanar) or Cb (planar) address; used for 4:2:0 input formats. Unused for picStruct 1 and 5 |
| `0x1C` | `ppInImg.bufferCrBusAddr` | Input Cr address, planar 4:2:0 (`0x00020000`) only. Unused for picStruct 1 and 5 |
| `0x20` | `ppInImg.bufferBusAddrBot` | Bottom-field luma address. Unused for picStruct 0 and 4 |
| `0x24` | `ppInImg.bufferBusAddrChBot` | Bottom-field chroma address; used for 4:2:0 input formats. Unused for picStruct 0 and 4 |
| `0x28` | `ppInImg.vc1MultiResEnable` | VC-1 only (MVD has no VC-1 decoder); no effect |
| `0x2C` | `ppInImg.vc1RangeRedFrm` | VC-1 range reduction; visibly changes the output. Must not be combined with `0x30` |
| `0x30` | `ppInImg.vc1RangeMapYEnable` | VC-1 luma range mapping |
| `0x34` | `ppInImg.vc1RangeMapYCoeff` | VC-1 luma range-mapping coefficient (0..7) |
| `0x38` | `ppInImg.vc1RangeMapCEnable` | VC-1 chroma range mapping |
| `0x3C` | `ppInImg.vc1RangeMapCCoeff` | VC-1 chroma range-mapping coefficient (0..7) |
| `0x40` | `ppInCrop.enable` | Enables the input crop rectangle below. SKATER sets 1 for video and 0 for color conversion |
| `0x44` | `ppInCrop.originX` | Crop X. Multiple of 16, at most the input width |
| `0x48` | `ppInCrop.originY` | Crop Y. Multiple of 16, at most the input height |
| `0x4C` | `ppInCrop.height` | Crop height. Multiple of 8, at least 16, at most the input height |
| `0x50` | `ppInCrop.width` | Crop width. Multiple of 8, at least the minimum input width, at most the input width |
| `0x54` | `ppInRotation.rotation` | 0 = none, 1 = 90° right, 2 = 90° left, 3 = horizontal flip, 4 = vertical flip, 5 = 180°. In video mode, rotation is not allowed with input formats `0x00010002`, `0x00010004`, `0x00100001` and `0x00200001` |
| `0x58` | `ppOutImg.pixFormat` | Output [pixel format](#pixel-formats). SKATER uses `0x00040002` |
| `0x5C` | `ppOutImg.width` | Output width. At least 16, at most 1920 |
| `0x60` | `ppOutImg.height` | Output height. At least 16, at most 4096 |
| `0x64` | `ppOutImg.bufferBusAddr` | Output address (luma plane or packed image) |
| `0x68` | `ppOutImg.bufferChromaBusAddr` | Output chroma-plane address; only for output format `0x00020001` |
| `0x6C` | `ppOutRgb.rgbTransform` | YUV→RGB coefficients: 0 = custom (from `0x84..0x94`), 1 = BT.601, 2 = BT.709 |
| `0x70` | `ppOutRgb.contrast` | Signed, -64..64 |
| `0x74` | `ppOutRgb.brightness` | Signed, -128..127 |
| `0x78` | `ppOutRgb.saturation` | Signed, -64..128 |
| `0x7C` | `ppOutRgb.alpha` | Alpha value written for 32-bit RGB output |
| `0x80` | `ppOutRgb.transparency` | Transparency bit for 16-bit RGB output (0 or 1) |
| `0x84..0x94` | `ppOutRgb.rgbTransformCoeffs.a..e` | Custom YUV→RGB coefficients, used when `0x6C` is 0 |
| `0x98` | `ppOutRgb.rgbBitmask.maskR` | Red mask for custom RGB formats (`0x00040000`, `0x00041000`) |
| `0x9C` | `ppOutRgb.rgbBitmask.maskG` | Green mask |
| `0xA0` | `ppOutRgb.rgbBitmask.maskB` | Blue mask |
| `0xA4` | `ppOutRgb.rgbBitmask.maskAlpha` | Alpha mask |
| `0xA8` | `ppOutRgb.ditheringEnable` | Dither when reducing to 16-bit RGB. SKATER sets 1 |
| `0xAC` | `ppOutMask1.enable` | Enables output mask/overlay 1. SKATER sets 0 |
| `0xB0..0xD4` | `ppOutMask1.*` | Mask 1: signed originX, originY; height, width; alphaBlendEna; blendComponentBase (physical address of the blend source); signed blendOriginX, blendOriginY; blendWidth, blendHeight |
| `0xD8` | `ppOutMask2.enable` | Enables output mask/overlay 2. SKATER sets 0 |
| `0xDC..0x100` | `ppOutMask2.*` | Mask 2, same layout as mask 1 |
| `0x104` | `ppOutFrmBuffer.enable` | Enables placing the output inside a larger frame buffer (below). SKATER sets 1 |
| `0x108` | `ppOutFrmBuffer.writeOriginX` | Signed X position of the picture in the frame buffer |
| `0x10C` | `ppOutFrmBuffer.writeOriginY` | Signed Y position. Must be even for `0x00020001` output |
| `0x110` | `ppOutFrmBuffer.frameBufferWidth` | Frame buffer width (row stride in pixels), at most 4096. Parts of the picture outside the frame buffer are clipped. Observed to require a multiple of 4 |
| `0x114` | `ppOutFrmBuffer.frameBufferHeight` | Frame buffer height. Must be even for `0x00020001` output |
| `0x118` | `ppOutDeinterlace.enable` | Deinterlace. Requires a 4:2:0 or monochrome input format |

Without the frame buffer block, the output is written as a tightly packed `width × height` image. With it, the picture is written at `(writeOriginX, writeOriginY)` inside a `frameBufferWidth × frameBufferHeight` image; the position may be negative, as long as part of the picture lands inside.

For output format `0x00020001`, `0x64` and `0x68` are the Y plane and the interleaved CbCr plane of the same picture.

For `0x6C` = 1 or 2, the postprocessor uses these coefficients, chosen by the input range:

| Transform | Range | a | b | c | d | e |
|---|---|---:|---:|---:|---:|---:|
| BT.601 | limited | 298 | 409 | 208 | 100 | 516 |
| BT.601 | full | 256 | 350 | 179 | 86 | 443 |
| BT.709 | limited | 298 | 459 | 137 | 55 | 544 |
| BT.709 | full | 256 | 403 | 120 | 48 | 475 |

Custom RGB masks must each be one contiguous run of bits (or zero), must not overlap, and must fit in 16 bits for 16-bit output.

An enabled overlay mask with alpha blending needs a nonzero, 8-byte-aligned blend source address. Mask and blend geometry have further hardware-dependent checks.

#### Dimension limits

The New 3DS's G1 has ASIC ID `0x67312398`. For this chip:

| | Color conversion | Video (combined mode) |
|---|---|---|
| Minimum input width | 16 | 48 |
| Minimum input height | 16 | 16 |
| Maximum input size | 4096 | 8176 (511 × 16); 16384 for WebP |
| Input size alignment | Multiple of 16 | Multiple of 8 |
| Output size | 16..1920 wide, 16..4096 high | Same |

The video decoders have tighter limits of their own (H.264 and VP8 at most 1920 wide).

Scaling is limited to 3× upscaling (height: `3 × input − 2`) and 1/70 downscaling, measured after cropping and rotation. Upscaling in one direction while downscaling in the other is rejected.

The limits 2048 (color conversion) and 4672 (video) that also appear in the code apply only to the older Hantro product `0x8170`, not to the New 3DS.

#### SKATER video-processing configuration

For video, SKATER gets the configuration, updates the fields below, and sets it again. The configuration does not change while a video is playing.

| Offset | Field | Value |
|---|---|---|
| `0x00` | Input format | `0x00020001`, fixed |
| `0x08` | Video range | From the stream (the "range" in SKATER's debug print `"H264 w=%d h=%d range=%d pics=%d multi=%d\n"`) |
| `0x0C`, `0x10` | Input width, height | From the stream |
| `0x40` | Crop enable | 1, fixed |
| `0x44..0x50` | Crop X, Y, height, width | From the stream |
| `0x54` | Rotation | 0, fixed |
| `0x58` | Output format | From player state |
| `0x5C`, `0x60` | Output width, height | From player state |
| `0x64` | Output address | From player state |
| `0x68` | Second output address | 0, fixed |
| `0xA8` | Dithering | 1, fixed |
| `0xAC`, `0xD8` | Masks 1 and 2 | 0, fixed |
| `0x104` | Frame buffer enable | 1, fixed |
| `0x108..0x114` | Frame buffer position and size | From player state |
| `0x118` | Deinterlace | 0, fixed |

### Pixel formats

These are Hantro's `PP_PIX_FMT_*` values, used for both the input and output formats in the configuration.

| Value | Hantro name | Layout | Input | Output |
|---|---|---|---|---|
| `0x00010001` | `YCBCR_4_2_2_INTERLEAVED` | Packed Y0 Cb Y1 Cr (YUYV) | Color conversion | Yes |
| `0x00010002` | `YCBCR_4_2_2_SEMIPLANAR` | Y plane + CbCr plane | Not usable¹ | No |
| `0x00010004` | `YCBCR_4_4_0` | 4:4:0 | Not usable¹ | No |
| `0x00010005` | `YCRYCB_4_2_2_INTERLEAVED` | Packed Y0 Cr Y1 Cb (YVYU) | Color conversion² | Yes² |
| `0x00010006` | `CBYCRY_4_2_2_INTERLEAVED` | Packed Cb Y0 Cr Y1 (UYVY) | Color conversion² | Yes² |
| `0x00010007` | `CRYCBY_4_2_2_INTERLEAVED` | Packed Cr Y0 Cb Y1 (VYUY) | Color conversion² | Yes² |
| `0x00010008..0x0001000B` | `*_4_2_2_TILED_4X4` | The four packed 4:2:2 orders above, in 4×4 tiles | No | Yes²³ |
| `0x00020000` | `YCBCR_4_2_0_PLANAR` | Y, Cb, Cr planes | Color conversion | No |
| `0x00020001` | `YCBCR_4_2_0_SEMIPLANAR` | Y plane + interleaved CbCr plane (NV12). The G1 decoder's native output | Both | Yes |
| `0x00020002` | `YCBCR_4_2_0_TILED` | Decoder tiled 4:2:0 | Video | No |
| `0x00080000` | `YCBCR_4_0_0` | Luma only | Video (monochrome streams)² | No |
| `0x00100001` | `YCBCR_4_1_1_SEMIPLANAR` | 4:1:1 | Not usable¹ | No |
| `0x00200001` | `YCBCR_4_4_4_SEMIPLANAR` | 4:4:4 | Not usable¹ | No |
| `0x00040000` | `RGB16_CUSTOM` | 16-bit RGB with masks from `0x98..0xA4` | No | Yes |
| `0x00040001` | `RGB16_5_5_5` | 16-bit, 5 bits per channel + 1 alpha bit | No | Yes |
| `0x00040002` | `RGB16_5_6_5` | 16-bit, R 15:11, G 10:5, B 4:0 | No | Yes |
| `0x00040003` | `BGR16_5_5_5` | 16-bit, channels reversed | No | Yes |
| `0x00040004` | `BGR16_5_6_5` | 16-bit, channels reversed | No | Yes |
| `0x00041000` | `RGB32_CUSTOM` | 32-bit RGB with masks from `0x98..0xA4` | No | Yes |
| `0x00041001` | `RGB32` | 32-bit ARGB | No | Yes |
| `0x00041002` | `BGR32` | 32-bit ABGR | No | Yes |

¹ Accepted by Hantro's postprocessor only when attached to a JPEG decoder, which MVD does not have. Other attempts fail with `0xD961710A` (invalid input format).
² Not allowed on the old `0x8170` Hantro product; allowed on the New 3DS.
³ Requires the tiled-output capability; output width, height and mask geometry must be multiples of 4, and the frame buffer block cannot be used.

Observed behavior when decoding H.264 with various input formats: `0x00020001` gives correct output; `0x00020002` gives slightly corrupted output (the decoder writes linear, not tiled, pictures); `0x00080000` gives grayscale output (luma only); everything else fails with `0xD961710A`.

The RGB names are Hantro's bit-layout names. libctru's `MVDSTD_OutputFormat` names the same values differently (`MVD_OUTPUT_BGR565 = 0x00040002`, `MVD_OUTPUT_RGB565 = 0x00040004`), so check which convention a source uses. With the default all-zero masks, the custom-mask formats have been observed to produce no output.

### Output structures

Replies always contain the whole structure, even on paths that did not fill it. Only trust the contents when the result says a picture or information is available.

#### H.264 information

Returned by [GetStatus](#getstatus). Hantro's `H264DecInfo`, with an extra word at `0x34`.

| Offset | Description |
|---|---|
| `0x00` | Picture width in pixels (a multiple of 16) |
| `0x04` | Picture height in pixels (a multiple of 16) |
| `0x08` | Video range (0 = limited, 1 = full) |
| `0x0C` | Matrix coefficients (from the VUI) |
| `0x10` | Crop left |
| `0x14` | Crop width |
| `0x18` | Crop top |
| `0x1C` | Crop height |
| `0x20` | Output pixel format (`0x00020001`, `0x00020002` or `0x00080000`) |
| `0x24` | Sample aspect ratio width |
| `0x28` | Sample aspect ratio height |
| `0x2C` | Monochrome stream |
| `0x30` | Interlaced stream |
| `0x34` | DPB mode (frame/field); -1 before the first sequence |
| `0x38` | Number of picture buffers |
| `0x3C` | Number of output buffers needed for [multibuffer](#setupoutputbuffers) mode |

#### H.264 picture

Returned by [ControlFrameRendering](#controlframerendering) and [H264Peek](#h264peek).

| Offset | Description |
|---|---|
| `0x00` | Picture width |
| `0x04` | Picture height |
| `0x08` | Crop left |
| `0x0C` | Crop width |
| `0x10` | Crop top |
| `0x14` | Crop height |
| `0x18` | Decoded picture virtual address (inside the work buffer) |
| `0x1C` | Decoded picture physical address |
| `0x20` | Picture ID from [ProcessNALUnit](#processnalunit) |
| `0x24` | IDR picture |
| `0x28` | Number of concealed (damaged) macroblocks |
| `0x2C` | Interlaced |
| `0x30` | Field picture |
| `0x34` | Top field |
| `0x38` | MVC view ID |
| `0x3C` | `u8` output layout (raster or tiled), followed by padding |

The decoded picture at `0x18` is in the decoder's native format (4:2:0 semiplanar, or luma only for monochrome). It stays valid only until the decoder reuses the buffer.

#### VP8 information

Returned by [Vp8GetInfo](#vp8getinfo). 0x28 bytes.

| Offset | Description |
|---|---|
| `0x00` | VP8 version |
| `0x04` | Profile |
| `0x08` | Coded width |
| `0x0C` | Coded height |
| `0x10` | Frame width, rounded up to a multiple of 16 |
| `0x14` | Frame height, rounded up to a multiple of 16 |
| `0x18` | Scaled width |
| `0x1C` | Scaled height |
| `0x20` | `u8`, always 0; bytes `0x21..0x23` are uninitialized |
| `0x24` | Output pixel format |

#### VP8 picture

Returned by [Vp8NextPicture](#vp8nextpicture) and [Vp8Peek](#vp8peek). 0x40 bytes.

| Offset | Description |
|---|---|
| `0x00` | Coded width |
| `0x04` | Coded height |
| `0x08` | Frame width |
| `0x0C` | Frame height |
| `0x10` | Luma stride |
| `0x14` | Chroma stride |
| `0x18` | Luma virtual address |
| `0x1C` | Luma physical address |
| `0x20` | Chroma virtual address |
| `0x24` | Chroma physical address |
| `0x28` | Picture ID (always 0) |
| `0x2C` | Intra frame (always 0) |
| `0x30` | Golden frame (always 0) |
| `0x34` | Concealed macroblocks (always 0) |
| `0x38` | Number of rows output (WebP slices) |
| `0x3C` | `u8` output layout, followed by padding |

#### VP6 information

Returned by [Vp6GetInfo](#vp6getinfo). 0x24 bytes: version (`0x00`), profile (`0x04`), frame width (`0x08`), frame height (`0x0C`), scaled width (`0x10`), scaled height (`0x14`), scaling mode (`0x18`), a `u8` that is always 0 (`0x1C`, followed by 3 uninitialized bytes), output pixel format (`0x20`).

#### VP6 picture

Returned by [Vp6NextPicture](#vp6nextpicture) and [Vp6Peek](#vp6peek). 0x24 bytes: frame width (`0x00`), frame height (`0x04`), picture virtual address (`0x08`), picture physical address (`0x0C`), picture ID (`0x10`, always 0), intra frame (`0x14`, always 0), golden frame (`0x18`, always 0), concealed macroblocks (`0x1C`, always 0), `u8` output layout (`0x20`) followed by padding.

### Work buffer size

[CalculateWorkBufSize](#calculateworkbufsize) takes a 12-byte configuration (request words 1..3) plus the video width and height (words 11 and 12). The result is the **largest** of the enabled estimates, or 0 if none is enabled.

| Byte | Description |
|---|---|
| `0x00` | Unused |
| `0x01` | Level estimate: enable (`bool`) |
| `0x02` | Level estimate: flags. Bit 0 = enable, bit 1 = add 1/6, bit 2 = unknown; the estimate runs if any bit is set |
| `0x03` | Level estimate: double the size (`bool`) |
| `0x04` | Level estimate: H.264 level index (below) |
| `0x05` | Estimate A: enable (`bool`) |
| `0x06` | Estimate A: number of reference frames |
| `0x07` | Estimate B: enable (`bool`) |
| `0x08` | Estimate B: number of reference frames |
| `0x09..0x0B` | Unused |

With `MBs = ceil(width / 16) * ceil(height / 16)` and `frame_size = 384 * MBs` (one 8-bit 4:2:0 picture):

* **Level estimate** (needs both byte 1 and byte 2 nonzero): look up `maxFrameMbs` and `maxDpbMbs` for the level. If `MBs > maxFrameMbs`, or `maxDpbMbs / MBs` rounds down to 0, the estimate is 0. Otherwise `refs = min(maxDpbMbs / MBs, 16)` and `size = frame_size * (refs + 1)`. If flag bit 1 or 2 is set, `size += size / 6`. If byte 3 is set, `size *= 2`. Finally `size += 0xFC8`.
* **Estimate A:** `frame_size * clamp(refs, 2, 16) + 0x10800`.
* **Estimate B:** `frame_size * clamp(refs, 2, 16) + 0x858`.

| Index | Level | maxFrameMbs | maxDpbMbs |
|---:|---|---:|---:|
| `0x00` | 1.0 | 99 | 396 |
| `0x01` | 1b | 99 | 396 |
| `0x02` | 1.1 | 396 | 900 |
| `0x03` | 1.2 | 396 | 2376 |
| `0x04` | 1.3 | 396 | 2376 |
| `0x05` | 2.0 | 396 | 2376 |
| `0x06` | 2.1 | 792 | 4752 |
| `0x07` | 2.2 | 1620 | 8100 |
| `0x08` | 3.0 | 1620 | 8100 |
| `0x09` | 3.1 | 3600 | 18000 |
| `0x0A` | 3.2 | 5120 | 20480 |
| `0x0B` | 4.0 | 8192 | 32768 |
| `0x0C` | 4.1 | 8192 | 32768 |
| `0x0D` | 4.2 | 8704 | 34816 |
| `0x0E` | 5.0 | 22080 | 110400 |
| `0x0F` | 5.1 | 36864 | 184320 |
| `0x10` | 5.2 | 36864 | 184320 |

The level index is not range-checked; values above `0x10` read past the table.

SKATER always uses the level estimate with flags `0x07`, level 3.2 and a fixed 854×480, regardless of the video:

```c
cfg.level_enable  = 1;
cfg.level_flags   = 0x07;
cfg.level_double  = 0;
cfg.level         = 0x0A;   /* 3.2 */
cfg.refs_a_enable = 0;
cfg.refs_b_enable = 0;
width  = 854;
height = 480;
```

This gives MBs = 54 × 30 = 1620, refs = min(20480 / 1620, 16) = 12, `622080 × 13 = 8087040`, plus 1/6 = 9434880, plus `0xFC8` = **9438920 bytes (`0x9006C8`)**, known as `MVD_DEFAULT_WORKBUF_SIZE`.

Tests have found that a stream using the maximum number of reference frames can need a few hundred bytes more than the level estimate. One reason is that each picture buffer is allocated 32 bytes larger than the picture, and more buffers are allocated than the DPB size (see the buffer count in [writeup2.md](writeup2.md#dpb-allocation)). Adding a few KiB of margin is safer.

## Result codes

`mvd:STD` codec and postprocessor commands return Hantro status codes converted to 3DS results. Positive statuses become `0x00017000 + status` (with one exception); negative statuses become error results with module 97 (MVD). Statuses without a case in MVD's conversion table become `0x00000000`: in particular, the postprocessor's "busy" status (-128) is reported as 0.

**Status results**

| Result | Hantro status | Meaning |
|---|---|---|
| `0x00017000` | 0 `*_OK` | Success. From NextPicture/ControlFrameRendering: no picture is ready |
| `0x00017001` | 1 `*_STRM_PROCESSED` | Input consumed without producing a picture (e.g. after an SPS or PPS) |
| `0x00017002` | 2 `*_PIC_RDY` | A picture was output. Returned by ControlFrameRendering and the other NextPicture commands |
| `0x00017003` | 3 `*_PIC_DECODED` | A picture was decoded |
| `0x00017004` | 4 `*_HDRS_RDY` | New stream headers: call GetStatus and configure output, then pass the remaining input again |
| `0x00017005` | 5 `H264DEC_ADVANCED_TOOLS` | The H.264 stream uses tools that need the software-assisted decoding path |
| `0x00017006` | 6 `H264DEC_PENDING_FLUSH` | H.264: pictures must be output before decoding can continue |
| `0x00017007` | 7 `*_NONREF_PIC_SKIPPED` | Non-reference picture skipped (ProcessNALUnit with skip enabled) |
| `0x00017038` | 6 `VP8DEC_SLICE_RDY` | VP8/WebP: a slice (band of rows) is ready |

**Errors**

| Result | Hantro status | Meaning |
|---|---|---|
| `0xE16170C9` | -1 `*_PARAM_ERROR` | Invalid parameter |
| `0xD96170CA` | -2 `*_STRM_ERROR` | Stream error |
| `0xD96170CB` | -3 `*_NOT_INITIALIZED` | Decoder or postprocessor not initialized |
| `0xD86170CC` | -4 `*_MEMFAIL` | Out of memory (work buffer or module heap) |
| `0xD96170CD` | -5 `*_INITFAIL` | Initialization failed |
| `0xD96170CE` | -6 `*_HDRS_NOT_RDY` | Headers not decoded yet |
| `0xD96170D0` | -8 `*_STREAM_NOT_SUPPORTED` | Unsupported stream |
| `0xD9617108..0xD961711B` | -64..-83 PP configuration errors | Invalid configuration (below) |
| `0xD96171C6` | -254 `*_HW_RESERVED` | Hardware busy |
| `0xD96171C7` | -255 `*_HW_TIMEOUT` (decoder), -257 (PP) | Hardware timeout |
| `0xF96171C8` | -256 `*_HW_BUS_ERROR` | Hardware bus error |
| `0xD96171C9` | -257 `*_SYSTEM_ERROR` (decoder), -259 (PP) | System error |
| `0xD96171CA` | -258 `*_DWL_ERROR` | Platform-layer error |
| `0xD96172C8` | -512 (PP) | Postprocessor combined-mode error |
| `0xD96172C9` | -513 (PP) | Decoder error reported to the postprocessor |
| `0xD9617383` | -999 `*_EVALUATION_LIMIT_EXCEEDED` | Evaluation limit |
| `0xD9617384` | -1000 `*_FORMAT_NOT_SUPPORTED` | Unsupported format or feature (e.g. MVC, VP7) |

**Configuration errors** from [SetConfig](#setconfig), in order from `0xD9617108`: input size, input address, input format, crop, rotation, output size, output address, output format, color adjustment, RGB masks, frame buffer, mask 1, mask 2, deinterlace, input picture structure, input range mapping, alpha blending not supported, deinterlacing not supported, dithering not supported, scaling not supported. For example:

| Result | Meaning |
|---|---|
| `0xD9617108` | Invalid input size |
| `0xD961710A` | Invalid input format |
| `0xD961710B` | Invalid crop |
| `0xD961710D` | Invalid output size |
| `0xD961710F` | Invalid output format |
| `0xD9617112` | Invalid frame-buffer placement |

## l2b:u and l2b2:u

L2B converts RGB pixel data into the GPU's tiled texture layout, with a fixed alpha value. The two services are identical and share a command handler, but drive separate engines.

| Service | Registers (physical) | DMA input FIFO | DMA output FIFO | DMA devices (in / out) | Interrupt |
|---|---|---|---|---|---|
| `l2b:u` | `0x10130000` | `0x10330000` | `0x10330200` | 23 / 24 | `0x45` |
| `l2b2:u` | `0x10131000` | `0x10331000` | `0x10331200` | 25 / 26 | `0x46` |

The session opens the engine; closing it stops any DMA and shuts the engine down.

| Command header | Name | Request (words after header) | Response (words after result) |
|---|---|---|---|
| `0x00010040` | SetInputFormat | `u8` format | — |
| `0x00020000` | GetInputFormat | — | `u8` format |
| `0x00030040` | SetOutputFormat | `u8` format | — |
| `0x00040000` | GetOutputFormat | — | `u8` format |
| `0x00050040` | SetTransferEndInterrupt | `bool` enable | — |
| `0x00060000` | GetTransferEndInterrupt | — | `bool` enabled |
| `0x00070000` | GetTransferEndEvent | — | `0x00000000`, event handle |
| `0x00080102` | SetSending | source address, total size, `s16` transfer unit, `s16` transfer gap, `0x00000000`, process handle | — |
| `0x00090000` | IsDoneSending | — | `bool` done |
| `0x000A0102` | SetReceiving | destination address, total size, `s16` transfer unit, `s16` transfer gap, `0x00000000`, process handle | — |
| `0x000B0000` | IsDoneReceiving | — | `bool` done |
| `0x000C0040` | SetInputLineWidth | `u16` width | — |
| `0x000D0000` | GetInputLineWidth | — | `u16` width |
| `0x000E0040` | SetInputLines | `u16` lines | — |
| `0x000F0000` | GetInputLines | — | `u16` lines |
| `0x00100040` | SetAlpha | `u16` alpha (low 8 bits used) | — |
| `0x00110000` | GetAlpha | — | `u16` alpha |
| `0x00120000` | StartConversion | — | — |
| `0x00130000` | StopConversion | — | — |
| `0x00140000` | IsBusyConversion | — | `bool` busy |
| `0x00150080` | SetPackageParameter | 8-byte [parameters](#l2b-parameters) | — |
| `0x00160000` | GetPackageParameter | — | 8-byte [parameters](#l2b-parameters) |
| `0x00170000` | PingProcess | — | `u8` number of open sessions |

Notes:

* The formats are:

  | Value | Input | Output |
  |---|---|---|
  | 0 | RGBA8888 (input alpha ignored) | RGBA8888 |
  | 1 | RGB888 | RGB888 |
  | 2 | RGBA5551 (input alpha ignored) | RGBA5551 |
  | 3 | RGB565 | RGB565 |

  Output alpha always comes from the alpha register: the whole byte for RGBA8888, its top bit for RGBA5551. RGBA8888 is stored as the bytes `AA BB GG RR` (the word `0xRRGGBBAA`), the GPU's texture order. The format setters do not range-check their argument.
* Line width and line count must be multiples of 8 from 8 to 1024. 1024 is stored in the register as 0, and the getters return 0 in that case.
* SetSending/SetReceiving configure a DMA transfer between the given buffer and the engine's FIFO: `transfer unit` bytes are moved per block, and the memory side skips `transfer gap` bytes after each block. Before sending, MVD cleans the source from the data cache; before receiving, it invalidates the destination. The DMA burst size is the largest of 64, 32, 16, … that divides the transfer unit.
* IsDoneSending/IsDoneReceiving check the DMA state without waiting.
* StartConversion starts the engine and returns immediately. Completion can be observed through the transfer-end event, IsBusyConversion, or IsDoneReceiving; these are separate signals, and the event may fire before all output has arrived.
* The transfer-end event handle belongs to the caller and must be closed.
* Scalar getters only write the low byte or halfword of their reply word.

### L2B parameters

| Offset | Description |
|---|---|
| `0x0` | `u8` input format |
| `0x1` | `u8` output format |
| `0x2` | `s16` line width |
| `0x4` | `s16` number of lines |
| `0x6` | `u16` alpha |

SetPackageParameter applies the fields in order and stops at the first invalid one, without undoing the earlier ones. GetPackageParameter's reply header claims five words after the result, but only the first two are written.

### L2B result codes

| Result | Meaning |
|---|---|
| `0xD8216FF9` | Already initialized / already open |
| `0xD8216FF8` | Not initialized / not open |
| `0xE0E16C02` | Invalid engine |
| `0xE0E16FFD` | Invalid width or line count |
| `0xC9416C01` | Conversion blocked |
| `0xD900182F` | Unknown command (reply header `0x00000040`) |
| `0xD9001830` | Malformed DMA request header or descriptor (reply header `0x00000040`) |

## y2r2:u

`y2r2:u` drives a second YUV→RGB converter in the New 3DS, at physical `0x10132000` (the original [Y2R](https://www.3dbrew.org/wiki/Y2R_Services) block is at `0x10102000`). Its command set is that of [`y2r:u`](https://www.3dbrew.org/wiki/Y2R_Services) (commands `0x01`..`0x2C`, with the same IDs, parameters and structures), plus one extra command:

| Command header | Name | Request | Response (words after result) |
|---|---|---|---|
| `0x002D0000` | GetConversionParams | — | 12-byte `Y2RU_ConversionParams` |

Differences and details:

* It uses the `y2r:u` input formats (0..4), output formats (0..3), rotations (0..3) and block alignments (0 = linear, 1 = 8×8 tiled). The output formats are the same four as [L2B](#l2bu-and-l2b2u), with the same byte order.
* DMA devices: Y 18, U 19, V 20, YUYV 21, RGB output 22. FIFOs are at physical `0x10332000` (Y), `0x10332080` (U), `0x10332100` (V), `0x10332180` (YUYV) and `0x10332200` (output). Interrupt `0x4E`.
* The client must call DriverInitialize (`0x002B0000`) after opening the service, and DriverFinalize (`0x002C0000`) before closing it. Closing the session also finalizes the driver.
* SetConversionParams (`0x002900C0`) reads exactly three words, the 12-byte packed `Y2RU_ConversionParams`. (libctru's `Y2RU_SetConversionParams` sends the header `0x002901C0`, claiming seven words.)
* GetConversionParams reconstructs the standard-coefficient field by comparing the current coefficients with the four presets, returning 4 if none matches. Its reply header claims eight words after the result, but only three are written.
* GetStandardCoefficient (`0x00210040`) with an index of 4 or more returns `0xE0E053ED`, but still copies 16 bytes of uninitialized data into the reply.
* SetInputLines(1024) returns success without changing the register. Other values must be 1..1023; width must be a multiple of 8 from 8 to 1024 (1024 stored as 0).
* Coefficient presets (raw 16-bit values):

  | Index | Coefficients |
  |---|---|
  | 0 (BT.601) | 256, 358, 182, 88, 453, 0xE991, 0x10EE, 0xE3A5 |
  | 1 (BT.709) | 256, 403, 119, 47, 475, 0xE6CD, 0x0A7C, 0xE24F |
  | 2 (BT.601 scaled) | 298, 408, 208, 100, 516, 0xE422, 0x10F2, 0xDD65 |
  | 3 (BT.709 scaled) | 298, 458, 136, 54, 540, 0xE0FC, 0x099C, 0xDBDF |

  The first five are 10-bit multipliers; the last three are signed offsets.

Result codes: already initialized `0xD82053F9`, not initialized `0xD82053F8`, invalid engine `0xE0E05002`, invalid dimensions `0xE0E053FD`, invalid coefficient index `0xE0E053ED`, conversion blocked `0xC9405001`. Unknown commands and malformed DMA requests return the same codes as L2B.

## Supported codecs, H.264 levels and profiles

The New 3DS G1 decoder (ASIC ID `0x67312398`) reports the following capabilities. MVD combines the hardware's configuration and fuse registers, and additionally forces MVC off:

| Codec | Status |
|---|---|
| H.264 | Supported, up to 1920 pixels wide, high-profile hardware tier |
| H.264 MVC (stereo) | Not supported (disabled by MVD) |
| VP8 | Supported |
| WebP (lossy, VP8 intra) | Supported |
| VP6 | Supported |
| VP7 | Not supported (fused off) |
| MPEG-2, MPEG-4, Sorenson Spark, VC-1, JPEG, AVS, RealVideo | Not supported (no software in MVD; MPEG-4 and Sorenson Spark are also fused off) |

The browser only uses H.264; see the [Internet Browser](https://www.3dbrew.org/wiki/Internet_Browser) page for the formats it plays.

The table below was created by playing test videos in the New 3DS Internet Browser:

| Level | Baseline | Main | High | High 10 | High 4:2:2 | High 4:4:4 Predictive |
|---|---|---|---|---|---|---|
| 1 | Yes | Yes | Yes | Yes? | Untested | Untested |
| 1b | Yes | Yes | Yes | Yes? | Untested | Untested |
| 1.1 | Yes | Yes | Yes | Yes? | Untested | Untested |
| 1.2 | Yes | Yes | Yes | Yes? | Untested | Untested |
| 1.3 | Yes | Yes | Yes | Yes? | Untested | Untested |
| 2 | Yes | Yes | Yes | Yes? | Untested | Untested |
| 2.1 | Yes | Yes | Yes | Yes? | Untested | Untested |
| 2.2 | Yes | Yes | Yes | Yes? | Untested | Untested |
| 3 | Yes | Yes | Yes | Yes? | Untested | Untested |
| 3.1 | Yes | Yes | Yes | Yes? | Untested | Untested |
| 3.2 | Yes | Yes | Yes | Yes? | Untested | Untested |
| 4 | Untested | Untested | Untested | Untested | Untested | Untested |
| 4.1 | Untested | Untested | Untested | Untested | Untested | Untested |
| 4.2 | Untested | Untested | Untested | Untested | Untested | Untested |
| 5 | Untested | Untested | Untested | Untested | Untested | Untested |
| 5.1 | Untested | Untested | Untested | Untested | Untested | Untested |
| 5.2 | Untested | Untested | Untested | Untested | Untested | Untested |

Test pages used (no longer online): [Baseline](http://mtheall.com/~mtheall/pie/baseline.html), [Main](http://mtheall.com/~mtheall/pie/main.html), [High](http://mtheall.com/~mtheall/pie/high.html), [High10](http://mtheall.com/~mtheall/pie/high10.html).

Notes on this table:

* **Levels 3.2 and below** match what SKATER allocates for: it always sizes the work buffer for level 3.2 at 854×480. Higher levels are untested. The decoder is limited to pictures 1920 pixels wide and by the size of the work buffer the application provides.
* **High 10 is doubtful.** MVD parses the SPS bit-depth fields but discards them, and allocates reference pictures with one byte per sample. The decoder therefore cannot produce 10-bit output; a High 10 stream is at best decoded to 8 bits, and may be decoded incorrectly. The High10 test page can no longer be checked to see whether its stream really used 10-bit samples. Treat High 10 as unsupported.
* **High 4:2:2 and High 4:4:4** are not supported: the decoder's picture storage and output formats are 4:2:0 (or monochrome) only.

## Corrections to earlier documentation

For readers comparing with older versions of this page:

* **Header codes of l2b:u queries.** The getters and query commands (`0x0002`, `0x0004`, `0x0006`, `0x0007`, `0x0009`, `0x000B`, `0x000D`, `0x000F`, `0x0011`, `0x0014`, `0x0016`, `0x0017`) take no parameters; their request headers end in `0000`, not `0080`. StartConversion and StopConversion are `0x00120000` and `0x00130000`. SetPackageParameter takes two words (`0x00150080`).
* **CalculateImageSize** takes width, height, format in that order (not format first), and its accepted formats are `0x00010001`, `0x00040002` and `0x00041002`. It also returns the size in response word 2.
* **CalculateWorkBufSize** returns the size in response word 2.
* **ControlFrameRendering's** process handle is in words 2..3 (header `0x00090042` has one normal word), and its argument is an end-of-stream flag rather than start/stop. `0x00017002` means "picture output", not "busy".
* **`0x00017004`** is "headers ready", which is why decoding stops with input remaining.
* **`0xF96171C8`** comes from Hantro status -256 (hardware bus error).
* **OverrideOutputBuffers** compares against the postprocessor's current display buffer, not always entry 0.
* **The 2048 / 4672 size limits** in the configuration checks apply only to an older Hantro product, not to the New 3DS.
* **"Unknown" commands** are now identified: `0x0006` enables MVC, `0x000B` peeks the next H.264 picture, `0x000C..0x0011` are the VP8/WebP/VP7 decoder, `0x0012..0x0017` the VP6 decoder, `0x0018`/`0x0019` create and destroy the postprocessor, `0x001B`/`0x001C` attach and detach it, and `0x0020` returns the next multibuffer output.
