# Codec definition follow-up

This 2026-09-28 pass addresses remaining item 3 after platform/runtime analysis. Two definitions are now applied in `mvd.i64`: capability `+0x18` is `jpegProgSupport`, and register ordinal 10 is `HWIF_DEC_ABORT_E`. There are **726 named register ordinals out of 730**. Four register definitions and the original names of the extra VP6/VP8 info bytes remain blocked as described below.

## Capability slot: recovered source layout

The earlier comparison used `source/inc/dwl.h`, whose `DWLHwConfig` orders members differently. The overlooked `source/inc/decapicommon.h:149–173` defines `DecHwConfig` with precisely the first **22 words** of MVD's capability record. All member offsets, widths and previously established names match; MVD appends three words at `+0x58`, `+0x5C` and `+0x60` for hardware error concealment, stride and field-DPB support.

The matching prefix is:

```
mpeg4Support, customMpeg4Support, h264Support, vc1Support,
mpeg2Support, jpegSupport, jpegProgSupport, maxDecPicWidth,
ppSupport, ppConfig, maxPpOutPicWidth, sorensonSparkSupport,
refBufSupport, tiledModeSupport, vp6Support, vp7Support,
vp8Support, avsSupport, jpegESupport, rvSupport,
mvcSupport, webpSupport
```

Thus `MvdDwlHwConfig.unresolvedWord6` is now `jpegProgSupport`, a `u32` at `+0x18`. The structure remains 100 bytes. This identification comes from a full layout match, not the zero stored there.

The behavioral result remains unchanged: `DWLReadAsicConfig` clears the record at `0x10DE14`, never subsequently writes this slot, and no active consumer was recovered. The reader instead encodes progressive JPEG capability through `jpegSupport == 2`, subject to synthesis/fuse checks. The recovered legacy name therefore does **not** establish an additional enabled JPEG service or capability.

## Ordinal 10: decoder abort control

The adjacent G2 package's `software/source/common/8170table.h` gives `HWIF_DEC_ABORT_E` the triple `(1, 1, 5)`. MVD ordinal 10 has that exact triple at `0x11A404`, between IRQ and IRQ-disable definitions. Cross-generation position alone would be insufficient.

Independent G1 evidence comes from NXP's Hantro driver: `IS_G1` identifies ASIC family `0x6731`; its release/reset paths write `HANTRODEC_DEC_ABORT` when stopping an enabled decoder. The corresponding header defines that mask as `0x20` in register word 1. The supplied console ID `0x67312398` belongs to that family. This corroborates abort-control semantics for the G1 register location, with the `HWIF_DEC_ABORT_E` spelling supplied by the adjacent Hantro table. See [NXP driver](https://coral.googlesource.com/linux-imx/+/refs/heads/release-beaker/drivers/mxc/hantro/hantrodec.c) and [register definitions](https://coral.googlesource.com/linux-imx/+/refs/heads/release-beaker/drivers/mxc/hantro/dwl_defs.h).

The enum now includes `HWIF_DEC_ABORT_E = 10`. An IDA comment records both sources and the limit: **no recovered MVD call or selector array selects ordinal 10**. This names the definition; it does not prove that the console's configured hardware implements abort, that MVD requests abort, or that the downloaded NXP driver was compiled into MVD.

## Remaining blockers

