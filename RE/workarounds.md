# Decoder workarounds and error recovery

This report consolidates the H.264 frame-number workaround and VP8 concealment/recovery paths in the supplied `mvd.i64`. The caller checks were re-examined on 2026-09-28. Addresses are IDA virtual addresses. Source paths below are relative to `/Users/elouan/Documents/git_dl/buildroot-ltc/system/hlibg1v6`.

The target is the single console revision assumed for this investigation: ASIC ID `0x67312398`, synthesis word 2 `0xC09A0000`, from the user-supplied GBATEK dump. These are reference values, not measurements from the database's placeholder MMIO.

| Mechanism | Purpose | Source attribution | Target applicability |
|---|---|---|---|
| H.264 frame-number bit-12 patch | Alter the hardware-facing slice header, with matching software boundary handling | Hantro-family decoder integration; extension absent from the supplied source revision | Disabled by the ASIC build gate |
| VP8 motion-vector concealment | Reconstruct an approximate picture after missing/damaged input | Additional helpers absent from the supplied source revision | Disabled by zero hardware error-concealment capability |
| VP8 freeze recovery | Output an existing reference and maintain recoverable decoder state | `vp8hwdFreeze` has a Hantro counterpart, extended in this binary | Ordinary freeze remains available; hardware-concealment-dependent latch setters are gated off |

VP8 concealment is error recovery, not an established silicon-erratum workaround. Neither an absent source match nor the descriptive `Mvd` naming establishes Nintendo authorship: the additional code may belong to another Hantro release or a platform customization.

## H.264 frame-number bit-12 workaround

### Provenance and activation

`InitWorkarounds` at `0x105824` matches the family of `source/common/workaround.c::InitWorkarounds`, but contains an extra H.264 case. The local source's workaround union is eight bytes and contains MPEG/RV flags; this binary's recovered union is sixteen bytes. The local revision does not provide the H.264 extension or the patch helper.

The only recovered caller of `InitWorkarounds` is `H264DecInit` at `0x103D0C`, with decoder mode zero. That mode enables the H.264 workaround by default. For ASIC product `0x6731`, build `>= 0x2390` clears it at `0x1058CE`. Target build `0x2398` therefore disables it. The inherited MPEG/RV cases in this shared routine are not evidence of exposed MPEG/RV decoder paths in MVD.

The H.264 container contains this view at `+0x3D04`:

| Relative offset | Member | Meaning |
|---|---|---|
| `+0x00` | `enabled` | ASIC applicability flag |
| `+0x04` | `reservedWord1` | Initialized to zero; no established H.264 use |
| `+0x08` | `frameNumMask` | Set to `0x1000` when enabled, otherwise left zero |
| `+0x0C` | `annexBPatchActive` | Successful patch of initially prefixed input; drives framing adjustments |

`H264DecInit` checks the flag at `0x103D1A` and sets the mask at `0x103D20`. The second word overlaps the MPEG start-code flag in `MvdDecoderWorkarounds`; that union overlap does not give it an H.264 meaning.

### Caller and stream mutation

`H264DecDecode` at `0x102F2C` resets `annexBPatchActive` for new input at `0x102F90`. Its only recovered call to `MvdH264PatchFrameNumBit12` is at `0x103404`, gated at `0x1033E2` by **both** `rlcMode == 0` and `workarounds.h264.enabled != 0`.

The call supplies the stream position before the just-consumed parser bytes, the corresponding restored length, the parsed slice-header `frameNum`, and the active SPS's `maxFrameNum`. Thus the helper reparses the encoded field using values already obtained by the decoder. It mutates the input buffer; it does not merely change a register or a private copy of the parsed frame number.

The recovered helper at `0x117F4E` has this prototype:

```c
u32 MvdH264PatchFrameNumBit12(
    u8 *stream, u32 length, u32 frameNum, u32 maxFrameNum,
    u32 *initialStartCodeBytes);
```

Its behavior is:

1. Initialize `*initialStartCodeBytes` to zero and exit unless `frameNum & 0x1000` is nonzero
2. Derive the frame-number width from `maxFrameNum`, giving `log2(maxFrameNum)` for the valid power-of-two SPS value
3. Recognize an initial Annex B prefix and record its length; prefixed input may be scanned past other NALs to a type 1, 5 or 20 NAL. Raw NAL input is also accepted
4. Skip the one-byte NAL header, or four bytes for type 20, and read three unsigned Exp-Golomb fields: `first_mb_in_slice`, `slice_type`, `pic_parameter_set_id`
5. Require the following fixed-width field to equal the supplied `frameNum`, then clear its numeric bit 12 in place

For the final write, relative to the parser's current byte pointer:

```text
bitIndex = bitPosInWord + frameNumBitWidth - 13
currentBytes[bitIndex >> 3] &= ~(0x80 >> (bitIndex & 7))
```

