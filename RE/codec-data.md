# Codec constant tables and scratch-state correction

**Later definition update:** [The source follow-up](codec-definition-followup.md) identifies capability `+0x18` as `jpegProgSupport` and register ordinal 10 as `HWIF_DEC_ABORT_E`. Earlier unresolved-name statements in this pass are historical. Four register ordinals and the VP6/VP8 constant-zero info-byte meanings remain blocked.

This pass attributes 67 additional constant arrays: 62 full literal-byte matches and five register-selector arrays matched after translating source register names to MVD ordinals. It also rechecks seven previously named arrays. The function inventory remains 501 entries; data symbols are counted separately.

## Method and evidence

The complete `.rodata` range `0x11A000..0x121000` was read in 512-byte chunks, with returned lengths checked. Integer-literal array initializers in the local Hantro `source/h264high`, `source/vp6`, `source/vp8` and `source/common` C files were flattened, encoded in little-endian order using their declared element widths, and compared against this range. Signed values were encoded at their declared width. Macro-based dimensions were resolved from the source headers and checked against the initializer element counts.

Of 75 candidate literal arrays of at least 16 bytes, 69 match in full. The selected placements cover 15,358 bytes; 62 newly named arrays account for 14,266 bytes. Five separately checked register-selector arrays add 320 bytes, bringing the new attribution to 14,586 bytes. This is a bounded scan, not an inventory of every constant in the module: smaller arrays, structures, computed initializers and general enum expressions were outside the literal scan.

The matching source revision is identified in [external-code.md](external-code.md). Exact table bytes strongly corroborate the existing function matches but do not prove that Nintendo used that exact checkout. Source-local names are preserved where useful; names assigned to compiler-emitted local initializer copies are identified below.

## Full literal matches

All declarations below were applied or were already present in IDA. Paths are relative to the reference tree's `source/`. “Existing” marks the seven arrays named before this pass. Array extents are explicit even where the source uses macros or an inferred first dimension.

