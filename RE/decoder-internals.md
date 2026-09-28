# Decoder internals and continued Hantro identification

This continuation prioritizes codec internals and register semantics, with the DWL/platform boundary handled last. Addresses refer to the supplied database. The function inventory records all applied prototypes; this document explains their relationships and the differences from the local source.

## Recovered layouts

The following sizes were checked in IDA against allocation sizes, array strides, copies and callback offsets. Source layouts were also examined with Clang targeting ARM32 and short enums; source sizes alone were not treated as proof of MVD layout.

| Type | MVD bytes | Evidence |
|---|---:|---|
| Linear memory descriptor | 12 | Virtual pointer, bus address, byte size; allocator and 12-byte picture-array strides |
| H.264 container | 15860 | Allocation/clear in `H264DecInit` |
| H.264 storage | 14864 | Begins at container byte 308; ASIC buffers begin at 15172 |
| H.264 DPB | 1680 | Explicit `0x690` backup/restore copies in `H264DecDecode` |
| H.264 buffered picture | 52 | Reference-list and picture-status indexing |
| H.264 output picture | 36 | Output-queue indexing |
| H.264 ASIC buffers | 176 | Five embedded linear descriptors and reference/control state |
| VP6 container | 2424 | Allocation/clear in `VP6DecInit` |
| VP6 ASIC buffers | 260 | Starts at 264, DWL pointer at 524 |
| VP6 parser state | 1480 | Starts at 768, PP block at 2248; header/probability accesses agree with source |
| VP8 container | 4224 | Allocation/clear in `VP8DecInit` |
| VP8 ASIC buffers | 812 | Starts at 268, DWL pointer at 1080 |
| VP8 parser state | 2612 | Starts at 1316; arithmetic coder starts at 3928 |
| Boolean arithmetic coder | 36 | Nine words; buffer pointer at 20, error flag at 32 |
| Reference-buffer controller | 228 | Initialization, statistics and setup offsets agree |
| Buffer queue | 16 | Age-array pointer, counter, count, previous anchor slot |
| Decoder-to-PP interface | 104 | PP start accesses plus callback-block boundaries |
| PP capability query | 20 | Five words in all three codecs |

The local `DWLLinearMem_t` includes a fourth `uid` word, which MVD does not have. Copying it into IDA would enlarge every embedded descriptor and corrupt downstream offsets. For example, the source DPB is 1820 bytes under ARM32 short-enum layout, while MVD is 1680. Its embedded descriptor plus 34 picture descriptors account for the 140-byte difference.

Short enums also matter: each H.264 DPB picture has two one-byte status values at offsets 24 and 25; PP status and multibuffer status occupy bytes 0 and 1 of the shared interface. Padding is explicit where required by the observed offsets.

The recovered H.264 storage includes typed SPS/PPS pointers, four DPBs, two picture-order states, current image, previous NAL header, four slice headers, stream state and output/concealment bookkeeping. The access-unit-boundary region, macroblock-layer payload and nested slice-reference commands are now typed; one additional storage word at `0x39DC` remains unresolved. VP8’s parser-tail word is `coeffProbsDecoded`, and its 40-byte tail now describes concealment/recovery state. See [codec-leaf-analysis.md](codec-leaf-analysis.md) for the evidence, branch differences and remaining limits.

## Decoder-to-PP contract

`MvdDecPpInterface` follows the source `DecPpInterface` through `progressiveSequence`, then adds `lumaStride` and `chromaStride` at offsets 96 and 100. The local header's corresponding structure is 96 bytes with short enums. `PPDecStartPp` at `0x106EB0` consumes these extra fields and compares nonzero strides with the input width.

| Container | PP instance | Start/end/query callbacks | Interface | Query |
|---|---:|---|---:|---:|
| H.264 | 15636 | 15640 / 15644 / 15648 | 15656 | 15760 |
| VP6 | 2248 | 2252 / 2256 / 2260 | 2264 | 2368 |
| VP8 | 3964 | 3968 / 3972 / 3976 | 3980 | 4084 |

