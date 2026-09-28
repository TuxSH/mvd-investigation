# Pixel packing and hardware-gated paths

This pass prioritizes the outstanding hardware questions before general platform/runtime work. It combines the saved database, the supplied G1 register dump, local Hantro/libctru source, and GBATEK's published hardware research. Reference confirmation is distinguished from a new console experiment; none was performed here. The subsequent [hardware-validation pass](hardware-validation.md) assumes one console revision at the user’s request and closes item 3’s static/reference work, with explicit hardware/erratum blockers.

## L2B and Y2R pixel order

**RGBA8888 uses increasing-address bytes `AA BB GG RR`, corresponding to the little-endian word `0xRRGGBBAA`.** This is supported by the published hardware findings, not just by the suggestive name RGBA8888.

The evidence chain is:

1. GBATEK's [GPU texture format table](https://problemkaputt.de/gbatek.htm#3dsgputextureformats) places red at bits 31:24, green at 23:16, blue at 15:8 and alpha at 7:0.
2. Its [L2B description](https://problemkaputt.de/gbatek.htm#3dsvideol2bregistersrgbtorgbaconverternew3ds) identifies RGB/RGBA conversion into texture order. The author's [original hardware investigation](https://forums.nesdev.org/viewtopic.php?start=210&t=18490), March 2020, also describes Y2R output with alpha in the low bits and reports observing L2B/Y2R FIFO output. This supplies hardware evidence beyond API enum names.
3. MVD programs these format fields directly. Its DMA configurations set `endianSwapSize` to zero. No software component shuffle occurs in the traced L2B/Y2R transfer paths. Thus the low-to-high bytes of the packed pixel reach memory in the order above.

For the documented hardware formats, the input/output IDs are:

| ID | L2B input | L2B/Y2R output | Packed component layout |
|---|---|---|---|
| 0 | RGBX8888; input alpha ignored | RGBA8888 | `0xRRGGBBAA`; bytes `AA BB GG RR` |
| 1 | RGB888 | RGB888 | `0xRRGGBB`; bytes `BB GG RR` |
| 2 | RGBX5551; input alpha ignored | RGBA5551 | R 15:11, G 10:6, B 5:1, A 0 |
| 3 | RGB565 | RGB565 | R 15:11, G 10:5, B 4:0 |

These IDs agree with local libctru's Y2R output enum. They are not Hantro PP format constants. L2B output alpha comes from its alpha register: the full byte for format 0 and its high bit for format 2. Consequently an RGBA8888-to-RGBA8888 operation need not preserve the input word even when RGB is unchanged.

Component packing and pixel addressing are separate. L2B produces tiled texture order; Y2R selects linear or tiled addressing using control bit 12. Swizzling reorders complete pixels, not the bytes within a pixel. Source/destination DMA gaps can additionally change where transfer units reside in memory.

An unambiguous example is R=`11`, G=`22`, B=`33`, alpha register=`A5` (hexadecimal). Format-0 output is word `0x112233A5`, or bytes `A5 33 22 11`. A future console confirmation can use an 8×8 constant RGB888 input containing repeated `33 22 11`, select output format 0, and compare the received bytes against that sequence. Constant color removes tiling from this specific check. This is an expected vector derived from the evidence, **not an executed test**. Exact 5/6-bit expansion, rounding and cross-format conversion behavior are not established by it.

## Control bits: resolved roles and retained uncertainty

The register masks and access widths below come from MVD; the DMA/DRQ interpretations are corroborated by the GBATEK hardware work linked above and its [Y2R register description](https://problemkaputt.de/gbatek.htm#3dsvideoy2rregistersyuvtorgbaconverter).

| Bit | L2B interpretation | Y2R interpretation | Confidence / database treatment |
|---|---|---|---|
| 22 | Input DMA enable | Input DMA enable | Semantic function names applied |
| 23 | Output DMA enable | Output DMA enable | Semantic function names applied |
| 24 | Input DRQ | Tentative Y input DRQ | L2B named; Y2R retains numeric name |
| 25 | Output DRQ | Tentative U input DRQ | L2B named; Y2R retains numeric name |
| 26 | No driver interpretation | Tentative V input DRQ | Numeric name retained |
| 27 | No driver interpretation | YUYV input DRQ | Semantic write-back name applied |
| 28 | No driver interpretation | Output DRQ | Semantic write-back name applied |
| 29 | Tentative DRQ interrupt enable | Tentative DRQ interrupt enable | Numeric names retained |

The correspondence is also consistent with the driver's receive/send DMA device selection: L2B input 23/25 and output 24/26; Y2R input 18–21 and output 22. Initialization enables bits 22 and 23; finalization disables both. This does not establish the exact interrupt condition associated with bit 29.

The status helpers perform a conditional read/modify/write of the **whole control word**. They do not issue a one-bit-only write. If several status bits are set, the write includes all of those bits. Therefore neither the helper name nor the code proves that only its nominal status is acknowledged. GBATEK itself leaves acknowledgement behavior tentative. Names use `WriteBack...Drq`, not an asserted `Clear...` or `Acknowledge...` operation.

Bank `+0x08` remains an incompletely characterized strobe/reset operation. Its observed writes and the driver's use are known; precise internal FIFO effects, DRQ timing and error-state behavior still require hardware evidence. Software acceptance of dimension 1024 also does not independently establish the hardware meaning of a zero dimension register.

## VP8 concealment defect: reachability on the reference state

The conditional lower-left histogram access in [codec-leaf-analysis.md](codec-leaf-analysis.md) remains a real instruction-level defect. Its normal decoder reachability can now be narrowed.

`DWLReadAsicConfig` at `0x10DEF0` extracts `hardwareEcSupport` from synthesis word 2, bits 13:12. With supplied value `0xC09A0000`, the result is zero. `VP8DecInit` zeroes all `0x1080` bytes of the container at `0x110EFE`. The only recovered explicit assignment to container `hardwareEcSupport`, offset `0x1050`, is at `0x11100A`: it copies the capability for VP8 when freeze concealment was not requested. Other initialization choices leave the zeroed member unchanged. IPC initialization exposes format/freeze/buffer/reference-format arguments, not an override for this capability.

Both calls into the concealment runner are in `VP8DecDecode`:

| Call | Required gate |
|---|---|
| `0x110BAE` | `hardwareEcSupport != 0`, freeze-until-key-frame latch clear, previous frame not a key frame |
| `0x110D16` | `hardwareEcSupport != 0`, plus the previous-frame/extrapolation/restart-position condition |

Instructions at `0x110B86..0x110B96` and `0x110CF4..0x110D0E` confirm these conditions, including the interior base pointer used to address the flag. The sole recovered downstream chain is:

```text
VP8DecDecode
  -> MvdVp8RunConcealment       0x10BE30
  -> MvdVp8ConcealMotionVectors 0x119150
  -> MvdVp8CollectNeighborVectors 0x114904
```

All 796 functions were decompiled for an additional typed-use scan. Seven functions use the VP8 container flag: initialization, decode, release, picture allocation, picture setup, stream-position setup and freeze handling; only initialization assigns it. The capability reader is the separate producer. Code/data references to the three helpers reveal only the calls above. This supplements rather than relies on IDA's empty structure-field xref query, which misses these recovered typed accesses.

**Conclusion:** the defect is not reachable through the recovered normal initialized decoder path under the supplied GBATEK capability state. Under the subsequently requested single-revision assumption, this resolves normal target reachability. It does not claim protection against arbitrary memory corruption or unrecognized indirect calls. Freeze/reference-picture recovery still exists when this motion-vector concealment feature is disabled.

## H.264 High10: stronger negative evidence, not a hardware result

The header parser at `0x117016` and `0x117022` decodes the two depth-minus-eight fields into the same scratch word. It checks decoding success but neither validates nor retains their values. Header acceptance therefore cannot demonstrate High10 decoding.

`h264bsdInitDpb` at `0x118084` supplies a second independent observation. Let `N` be the macroblock count:

| Configuration | Sample bytes | Motion-vector bytes | Allocation before optional second chroma |
|---|---:|---:|---:|
| Ordinary 4:2:0 path | `384*N` | 0 | `384*N + 32` |
| High-profile 4:2:0 path | `384*N` | `64*N` | `448*N + 32` |
| High-profile monochrome | `256*N` | `64*N` | `320*N + 32` |

The instructions selecting 256/384 and 320/448 are at `0x118128..0x118142`. Optional second chroma adds `128*N` bytes; it is not selected by bit depth. `H264DecDecode` passes `h264ProfileSupport == 3` as `highSupported` to software-resource allocation. This is a profile-tier flag, not ten-bit sample allocation.

A 4:2:0 macroblock has 384 samples. Reserving 384 bytes and placing motion-vector data immediately afterward is the native eight-bit layout. This strongly rules against interpreting the exposed DPB as native 10-bit samples. It does **not** alone exclude a hypothetical internal high-depth decode with eight-bit output, nor distinguish rejection, incorrect decode, and correct downconversion for a High10 stream. No such hardware behavior was measured. High10 support should remain unclaimed; settling that residual question requires identified-console tests with a genuine ten-bit stream and a known output reference.

## Frame-number workaround

The [previously recovered mechanism](h264-dpb-prediction.md) remains the supported conclusion: software conditionally clears frame-number bit 12 in the stream and accommodates that change in access-unit boundary checks. `InitWorkarounds` disables the workaround for product `0x6731`, builds at least `0x2390`; supplied ID `0x67312398` therefore disables it.

Neither the local source revision nor the available register descriptions explain the silicon failure that necessitated the extension. The patch and build threshold do not distinguish a parser problem from reference-number arithmetic or another internal fault. No more specific defect name is justified.

## Database updates and validation

Eight existing helpers receive DMA/DRQ semantic names. A one-byte `MvdRgbFormat` enum is applied to seven function prototypes, both L2B package format members and the Y2R package output member. Raw shifted-register writers retain integer arguments. The L2B/Y2R package sizes stay 8/12 bytes; transport widths and malformed-value behavior are unchanged. The two output-format getters now use a named halfword local. Comments link the hardware evidence and record capability-gated concealment calls and the fixed H.264 sample layout.

Changes were saved and reopened for type, name and representative decompilation checks. No function was added: totals remain 796 functions, 170 unnamed and 598 inventoried. No instruction bytes, source trees or live registers were modified.

The subsequent [hardware-validation report](hardware-validation.md) locates the published Y2R rounding model and a DMA-event timing caveat, and marks the remaining live High10, L2B precision, Y2R2 edge, dimension-zero and tentative status/IRQ/strobe questions blocked on console evidence. The H.264 workaround is inactive on the assumed single target revision; its historical silicon cause needs an external erratum. Pixel packing and target concealment reachability are resolved. General SDK/runtime attribution remains deferred.