| Address | IDA declaration | Bytes | Source | Status |
|---|---|---:|---|---|
| `0x11A2A0` | `const i32 mbDataPerFormat[13][2]` | 104 | `common/refbuffer.c` | New |
| `0x11A308` | `const u32 g_hantroRegisterMasks[33]` | 132 | `common/regdrv.c` | Existing |
| `0x11C710` | `const u32 h264List0InitialValues[34]` | 136 | `h264high/h264hwd_asic.c` | New |
| `0x11C798` | `const u32 h264List1InitialValues[34]` | 136 | `h264high/h264hwd_asic.c` | New |
| `0x11C820` | `const u32 h264ListPInitialValues[34]` | 136 | `h264high/h264hwd_asic.c` | New |
| `0x11CBB8` | `const u32 mcFilter[8][6]` | 192 | `vp8/vp8hwd_asic.c` | New |
| `0x11CC78` | `const u16 YDcQLookup[128]` | 256 | `vp8/vp8hwd_asic.c` | New |
| `0x11CD78` | `const u16 YAcQLookup[128]` | 256 | `vp8/vp8hwd_asic.c` | New |
| `0x11CE78` | `const u16 Y2DcQLookup[128]` | 256 | `vp8/vp8hwd_asic.c` | New |
| `0x11CF78` | `const u16 Y2AcQLookup[128]` | 256 | `vp8/vp8hwd_asic.c` | New |
| `0x11D078` | `const u16 UvDcQLookup[128]` | 256 | `vp8/vp8hwd_asic.c` | New |
| `0x11D178` | `const u16 UvAcQLookup[128]` | 256 | `vp8/vp8hwd_asic.c` | New |
| `0x11D278` | `const u32 Vp7DefaultScan[16]` | 64 | `vp8/vp8hwd_decoder.c` | New |
| `0x11D2B8` | `const u32 vp70FeatureBits[4]` | 16 | `vp8/vp8hwd_headers.c` | New |
| `0x11D2C8` | `const u32 vp71FeatureBits[4]` | 16 | `vp8/vp8hwd_headers.c` | New |
| `0x11D304` | `const u32 cabacInitValues[920]` | 3680 | `h264high/h264hwd_cabac.c` | New |
| `0x11E164` | `const u32 mvdOffs[4]` | 16 | `h264high/h264hwd_macroblock_layer.c` | New |
| `0x11E174` | `const u32 CeilLog2NumSliceGroups[8]` | 32 | `h264high/legacy/h264hwd_pic_param_set.c` | New |
| `0x11E194` | `const u32 default4x4Intra[16]` | 64 | `h264high/legacy/h264hwd_seq_param_set.c` | Existing |
| `0x11E1D4` | `const u32 default4x4Inter[16]` | 64 | `h264high/legacy/h264hwd_seq_param_set.c` | Existing |
| `0x11E214` | `const u32 default8x8Intra[64]` | 256 | `h264high/legacy/h264hwd_seq_param_set.c` | Existing |
| `0x11E314` | `const u32 default8x8Inter[64]` | 256 | `h264high/legacy/h264hwd_seq_param_set.c` | Existing |
| `0x11E414` | `const u32 zigZag4x4[16]` | 64 | `h264high/legacy/h264hwd_seq_param_set.c` | Existing |
| `0x11E454` | `const u32 zigZag8x8[64]` | 256 | `h264high/legacy/h264hwd_seq_param_set.c` | Existing |
| `0x11E574` | `const u8 codedBlockPatternIntra4x4[48]` | 48 | `h264high/legacy/h264hwd_vlc.c` | New |
| `0x11E5A4` | `const u8 codedBlockPatternInter[48]` | 48 | `h264high/legacy/h264hwd_vlc.c` | New |
| `0x11E5D4` | `const u8 VP6HWModeVq[3][16][20]` | 960 | `vp6/vp6gconst.c` | New |
| `0x11E994` | `const u8 VP6HWBaselineXmittedProbs[4][2][10]` | 80 | `vp6/vp6gconst.c` | New |
| `0x11E9E4` | `const u8 VP6HWMvUpdateProbs[2][17]` | 34 | `vp6/vp6gconst.c` | New |
| `0x11EA14` | `const u8 VP6HW_DefaultMvLongProbs[2][8]` | 16 | `vp6/vp6gconst.c` | New |
| `0x11EA28` | `const u8 VP6HW_DefaultScanBands[64]` | 64 | `vp6/vp6gconst.c` | New |
| `0x11EA68` | `const i32 VP6HW_BicubicFilterSet[17][8][4]` | 2176 | `vp6/vp6gconst.c` | New |
| `0x11F2E8` | `const u8 VP6HWDcUpdateProbs[2][11]` | 22 | `vp6/vp6gconst.c` | New |
| `0x11F2FE` | `const u8 VP6HW_ScanBandUpdateProbs[64]` | 64 | `vp6/vp6gconst.c` | New |
| `0x11F33E` | `const u8 VP6HW_ZrlUpdateProbs[2][14]` | 28 | `vp6/vp6gconst.c` | New |
| `0x11F35A` | `const u8 VP6HW_ZeroRunProbDefaults[2][14]` | 28 | `vp6/vp6gconst.c` | New |
| `0x11F376` | `const u8 VP6HWAcUpdateProbs[3][2][6][11]` | 396 | `vp6/vp6gconst.c` | New |
| `0x11F57C` | `const u8 VP6HWDeblockLimitValues[64]` | 64 | `vp6/vp6gconst.c` | New |
| `0x11F5BC` | `const u8 VP6HWtransIndexC[64]` | 64 | `vp6/vp6gconst.c` | New |
| `0x11F5FC` | `const u8 MvUpdateProbs[2][19]` | 38 | `vp8/vp8hwd_probs.c` | New |
| `0x11F622` | `const u8 Vp8DefaultMvProbs[2][19]` | 38 | `vp8/vp8hwd_probs.c` | New |
| `0x11F648` | `const u8 Vp7DefaultMvProbs[2][17]` | 34 | `vp8/vp8hwd_probs.c` | New |
| `0x11F66A` | `const u8 CoeffUpdateProbs[4][8][3][11]` | 1056 | `vp8/vp8hwd_probs.c` | New |
| `0x11FA8A` | `const u8 DefaultCoeffProbs[4][8][3][11]` | 1056 | `vp8/vp8hwd_probs.c` | New |
| `0x11FED8` | `const u16 coeffToken0_0[32]` | 64 | `h264high/h264hwd_cavlc.c` | New |
| `0x11FF18` | `const u16 coeffToken0_1[48]` | 96 | `h264high/h264hwd_cavlc.c` | New |
| `0x11FF78` | `const u16 coeffToken0_2[56]` | 112 | `h264high/h264hwd_cavlc.c` | New |
| `0x11FFE8` | `const u16 coeffToken0_3[32]` | 64 | `h264high/h264hwd_cavlc.c` | New |
| `0x120028` | `const u16 coeffToken2_0[32]` | 64 | `h264high/h264hwd_cavlc.c` | New |
| `0x120068` | `const u16 coeffToken2_1[32]` | 64 | `h264high/h264hwd_cavlc.c` | New |
| `0x1200A8` | `const u16 coeffToken2_2[128]` | 256 | `h264high/h264hwd_cavlc.c` | New |
| `0x1201A8` | `const u16 coeffToken4_0[64]` | 128 | `h264high/h264hwd_cavlc.c` | New |
| `0x120228` | `const u16 coeffToken4_1[128]` | 256 | `h264high/h264hwd_cavlc.c` | New |
| `0x120328` | `const u16 coeffToken8[64]` | 128 | `h264high/h264hwd_cavlc.c` | New |
| `0x1203A8` | `const u16 coeffTokenMinus1_0[8]` | 16 | `h264high/h264hwd_cavlc.c` | New |
| `0x1203B8` | `const u16 coeffTokenMinus1_1[32]` | 64 | `h264high/h264hwd_cavlc.c` | New |
| `0x1203F8` | `const u8 totalZeros_1_0[32]` | 32 | `h264high/h264hwd_cavlc.c` | New |
| `0x120418` | `const u8 totalZeros_1_1[32]` | 32 | `h264high/h264hwd_cavlc.c` | New |
| `0x120438` | `const u8 totalZeros_2[64]` | 64 | `h264high/h264hwd_cavlc.c` | New |
| `0x120478` | `const u8 totalZeros_3[64]` | 64 | `h264high/h264hwd_cavlc.c` | New |
| `0x1204B8` | `const u8 totalZeros_4[32]` | 32 | `h264high/h264hwd_cavlc.c` | New |
| `0x1204D8` | `const u8 totalZeros_5[32]` | 32 | `h264high/h264hwd_cavlc.c` | New |
| `0x1204F8` | `const u8 totalZeros_6[64]` | 64 | `h264high/h264hwd_cavlc.c` | New |
| `0x120538` | `const u8 totalZeros_7[64]` | 64 | `h264high/h264hwd_cavlc.c` | New |
| `0x120578` | `const u8 totalZeros_8[64]` | 64 | `h264high/h264hwd_cavlc.c` | New |
| `0x1205B8` | `const u8 totalZeros_9[64]` | 64 | `h264high/h264hwd_cavlc.c` | New |
| `0x1205F8` | `const u8 totalZeros_10[32]` | 32 | `h264high/h264hwd_cavlc.c` | New |
| `0x120618` | `const u8 totalZeros_11[16]` | 16 | `h264high/h264hwd_cavlc.c` | New |
| `0x120628` | `const u8 totalZeros_12[16]` | 16 | `h264high/h264hwd_cavlc.c` | New |

