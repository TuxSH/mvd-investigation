# mvd:STD command ABI

## Dispatch and state

Dispatcher: `0x1124E8`. Its session context begins with a decoder pointer at `+0` and a postprocessor pointer at `+4`; the decoder slot is shared by the H.264, VP6 and VP8 families. These are alternative decoder lifetimes, not three independently stored instances.

The dispatcher selects the low byte of the command ID (`header >> 16`). For commands with translated handles/buffers it checks the expected header/descriptor shape; many scalar-only commands go directly to their handler without an equivalent full-header check. The table gives the normal request encoding, not a promise that every malformed header is rejected.

A process handle uses the two-word shared-handle descriptor (`0`, handle). Mapped read/write buffers have descriptor low nibbles `0xA`/`0xC`. Most replies contain one normal result word; larger results are listed below. The service constructs a command-zero error reply for invalid descriptors or unknown commands.

## Command table

| ID | Request header | Meaning | Handler | Request after header | Reply after header |
|---|---|---|---|---|---|
| `0x01` | `0x00010082` | Initialize | `0x112D88` | client work-buffer VA, size; process handle | result |
| `0x02` | `0x00020000` | Shutdown | `0x112CBC` | — | result |
| `0x03` | `0x00030300` | CalculateWorkBufSize | `0x11307C` | 12-word packed request; meaningful bytes and dimensions below | result, byte count |
| `0x04` | `0x000400C0` | CalculateImageSize | `0x1130B2` | width, height, output format | result, byte count |
| `0x05` | `0x00050100` | H264Initialize | `0x113150` | s8 noOutputReordering, s8 freezeConcealment, s8 displaySmoothing, referenceFrameFormat | result |
| `0x06` | `0x00060000` | H264EnableMvc | `0x112CA8` | — | result |
| `0x07` | `0x00070000` | H264Release | `0x112D16` | — | result |
| `0x08` | `0x00080142` | H264Decode / ProcessNALUnit | `0x112C04` | stream VA, bus address, size, picture ID, skipNonReference; process handle | result, current stream VA, current bus address, bytes left |
| `0x09` | `0x00090042` | H264NextPicture / ControlFrameRendering | `0x112EC0` | s8 endOfStream; process handle | result, 0x40-byte picture |
| `0x0A` | `0x000A0000` | H264GetInfo | `0x112CE0` | — | result, 0x40-byte info |
| `0x0B` | `0x000B0000` | H264Peek | `0x113164` | — | result, 0x40-byte picture |
| `0x0C` | `0x000C0100` | Vp8Initialize | `0x113118` | u8 format, s8 freezeConcealment, frame-buffer count, referenceFrameFormat | result |
| `0x0D` | `0x000D0000` | Vp8Release | `0x112C9C` | — | result |
| `0x0E` | `0x000E0202` | Vp8Decode | `0x1131E0` | 8-word VP8DecInput; process handle | result, one output word |
| `0x0F` | `0x000F0042` | Vp8NextPicture | `0x112E8C` | s8 endOfStream; process handle | result, 0x40-byte picture |
| `0x10` | `0x00100000` | Vp8GetInfo | `0x112C7E` | — | result, 0x28-byte info |
| `0x11` | `0x00110000` | Vp8Peek | `0x11313E` | — | result, 0x40-byte picture |
| `0x12` | `0x001200C0` | Vp6Initialize | `0x113108` | s8 freezeConcealment, frame-buffer count, referenceFrameFormat | result |
| `0x13` | `0x00130000` | Vp6Release | `0x112C90` | — | result |
| `0x14` | `0x001400C2` | Vp6Decode | `0x113178` | stream VA, bus address, size; process handle | result, one output word |
| `0x15` | `0x00150042` | Vp6NextPicture | `0x112E58` | s8 endOfStream; process handle | result, 0x24-byte picture |
| `0x16` | `0x00160000` | Vp6GetInfo | `0x112C6C` | — | result, 0x24-byte info |
| `0x17` | `0x00170000` | Vp6Peek | `0x11312C` | — | result, 0x24-byte picture |
| `0x18` | `0x00180000` | PpInitialize | `0x1130C8` | — | result |
| `0x19` | `0x00190000` | PpRelease | `0x113248` | — | result |
| `0x1A` | `0x001A0000` | PpGetResult / standalone conversion | `0x112D04` | — | result |
| `0x1B` | `0x001B0040` | PpEnableCombinedMode | `0x112FB4` | u8 decoder type | result |
| `0x1C` | `0x001C0000` | PpDisableCombinedMode | `0x112FCA` | — | result |
| `0x1D` | `0x001D0042` | PpGetConfig | `0x112CF2` | size word; writable mapped buffer | result; returned mapped-buffer descriptor/address |
| `0x1E` | `0x001E0044` | PpSetConfig | `0x112D24` | size word; process handle; readable mapped buffer | result; returned mapped-buffer descriptor/address |
| `0x1F` | `0x001F0902` | SetupOutputBuffers | `0x112EF4` | count, 17 VA pairs, per-buffer size; process handle | result |
| `0x20` | `0x00200002` | GetNextOutput | `0x112DE8` | process handle | result, luma/RGB client VA, chroma client VA |
| `0x21` | `0x00210100` | OverrideOutputBuffers | `0x112FE0` | current luma VA, current chroma VA, replacement luma VA, replacement chroma VA | result |