H.264 has an additional display callback at 15652, 17 sent-picture pointers at 15780, two queued-picture pointers at 15848 and a maximum multibuffer ID at 15856. `h264PpMultiInit`, `h264PpMultiMvc`, `h264PpMultiAddPic` and `h264PpMultiRemovePic` manage these exact fields.

The query words are tiled mode, pipeline accepted, deinterlace, multibuffer and configuration changed. VP6/VP8 prepare helpers query the PP before setting the interface's run state. With pipelining, the decoder supplies dimensions and layout while setting input bus addresses to zero; without it, the PP receives reference/output buffer addresses. H.264 additionally coordinates reordered output and separate field pictures.

## H.264 decode path

The source match extends from the API into `h264bsdDecode`, bitstream readers, Exp-Golomb decoding, SPS parsing, software-resource allocation, reference-picture access, ASIC setup and post-picture bookkeeping.

1. `H264DecDecode` validates the self pointer and input, then parses until a picture/header/status boundary
2. Header changes invoke `h264bsdAllocateSwResources` and `h264AllocateResources`, including DPB sizing and ASIC buffer allocation
3. VLC mode uses `H264SetupVlcRegs`; RLC mode uses `PrepareIntra4x4ModeData`, `PrepareMvData` and `PrepareRlcCount`
4. The active DPB and picture-order state are copied before running hardware, permitting restoration on the advanced-tools/fallback path
5. `H264RunAsic` programs reference/layout state, coordinates PP, waits, reads IRQ status and updates the stream cursor from the hardware stream-base register
6. `h264UpdateAfterPictureDecode` updates reference/output bookkeeping; next-picture retrieval is a separate operation

The internal API state values observed here are 1 for initialized decoding, 2 for continuation after a hardware buffer-empty result, and 3 for a deferred header transition while prior output is drained. A hardware buffer-empty condition preserves continuation state and returns stream-processed. A header-ready result is therefore not equivalent to picture-ready, and successful decode does not imply that a display picture is immediately available.

`h264InitPicFreezeOutput` copies a usable reference when possible, otherwise fills concealed output with neutral samples; it updates error-macroblock counts. `h264bsdFindNextStartCode` and `h264CheckCabacZeroWords` support recovery and stream-position handling.

`DecSetupTiledReference` is a four-argument MVD variant: registers, tiled support, DPB mode, interlaced flag. It enables tiled mode when supported and either the stream is progressive or field-DPB mode is selected. The local source has only three arguments and disables tiled references for interlaced streams. With the supplied reference registers, field-DPB support is zero, so that software branch is not proof of field-DPB availability on the referenced hardware.

The synthesis H.264 tier and parser acceptance still do not establish High10 decoding. No bit-depth or profile conformance tests were performed.

## VP6 decode path

The full `PB_INSTANCE` field layout matches the source after accounting for alignment: stream buffer and two arithmetic coders, Huffman-state pointer, version/profile/frame flags, dimensions, filtering controls, motion/vector probabilities, coefficient probabilities and scan-order tables.

`Vp6StrmInit`, `VP6HWLoadFrameHeader` and `VP6HWDecodeProbUpdates` precede the hardware path. `VP6HwdAsicProbUpdate` packs probability data into the ASIC table; `VP6HwdAsicInitPicture` programs references/filtering; `VP6HwdAsicStrmPosUpdate` sets partition addresses and bit offsets. `vp6PreparePpRun` and `VP6HwdAsicRun` coordinate PP and completion.

A dimension change releases/reallocates picture descriptors, sets state 3 and returns headers-ready. The following decode call consumes that saved header state and resumes picture processing, returning to state 1. Reference and golden buffers are updated on a successful frame; concealment can instead select the previous reference. The frame queue excludes active references when selecting the next output slot.