### Identical bytes require consumer evidence

* The three 34-word initial lists in `H264InitRefPicList` have identical contents. Their copies into list 0, list 1 and list P identify the placements at `0x11C710`, `0x11C798` and `0x11C820`. The applied names `h264List0InitialValues`, `h264List1InitialValues` and `h264ListPInitialValues` describe emitted copies of source-local initializers, not upstream global symbols.
* `YAcQLookup` and `UvAcQLookup` are byte-identical. The quantizer consumers in `VP8HwdAsicInitPicture`, including `HWIF_QUANT_1` for Y AC and `HWIF_QUANT_5` for chroma AC, distinguish `0x11CD78` from `0x11D178`.
* The VP7 default scan and H.264 4x4 zigzag scan also share values. The VP7 reset consumer at `0x119530` selects `Vp7DefaultScan` at `0x11D278`; the H.264 scaling parser uses `zigZag4x4` at `0x11E414`.
* `mvdOffs` at `0x11E164` is the source's motion-vector-difference offset table. The name is unrelated to Nintendo's MVD service name.
* `g_hantroRegisterMasks` retains its earlier descriptive global name; its source counterpart is `regMask`.

`Y2AcQLookup` initially contained an interior auto-generated data-pointer interpretation. Its complete 256-byte span was redefined as the verified `u16[128]` array, removing that misleading pointer interpretation. No bytes were patched.

