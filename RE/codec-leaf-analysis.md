# Codec parsing, reconstruction and concealment

This pass follows the requested order: source attribution, internal layouts and register semantics first; service/hardware uncertainties and platform glue remain deferred. It adds 105 function names/prototypes to the previous 316-entry inventory, for 421 documented functions. Six additions are descriptive VP8 concealment names without an identified counterpart in the supplied source revision. The others match Hantro routines by behavior, constants, call relationships and field accesses, sometimes with branch-specific differences. This is not an exact-release attribution.

## H.264 source coverage

The source match now follows `h264bsdDecode` through NAL extraction, parameter-set activation, access-unit detection, slice headers, reference-list commands, slice-group maps, CAVLC macroblocks and hardware RLC preparation. Representative anchors are:

| Binary | Function | Reference below `source/h264high/` |
|---|---|---|
| `0x117E0E` | `h264bsdExtractNalUnit` | `legacy/h264hwd_byte_stream.c` |
| `0x116994` | `h264bsdDecodeNalUnit` | `legacy/h264hwd_nal_unit.c` |
| `0x1150E0` | `h264bsdCheckAccessUnitBoundary` | `h264hwd_storage.c` |
| `0x114EF8` | `h264bsdActivateParamSets` | `h264hwd_storage.c` |
| `0x116C68` | `h264bsdDecodePicParamSet` | `legacy/h264hwd_pic_param_set.c` |
| `0x117610` | `h264bsdDecodeSliceHeader` | `legacy/h264hwd_slice_header.c` |
| `0x1090B8` / `0x1018A4` | `RefPicListReordering` / `DecRefPicMarking` | `legacy/h264hwd_slice_header.c` |
| `0x117338` | `h264bsdDecodeSliceData` | `h264hwd_slice_data.c` |
| `0x11686C` / `0x1166C0` | `h264bsdDecodeMacroblockLayerCavlc` / `h264bsdDecodeMacroblock` | `h264hwd_macroblock_layer.c` |
| `0x10EC20` | `h264bsdDecodeResidualBlockCavlc` | `h264hwd_cavlc.c` |
| `0x111AF8` | `WriteRlcToAsic` | `h264hwd_macroblock_layer.c` |
| `0x108CFC` / `0x108EB2` | `PrepareInterPrediction` / `PrepareIntraPrediction` | `h264hwd_inter_prediction.c` / `h264hwd_intra_prediction.c` |
| `0x118864` / `0x1155E0` | `h264bsdReorderRefPicList` / `h264bsdCheckGapsInFrameNum` | `h264hwd_dpb.c` |

The four CAVLC table readers were independently compared: `DecodeCoeffToken` (`0x101B50`), `DecodeLevelPrefix` (`0x101C8A`), `DecodeTotalZeros` (`0x1024F8`) and `DecodeRunBefore` (`0x102258`). The nC ranges, lookahead shifts, threshold constants, chroma-DC special case and packed value/length results agree. `DecodeLevelPrefix` returns `0xFFFFFFFE` when no prefix is found; the residual-block routine returns `0xFFFFFFFF` on stream error/end. These distinct internal sentinels are not service result codes.

Macroblock prediction, residual and RLC structures now replace byte arrays. A macroblock-layer record is 1124 bytes, including 468 16-bit RLC words and 28 coefficient-count bytes. Persistent macroblock state is 160 bytes with 16 short motion vectors and four neighbor pointers. Slice headers contain two 276-byte reordering records, each with 17 commands, and a 716-byte reference-marking record with 35 MMCO entries. Array bounds here describe storage capacity, not accepted command counts for every bitstream.

### Access-unit boundary variant

MVD's access-unit state is **76 bytes**, compared with 72 in the reference layout. An extra word at offset `0x24` follows `prevFrameNum`. In `h264bsdDecode`, it is assigned:

```c
aub.maskedPrevFrameNum = aub.prevFrameNum & ~decoder->workarounds.h264.frameNumMask;
```

`h264bsdCheckAccessUnitBoundary` compares the new frame number with both stored values. The frame-number difference contributes to a boundary only when it matches neither. Other checks still include field flags, reference/non-reference status, picture-order values, IDR status/ID and the relevant view state. The extra member is named `maskedPrevFrameNum`. The later [DPB/prediction pass](h264-dpb-prediction.md) traced it to an in-place bit-12 patch and the G1 build gate; the exact silicon fault remains unknown.

