# H.264 DPB, prediction and frame-number workaround

This pass adds 50 names/prototypes to the previous 421-entry inventory, for 471 documented functions. Forty-nine have identifiable Hantro counterparts; `MvdH264PatchFrameNumBit12` is a descriptive name for code absent from the supplied reference revision. The service/hardware questions and remaining platform work were left for last as requested. Addresses below are database virtual addresses.

## DPB allocation and lifetime

| Address | Applied name | Role |
|---|---|---|
| `0x118084` | `h264bsdInitDpb` | Allocate reference pictures, assign backing-buffer indices and allocate output records |
| `0x11894C` | `h264bsdResetDpb` | Reuse compatible allocation or free/reinitialize it |
| `0x10DC52` | `h264bsdFreeDpb` | Release reference allocations and the output-record array |
| `0x10D4FE` | `DpbBufFree` | Decrement reference/fullness counts when a picture becomes unused |
| `0x10AED0` | `OutBufFree` | Clear output ownership and recycle unowned backing buffers when smoothing is enabled |

These match `source/h264high/h264hwd_dpb.c`. Initialization clears the 1680-byte DPB, clamps the requested maximum reference count to at least one, sets the long-term-index sentinel to `0xFFFF`, and records `prevOutIdx = 0xFF`. With output reordering disabled, DPB size is the clamped reference count; otherwise it is the requested level-derived size.

Backing-buffer count begins at `dpbSize + 1`. Display smoothing adds one buffer for non-reordered output, or another `dpbSize + 1` buffers for reordered output. Without smoothing, multibuffer PP adds one. The first `dpbSize + 1` backing buffers belong to DPB slots; extras enter the free-buffer array. The ownership bits are 1 for DPB and 2 for output.

For high-profile-capable decoding, each picture payload is `macroblocks * (sampleBytes + 64)`, where `sampleBytes` is 256 for monochrome or 384 otherwise. The extra 64 bytes per macroblock hold direct motion vectors. Without that capability, the payload uses 384 bytes per macroblock. A requested second chroma output adds 128 bytes per macroblock for non-monochrome output, after recording its offset.

Two differences from the supplied source are explicit in the binary: each reference-buffer allocation requests **payload + 32 bytes**, and there is **no explicit clear of the allocated output-record array** after `DWLmalloc` returns. The output array uses 36-byte records. These observations do not establish a purpose for the extra 32 bytes or the contents returned by the allocator.

Reset avoids reallocating when picture area and effective DPB size agree, while updating maximum references, maximum frame number, no-reordering state, the long-term sentinel and `flushed`. Allocation failure returns the internal `0xFFFF` value. The functions named `DpbBufFree` and `OutBufFree` perform bookkeeping, not general heap frees; the separate platform allocator behavior remains documented in [platform-glue.md](platform-glue.md).

## Reference status and ABI corrections

The two status bytes in each 52-byte DPB picture have these source-confirmed values:

| Value | Meaning |
|---:|---|
| 0 | UNUSED |
| 1 | NON_EXISTING |
| 2 | SHORT_TERM |
| 3 | LONG_TERM |
| 4 | EMPTY |

Field selector 0 means top, 1 bottom and 2 frame. `IsReference` requires both field statuses to be references for a frame selector; `IsReferenceField` accepts either field. `IsShortTerm` includes NON_EXISTING as well as SHORT_TERM. `IsExisting` excludes NON_EXISTING, UNUSED and EMPTY. `IsLongTerm`, `IsLongTermField`, `IsShortTermField` and `IsUnused` now have corresponding pointer/selector prototypes.

An unusual source ABI is preserved: **`IsReference` at `0x10F5D0` takes a DPB picture by value**, not by pointer. Its first 16 bytes arrive in R0–R3 and its remaining 36 bytes on the stack; the field selector follows at caller SP+36. The prologue pushes R0–R3 to obtain a contiguous local picture. This explains the large copies in comparison routines. Fresh decompilation still sometimes splits the copied aggregate among overlapping temporaries; those are decompiler artifacts, not additional arguments or firmware copies to unrelated objects.

`FindDpbPic` (`0x10D54C`) uses four arguments. Disassembly showed the saved R3 field selector being passed to status predicates even when the initial pseudocode omitted it. Short-term lookup compares `frameNum`; long-term lookup compares `picNum`; missing pictures return `-1`.

The applied binary interfaces omit unused source arguments from `SlidingWindowRefPicMarking`, `Mmcop4`, `Mmcop5` and `h264DpbUpdateOutputList`. Callers may still place unused source arguments in registers. That does not make them consumed arguments in these implementations.

## Marking and memory-management operations

`h264bsdMarkDecRefPic` (`0x118394`) handles non-reference, IDR, adaptive marking and sliding-window cases. It also distinguishes the second field of a picture, coordinates delayed output/smoothing, and writes picture ID, error count, IDR, tiled-mode and field-picture metadata.