## H.264 register-selector arrays

These are not literal matches to the Linux enum numbers. Each source `HWIF_*` token was translated through the recovered MVD register enum, and all 16 resulting words were compared. The source is `h264high/h264hwd_asic.c`. Each table is typed as `const MvdHwIf[16]` (64 bytes).

| Address | Name | Role |
|---|---|---|
| `0x11C5D0` | `refBase` | Reference-buffer base registers |
| `0x11C610` | `refPicNum` | Reference picture-number registers |
| `0x11C650` | `refPicList0` | B-slice forward initial list |
| `0x11C690` | `refPicList1` | B-slice backward initial list |
| `0x11C6D0` | `refPicListP` | P-slice initial list |

The source's `refFieldMode` and `refTopc` arrays did not produce full matches after the same translation. This does not establish absence of their behavior: the compiler or branch can express the register selection differently.

## CABAC, picture-order and scaling buffer

`AllocateAsicBuffers` (`0x1012B0`) allocates `0xFC8` bytes for `cabacInit` when `h264ProfileSupport != 1`; its successful path copies `0xE60` bytes from `cabacInitValues`. The source constants and later binary consumers give the following layout:

| Byte offset | Capacity | Meaning | Binary evidence |
|---|---:|---|---|
| `0x000` | 3680 | CABAC initialization words | Copy at `0x101336` from `0x11D304` |
| `0xE60` | 136 | Up to 34 picture-order-count words | `H264SetupVlcRegs` computes `virtualAddress + 920` at `0x105760`; writes 32 reference-field counts followed by one or two current-picture counts |
| `0xEE8` | 224 | Six 4x4 and two 8x8 scaling lists | `H264RunAsic` computes `virtualAddress + 954` at `0x104D60` and packs/copies 224 bytes |

The tail is runtime data, not an additional portion of the read-only CABAC table. This establishes the software layout and consumers, not successful execution of any particular profile or bit depth.

## VP8 stack storage is reused

`VP8DecDecode` (`0x1107CC`) had a misleading 100-byte capability local. Disassembly shows a 128-byte region at `SP+0x20..SP+0x9F`, used at different times for saved pointers and two overlapping capability records:

* `DWLReadAsicConfig` at `0x110892` receives `SP+0x3C`; its width comparison at `0x1108AC` reads the record's `maxDecPicWidth` at `SP+0x58`.
* The call at `0x110AA6` receives `SP+0x20`; the dimension check reads `maxDecPicWidth` at `SP+0x3C`. This path returns headers-ready or an error, so the previous saved pointer values are no longer needed.
* The store at `0x110878` puts `&instance->decoder.mbModeLfDelta[1]` at `SP+0x38`. It is a saved pointer, despite the old decompilation calling the slot `config.unresolvedWord6`.

