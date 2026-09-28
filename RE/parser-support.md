# Parser metadata and codec support

This pass adds 30 function names/prototypes, bringing the inventory to 501. Twenty-eight additions have standalone Hantro source counterparts; two descriptive helpers correspond to register-setup sequences factored differently from the supplied source. An earlier SAR helper name was also corrected. Addresses are IDA virtual addresses. Service/hardware questions and platform glue remain last.

## VUI and HRD

`h264bsdDecodeVuiParameters` at `0x117A94` and `h264bsdDecodeHrdParameters` at `0x10EFA0` match `source/h264high/legacy/h264hwd_vui.c`. Evidence includes the complete syntax order, field offsets, end-of-stream checks, default constants, scaling shifts and error policy. Their types are the reference `vuiParameters_t` (952 bytes) and `hrdParameters_t` (412 bytes). `MvdH264Sps.vuiParameters` at `+0x58` now points to the former; the SPS remains 708 bytes. See the full offset maps in [database-layouts.md](database-layouts.md).

VUI contains aspect ratio and extended SAR, overscan, video format/range, color primaries/transfer/matrix, chroma sample positions, timing, two HRD records and bitstream restrictions. HRD holds three 32-entry arrays: scaled bit rates, scaled CPB sizes and constant-bit-rate flags. The three removal/output delay lengths are decoded as five-bit values plus one; time-offset length is stored directly.

Important observed behavior, also present in the reference:

* The VUI object is cleared before parsing. Absent video-signal information defaults to format 5 and color identifiers 2. Absent restriction information defaults to motion vectors over boundaries enabled, byte/bit denominators 2/1, horizontal/vertical motion-vector limits 16, reorder count 16 and frame-buffer limit 16.
* Absent NAL HRD uses rate/size defaults 288000001; absent VCL HRD uses 240000001. Both use CPB count 1 and delay lengths 24.
* Any nonzero return from an HRD parser makes the VUI parser return success immediately. Later VUI fields remain as initialized; success therefore does not prove that every optional syntax element was consumed.
* The binary omits the reference's optional pedantic checks for zero timing values and several restriction limits. Chroma sample locations still reject values greater than 5.
* HRD rejects a resulting CPB count greater than 32 and rejects raw rate/size values equal to `UINT32_MAX` before incrementing them. The subsequent scaled arithmetic is 32-bit and has no separate overflow check.

The SPS caller at `0x116F78` allocates exactly 952 bytes. A VUI return of `UINT32_MAX` forces the restriction flag and restores `maxDecFrameBuffering` to the level-derived DPB size; other nonzero results propagate. When restrictions apply, it rejects `numReorderFrames > maxDecFrameBuffering`, a limit below `numRefFrames`, or a limit above the level-derived DPB size. Zero is converted to one for the final DPB allocation limit. These are software parser behaviors, not evidence of hardware profile or bit-depth conformance.

## Aspect ratio and the corrected attribution

| Address | Correct source counterpart | Role |
|---|---|---|
| `0x114B8E` | `h264GetSarInfo`, `h264decapi.c` | Expand predefined IDC values into an output ratio; delegate IDC 255 |
| `0x1150C4` | `h264bsdAspectRatioIdc`, `h264hwd_decoder.c` | Read the active SPS/VUI aspect-ratio code, or return zero |
| `0x1189C0` | `h264bsdSarSize`, `h264hwd_decoder.c` | Return explicitly encoded width/height only for extended SAR, otherwise zero/zero |

The initial inventory had assigned `h264bsdSarSize` to `0x114B8E`. That was inaccurate: this routine has the predefined-ratio switch and calls the actual SAR-size accessor. The inventory and database names are corrected, and an explicit correction was appended to the old function comment.

The switch preserves the reference implementation's unusual literal results: IDC 6 returns 24:1 and IDC 7 returns 20:11. These are observed code values, not a claim that they are the intended H.264 standard ratios. Unknown IDC values yield zero/zero.

## SPS, picture order and slice groups