| Operation | Binary implementation | Action |
|---:|---|---|
| 1 | `Mmcop1`, `0x105D1C` | Mark the selected short-term picture/field unused |
| 2 | Inlined in `h264bsdMarkDecRefPic` | Find a long-term picture/field and mark it unused |
| 3 | `Mmcop3`, `0x105D98` | Remove a conflicting long-term assignment and convert a short-term reference |
| 4 | `Mmcop4`, `0x105E78` | Set the long-term-index limit and invalidate references beyond it |
| 5 | `Mmcop5`, `0x10AE80` | Mark references unused, drain displayable pictures and reset reference numbering |
| 6 | `Mmcop6`, `0x105F14` | Assign a long-term index to the current picture/field |

The field forms convert doubled picture numbering into a frame number and field parity. MMCO 3 rejects non-existing pictures via `IsExisting`. MMCO 1/2/3 results are discarded by the parent, as in the supplied source; this should not be described as propagation of every helper error.

The sliding-window helper removes the oldest short-term reference when the maximum is reached. `SetStatus` (`0x10D53E`) changes one field or both. `SetPoc` (`0x10D3A4`) similarly updates one or both picture-order counts. `GetPoc` (`0x10D380`) substitutes `INT_MAX` for empty fields and returns the minimum.

### Confirmed source difference in MMCO 6

At `0x105FBC`, unsigned `BHI` rejects `numRefFrames > maxRefFrames`. Equality therefore reaches the insertion path, whereas the local C source requires strict `<` before insertion. The binary's parent additionally checks for `numRefFrames > maxRefFrames` after adaptive marking and reports failure. The helper can already have modified state before that parent check. This is a static behavioral difference; no malformed-stream experiment or externally observable consequence is claimed.

## Sorting, hardware reference lists and display output

`ShellSort` (`0x10DAD0`) and `ShellSortF` (`0x10DB60`) use gaps 7, 3 and 1 over `dpbSize + 1` indices. Their comparison helpers now have signed return types:

* `ComparePictures` / `CompareFields`: short-term references by descending picture number, then long-term references by ascending long-term number; the frame comparator additionally orders non-reference pictures needed for display
* `ComparePicturesB` / `CompareFieldsB`: short-term references around the current POC, followed by long-term references. The field comparator uses available short-term fields and an inclusive `<= currentPoc` partition, while the frame comparator uses `< currentPoc`

`H264InitRefPicList` (`0x10452C`) constructs B-list 0, B-list 1 and the P list, programs the corresponding register fields and copies P-list indices into the DPB and its backup for error handling. Its source counterparts `H264InitRefPicList1` (`0x1047E4`) and `H264InitRefPicList1F` (`0x1048D4`) rearrange POC partitions to form list 1. The frame helper swaps the first two entries when the active lists would otherwise be identical. The source's MVC branches remain present in these helpers despite MVD's separately documented capability filtering.

`OutputPicture` (`0x10D3BC`) contains the source `FindSmallestPicOrderCnt` scan inline. It selects the displayable picture with the smallest available-field POC, clears `toBeDisplayed`, copies metadata into a 36-byte output record, and sets output ownership. It reports incomplete field pictures through `fieldPicture` and `topField`. The ring has `dpbSize + 1` entries; on overflow it advances the read index and discards the oldest queued record, matching the source's damaged-MVC recovery branch.

`h264DpbUpdateOutputList` (`0x10B20A`) immediately queues the current picture in no-reordering mode; otherwise it emits pictures while DPB fullness exceeds capacity. It can swap the extra current-picture slot into an unused reference slot so the normal reference range remains usable. `h264DpbAdjStereoOutput` (`0x114B58`) reconciles view output counts using the source's queue adjustment.

`h264bsdDpbOutputPicture` (`0x117DC4`) returns null when the queue is empty or `noOutput` is set. Otherwise it advances the ring, records the backing-buffer index, calls `OutBufFree`, and returns the queued record. This completes the link to the already named `h264bsdNextOutputPicture` wrapper.

## Intra prediction and compact neighbor tables

`Intra16x16Prediction` (`0x105936`), `Intra4x4Prediction` (`0x105A9C`), `DetermineIntra4x4PredMode` (`0x102B44`), `CheckIntraChromaPrediction` (`0x1013E4`) and `GetIntraNeighbour` (`0x10D5FA`) match `source/h264high/h264hwd_intra_prediction.c`.

They validate neighbor availability, apply constrained-intra restrictions, derive the predicted 4×4 mode, and assemble ASIC macroblock control words. In this hardware decoder these helpers prepare mode/control state; their names do not imply that they reconstruct all output pixels in software. Chroma mode 0 needs no neighbor, mode 1 needs A, mode 2 needs B, and mode 3 needs A/B/D. The 16×16 mode is derived from the macroblock type by `h264bsdPredModeIntra16x16` (`0x11885C`).

