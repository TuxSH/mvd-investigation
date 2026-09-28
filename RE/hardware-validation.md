# Hardware questions: completion and blockers

## Scope and disposition

This pass follows the request to finish the hardware questions until complete or blocked, assuming **one console revision**. The supplied ASIC ID `0x67312398` and synthesis/fuse values in [hardware.md](hardware.md) are the target state for this investigation. Matching a second console revision is no longer an outstanding task. This is an explicit analysis assumption, not a new register capture.

| Question | Disposition | What remains necessary |
|---|---|---|
| RGBA8888 byte order | Resolved against published hardware findings and MVD's DMA configuration | None for this analysis |
| VP8 motion-vector concealment defect reachable normally? | Resolved: no, the target's capability gate is zero | None under the single-revision assumption |
| H.264 frame-number workaround active? | Resolved: no, target build exceeds the disabling threshold | None for target applicability |
| Historical silicon cause of that workaround | Blocked on missing erratum or matching source commentary | An explanation of the affected G1 design; the inactive path cannot establish the historical failure |
| Actual High10 decoding behavior | Blocked on missing console execution/output evidence | A genuine ten-bit bitstream and captured decoder results/output |
| Y2R arithmetic and rounding | Published tested reference model located | Direct Y2R2 confirmation and dithering/other edge cases remain blocked on hardware |
| L2B cross-format quantization/expansion | Blocked on hardware | Discriminating input/output samples |
| Zero-valued dimension register | Driver encoding resolved; hardware counter behavior blocked | Actual conversion sizes and completion results with zero encoded dimensions |
| DMA/interrupt timing and tentative IRQ/status/strobe bits | Software contract and one published timing caveat resolved; exact hardware behavior blocked | Timestamped DMA/event/register observations |

The available IDA session is an offline database with placeholder MMIO, not a live console. No executable converter/decoder model of the target silicon or captured results for these remaining cases was supplied. Re-running static analysis cannot manufacture the missing output or timing observations. General platform/runtime work was not started during this pass.

## High10: evidence checked and why it remains blocked