Disassembly at `0x118008`–`0x118022` establishes the MSB-first byte mask and store. This description covers the observed parser sequence; the helper is not a general slice-header validator using every SPS/PPS variation.

The return value is a framing signal as well as a patch result: **raw NAL input can be modified while the helper returns zero**. One means that the patch succeeded and the input initially had a start-code prefix. `initialStartCodeBytes` describes only that initial prefix, not the eventual slice offset after scanning other NALs.

At `0x10340C` the caller saves the result. When it is nonzero and the prefix length is nonzero, `0x10341E`–`0x103426` advance the hardware stream virtual/bus addresses and shorten its length by that prefix. Later, `0x1038B0`–`0x1038D4` use the flag to recover the next start code and update stream position/length after processing. These uses explain why `annexBPatchActive` must not be interpreted as a universal “input bytes changed” flag.

### Software access-unit boundary accommodation

Before the access-unit boundary check, `h264bsdDecode` assigns at `0x115E40`:

```text
aub.maskedPrevFrameNum = aub.prevFrameNum & ~workarounds.h264.frameNumMask
```

`h264bsdCheckAccessUnitBoundary` treats a frame-number difference as a boundary only if the new number matches neither the original previous value nor this masked value. Field flags, reference status, picture order, IDR and view checks remain in effect. With a zero mask, the two stored comparisons are equivalent. This is a targeted accommodation for the patched header, not global renumbering of all decoder/reference state.

The access-unit structure has an additional word at `+0x24` and is 76 bytes, versus 72 in the supplied source. See [the access-unit analysis](codec-leaf-analysis.md) and [recovered layouts](database-layouts.md).

### What remains unknown

The binary establishes which bit is cleared, when, and how software tolerates the altered header. It does **not** establish the historical silicon failure that required it. No matching erratum or explanatory source revision was located. It is therefore inappropriate to describe a specific frame-counter overflow, reference-buffer defect, or wraparound failure as proven.

Under the target assumption the workaround is inactive, so missing historical attribution does not leave target applicability unresolved. An affected-silicon erratum or matching source commentary is needed to establish the original reason; see [hardware completion/blockers](hardware-validation.md).

A decompiler pitfall is already commented in IDA: a stack word set to one participates in an OR check after the first Exp-Golomb read. It does not require a nonzero `first_mb_in_slice`; Hex-Rays merges that word and the output into an oversized local array.

## VP8 motion-vector concealment

### Capability and entry conditions

`VP8DecInit` at `0x110EA0` zeroes the container. The only recovered assignment capable of making `hardwareEcSupport` nonzero is at `0x11100A`: it copies the hardware configuration value for VP8 format when caller-requested freeze concealment is disabled. Other initialization cases leave it zero. The configuration reader extracts synthesis word 2 bits 13:12 at `0x10DEF0`; `0xC09A0000` yields zero. No IPC override was found.

Both calls from `VP8DecDecode` at `0x1107CC` require nonzero `hardwareEcSupport`:

| Call | Additional conditions | Action |
|---|---|---|
| `0x110BAE`, missing/empty-input path | Freeze-until-key-frame latch clear and previous picture not a key frame | Reset restart X/Y and run temporal extrapolation |
| `0x110D16`, damaged-picture/hardware-error path | Previous picture not a key frame, or extrapolation disabled, or either restart coordinate nonzero | Run concealment from the selected restart point |

The second gate is at `0x110CF4`–`0x110D0E`. Failing it takes ordinary corrupted-reference handling. A previous key frame by itself does not categorically prohibit this second entry: the other alternatives matter.

The [reachability audit](hardware-followup.md) checked the recovered flag uses and caller chain. With normal initialization and the assumed target state, neither entry reaches the motion-vector helpers. This is independent of whether the binary contains their implementations.

### Reconstruction path

| Function | Address | Role |
|---|---|---|
| `MvdVp8InitConcealment` | `0x1194A0` | Allocate and initialize vector workspace |
| `MvdVp8ReleaseConcealment` | `0x119568` | Release workspace |
| `MvdVp8RunConcealment` | `0x10BE30` | Generate vectors and run the ASIC in concealment mode |
| `MvdVp8ConcealMotionVectors` | `0x119150` | Temporal extrapolation and spatial replacement |
| `MvdVp8AccumulateConcealmentVector` | `0x10AE14` | Accumulate weighted vector components at checked block positions |
| `MvdVp8CollectNeighborVectors` | `0x114904` | Collect neighbors and choose a reference/vector |

These six helpers have no located exact counterparts in the supplied source revision. Their `Mvd` names describe recovered behavior, not established authorship.

The workspace allocates 36 bytes per 4×4 vector block, with 16 blocks per macroblock at the traced call. Packed vectors encode signed X in bits 31:18, signed Y in bits 17:5, and reference ID in bits 2:0; bits 4:3 remain unassigned.

