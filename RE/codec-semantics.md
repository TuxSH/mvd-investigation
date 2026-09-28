# Codec semantic closure and remaining definition gaps

This pass on 2026-09-28 follows the request to prioritize codec semantics before general platform glue. It examines the extra H.264 MVC state, the unused capability word, the additional VP6/VP8 info bytes and the five unnamed register ordinals. It also resolves two outstanding H.264 decompiler representation issues.

## H.264: API enable flag versus prefix-NAL latch

The adjacent words in `MvdH264Storage` have different lifecycles:

| Offset | Current name | Established role |
|---|---|---|
| `+0x39DC` | `mvcEnabled` | API request/NAL acceptance flag; also affects reported PP buffer requirements |
| `+0x39E0` | `mvcDpbLimit` | Prefix-NAL latch used to limit the requested DPB size |

The second word was previously named `mvc`. The new name describes its recovered consumer rather than treating it as the request flag, current view, or count of views. It remains a four-byte member; no offsets changed.

### Lifecycle

1. `H264DecInit` clears the allocated container. `h264bsdInitStorage` at `0x1182A8` independently clears all `0x3A10` storage bytes, including both flags
2. `H264DecSetMvc` writes `mvcEnabled = 1` at `0x10451C` only after the capability check. It does not set `mvcDpbLimit`
3. `h264bsdDecode` checks `mvcEnabled` at `0x115D4C` before accepting MVC NAL types 14, 15 and 20. In the prefix-NAL case, `0x11660A`–`0x11660C` copy it into `mvcDpbLimit`, while setting the current view to zero and updating `nonInterViewRef`
4. `h264bsdAllocateSwResources` reads the latch at `0x115070` and caps its requested DPB-size argument at eight
5. `h264bsdResetStorage` resets picture/macroblock decoding counters and flags, not the MVC latch. Parameter-set activation switches the active SPS, DPB and slice-header pointers without clearing it. SPS/PPS storage and resource shutdown likewise supply no recovered clear of this word

The full 796-function typed-expression scan found the prefix assignment and allocation read as the only explicit accesses to the second word. Inspection of initialization, reset, parameter-set and shutdown paths accounts for the bulk-clear lifecycle. Thus the recovered behavior is a latch retained across ordinary picture resets until decoder reinitialization, not a per-picture active-view flag. This is a statement about the recovered code; a typed-reference scan alone cannot exclude every unrecognized pointer alias or missing function boundary.

An accepted prefix traverses the nonzero enable gate, so the copy normally sets the latch to one. The `view`, `viewId`, `numViews` and `nonInterViewRef` members remain separate. A subset SPS supplies view IDs, while slice processing and parameter-set activation choose the current view; none of those values should be inferred from `mvcDpbLimit` alone.

On the assumed target, `DWLReadAsicConfig` unconditionally clears MVC support. Normal command `0x06` fails the capability check before setting `mvcEnabled`, and the prefix-NAL path cannot set the latch. This analysis does not establish usable MVC decoding on the console.

### The eight-picture limit is not an absolute allocation limit

For the base view, `h264bsdAllocateSwResources` starts with the active SPS's `maxDpbSize`. For the dependent view, it takes the larger of that value and the base-view SPS's value. A set `mvcDpbLimit` then applies:

```text
requestedDpbSize = min(requestedDpbSize, 8)
```

That value is passed separately from `activeSps->numRefFrames` to `h264bsdResetDpb` and ultimately `h264bsdInitDpb`. The latter establishes:

```text
maxRefFrames = max(inputMaxRefFrames, 1)
effectiveDpbSize = noReordering ? maxRefFrames : requestedDpbSize

totalBuffers = effectiveDpbSize + 1
if displaySmoothing:
    totalBuffers += noReordering ? 1 : effectiveDpbSize + 1
else if multiBufferPp:
    totalBuffers = effectiveDpbSize + 2
```

The same effective-size selection appears in the reset routine's reuse decision. Therefore the latch caps the requested reorder-buffer size, **not necessarily the effective DPB size when reordering is disabled**, and certainly not the total number of allocated picture buffers. The no-reordering decision also includes POC type 2 and suitable VUI bitstream restrictions, not just the initialization argument. The SPS parser separately permits up to sixteen references for ordinary SPS input and fifteen for subset-SPS input; the eight limit does not replace those parser checks.

These are static control-flow and allocation findings, not a claim that every dormant MVC combination is valid or has been exercised. See [DPB analysis](h264-dpb-prediction.md) for the allocation payload and output queues.

## Capability word at +0x18

`DWLReadAsicConfig` at `0x10DE0C` clears its entire 100-byte output at `0x10DE14`, then populates the documented synthesis/fuse-derived fields. It never overwrites `unresolvedWord6` at `+0x18`.

The reader has twelve recovered call sites in nine functions: H.264 initialization/MVC enable, three PP helpers, VP6 initialization, VP8 initialization, VP8 decode and VP8 picture setup. Their capability-buffer lifetimes and reads were examined. No active read of this word was recovered. The all-function typed scan also found no member access. Earlier apparent reads in `VP8DecDecode` were saved pointers sharing stack storage with later capability records; they are already modeled by the [VP8 scratch union](codec-readability.md).

The useful behavioral conclusion is **an unused, zero-initialized capability slot in this build**. Its original API name or intended feature cannot be inferred from zero alone. It remains named `unresolvedWord6`; no unsupported codec capability was assigned to it. The differently ordered local `source/inc/dwl.h` is not an exact layout match and supplies no definition for this extra slot.