## Lifecycle and decoding

For H.264 with postprocessing, the logical sequence is initialize work-buffer access (`01`), initialize decoder (`05`), initialize PP (`18`), attach PP to H.264 (`1B`, value 1), decode (`08`), inspect info after headers (`0A`), configure output (`1E`), and retrieve pictures (`09`). Retrieval invokes the linked postprocessor as needed. At end of stream, use a nonzero `endOfStream` to flush pending output. Detach (`1C`), release PP (`19`), release decoder (`07`), then shut down (`02`). This explains the corresponding sequence in libctru.

`09` is an end-of-stream-aware picture dequeue operation. A zero argument means normal operation; a nonzero value requests draining/flush behavior. Its returned `0x17002` indicates a populated picture. It is not simply an asynchronous conversion-busy flag. `0B` peeks rather than dequeuing.

For standalone pixel conversion, no decoder is needed: initialize, initialize PP, set configuration, call `1A`, then release PP and shut down. `PPGetResult` checks idle state and, when no decoder is attached, runs the PP and waits for it. With a decoder attached it returns the stored combined-operation result instead.

`05` arguments correspond directly to Hantro's no-output-reordering, freeze concealment, display smoothing, and reference-frame format. The three byte-sized values are sign-extended by the IPC dispatcher. The reference-format word uses bit 0 for tiled references and bit 30 for field-DPB support in this build. These are not pixel-format values.

`06` invokes `H264DecSetMvc`. It checks the capability structure's MVC field, which this build forces to zero. Thus its existence in the ABI does not establish working MVC support. The gated write is now identified as `storage.mvcEnabled = 1` at `+0x39DC`; [the MVC state analysis](h264-mvc-state.md) distinguishes this request flag from the neighboring parser state.

### Profile and bit-depth limits

The SPS parser `h264bsdDecodeSeqParamSet` at `0x116F78` has the Hantro high-profile syntax path for `profile_idc >= 100`. It reads `chroma_format_idc`, then both unsigned Exp-Golomb `bit_depth_*_minus8` fields. The latter values are overwritten in a temporary and are neither retained in the recovered SPS structure nor numerically validated there. The chroma-format value is also not bounded to one in this block. This matches the supplied source's parsing behavior, including its ineffective chroma-range check.

Consequently, an accepted SPS or a successful browser playback experiment does not by itself establish native 10-bit, 4:2:2 or 4:4:4 reconstruction. This analysis does not validate the wiki's empirical High10 support table. It establishes the parser behavior and capability-dependent decoder paths; an actual profile/bit-depth support matrix still requires correctly characterized streams and hardware output comparison. Likewise, the work-size helper's 17-entry level table is allocation logic, not a list of guaranteed hardware-supported levels.

## Other codec groups

`0C` format values are 1 = VP7, 2 = VP8, 3 = WebP. Capability checks are performed for VP7 or VP8/WebP. Frame-buffer counts are capped at 16, with minima 3 for VP7 and 4 for VP8; WebP uses one. `12` initializes VP6, with frame-buffer count clamped to 3..16. Concealment can be disabled by the old-product compatibility path.

For PP attachment (`1B`), the function first range-checks 1..11 but the compiled switch implements **only** 1 = H.264, 6 = VP6, 9 = VP8, 10 = WebP. Other values return parameter error. VP7 initialization exists, but there is no separate VP7 PP attachment case; do not infer full combined-mode support from the range check.

