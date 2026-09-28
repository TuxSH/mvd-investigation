# Function and constant attribution coverage

**Current update:** the [platform/SDK/runtime pass](platform-sdk-runtime.md), with [heap analysis last](heap-runtime.md), closes all 170 entries left after the auxiliary-driver pass and resolves the last two deferred data heads. The database now has 800 functions and zero `sub_*` names. Exact SDK/runtime source identities remain qualified. The sections below retain the earlier audit checkpoint.

The remaining-function sweep found no additional routine that this pass can confidently name as Hantro codec code. All 265 remaining `sub_*` entries were decompiled and reviewed with their reference context. They fall into service/driver, SDK, startup and runtime work queues, detailed in [remaining-functions.md](remaining-functions.md). This is a classification of existing entries, not proof that every instruction or indirect target has been identified.

At this audit checkpoint the database had **796 function entries**, **265 `sub_*` names** and **501 documented names/prototypes**. These are different inventories: some useful names predate the documented attribution work. The 265 entries were deliberately left unnamed pending the requested final platform phase; inventing exact SDK symbols or applying ordinary C prototypes to unfamiliar runtime ABIs would overstate the evidence.

## Scope and method

1. Enumerate every IDA function entry and select every name beginning `sub_`, without relying on proximity to a codec or a codec-name substring. All 265 decompiled successfully. Review bodies alongside incoming references and outgoing code references; these include address-taken references and branches, not only calls.
2. Inspect constant references from **all 501 functions in function-map.md** into the complete `.rodata` range `0x11A000..0x121000`. Resolve interior addresses to their IDA item heads before deciding whether a table is already named. This avoids counting interior references into `mcFilter` or `vp71FeatureBits` as new arrays.
3. Follow the nine remaining auto-named data heads to their consumers. Two are codec-related arrays, attributed below; seven are deferred platform/service data. Match full layouts and the source initializer where available, rather than accepting a repeated short byte pattern.
4. Revisit the eight earlier source-table nonmatches using source references and conditional compilation. Their disposition is recorded in [codec-data.md](codec-data.md).

This scan does not enumerate every instruction-immediate constant, literal pool, address synthesized at runtime, global in writable storage, or object outside the documented function inventory. Nor does successful decompilation validate every inferred argument or function boundary. The result closes the broad *existing unnamed-function triage* task and supplies a concrete deferred queue; it does not claim complete binary attribution.

## Two additional typed tables

| Address | Applied declaration | Bytes | Evidence |
|---|---|---:|---|
| `0x11E554` | `const u32 *const h264ScalingListDefaults[8]` | 32 | Compiler-emitted initializer for source `ScalingList`'s local `defList[8]` |
| `0x11A0D0` | `const MvdH264LevelLimit g_mvdH264LevelLimits[17]` | 204 | Three-word records read by `MvdMaxDpbFramesForLevel`; values already documented in the work-buffer analysis |

The first has an exact source counterpart in `source/h264high/legacy/h264hwd_seq_param_set.c:117`. Its eight pointers select `default4x4Intra` (`0x11E194`) three times, `default4x4Inter` (`0x11E1D4`) three times, `default8x8Intra` (`0x11E214`) and `default8x8Inter` (`0x11E314`). `ScalingList` (`0x1091FC`) copies all 32 bytes at `0x10920E`, selects `defList[index]` at `0x10925C`, and reads its elements at `0x109286`. The former `s32 dst[8]` stack local is now the source-compatible `const u32 *defList[8]`; regenerated pseudocode displays pointer indexing instead of integer-address arithmetic. The global name describes emitted initializer storage, not a claimed global identifier in Hantro's source.

The second is a descriptive MVD work-size table, not a newly proven Hantro source match. `MvdH264LevelLimit` is 12 bytes: `u32 levelIndex`, `u32 maxFrameMbs`, `u32 maxDpbMbs`, at offsets 0/4/8. The first member contains 0..16, matching the local libctru `MVD_H264_LEVEL_*` index vocabulary. The helper at `0x119D78` indexes by the caller's level and reads members 4/8; it does not compare the stored index or bounds-check the incoming one. The complete values and sizing formula remain in [memory-and-results.md](memory-and-results.md). IDA now displays `levelLimits->maxFrameMbs` and `levelLimits->maxDpbMbs`, with descriptive `pictureMbs`, `levelLimits` and `dpbFrames` locals. This improves representation of an existing finding rather than changing the documented formula.

Together these add 236 bytes. The three constant-data passes now contain **88 newly annotated arrays covering 15,802 bytes**; this count includes source-derived and descriptive arrays and excludes previously named tables rechecked during those passes.

## Deferred data heads from the reference audit

These seven heads remain in the platform queue. Their current IDA item sizes are not accepted object extents; no speculative array sizes or prototypes were applied.

| Head | Observed context | Remaining work |
|---|---|---|
| `0x11A000` | L2B interrupt-ID bytes `45 46`; initial DMA-ID classification corrected by BindInterrupt consumers | Resolved as `g_l2bInterruptIds[2]` in the driver pass |
| `0x11A004` | L2B process pseudo-handle | Resolved as `const Handle g_l2bCurrentProcessPseudoHandle`, value `0xFFFF8001` |
| `0x11A014` | Zero handle passed by service/decoder wrappers | Resolved as a separate four-byte `g_mvdNoClientProcessHandle`; no adjacent extent claimed |
| `0x11A088` | Y2R process pseudo-handle | Resolved as `const Handle g_y2rCurrentProcessPseudoHandle`, value `0xFFFF8001` |
| `0x11A08C` | Y2R coefficient initialization bytes | Resolved as the 64-byte standard-coefficient matching table |
| `0x11A19C` | DWL/platform memory paths; first word `0xFFFF8001` | Resolved as four-byte `g_mvdCurrentProcessPseudoHandle`; adjacent `0x11A1A0` is a separate zero handle |
| `0x11A1A4` | `MvdY2rReadStandardCoefficients` | Resolved as four 16-byte standard-coefficient presets |

Two additional interior-reference groups belong to already typed `mcFilter` (`0x11CBB8`) and `vp71FeatureBits` (`0x11D2C8`). They are not new attribution gaps.

## Limits and next work

The later [codec semantic closure pass](codec-semantics.md) resolves the prefix-latch lifecycle of storage `+0x39E0` (now `mvcDpbLimit`), confirms separate SPS/PPS stack objects, and replaces the failing shifted pointer with a persistent picture-state view. It establishes zero-only behavior for capability `+0x18` and the extra VP6/VP8 info byte. Their original field meanings and five register definitions remain unavailable; these are now explicit definition/provenance gaps rather than untraced active codec behavior. Hardware-dependent questions retain their separate status.

The deferred function inventory also identifies ABI traps for the final phase. Division-family entries at `0x10E9E8` and `0x10F668` must be reviewed for register-pair quotient/remainder behavior before assigning prototypes. Memory-fill veneers can enter the middle of a shared routine, and the sending-path entry `0x10B97C` originally appeared to jump to `0x10BBE4` (the later driver pass corrects it to an ARM SVC `0x53` veneer). These are reasons to preserve uncertainty now, not additional codec algorithms.

## Database validation

Both arrays were checked for names, declared sizes and actual IDA item extents. The pointer array initially retained four-byte item boundaries despite its 32-byte type; its verified span was redefined without changing bytes. All eight pointer targets were checked by name. Reopening after save confirmed the typed local array and named level-limit accesses. Function counts stayed unchanged. Markdown links, inventory row count and whitespace were checked. No firmware instructions, live MMIO, reference-source files or decoder execution were involved.