NAL headers, access-unit state, macroblock payload and slice commands are typed throughout `MvdH264Storage`. The 14864-byte storage and 15860-byte container sizes remain unchanged. The storage word at offset `0x39DC` (`unresolvedWord3703`) remains unidentified.

### Prototype and behavioral cautions

* `h264bsdExtractNalUnit` can compact away emulation-prevention bytes in place on the RLC path; its applied input pointer is mutable
* `h264bsdCompareSeqParamSets` can copy scaling-list data into the stored SPS, despite its comparison-like name
* `h264bsdGetRefPicData` returns a reference slot index or `-1` in this build, rather than a data pointer
* `h264bsdCheckPriorPicsFlag` uses four arguments in the binary; the reference's additional NAL argument is unused. `PredWeightTable` uses three; the initial decompilation's fourth argument was spurious
* Small enum fields in macroblock records are bytes, with explicit padding before word members. Importing the source with default four-byte enums would produce wrong offsets

## VP8 entropy recovery and concealment

The boolean coder, VP7/VP8 frame-header parsers, segmentation, loop-filter adjustments, coefficient/motion-vector probability updates and VP7 scan preparation have source counterparts in `source/vp8/vp8hwd_bool.c`, `vp8hwd_headers.c` and `vp8hwd_probs.c`. Their names and prototypes are applied. Software VP7 parsing remains present even though the supplied reference capability state disables VP7 initialization.

The final word of `MvdVp8Decoder`, at offset `0xA30`, is now `coeffProbsDecoded`. `DecodeVp8FrameHeader` (`0x1028C0`) sets it after coefficient-probability updates succeed, before the remaining skip/mode/motion-vector header fields. It does **not** mean that the whole frame header was successfully parsed.

`vp8hwdFreeze` (`0x1193E4`) increments picture bookkeeping, selects the previous reference for output and disables PP pipelining. If coefficient probabilities were decoded and entropy refresh is disabled, it restores the saved entropy block and VP7 scan order. Otherwise, when hardware concealment is supported and an entropy refresh has been seen, it latches `forceFreezeUntilKeyFrame`. It also clears the previous motion-vector buffer when present. This extends the source counterpart and explains the previously opaque parser word and container flags.

The final 40 bytes of `MvdVp8Container` now contain concealment-active state, restart X/Y, previous-key-frame state, the freeze latch, entropy-refresh history and a 16-byte workspace descriptor. Full offsets are in [database-layouts.md](database-layouts.md).

### Concealment helpers without a local source match

| Address | Applied descriptive name | Role |
|---|---|---|
| `0x1194A0` | `MvdVp8InitConcealment` | Initialize dimensions/count and allocate accumulator workspace |
| `0x119568` | `MvdVp8ReleaseConcealment` | Release the accumulator pointer |
| `0x10BE30` | `MvdVp8RunConcealment` | Generate motion vectors, configure a concealment run and clear the active flag afterward |
| `0x119150` | `MvdVp8ConcealMotionVectors` | Temporal extrapolation and spatial replacement |
| `0x10AE14` | `MvdVp8AccumulateConcealmentVector` | Bounds-check a 4×4-block location and add weighted vector components |
| `0x114904` | `MvdVp8CollectNeighborVectors` | Collect up to 20 surrounding vectors and choose reference ID 0, 4 or 5 |

The `Mvd` prefix records that these exact functions were not located in the supplied reference revision. They may come from another Hantro branch; it does not establish Nintendo authorship.

Initialization allocates `36 * (width >> 4) * (height >> 4) * vectorsPerMb` bytes; the traced caller uses 16 vectors per macroblock. Each 36-byte accumulator has three weights and three signed X/Y sums. Packed hardware vectors contain signed X in bits 31:18, signed Y in bits 17:5, and reference ID in bits 2:0. Arithmetic shifts in the disassembly establish the signed extraction; bits 4:3 are not assigned a meaning here.

In extrapolation mode, the routine clears the workspace and processes previous-frame macroblocks whose reference ID is zero. Each 4×4 vector is projected by the negative motion vector. The low fractional components select weights for up to four adjacent accumulator positions. The weighted sums later produce last-reference vectors for the damaged region; zero weight produces a zero vector.

