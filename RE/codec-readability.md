# Codec pointer lifetimes and scratch views

This continuation examines five already-attributed Hantro functions and makes persistent readability repairs in four of them. It does not add function attributions or change executable bytes. The source reference remains the local `hlibg1v6` tree; these are analysis types and names, not claims about exact original local declarations.

## VP8 filter and reference-buffer lifetimes

`VP8HwdAsicInitPicture` (`0x10BFF0`) reuses two stack words:

| Stack slot | First lifetime | Later lifetime |
|---|---|---|
| `SP+0x84` | `tapRegisterRow = Vp8TapRegId[j]` at `0x10C584` | `referencePictureId`, initialized at `0x10C684` and passed to `RefbuSetup` |
| `SP+0x88` | `filterRow = mcFilter[j]` at `0x10C57C` | `refbu = &decoder->refBufferCtrl` at `0x10C68A` |

The old frame type forced the second slot to `MvdRefBuffer *` throughout the function. Hex-Rays consequently displayed filter coefficients as `refbu->decModeMbWeights[...]`. After removing that whole-slot pointer interpretation and setting split lifetimes at the later assignments, the decompiler retains separate locals. Their types are `MvdHwIf *`, `u32`, `u32 *` and `MvdRefBuffer *` respectively.

The central filter write at `0x10C594` now reads:

```c
SetDecRegister(registers, tapRegisterRow[v26], filterRow[v26 + 1]);
```

This matches `TapRegId[i][j]` and `mcFilter[i][j+1]` in `source/vp8/vp8hwd_asic.c`. The outer loop covers eight filter rows, and the inner loop writes four central taps. The additional outer taps for rows 2, 4 and 6 remain explicit register writes. Later reference-buffer calls use only `refbu`; the same stack address does not imply a shared object.

## VP8 interior pointers

Seven locals in the same function now preserve their parent and byte displacement using IDA shifted-pointer metadata:

| Local | Parent type | Bias | Named accesses exposed |
|---|---|---|---|
| `ppStateBase` | `MvdVp8Container` | `0x1000` | `userMem`, `sliceHeight`, `intraOnly` |
| `pictureIndexBase` | `MvdVp8AsicBuffers` | `0x1C0` | Output, reference, golden and alternate buffer indices |
| `loopFilterLevelBase` | `MvdVp8Decoder` | `0x80` | `loopFilterLevel` |
| `loopFilterDeltaBase` | `MvdVp8Decoder` | `0xA00` | `segmentLoopfilter[3]`, `modeRefLfEnabled`, reference/mode deltas |
| `boolCoderBase` | `MvdVp8Container` | `0xF40` | `bc.range`, across the decoder/Boolean-coder boundary |
| `referenceProbabilityBase` | `MvdVp8Decoder` | `0x100` | `probRefGolden` |
| `concealmentStateBase` | `MvdVp8Container` | `0x1040` | Tiled-reference enable, hardware EC support, concealment state and start macroblock coordinates |

The parents and biases follow actual address formation and the already-recovered layouts. They do not move members or change allocation sizes. In particular, a pointer originally formed from `mbModeLfDelta[1]` subsequently accesses a Boolean-coder member in the enclosing container; typing it only as an array of mode deltas obscured that relationship.

## VP8 decode scratch union

The prior [scratch layout](codec-data.md) correctly recovered the 128-byte reused region in `VP8DecDecode` (`0x1107CC`), but Hex-Rays often chose capability members during saved-pointer lifetimes. This continuation saves **35 instruction-specific union selections**. Both comparisons now display their proper capability record:

| Instruction/expression | Correct view |
|---|---|
| `0x110878` and subsequent saved-pointer operations | `scratch.saved` |
| `0x1108AC`, after config read at `0x110892` | `scratch.slice.config.maxDecPicWidth` |
| `0x110AB2`, part of condition rendered at `0x110AE2` | `scratch.dimensions.maxDecPicWidth` |
| `0x110AD4`, same dimension-validation path | `scratch.dimensions.webpSupport` |

The capability reader at `0x110AA6` fills the unshifted `dimensions` record. The earlier reader fills `slice.config`, starting seven words later. No new meaning is assigned to capability `unresolvedWord6`: its apparent pointer uses are now displayed as saved-pointer accesses.

Seven saved-pointer members also have shifted types:

| Member(s) | Parent | Bias |
|---|---|---|
| `keyFrame`, `keyFrameAlias` | `MvdVp8Decoder` | `0x1C` |
| `modeLfDelta`, `modeLfDeltaAlias` | `MvdVp8Container` | `0xF40` |
| `chromaTail` | `MvdVp8AsicBuffers` | `0x1C0` |
| `previousOutputIndex` | `MvdVp8AsicBuffers` | `0x200` |
| `refreshAlternate` | `MvdVp8Decoder` | `0x5C` |

This exposes `frameTagSize`, `coeffProbsDecoded`, `pp.ppInstance`, `showFrame`, `refreshEntropyProbs` and output-buffer indices where the old output showed capability integers plus pointer arithmetic. Four additional locals preserve container-relative bases: `ppStateBase` (`0x1000`), `concealmentStateBase` (`0x1040`), `referenceStateBase` (`0x500`) and `dimensionStateBase` (`0x100`). The latter two expose parsed dimensions/version and ASIC dimensions respectively.

Some copied interior pointers still lose their parent type in later temporaries, and an eight-byte clear spanning adjacent DC predictor/match fields remains an aggregate expression. Those are residual display limitations, not newly inferred memory corruption.

## H.264 aliases and additional MVC evidence

At this checkpoint, `h264bsdDecode` (`0x115C28`) had a persistence limitation for `pictureStateBase`, at storage `+0x1F80`. Typing the frame member at `SP+0x5C0` temporarily produced named accesses, but the reopen check reverted it to `s32 *`. **Resolved later:** the [codec semantic pass](codec-semantics.md) uses an ordinary `MvdH264PictureStateView *`, with named `newPicture` and `currImage.data` accesses verified after reopening. The same pass verifies separate, correctly typed SPS/PPS locals; the earlier overlay item is closed.

Two biased locals in `h264bsdAllocateSwResources` (`0x115004`) now expose `storage.dpb` and `storage.useSmoothing`. A container-relative local in `H264DecGetInfo` (`0x103AD4`) exposes `dpbMode` and `tiledReferenceEnable`.

Tracing named MVC consumers adds two observations to [the MVC state analysis](h264-mvc-state.md):

* At `0x103B74`, `H264DecGetInfo` tests **`mvcEnabled` at storage `+0x39DC`** and doubles `multiBuffPpSize`. Before doubling, that value is 2 when DPB output reordering is disabled, otherwise `dpbSize+1`. The source uses its single `mvc` flag for the same operation.
* At `0x115070..0x11507A`, `h264bsdAllocateSwResources` tests **`mvc` at storage `+0x39E0`** and clamps `maxDpbSize` to 8 when it is at least 8. This occurs after selecting the active SPS size, or the larger active/base-view SPS size for a nonzero view, and before `h264bsdResetDpb`. This cap is absent from the supplied allocation routine.

Thus the enable/request flag affects advertised buffer requirements, while the prefix-populated state also affects allocation. This pass initially retained the `mvc` name. The later semantic pass names it `mvcDpbLimit` after tracing the latch lifecycle and distinguishes the capped argument from the effective no-reordering size and total allocation. The software capability gate still prevents the normal enable API from reaching its write.

## Method and verification

The database was saved and its MCP worker closed before installed batch IDA operations; it was reopened afterward. A temporary recovery copy is `/tmp/mvd-before-alias9.i64`. The pass used `LVINF_SPLIT` at the actual later instruction addresses, persistent local/frame types and saved union selections. Initial split/type attempts did not alter the rendered output; the final read-back, rather than a successful API return alone, establishes the repairs.

The resulting pseudocode was regenerated and checked for separate filter/reference-buffer lifetimes, named interior-pointer accesses and both proper capability comparisons. Parent sizes remain H.264 storage/container **14864/15860**, VP8 container/parser/ASIC **4224/2612/812**, and VP8 saved-pointer/scratch records **36/128** bytes. Counts remain **796 functions, 265 `sub_*` names and 501 inventoried names/prototypes**.

The MVC search covered named H.264 functions and their rendered member accesses; aliases and unclassified code can evade that search. It is not a proof of exhaustive state usage. The five unresolved register ordinals, capability word and extra info byte retain uncertain names. No decoder execution, firmware patch, reference-source edit or platform attribution was performed.