## VP6/VP8 additional info bytes

The two fields previously called `extraFlag` are now named `constantZero`, preserving their one-byte types:

| Output | Offset | Writer | Behavior after successful validation |
|---|---|---|---|
| `MvdVp6Info` | `+0x1C` | `VP6DecGetInfo`, `0x109618` | `STRB` of zero |
| `MvdVp8Info` | `+0x20` | `VP8DecGetInfo`, `0x110E94` | `STRB` of zero |

No nonzero writer, test or transformation was found. The wrapper functions call the library query and convert its result. `MVDSTD_HandleCommands` copies the complete 36-byte VP6 or 40-byte VP8 record into the reply. It does not interpret the extra byte. The supplied `source/inc/vp6decapi.h` and `vp8decapi.h` have no corresponding extra field.

This establishes the wire behavior without guessing a historical field name such as DPB mode. **Only the byte is zeroed, not the whole four-byte slot.** The next three bytes are padding that the getter does not initialize; the dispatcher copies them as part of the record. Neither getter's early error return guarantees valid metadata. Consumers should use successful result codes, read the defined byte widths and ignore padding.

The field's original intended meaning remains unavailable in the supplied source revision. Its behavior in this build is no longer an open codec question.

## Five register ordinals: use audit

The scan enumerated `SetDecRegister` and `GetDecRegister` calls across all 796 functions, including numeric enum values rather than trusting Hex-Rays's composite enum names. Nonliteral field arguments were traced through their assignments and eleven selector arrays (`refPicList0/1/P`, `refBase`, `refPicNum`, VP6 scan/tap, and VP8 partition/scan/tap arrays).

| Ordinal | Register location | Recovered use |
|---|---|---|
| 8 | Word 1 bit 11 | No selection by recovered accessor calls or selector arrays |
| 10 | Word 1 bit 5 | No selection by recovered accessor calls or selector arrays |
| 128 | Word 7 bit 13 | No selection by recovered accessor calls or selector arrays |
| 282 | Word 20, all 32 bits | No selection by recovered accessor calls or selector arrays |
| 598 | Word 58 bit 31 | Only `MvdInitDecoderRegisters` at `0x10DDE2`, writing zero |

The first four remain definitions in the register table without an identified ordinal-specific consumer. This does not make their physical bits untouched: other codec fields can alias the same bits, and register-bank transfers can copy whole words. For example, ordinal 282 shares the full-word location with `HWIF_REFER6_BASE` at ordinal 281, and the H.264 reference-base selector uses **281**, not 282. Similarly, the VP8 `HWIF_CH_MV_RES` definition shares word 7 bit 13 but lies in a different source-table sequence; this does not establish the intended VC1-side meaning of ordinal 128.

The local Hantro register definitions and the [upstream Linux v6.12 G1 definitions](https://github.com/torvalds/linux/blob/v6.12/drivers/media/platform/verisilicon/hantro_g1_regs.h) supply no justified names for these five entries. The latter's interrupt definitions omit bits 11 and 5. Zeroing ordinal 598 does not establish whether it controls an absent, disabled or reserved feature. The five names remain unresolved pending matching branch definitions or hardware documentation; repeated scans of the same zero-only or absent consumers will not recover that information.

## H.264 stack and pointer representation

Fresh decompilation shows two separate, correctly sized locals in `h264bsdDecode`:

| Local | Offset from the body SP | Bytes |
|---|---|---:|
| `seqParamSet` | `+0x004` | 708 |
| `picParamSet` | `+0x2C8` | 676 |
| `strm` | `+0x56C` | 28 |

The SPS ends exactly where the PPS begins; the PPS ends at the stream record. Their parser calls, cleanup and subset-SPS view-ID extraction now use the correct members. The previous outstanding “SPS/PPS overlays” item was stale: the current saved types and regenerated output do not show those earlier aliases. The locals were given source-consistent names, and the NAL local was named `nalUnit`.

The `pictureStateBase` shifted-pointer type had repeatedly failed to survive reopening. A plain pointer to the new **44-byte `MvdH264PictureStateView`** now represents the actual contiguous span at storage `+0x1F80`: the tail of `aub`, followed by `currImage`. This is an analysis view, not a new allocation or a changed parent layout. The local is named `pictureState`, and its ordinary pointer type survives save/reopen. Fresh output displays `pictureState->newPicture` and `pictureState->currImage.data` instead of indexed words 6 and 7.

## Validation and disposition

The complete initial scan decompiled all 796 functions without an error. Source comparisons, the relevant byte stores and MVC load/store instructions were inspected. The eleven selector tables contained none of the five unknown ordinals. Named-member searches were supplemented by caller inspection, bulk initialization/reset checks and the already-recovered pointer aliases; their limits are stated above.

Three structure members were renamed, the H.264 local names/view were applied, and evidence comments were added. Saving and reopening confirmed the resulting member accesses and types. Sizes remain: H.264 storage/container 14864/15860, SPS/PPS 708/676, and VP6/VP8 info 36/40 bytes. Function counts remain 796 total, 170 `sub_*`, and 598 inventoried functions. No function source attribution or register name was invented, and no executable bytes, reference source or live hardware were changed.

The requested semantic pass is complete to the available evidence: MVC latch behavior, zero-field behavior and the two tracked H.264 display issues are resolved. Original meanings of the zero-only fields and five register entries remain definition/provenance gaps. The separate [hardware blockers](hardware-validation.md) and [platform/runtime queue](remaining-functions.md) retain their status.