The spatial pass replaces macroblocks tagged with reference ID 1. It collects four edge groups and four corners, votes among reference IDs 0/4/5, averages vectors using the selected reference, and writes that vector into all 16 subblocks. Extrapolation mode performs this pass before the restart macroblock, then fills the remainder from temporal accumulators. Without extrapolation, the spatial pass begins at the restart macroblock and extends to the end. The vote favors ID 0 on ties involving 0, then ID 4 over 5 on their tie.

`MvdVp8RunConcealment` clears the key-frame flag, generates vectors, sets `concealmentActive`, optionally resets restart X/Y to zero, then invokes picture setup, stream-position setup and the ASIC runner. Picture setup disables writing new motion vectors and points `DIR_MV_BASE` at the generated buffer. Field 579, word 48 bits 13:12, is zero normally and one during this path. It is now named `MVD_HWIF_VP8_CONCEALMENT_MODE`; values 2 and 3 remain unknown.

### Disassembly findings and limits

* At `0x1192A2`, Hex-Rays emits `MEMORY[4]` / `MEMORY[0xC]` in the non-extrapolation path. The actual loads use the preserved state pointer in R5. Comments record this decompiler error
* At `0x119236`, the conditional weighted-neighbor updates depend on either fractional component being nonzero, not only X
* The neighbor collector clears a 32-byte reference histogram. Hex-Rays still splits it into a four-word array and separate reference-4/reference-5 locals; the attempted aggregate local type was not accepted. Descriptive names and a comment preserve the verified layout without claiming the decompiler representation is fixed
* The lower-left vote has a real missing boundary guard in the disassembly: when X is nonzero and the row is outside the allowed lower-neighbor range, the slot remains `0xFFFFFFFF`, yet `0x114AE8` still indexes the histogram with it. The shifted index is `-4`, so the increment addresses the word immediately below SP. This is an observed conditional out-of-bounds access, not a tested exploit or an established end-user trigger

The supplied GBATEK reference decodes to hardware error-concealment support zero. These routines therefore do not prove this path executes on that reference hardware. No hardware decode or concealment experiment was performed.

## VP6 entropy and Huffman code

Source attribution now covers mode probabilities, motion-vector entropy updates, scan-order construction, context updates, boolean readers, probability-to-Huffman conversion, sorted-node insertion, tree construction and lookup-table generation. Counterparts are in `source/vp6/vp6dec.c`, `vp6decodemode.c`, `vp6decodemv.c`, `vp6scanorder.c`, `vp6booldec.c`, `vp6strmbuffer.c` and `vp6huffdec.c`.

`VP6HWAllocateHuffman` (`0x109924`) allocates `0x11C0` bytes, matching the recovered 4544-byte `MvdVp6Huffman`. It contains DC/AC/zero-run probabilities and trees, then the hardware lookup tables. Nodes are four bytes: two 16-bit token-or-pointer records. The selector is bit 0 and the token/index occupies bits 7:1. Sorted work nodes are 12 bytes. These layouts match the observed shifts, strides and table offsets, not merely the source field order.

`VP6HW_BuildHuffTree` (`0x10C9E0`) uses `InsertSorted` (`0x1058F0`) to combine weighted nodes. `VP6HW_CreateHuffmanLUT` (`0x10C9BE`) uses recursive `ProcessTreeNode` (`0x10CD8E`) to emit 16-bit entries with code and length packed into them. `VP6HW_ConvertDecodeBoolTrees` (`0x10A3DC`) drives the DC/AC/zero-run conversions. The parser's `huff` pointer now has this type; the 1480-byte parser size is unchanged.

`VP6HWConfigureMvEntropyDecoder` consumes only the parser pointer in this binary; the reference's extra frame-type parameter is unused. The applied prototype reflects the actual argument use.

## Remaining codec work

The later [DPB/prediction pass](h264-dpb-prediction.md) maps the main DPB marking/output routines, reference lists, intra-prediction leaves and neighbor helpers, and explains the workaround-mask mechanism and build gate. The extra H.264 storage word remains unresolved, and remaining unnamed parser/support routines still need an inventory sweep. Six register ordinals remain unnamed, with alias constraints in [register-fields.md](register-fields.md). The current 471-entry inventory is a progress record within a 797-function database, not a claim that every other function is unnamed or that all codec behavior is understood.

Service/hardware uncertainties, including L2B format behavior and runtime High10 support, and the remaining platform/SDK work are still last in the requested ordering. No executable code, reference source or MMIO contents were modified.