The [3DBrew table](https://www.3dbrew.org/wiki/MVD_Services#Supported_H.264_Levels_and_Profiles) reports High10 results from browser tests. Its linked original test is `http://mtheall.com/~mtheall/pie/high10.html`. On 2026-09-28 both the original HTTP URL and its HTTPS equivalent returned HTTP 404. An Internet Archive index request returned the archive's temporary-unavailability page. Thus the test stream's SPS, actual sample precision and recorded decoder output could not be checked. The table remains a published claim, not verified evidence of ten-bit reconstruction here.

The target binary supplies two independent negative indicators, detailed in [hardware-followup.md](hardware-followup.md): SPS depth fields are discarded, and reference-picture allocation uses eight-bit sample storage. The [upstream Hantro Linux driver at v6.12](https://github.com/torvalds/linux/blob/v6.12/drivers/media/platform/verisilicon/hantro_drv.c#L257) also rejects nonzero H.264 `bit_depth_luma_minus8`. That is corroborating evidence about another driver in the IP family, not a proof of how Nintendo's different software/hardware combination responds to the stream.

The remaining distinction is observable behavior: rejection, corrupt output, or correct reconstruction followed by eight-bit output. None follows solely from accepting profile ID 110 or decoding an SPS successfully. Native ten-bit DPB output is inconsistent with the recovered storage layout; true High10 decoding remains unclaimed.

A sufficient console experiment needs:

* A small valid eight-bit control stream and a genuine ten-bit 4:2:0 stream, for example 64×64, with at least one reference-dependent picture
* Recorded SPS values showing `profile_idc=110` and both depth-minus-eight values equal to 2 in the ten-bit case; a filename or container codec label is insufficient
* A software-decoded reference and a pattern exercising sample precision, rather than only a flat image
* Every MVD decode/next-picture result and the decoded output before optional PP conversion when accessible; otherwise the exact PP configuration and comparison transformation

No stream was decoded on a console during this pass. An unavailable old web test is not a reason to classify either success or failure as established.

## Y2R numerical model located

Yuri Kunde Schlesner's [original conversion implementation](https://github.com/azahar-emu/azahar/blob/3e6663da433d98a0bf4db1256ea3ccdefd404a0c/src/core/hw/y2r.cpp), dated 2015-06-08, describes the arithmetic as matching the hardware in the author's tests. The same author's [libctru addition](https://github.com/devkitPro/libctru/commit/8a7601098887e6c723a3f32b9283370d480e90e2) documents a bias of 0.75. These are related evidence from the same researcher, not two independent hardware confirmations.

With raw unsigned ten-bit multipliers `C0..C4`, signed 16-bit offsets `Or, Og, Ob`, and eight-bit input samples, the published model is:

```text
Tr = C0*Y + C1*V
Tg = C0*Y - C2*V - C3*U
Tb = C0*Y + C4*U

channel(T, O) = clamp_0_255(((T >> 3) + O + 24) >> 5)
```

The shifts are arithmetic. The effective bias is `24/32 = 0.75`, not ordinary nearest rounding with a half-unit bias. MVD's coefficient writer masks the five multipliers to ten bits and writes all sixteen bits of each offset, consistent with this representation. This is a published Y2R model; transferring it to the separate Y2R2 block is a supported hypothesis, not a measured result in this project. It does not settle dithering, L2B expansion, or the exact precision of every intermediate silicon operation.

A compact discriminator sets `C0=64`, the other multipliers and offsets to zero, disables both dithering modes, and supplies Y values 0, 1, 2, 3. The model predicts grayscale values **0, 1, 1, 1**. A half-unit-bias model instead gives **0, 0, 1, 1**. Those sequences were calculated on the host; they were not received from Y2R2.

The [public Y2R hardware-test branch](https://github.com/yuriks/hwtests/tree/b0672822b89add7c283f9c141a939f54fbfaab82/source/tests/y2r) was also inspected. Its active tests convert 128×128 images through `y2r:u`, use RGB24 output and save PNGs after exchanging R/B bytes. It contains no corresponding checked result set in that tree, no active 1024-dimension case, and no L2B/Y2R2-specific test. Its existence therefore does not close these outstanding cases.

## L2B conversion precision

Known packing and alpha replacement do not determine expansion of five/six-bit channels to eight bits. A few constant-color blocks distinguish plausible mappings without a large test suite:

| Input channel | Zero-fill low bits | Replicate high bits | Scale to 255, round nearest |
|---|---:|---:|---:|
| Five-bit value 3 | 24 | 24 | 25 |
| Five-bit value 4 | 32 | 33 | 33 |
| Six-bit value 11 | 44 | 44 | 45 |
| Six-bit value 16 | 64 | 65 | 65 |

These are candidate predictions, not measured conversion rules. Use RGBA5551 or RGB565 input with RGBA8888 output and fixed alpha, one channel at a time. Conversely, eight-bit values near a quantization boundary distinguish truncation from rounding when producing 5551/565. Constant 8×8 blocks remove pixel swizzling as a source of ambiguity. Actual samples are required before naming an expansion or quantization rule in IDA.

## Dimension-zero behavior and a testing trap

The four setters were rechecked in the saved database:

| Operation | Setter | Behavior for argument 1024 |
|---|---|---|
| L2B width | `0x10FD7C` | Writes zero |
| L2B line count | `0x10FD44` | Writes zero |
| Y2R width | `0x10F9C4` | Writes zero |
| Y2R line count | `0x10F998` | Returns success without writing |

Thus three setters clearly encode the API maximum as zero, but the silicon counter interpretation remains unobserved. An emulator change permitting 128 tiles per row does not demonstrate what a zero MMIO value means on this target.

In particular, **`SetInputLines(1024)` is not a valid experiment for Y2R zero-height semantics**. If height was explicitly set to eight beforehand, that call retains eight. During `MvdY2rRegistersInitialize`, the call at `0x1100BC` is likewise a no-op after the earlier bank `+0x08` strobe. The final height then depends on the strobe's unresolved effects and previous hardware state. The driver does not explicitly initialize height to zero there. An IDA comment now records this dependency.

Width-zero tests can use ordinary 1024-width IPC configuration while fixing height at eight. A raw Y2R zero-height test needs a path that actually writes zero and confirms readback; it must not use the successful no-op as a substitute. Record received byte count and completion state to distinguish a maximum-count encoding from immediate completion or a stalled transfer.

## DMA events and timing

The local libctru header, at checkout `0a376398d19505df3c236342827a3a72a42dd41e`, records an early Y2R completion-event observation for a 400×240 image with a one-line receive transfer unit; two/four/eight-line units avoided it in those observations. See [Y2RU_SetReceiving documentation](https://github.com/devkitPro/libctru/blob/0a376398d19505df3c236342827a3a72a42dd41e/libctru/include/3ds/services/y2r.h). This is a concrete published caveat, not a measured timing guarantee for Y2R2 or L2B.

MVD provides separate observables: the conversion event, control busy bit and DMA-handle completion. `MvdDmaIsDone` at `0x10B874` polls the DMA handle with a zero timeout; a zero handle reports done. Consequently a completion sample is meaningful only after successful setup of a real DMA handle. The conversion event alone is insufficient evidence that all destination bytes have arrived.

The smallest useful timing capture uses a sentinel-filled output buffer and records event time, busy state, receive-DMA completion and final byte contents for one-line versus eight-line receive units. Record input mode and pixel format as well. Exact throughput, FIFO thresholds, IRQ bit 29, status acknowledgement and the reset/strobe effects cannot be inferred from service call order. The offline database supplies no clocked peripheral execution; those questions remain blocked.

## Inactive H.264 workaround and completion criteria

The [consolidated workaround report](workarounds.md) documents the H.264 stream mutation and boundary accommodation, VP8 concealment entry conditions, ordinary freeze recovery, and the limits of source attribution.

`InitWorkarounds` compares the low-halfword build against `0x2390` and clears the mode-zero workaround at `0x1058CE`. With target ID `0x67312398`, its applicability is settled: inactive. The lower-left VP8 concealment issue is likewise excluded from normal target execution by its zero support flag. No revision survey is required under the user's assumption.

The exact historical H.264 defect remains unclassified. The supplied `source/common/workaround.c` lacks this H.264 extension. Targeted public searches for the build threshold, frame-number mask and G1 workaround did not locate an explanatory erratum or matching commentary. This missing historical information is separate from an unresolved active console behavior.

This closes the static/reference portion of item 3. The remaining rows are explicitly **blocked on console observations or an external silicon erratum**, with the evidence needed to reopen each recorded above. No new source provenance, function names or types are claimed. Three instruction comments were added in IDA, and the single-revision assumption is reflected in the current hardware/status documentation. No firmware bytes, reference trees or live hardware were changed.