IDA now has a 128-byte `MvdVp8DecodeScratch` union and a correctly sized stack variable, with saved-pointer, dimension-config and shifted slice-config views. See [database-layouts.md](database-layouts.md). At this checkpoint Hex-Rays still selected incorrect union members. The later [readability pass](codec-readability.md) saves instruction-specific selections: the slice-width comparison now displays `scratch.slice.config.maxDecPicWidth`, and the dimension comparison displays `scratch.dimensions.maxDecPicWidth`. Saved-pointer sites also display their proper view and shifted pointer types. The union models compiler storage reuse; it is not asserted to be a source-declared type or a runtime memory-corruption finding.

## Unknown fields remain unknown

The capability writer clears all 100 bytes and does not explicitly populate `MvdDwlHwConfig.unresolvedWord6` at `+0x18`. The apparent VP8 uses above do not identify it. The extra byte in VP6/VP8 info is still written as zero; its location alone is insufficient to call it DPB mode or another familiar source field.

A bounded disassembly-text search for the H.264 storage extension at `+0x39DC` and its current member name produced no additional semantic consumer. This is not proof that it is unused: biased pointers, aliases and bulk clears can obscure direct references. The later [MVC state pass](h264-mvc-state.md) resolves that word as `mvcEnabled`, using an API writer and an aliased parser read missed by this search. The five register ordinals 8, 10, 128, 282 and 598 remain unresolved; no semantic names were invented for them.

## Nonmatches and validation

The six literal candidates without full matches were `stuffingTable`, `h264bsdQpC`, `VP6HWCoeffToBand`, `VP6HWMode2Frame`, `VP6HW_BilinearFilters` and `VP6HWCoeffToHuffBand`. Source-version differences, elimination of unused data or compiler representation changes are possible; these nonmatches do not prove a missing codec feature.

All 74 selected tables (69 literal and five selector arrays) were read back from IDA with the expected name, address and size. The scratch union and frame variable both read back as 128 bytes, and regenerated pseudocode confirms both capability-call destinations. Initial local-type application did not resize the old variable; declaring the correct stack span did, and the final result was checked. The function inventory remains 501, with 266 `sub_*` names in the database; sampled remaining functions are predominantly platform/service/runtime code, but the remainder is not fully classified.

This pass changes analysis annotations and documentation only. No decoder execution, MMIO writes, firmware-byte patches or reference-source edits were performed. Remaining work includes constants outside this scan, unresolved field semantics and custom-code readability. Deferred service/hardware questions remain after those tasks, with platform/SDK/runtime attribution last.

## Continuation: small, structured and codec-selector tables

A subsequent pass adds 19 arrays covering 980 bytes, bringing the two data passes to 86 new arrays covering 15,566 bytes. This extends the earlier scan to small initializers, two explicitly checked record layouts, and VP6/VP8 register-name macros. It does not count the seven already named arrays rechecked above as new work.