## VP8 / VP7 / WebP decode path

`vp8hwdDecodeFrameTag` extracts version, key-frame status, show-frame flag and first-partition size. `vp8hwdDecodeFrameHeader` dispatches VP7 versus VP8 syntax; `vp8hwdSetPartitionOffsets` computes coefficient partition positions, bounds them to the input and reports overflow. VP7 syntax exists in software even though the supplied reference synthesis/fuse state disables VP7 initialization.

The parser holds both current and saved entropy contexts. When entropy refresh is disabled, the API restores the saved probabilities and VP7 scan order after the hardware run. Reference updates handle independent last/golden/alternate refresh and copy operations.

The internal states are 1 (initialized), 3 (new headers), 4 (normal decoding) and 5 (middle of a sliced picture). A dimension change returns headers-ready in state 3; the next call allocates pictures and enters state 4. A slice IRQ returns slice-ready and enters state 5; `VP8HwdAsicContPicture` resumes with the remaining rows. WebP uses the intra-only path, including optional sliced output and caller-supplied luma/chroma buffers.

Compared with the local source, MVD's ASIC structure adds stride controls, separate arrays of luma/chroma memory descriptors, two motion-vector buffers, and 16-entry arrays of user luma/chroma virtual and bus addresses. Field names for the two encoded stride words intentionally do not assert a byte unit. The capability bits at synthesis-word-2 bits 13:12 and 11 are now identified by their consumers as hardware error concealment and programmable stride respectively.

For ordinary VP8 video the binary rejects dimensions below 48 pixels, width beyond the configured decoder limit, or padded area above `0x1000000`. The WebP-capable intra-only branch permits a different dimension path, up to `0x4000` in each direction, with additional allocation/area/slicing restrictions. This is a software validation path, not a tested maximum image size.

## PP and register refinements

The last opaque PP scaling block is now named: fast-scaling support, horizontal/vertical enable and disable flags, normal/fast coefficients, horizontal rounding workaround and fast-scale mode. The final PP-container word is WebP support.

New source matches include framebuffer clipping, dithering selection, custom RGB masks, RGB transform coefficients, scaling setup and width/height validation. The coefficient-rounding workaround and the reference hardware's capabilities are detailed in [hardware.md](hardware.md).

Twenty-five additional register semantics were recovered from callers, using an `MVD_HWIF_` prefix. Another source name, `HWIF_DEC_IRQ`, was confirmed from its exact triple and IRQ-clear callers. The later [parser/support pass](parser-support.md) identifies ordinal 19's data-discard alias, bringing the current total to 725 of 730 names. The five remaining ordinals are 8, 10, 128, 282 and 598. Field 579 is `MVD_HWIF_VP8_CONCEALMENT_MODE`: zero for ordinary decoding, one for the generated-motion-vector concealment path; encodings 2 and 3 remain unresolved.

## Remaining work

The [codec leaf pass](codec-leaf-analysis.md) extends attribution through entropy decoding and macroblock/RLC preparation. The [DPB/prediction pass](h264-dpb-prediction.md) covers the main H.264 allocation, marking, output, reference-list and intra-prediction paths and explains the frame-number mask. The [parser/support pass](parser-support.md) extends the function inventory to 501; the [codec-data pass](codec-data.md) and its continuation add 86 table names/types and clarify VP8 stack-storage reuse. Remaining data, custom-code readability and unresolved fields precede deferred service/hardware questions and SDK/runtime attribution. The H.264 storage extension word and exact silicon fault behind the workaround remain open. The function inventory is a progress record, not a claim that all 796 current function entries have been identified.

The continued platform findings are in [platform-glue.md](platform-glue.md). No decoder was executed and no live MMIO state was changed.

The [control-flow correction](control-flow-corrections.md) restores the shared `h264bsdDecode` error epilogue and marks its Thumb long branch correctly. Failed NAL extraction and failed slice-data decoding now display direct error returns, matching the binary and source.