| Item | Established evidence | Why no further name was applied |
|---|---|---|
| Ordinal 8, word 1 bit 11 | G2 defines `HWIF_DEC_ABORT_INT` at the same location and in the same IRQ neighborhood | No recovered MVD ordinal consumer; the independent Linux IRQ-expansion patch explicitly labels abort status as G2-and-later. The G1 control-bit evidence above does not independently establish this status bit |
| Ordinal 128, word 7 bit 13 | Located in the VC1 definition sequence between `PQINDEX` and `BILIN_MC_E`; no recovered consumer | VP8 `CH_MV_RES` aliases the bit in a different codec sequence. VC1 `FAST_UVMC_E` is elsewhere, at word 5 bit 20. Neither is a supported assignment here |
| Ordinal 282, word 20 full width | Aliases ordinal 281 `REFER6_BASE`, next to VP8 stride/chroma definitions; no recovered consumer | H.264 reference selectors use 281. Rockchip VP8's word-20 scan-map fields do not identify a distinct full-word alias |
| Ordinal 598, word 58 bit 31 | Only recovered selection is initialization to zero at `0x10DDE2`; supplied register dump has zero at this word | A zero write does not identify a feature. Rockchip's VP8 header names the whole word `reg58_debug` without defining this bit; its H.264 header leaves words 58 onward as an array |
| VP6 info `+0x1C`, VP8 info `+0x20` | Each getter writes one zero byte; three following padding bytes are not written; whole 36/40-byte records are copied by IPC | Available local API headers omit these bytes. No consumer distinguishes their meanings; `constantZero` remains the accurate descriptive name. Do not infer DPB mode or interlacing from position alone |

The [Linux patch](https://lkml.iu.edu/hypermail/linux/kernel/2010.1/04574.html) is useful negative evidence for transferring the abort-status name indiscriminately. It is not a specification for the console revision. Hardware capability and original source spelling remain separate questions.

Closing these blockers requires a matching Hantro branch/API header or register specification, or an additional consumer that constrains the field. Repeating the previous all-code accessor scan cannot recover names from absent uses or zero-only writes. No alternate console revision is assumed.

## Search scope and provenance

The original G1 register table, its two identical testbench copies and the distinct bootloader G1 copy were compared. None supplies the missing ordinal definitions beyond aliases already known. The adjacent G2 table supplied the abort-bit candidates. Rockchip's primary MPP headers and VP8 implementation were checked at commit `14729dd578e570e5f00fd1dd2113f5429012d64b`: [H.264 VDPU1 registers](https://github.com/rockchip-linux/mpp/blob/14729dd578e570e5f00fd1dd2113f5429012d64b/mpp/hal/rkdec/h264d/hal_h264d_vdpu1_reg.h), [VP8 VDPU1 registers](https://github.com/rockchip-linux/mpp/blob/14729dd578e570e5f00fd1dd2113f5429012d64b/mpp/hal/vpu/vp8d/hal_vp8d_vdpu1_reg.h), and [VP8 setup](https://github.com/rockchip-linux/mpp/blob/14729dd578e570e5f00fd1dd2113f5429012d64b/mpp/hal/vpu/vp8d/hal_vp8d_vdpu1.c). These comparisons do not claim Rockchip or G2 code inclusion in MVD.

Relevant source SHA-256 values, allowing the mutable local/NXP references to be distinguished:

| Source | SHA-256 |
|---|---|
| Local G1 `source/inc/decapicommon.h` | `9d0bd267ee2520cf4317d51bb434f61ae16a2ff440810355abd85d1cd4b0927e` |
| Local G2 `software/source/common/8170table.h` | `4be495d25e6fd758bf02985cd2b72f05980cb5890efa8d6d6900266683b700fe` |
| NXP `hantrodec.c`, decoded source | `2776f3bc0b6c5186ef0d9d757d3611615a4e06112c68a46d6c787a64a66953c8` |
| NXP `dwl_defs.h`, decoded source | `4f6de9569c08f1b74b91372644cc03b1e1aac74159fb9bdbae5c5be39453ca40` |

## Database changes and validation

The pass renames one structure member, adds one enum member, and updates two evidence comments. The capability prefix was compared programmatically against all 22 source fields and offsets before renaming. The register entry was checked as `(1,1,5)`. The database was saved and reopened; the capability member and its 100-byte parent layout persisted. Function counts remain 800 total and zero `sub_*`; function attribution and prototypes are unchanged.

Loaded code/read-only bytes in `[0x100000, 0x1207E8)` have identical before/after SHA-256 `d9fe0e95ca381193b9903a2df6a1a3812e6ad1849be61920b3552ee2768dfb34`. No firmware bytes, MMIO values, reference source files or live hardware were changed.