`DecodeMvcExtension` at `0x101EFA` matches the SPS helper in `legacy/h264hwd_seq_param_set.c`. It stores the two-view count/IDs in the SPS tail and consumes reference lists, applicable operations and optional MVC VUI syntax without retaining most of it. Several discarded-field reads and HRD calls do not propagate errors, just as in the reference. This parser's presence does not override the already documented clearing of advertised MVC support.

`GetDpbSize` at `0x102C34` implements the reference level/maximum-picture-size table. It returns `min(16, levelDpbBytes / (384 * picSizeInMbs))` for supported levels and valid picture sizes, with `0x7FFFFFFF` as the invalid result. This is distinct from the service work-buffer sizing helper.

`h264bsdDecodePicOrderCnt` at `0x116A5E` matches `legacy/h264hwd_pic_order_cnt.c`. Its four arguments are now typed as POC state, SPS, slice header and NAL metadata. It handles all three POC types, frame-number/LSB wraparound, reference/nonreference pictures, field/frame output and MMCO 5 resets. It writes the two-element POC array and previous-picture state; its source return type is void, replacing the decompiler's incidental integer return.

`DecodeBoxOutMap` (`0x101A3C`) and `DecodeForegroundLeftOverMap` (`0x101C20`) match the slice-group-map source. The former walks an expanding box in the requested direction until group 0 has the requested number of map units. The latter initializes all units to the leftover group and paints rectangular foreground groups in descending order. Their map/rectangle arguments are now pointer types.

`h264bsdInitMbNeighbours` (`0x118204`) fills the A/B/C/D pointers in each 160-byte macroblock record, nulling unavailable left, upper, upper-right and upper-left neighbors. `h264bsdInitStorage` (`0x1182A8`) clears 14864 bytes, sets invalid SPS/PPS IDs to 32/256 and sets the access-unit first-call flag.

`h264bsdValidParamSets` (`0x118C3C`) searches the 256 PPS slots for a PPS whose referenced SPS exists and passes `CheckPps`. It returns zero for success. `h264bsdCheckValidParamSets` (`0x1159C2`) converts that result into a positive Boolean. `h264bsdRbspTrailingBits` (`0x10AF3C`) consumes the remaining bits in the byte and reports end of stream; the optional source check of the actual stuffing pattern is absent.

## Scaling matrices and data attribution

`ScalingList` (`0x1091FC`) and `FallbackScaling` (`0x102BA8`) match the SPS source. The argument is now a pointer to 64-byte rows, covering eight lists. The first six lists use 16 values, the last two 64. Explicit deltas wrap modulo 256; a first zero scale selects the corresponding default. The signed Exp-Golomb reader's error status is discarded in both the source and binary. Fallback selects the default at indices 0, 3, 6 and 7, and copies the preceding 4x4 list for the other indices.

All six tables below were compared byte-for-byte against source initializers, then named and typed as constant `u32` arrays:

| Address | Name | Entries |
|---|---|---:|
| `0x11E194` | `default4x4Intra` | 16 |
| `0x11E1D4` | `default4x4Inter` | 16 |
| `0x11E214` | `default8x8Intra` | 64 |
| `0x11E314` | `default8x8Inter` | 64 |
| `0x11E414` | `zigZag4x4` | 16 |
| `0x11E454` | `zigZag8x8` | 64 |

## PP and reference-buffer support

`PPCheckTiledOutput` (`0x106A2C`) matches `pp/ppinternal.c`: only the four packed tiled-4x4 4:2:2 formats are accepted; output dimensions and enabled mask origins/sizes must be multiples of four; an enabled output framebuffer is rejected. Later failures can replace an earlier error result. `PPContinuousCheck` (`0x10D2FA`) checks that RGB bitmasks have one contiguous run of set bits; zero is accepted.

`PPDecSetOutBuffer` (`0x106D88`) selects the indexed output addresses and records setup ID plus input addresses for reruns. MVD additionally saves both bottom-field addresses, matching the previously recovered 20-byte buffer record. `PPDecWaitResult` (`0x1074A0`) rejects null/unlinked instances, waits only while PP is running, and otherwise returns `PP_BUSY`.