`0E` consumes the eight fields of `VP8DecInput`: stream VA, stream bus address, byte length, WebP slice height, optional luma VA, luma bus address, optional chroma VA, chroma bus address. The service stages the input stream, but the additional user-picture pointers are forwarded with the input structure. Their accepted use depends on the decoder's WebP and buffer-mode paths.

## Output structures

All offsets below are bytes. Pointer values are 32-bit.

### H.264 info, 0x40 bytes (`0A`)

| Offset | Field |
|---|---|
| 00, 04 | stored picture width, height |
| 08, 0C | video range, matrix coefficients |
| 10, 14, 18, 1C | crop left, crop width, crop top, crop height |
| 20 | output pixel format |
| 24, 28 | sample aspect ratio width, height |
| 2C, 30 | monochrome, interlaced sequence |
| 34 | DPB storage mode: frame/field state, initialized to -1 before sequence setup |
| 38, 3C | picture buffer count, multibuffer PP requirement |

### H.264 picture, 0x40 bytes (`09`, `0B`)

`00/04`: stored width/height; `08/0C/10/14`: crop left/width/top/height; `18/1C`: output pointer/bus address; `20`: picture ID; `24`: IDR flag; `28`: concealed macroblock count; `2C`: interlaced; `30`: field picture; `34`: top field; `38`: MVC view ID; `3C`: output layout byte (raster/tiled), followed by padding. Peek does not necessarily populate every field that dequeue populates. Replies copy the full structure even on paths that do not fill it; consumers must inspect the result before using fields.

### VP8 info, 0x28 bytes (`10`)

`00/04`: version/profile; `08/0C`: coded width/height; `10/14`: rounded frame width/height; `18/1C`: scaled width/height; `20`: `constantZero`, one byte written as zero on success; `21..23`: padding not initialized by the getter; `24`: output pixel format. The original purpose of the extra byte is unknown, but its [zero-only behavior is established](codec-semantics.md).

### VP8 picture, 0x40 bytes (`0F`, `11`)

`00/04`: coded width/height; `08/0C`: frame width/height; `10/14`: luma/chroma strides; `18/1C`: luma pointer/bus address; `20/24`: chroma pointer/bus address; `28`: picture ID; `2C`: intra flag; `30`: golden-frame flag; `34`: concealed macroblocks; `38`: slice rows; `3C`: output layout byte plus padding. Strides select stored stride values when nonzero, otherwise the frame stride. Several picture metadata fields are explicitly zeroed in this build.

### VP6 info and picture, each 0x24 bytes (`15`..`17`)

Info: version, profile, frame width, frame height, scaled width, scaled height, scaling mode, `constantZero` byte at `1C`, padding at `1D..1F`, output pixel format at `20`. The byte is zero on success; the getter does not initialize the padding. Both VP6/VP8 replies copy their full records, including padding, and early errors do not guarantee initialized metadata.

Picture: frame width, frame height, output pointer, output bus address, picture ID, intra flag, golden-frame flag, concealed macroblocks, output layout byte plus padding. The observed next-picture routine zeroes picture ID, intra/golden flags, and concealed-macroblock count.

## Configuration transport

`1D` copies `0x11C` bytes from the current Hantro configuration. `1E` passes the mapped configuration directly to `PPSetConfig`. In both cases the explicit size word is not used to bound the configuration access; descriptor length is recovered mainly to build the returned descriptor. Do not interpret the caller-supplied size as a server-side structure-version negotiation.

The lower PP routine can overwrite the output addresses in the supplied configuration when multibuffer mode is active. Other state changes include keeping the previous configuration and installing standard color-transform coefficients in its internal copy. See [postprocessor.md](postprocessor.md).

## Multibuffer output

`1F` translates client VAs into bus addresses, saves both representations and calls `PPDecSetMultipleOutput`. The Hantro routine requires 1..17 entries, a linked decoder and nonzero luma addresses, with a legacy-product restriction. Its validation happens **after** the service wrapper's translation loop; the wrapper does not clamp that first loop to 17.

`20` asks PP for the next selected output, then matches its luma bus address to saved mappings and returns the corresponding client VA pair. It can rerun PP when a buffered picture's configuration ID differs from the current setup. It does not mean dequeue the next compressed input frame.

`21` replaces the output at PP's current display index if both current addresses match, with idle/combined/multibuffer checks. It is not inherently restricted to entry zero. The wrapper saves one extra replacement mapping on its first override only, so repeated overrides are not a general-purpose refresh of every VA mapping.

See [memory-and-results.md](memory-and-results.md) for static implementation defects and [methodology.md](methodology.md) for verification limits.