| Address | Applied declaration | Bytes | Source |
|---|---|---:|---|
| `0x11A204` | `const MvdMemAccess memStatsPerFormat[13]` | 156 | `common/refbuffer.c` |
| `0x11C938` | `const MvdHwIf Vp6ScanTblRegId[64]` | 256 | `vp6/vp6hwd_asic.c` |
| `0x11CA38` | `const MvdHwIf Vp6TapRegId[8][4]` | 128 | `vp6/vp6hwd_asic.c` |
| `0x11CAB8` | `const MvdHwIf Vp8ScanTblRegId[16]` | 64 | `vp8/vp8hwd_asic.c` |
| `0x11CAF8` | `const MvdHwIf Vp8DctBaseId[8]` | 32 | `vp8/vp8hwd_asic.c` |
| `0x11CB18` | `const MvdHwIf Vp8DctStartBit[8]` | 32 | `vp8/vp8hwd_asic.c` |
| `0x11CB38` | `const MvdHwIf Vp8TapRegId[8][4]` | 128 | `vp8/vp8hwd_asic.c` |
| `0x11EA06` | `const u8 VP6HW_DefaultMvShortProbs[2][7]` | 14 | `vp6/vp6gconst.c` |
| `0x11EA24` | `const u8 VP6HW_DefaultIsShortProbs[2]` | 2 | `vp6/vp6gconst.c` |
| `0x11EA26` | `const u8 VP6HW_DefaultSignProbs[2]` | 2 | `vp6/vp6gconst.c` |
| `0x11F504` | `const LINE_EQ VP6HWDcNodeEqs[5][3]` | 120 | `vp6/vp6gconst.c` |
| `0x11FEAA` | `const u8 runBefore_1[2]` | 2 | `h264high/h264hwd_cavlc.c` |
| `0x11FEAC` | `const u8 totalZeros_14[4]` | 4 | `h264high/h264hwd_cavlc.c` |
| `0x11FEB0` | `const u8 runBefore_3[4]` | 4 | `h264high/h264hwd_cavlc.c` |
| `0x11FEB4` | `const u8 runBefore_2[4]` | 4 | `h264high/h264hwd_cavlc.c` |
| `0x11FEB8` | `const u8 totalZeros_13[8]` | 8 | `h264high/h264hwd_cavlc.c` |
| `0x11FEC0` | `const u8 runBefore_6[8]` | 8 | `h264high/h264hwd_cavlc.c` |
| `0x11FEC8` | `const u8 runBefore_5[8]` | 8 | `h264high/h264hwd_cavlc.c` |
| `0x11FED0` | `const u8 runBefore_4[8]` | 8 | `h264high/h264hwd_cavlc.c` |

The `Vp6`/`Vp8` prefixes distinguish source-local arrays with the same spelling. Their source names are `ScanTblRegId`, `TapRegId`, `DctBaseId` and `DctStartBit`. The six selector tables contain 160 words and match after resolving `SCAN`, `TAP`, `BASE`, `OFFSET` and the `HWIF_VP6HWPART2_BASE = HWIF_RLC_VLC_BASE` alias. This last alias explains ordinal 211 in `Vp8DctBaseId[0]`; it does not identify any of the five unresolved register fields.

### Consumer checks and layout recovery

* `DecodeTotalZeros` (`0x1024F8`) selects the eight-byte `totalZeros_13` and four-byte `totalZeros_14` arrays for their corresponding cases. `DecodeRunBefore` (`0x102258`) selects `runBefore_1..6` by `zerosLeft`. The two-byte `runBefore_1` pattern occurs at four addresses, but its case-1 read at `0x102278` identifies `0x11FEAA`.
* `VP6HWDecodeProbUpdates` (`0x10A01C`) copies two bytes to `IsMvShortProb`, 14 to `MvShortProbs`, and two to `MvSignProbs` at `0x10A068`, `0x10A074` and `0x10A080`. The sign-default bytes also occur at hundreds of unrelated positions; the copy source is decisive.
* `memStatsPerFormat` uses the existing 12-byte `MvdMemAccess` layout, matching source `memAccess_t`: three `u32` members `latency`, `nonseq` and `seq`. Its 13 rows match all 156 bytes. `InitMemAccess` (`0x1057DC`) indexes it by decoder mode and halves the sequential count for a 64-bit bus. These are source model constants, not measured MVD bus timings.
* Source `LINE_EQ` is two signed 32-bit members `M` and `C`, at offsets 0/4, with size 8. The 5×3 array contains 120 bytes, including negative intercepts. `VP6HWConfigureContexts` (`0x109B30`) now displays the observed calculation as `((DcProbs[11*i+k] * VP6HWDcNodeEqs[k][j].M + 128) >> 8) + VP6HWDcNodeEqs[k][j].C`, clamped to 1..255. The loop covers two planes, three contexts and five nodes.
* VP6 and VP8 have separate copies of the identical tap-selector table. `VP6HwdAsicInitPicture` selects `0x11CA38`; `VP8HwdAsicInitPicture` selects `0x11CB38`. The 16-entry VP8 scan table also matches the prefix of the 64-entry VP6 table; their consumers distinguish the placements. Index zero in both scan arrays is an unused zero placeholder, not an assertion of `HWIF_DEC_PIC_INF` semantics.
* `VP8HwdAsicStrmPosUpdate` (`0x10BEB0`) selects the coefficient partition's aligned bus address through `Vp8DctBaseId[i]` and its bit offset through `Vp8DctStartBit[i]`. Typing the two adjacent arrays removes the misleading old `dword_11CAF8[i+8]` expression for the second table.