`MvdPpSetDecoderInput` (`0x106E3C`) has a descriptive name because its equivalent code is embedded inside the reference `PPDecStartPp`. It sets picture structure and all four input base addresses. For monochrome input, each chroma base is set to its corresponding luma base. Both this helper and `PPDecSetOutBuffer` subtract 240 bytes from the PP register-array pointer when using global decoder-register ordinals: PP begins at word 60. Hex-Rays can display this arithmetic as `pp[-1]`; it does not establish access to a previous container.

`h264PpMultiFindPic` (`0x114C30`) searches the sent-picture pointer array through `multiMaxId`, returning the matching index or the first index past that limit. `h264UseDisplaySmoothing` (`0x114EE6`) returns the Boolean value of `storage.useSmoothing`.

Four functions match `common/refbuffer.c`: `InitMemAccess` (`0x1057DC`), `UpdateMemModel` (`0x109298`), `GetSettings` (`0x102D54`) and `DecideParityMode` (`0x101A10`). They initialize per-format memory costs, calculate buffering-cycle costs, predict useful coverage/vertical offset and decide whether opposite-field parity is worthwhile. `GetSettings` disables reference buffering at widths of 16 macroblocks or less, compares predicted savings to buffering cost and quantizes the signed vertical offset to multiples of 16. These are software heuristics, not measured memory timings.

`vp8hwdResetProbs` (`0x1195C0`) matches `vp8/vp8hwd_probs.c`: reset intra-mode probabilities, choose 19-entry VP8 or 17-entry VP7 motion-vector contexts, then copy the 4×8×3×11 coefficient defaults. `VP6HwdAsicReleaseMem` (`0x10CBE0`) matches the probability-table release/descriptor-clear helper in `vp6/vp6hwd_asic.c`.

## Common register initialization and alias 19

`MvdInitDecoderRegisters` (`0x10DD30`) is called by H.264, VP6 and VP8 initialization. It factors register setup that is repeated within the reference codec initializers. Its descriptive name does not claim an exact standalone source symbol.

The setup selects output endian 1, input endian 0, stream endian 1, maximum burst 16, advanced prefetch enabled with threshold 8, all three 32-bit swaps enabled, hardware timeout enabled, internal clock gating disabled, IRQ delivery enabled and zero AXI IDs. The priority field is used for product `0x8170`; other products use single-command-disable. It also clears latency, data discard and unresolved ordinal 598.

Disassembly at `0x10DD96` proves ordinal 19 is written, between latency and output swap. Its triple is word 2, width 1, shift 18, identical to ordinal 20. The complete sequence matches the reference `HWIF_DEC_DATA_DISC_E` operation, supplying semantic evidence beyond the duplicate triple. Ordinal 19 is now `MVD_HWIF_DEC_DATA_DISC_E_ALIAS`. There are 725 named register fields; ordinals 8, 10, 128, 282 and 598 remain unnamed. A composite enum rendering of ordinal 598 must not be interpreted as known semantics.

## Verification and remaining work

All 30 new function names and the corrected SAR name were read back, along with the four PP callback names vulnerable to type-propagation renaming. IDA accepted all final prototypes. HRD/VUI/SPS sizes are 412/952/708; H.264 storage/container, PP container, VP8 parser and VP6 container retain their previous sizes. The six scaling tables total 960 bytes and match the source exactly. The database was saved; no firmware instructions, MMIO placeholders or reference files were modified.

There are 266 `sub_*` names across the 797-function database. This is not a count of remaining codec algorithms: the largest remaining functions are now concentrated in platform/service/runtime regions, and the remaining inventory needs classification. The H.264 storage word at `+0x39DC`, unused capability word and extra VP6/VP8 info byte remain unresolved. The five register fields above, remaining data attribution and custom-code readability are the next static-analysis work. L2B behavior, High10 execution and runtime concealment-defect reachability remain deferred, followed by remaining platform/SDK/runtime glue.