Temporal extrapolation projects previous-picture vectors using reference ID zero into weighted accumulators. Spatial replacement collects up to twenty surrounding vectors, votes among reference IDs 0/4/5, averages vectors for the chosen reference, and replaces an invalid macroblock's sixteen vectors. ID zero wins ties involving zero; four wins a tie with five. In extrapolation mode the spatial pass covers the region before the restart macroblock and temporal results fill the remainder. Without extrapolation, spatial replacement covers the restart macroblock through the end. The [codec leaf report](codec-leaf-analysis.md) records the detailed weighting and disassembly checks.

`MvdVp8RunConcealment` clears the key-frame flag, chooses previous/current MV buffers, generates vectors, and sets `concealmentActive`. It then calls ASIC picture setup, stream-position setup and the ASIC runner before clearing the active flag. Extrapolation also resets the restart coordinates. Picture setup disables writing new motion vectors and supplies the generated buffer through `DIR_MV_BASE`. Register field 579, word 48 bits 13:12, changes from zero to one (`MVD_HWIF_VP8_CONCEALMENT_MODE`); values two and three remain unknown.

This produces an approximate reconstructed picture using estimated motion. It cannot recover the original missing compressed information exactly.

### Defect within the inactive helper chain

The neighbor collector has a missing lower-left boundary guard. When X is nonzero but the row is outside the permitted lower-neighbor range, the slot can retain `0xFFFFFFFF`; the histogram increment at `0x114AE8` nevertheless uses that value. Its shifted index addresses the word immediately below SP. This is a conditional out-of-bounds access verified in disassembly, not a demonstrated exploit or established end-user trigger.

The zero capability gate excludes this helper chain from normal target execution. The defect in the recovery implementation is separate from the damaged-stream condition that concealment attempts to handle.

## VP8 freeze and entropy recovery

`vp8hwdFreeze` at `0x1193E4` has a counterpart in `source/vp8/vp8decapi.c` around line 1028. Both perform picture/output bookkeeping and disable PP pipelining. This binary adds entropy restoration, a freeze latch and MV-buffer clearing.

The routine increments the picture count, selects the previous reference for output, and increments output count only for a displayed frame. When coefficient probabilities have been decoded but entropy refresh is disabled, it restores the saved `0x44D`-byte entropy block and the 64-byte VP7 scan order. The parser flag `coeffProbsDecoded` at decoder `+0xA30` means that coefficient-probability updates completed, not that the entire header was valid.

Otherwise, when hardware concealment is supported and an entropy refresh has been seen, the routine sets `forceFreezeUntilKeyFrame` at `0x119446`. It also marks the picture broken and clears the previous MV buffer when present. A second latch setter exists after missing-input concealment: `0x110C14`–`0x110C1C` set both `pictureBroken` and the latch if entropy-refresh history is nonzero.

The normal decode decision at `0x110B54` permits decoding if the picture is a key frame, the picture-broken flag is clear, or both caller-requested `intraFreeze` and the extra freeze latch are clear. Otherwise it routes to freeze recovery. A **successfully decoded key frame**, on the ASIC completion path at `0x110D6C`–`0x110D7C`, clears both `pictureBroken` and `forceFreezeUntilKeyFrame`. Merely encountering a key-frame header does not establish recovery.

Unlike the motion-vector helper chain, the entire freeze routine is not guarded by hardware concealment support. Therefore ordinary freeze and entropy restoration remain relevant on the target. The two hardware-concealment-dependent latch-setting paths do not activate under its zero support flag. A `VP8DEC_PIC_DECODED` result on a recovery path can consequently represent a repeated reference picture rather than a newly reconstructed source picture.

The container fields are `pictureBroken` at `+0x1008`, `intraFreeze` at `+0x100C`, `hardwareEcSupport` at `+0x1050`, `concealmentActive` at `+0x1058`, restart X/Y at `+0x105C/+0x1060`, `previousKeyFrame` at `+0x1064`, `forceFreezeUntilKeyFrame` at `+0x1068`, and `entropyRefreshSeen` at `+0x106C`. See [database layouts](database-layouts.md) for full types and workspace layout.

## Other workaround and evidence limits

The separate postprocessor horizontal coefficient-rounding workaround in `PPInitHW` applies when `DWLReadAsicID() >> 3 == 216408617`, corresponding to IDs `0x67311148..0x6731114F`. It is also inactive for target `0x67312398`; see [hardware.md](hardware.md). This report does not reinterpret inherited MPEG/RV workaround branches as active MVD services.

This documentation pass checked the existing source comparisons and recovered layouts, followed the H.264 patch and VP8 concealment callers, and re-examined the access-unit mask and freeze-latch lifecycle in the decompilation. Earlier disassembly findings are identified by address where relevant. No firmware instructions, input streams, reference source trees or MMIO were changed, and no hardware decode experiment was performed. The newly clarified caller conditions require no speculative runtime test to document; the historical H.264 silicon cause remains explicitly unknown.