The candidate search also encountered partially initialized automatic arrays such as `distrVer` and `newOrder`. Their explicit `{0}` is not a complete four-byte constant table: declared dimensions and storage use exclude them. Short-pattern matches alone were not promoted to symbols. The PP source scan produced no additional accepted literal arrays under this method.

### VP8 pointer alias, subsequently repaired

In `VP8HwdAsicInitPicture`, the stack slot at `SP+0x88` holds `mcFilter[j]` at `0x10C57C`, then `&decoder->refBufferCtrl` at `0x10C68A`. Hex-Rays merges these lifetimes under a `MvdRefBuffer *` local and renders a filter read at `0x10C594` as `refbu->decModeMbWeights[v26+1]`. Its actual value is `mcFilter[j][v26+1]`, corroborated by the source loop. The subsequent [readability pass](codec-readability.md) separates the lifetimes into `filterRow` and `refbu` and also splits the tap-table pointer from the later reference-picture ID. Fresh pseudocode now reads `filterRow[v26+1]`.

All 19 final symbols were read back with the expected size and name; the adjacent DCT arrays required recreating the start-bit item after splitting the previous larger item. Regenerated pseudocode confirms the structured cost/equation accesses and separate DCT selectors. `LINE_EQ` and `MvdMemAccess` read back as 8 and 12 bytes. A separate [H.264 control-flow correction](control-flow-corrections.md) removes one false function entry: the current database has 796 functions, of which 265 retain `sub_*` names. The documented function inventory remains 501.

## Attribution-coverage continuation

A reference audit of all 501 documented functions adds two arrays omitted by the earlier initializer scans: the 32-byte `h264ScalingListDefaults` at `0x11E554` and the 204-byte `g_mvdH264LevelLimits` at `0x11A0D0`. The former is the source-local pointer initializer in `ScalingList`; the latter gains a descriptive 12-byte record type for the previously documented MVD sizing table. The three data passes now total **88 newly annotated arrays and 15,802 bytes**. Full consumer evidence, the corrected stack pointer array and seven deferred platform data heads are in [attribution-coverage.md](attribution-coverage.md).

### Disposition of earlier nonmatches

Searching identifiers across the supplied `source/` tree gives more specific evidence than merely suggesting a source-version difference:

| Nonmatching source table | Source-reference disposition |
|---|---|
| `stuffingTable` | Its check in `h264high/legacy/h264hwd_util.c:155` is under `HANTRO_PEDANTIC_MODE`; the recovered trailing-bits routine omits that pattern check |
| `h264bsdQpC` | Definition and extern declaration only; no consumer found in this source tree |
| `VP6HWCoeffToBand`, `VP6HWCoeffToHuffBand` | Uses in `vp6/vp6scanorder.c:106,112` are inside `#if 0` |
| `VP6HWMode2Frame`, `VP6HW_BilinearFilters` | Definitions only; no consumer found in this source tree |
| `refFieldMode`, `refTopc` | Definitions only in `h264high/h264hwd_asic.c`; no consumer found in this source tree |

These observations are consistent with unused-data elimination or configuration differences. They do not prove the exact compiler/linker settings, establish absence of a codec feature, or justify assigning a different equal-prefix table. They close the immediate source-consumer follow-up for these eight nonmatches.