`MvdH264Neighbour` is exactly two bytes: a one-byte macroblock selector followed by the block index. Selectors 0/1/2/3 identify A/B/C/D, 4 the current macroblock, and `0xFF` an unavailable neighbor. `h264bsdGetNeighbourMb` (`0x10D5CE`) maps those selectors to pointers. `h264bsdIsNeighbourAvailable` (`0x10EB82`) requires a non-null pointer with the same slice ID.

Three constant tables were compared byte-for-byte with the source initializers and typed as `const MvdH264Neighbour[24]`:

| Address | Applied table name | Accessor |
|---|---|---|
| `0x11C8A8` | `N_A_4x4B` | `h264bsdNeighbour4x4BlockA`, `0x10EBA4` |
| `0x11C8D8` | `N_B_4x4B` | `h264bsdNeighbour4x4BlockB`, `0x10EB98` |
| `0x11C908` | `N_D_4x4B` | `h264bsdNeighbour4x4BlockD`, `0x118838` |

Each covers 16 luma blocks, four Cb blocks and four Cr blocks. The reference's C accessor/table are under `#if 0`; no corresponding binary function was invented. `PrepareInterPrediction`, already identified in the previous pass, resolves references through `h264bsdGetRefPicData` and writes partition/filter/neighbor/RLC controls; its direct callees are now named.

## Frame-number bit-12 workaround

`InitWorkarounds` (`0x105824`) is a counterpart of `source/common/workaround.c`, with an H.264 extension absent from that checkout. Decoder mode zero enables the H.264 workaround by default. For product `0x6731`, build `>= 0x2390` disables it. Thus the supplied **reference** ID `0x67312398` selects the disabled path. This is an inference from the provided dump and the binary gate, not a live register read.

`H264DecInit` sets the frame-number mask to `0x1000` when enabled. The old four-word array in the container is now a 16-byte `MvdDecoderWorkarounds` union, with an H.264 view containing `enabled`, `reservedWord1`, `frameNumMask` and `annexBPatchActive`. The second H.264 word is initialized to zero but has no established H.264 role; it overlaps the MPEG start-code flag in the shared union. The MPEG and RV fields have source counterparts.

`MvdH264PatchFrameNumBit12` (`0x117F4E`) has no located counterpart in the local reference and uses a descriptive name. It:

1. Exits unless the supplied frame number has bit 12 set, and derives its encoded width from `maxFrameNum`
2. Recognizes an initial Annex B start-code prefix; when prefixed, it can scan past other NAL types to type 1, 5 or 20
3. Skips the one-byte NAL header, or four bytes for type 20, and decodes three unsigned Exp-Golomb fields before looking at the fixed-width frame number
4. Requires that value to equal the supplied frame number, then clears its encoded bit 12 **in the caller's stream buffer**

The bit location is `(bitPosInWord + frameNumBitWidth - 13)`, relative to the current stream pointer, with an MSB-first byte mask. Disassembly at `0x118008`–`0x118022` confirms the in-place byte write. The applied input pointer is mutable.

The return value is not simply “a byte was changed”: a raw NAL can be patched and still return zero. It returns one only after a successful patch when the input initially had a start-code prefix. `initialStartCodeBytes` reports that initial prefix length, not the eventual slice's position after scanning. The caller uses the flag for stream-base/length adjustment and later start-code recovery.

The access-unit boundary mask previously recovered in [codec-leaf-analysis.md](codec-leaf-analysis.md) now has an explanation: software accepts the previously parsed frame number or its bit-12-cleared form while the hardware-facing stream can be modified. The exact silicon defect motivating this workaround remains unknown.

A stack word set to one participates in an OR check after the first Exp-Golomb read. It must not be misread as a requirement that `first_mb_in_slice` be nonzero: the saved one makes that check true in the observed path. Hex-Rays merges this word and the output value into an oversized local array; a comment records the disassembly finding.

## Validation and remaining work

The two-byte neighbor record, 16-byte workaround view/union, 52-byte DPB picture, 1680-byte DPB, 14864-byte storage and 15860-byte container were read back from IDA. Names/prototypes, three typed tables and comments were applied; ten locals were renamed in the unmatched bit-patching helper. Firmware instructions and stream data in the database were not patched.

The remaining codec task is an inventory sweep of still-unnamed functions and data, especially parser/support routines outside this traced path. The database still has 296 `sub_*` names across codec and non-codec code. H.264's storage word at `+0x39DC` remains unknown; a field-xref query returned none, which is insufficient to call it unused. The unused capability word, extra VP6/VP8 info byte and six unnamed register ordinals remain open. The precise hardware fault behind the now-understood frame-number workaround is also unknown. Hardware conformance, L2B semantics and remaining platform/SDK work remain last.
