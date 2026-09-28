# Hantro G1 hardware interface

## Register layout

The module uses the G1 register bank at virtual address `0x1ED07000`. The table-driven accessors are `SetDecRegister` (`0x10F324`) and `GetDecRegister` (`0x10E0FC`); masks start at `0x11A308`, and register-field triples at `0x11A38C`. Each triple holds register word index, field width and bit shift. A field's byte offset in the hardware bank is four times its word index.

| Bank offset | Purpose | Evidence |
|---|---|---|
| `0x000` | ASIC ID, product in high halfword | `DWLReadAsicID`, `0x10E044` |
| `0x004` | Decoder control/IRQ fields | Register table word 1 |
| `0x0C8` | Decoder synthesis configuration | `DWLReadAsicConfig`, `0x10DE0C` |
| `0x0D8` | Additional decoder synthesis configuration | Same |
| `0x0E4` | Decoder fuse status | `DWLReadAsicFuseStatus`, `0x101788` |
| `0x0F0` | PP control/IRQ fields, first PP shadow word | PP run/flush/refresh |
| `0x18C` | PP fuse status | Fuse reader |
| `0x190` | PP synthesis configuration | Configuration reader |

The PP container holds **41 register words**, representing bank words 60 through 100. Several PP calls form a pointer 60 words before this array when invoking the common register accessor. That is an indexing convention: the common register table uses absolute bank word numbers. It is not evidence of a full decoder shadow array inside the PP object.

## Register-name transfer

The binary table has 730 entries, versus 701 in the local source including aggregate IRQ entries. Copying the Linux enumeration unchanged would silently assign incorrect names after insertions.

Ordered `(word, width, shift)` sequences were aligned against `source/common/8170table.h`, using matching runs of at least three triples. This yielded 698 transferred names in the database's `MvdHwIf` enum. Subsequent call-site analysis identified 26 additional fields, including PP dimension/mask/clipping extensions, the 13-bit display width, VP8 stride/chroma controls and H.264 field-DPB mode. These use an `MVD_HWIF_` prefix to distinguish recovered semantics from transferred source names. The isolated source `HWIF_DEC_IRQ` entry was also confirmed from IRQ-clear callers, bringing that checkpoint to 725 named entries. The [definition follow-up](codec-definition-followup.md) adds `HWIF_DEC_ABORT_E` at ordinal 10 using independent G1 driver evidence, bringing the current total to 726; four remain unresolved. Field 19 is now `MVD_HWIF_DEC_DATA_DISC_E_ALIAS`, supported by the shared initializer’s source-matched data-discard operation at `0x10DD96`, in addition to its duplicate triple. Field 579 is now named `MVD_HWIF_VP8_CONCEALMENT_MODE` from the normal/concealment setup paths (values 0/1); values 2/3 remain unknown. Repeated triples are common because fields overlap for different codecs; sequence context is part of the evidence, and equal triples alone do not establish a semantic match. See [register-fields.md](register-fields.md) for the exact mapping.

Register names inherited from other codec modes do **not** imply that their corresponding software decoders are compiled or accessible through IPC. The common register map spans more modes than this module exposes.

## Capability filtering

`DWLReadAsicConfig` first clears a 100-byte structure, reads synthesis words, conditionally reads fuse status for the applicable product IDs, and limits capabilities/dimensions to the fuse values. This differs from the local source's 84-byte capability structure. Applying that source structure wholesale would place several fields incorrectly.

The function extracts H.264, MPEG-4, VC-1, MPEG-2, JPEG, VP6, VP7, VP8, AVS, RealVideo, PP, reference-buffer and layout-related capabilities even though not all corresponding software codecs exist here. Decoder width combines low eleven bits of the first synthesis word with extension bits in the second. PP width uses thirteen bits of the PP synthesis word. PP presence gates both its configuration word and width.

Crucially, the MVC capability at structure word 20 is assigned **zero unconditionally** before fuse filtering, and is never enabled later. `H264DecSetMvc` checks it and returns format-not-supported. This is a concrete software restriction independent of unknown fuse values.

The product gates recognize legacy `0x8170` and special `0x6731` paths, with other version thresholds around `0x8190` and `0x9170`. Seeing those constants in branches does not identify the actual console ASIC ID. PP limits and feature workarounds vary by these branches.

`MvdDwlCreate` (`0x118CB0`) accepts client types 1 (H.264), 4 (PP), 7 (VP6) and 10 (VP8 family). `PPDecCombinedModeEnable` accepts only switch cases 1 (H.264), 6 (VP6), 9 (VP8) and 10 (WebP), although an initial range check admits 1..11. The two numbering schemes are different. They should not be conflated with either PP pixel formats or VP8DecFormat values.

`MvdBindDecoderInterrupt` (`0x114264`) creates the decoder event and binds interrupt `0x4F` once, guarded by a global initialized flag. L2B initialization reads interrupt IDs `0x45`/`0x46` from `0x11A000` for its two engines. These bindings are independent of whether the database contains a live register snapshot.

## Supplied GBATEK register dump

The user supplied the following GBATEK excerpt during the continuation. It is reference hardware evidence, **not a register capture from this database or an independently repeated measurement**. No placeholder MMIO bytes were patched with these values.

| Physical address | Bank offset | Value | Interpretation |
|---|---|---|---|
| `0x10207000` | `0x000` | `0x67312398` | Product `0x6731`, revision low halfword `0x2398` |
| `0x102070C8` | `0x0C8` | `0x07B4AF80` | Decoder synthesis word 1 |
| `0x102070D8` | `0x0D8` | `0xC09A0000` | Decoder synthesis word 2 |
| `0x102070E4` | `0x0E4` | `0x8516FFFF` | Decoder fuse word |
| `0x1020718C` | `0x18C` | `0xFFFFFFFF` | PP fuse word |
| `0x10207190` | `0x190` | `0xFF874780` | PP synthesis word |

The excerpt reports a 512-byte bank mirrored throughout `0x10207200..0x10207FFF`. Its R/W entries with `FFFFFFFF` are not treated as normal initialized decoder state. Physical offsets agree with the bank used at virtual `0x1ED07000` by MVD.

Applying the binary's synthesis extraction, product gates and fuse filtering to those six reference values gives:

| Capability | Synthesis | Fuse / software effect | Effective report |
|---|---:|---|---:|
| H.264 | 3 | Enabled | 3 |
| MPEG-4 | 1 | Fuse disabled | 0 |
| Sorenson Spark | 1 | Fuse disabled | 0 |
| VP6 | 1 | Enabled | 1 |
| VP7 | 0 | Fuse also disabled | 0 |
| VP8 | 1 | Enabled | 1 |
| WebP | 1 | Shares VP8 fuse check | 1 |
| MPEG-2, VC-1, JPEG, AVS, RealVideo, custom MPEG-4 | 0 | No enablement | 0 |
| MVC | 0 | Also unconditionally cleared by software | 0 |
| Maximum decoder width | 1920 | Fuse limit 1920 | 1920 pixels |
| Maximum PP output width | 1920 | Fuse limit 4096 | 1920 pixels |
| Reference buffer support | 1, plus synthesis/product flags | Fuse enabled | Bitmask 11 |
| Tiled references | 1 | No further removal | 1 |
| Hardware error concealment | 0 | — | 0 |
| Programmable stride | 0 | — | 0 |
| Field DPB ordering | 0 | — | 0 |

The H.264 capability value 3 is the library's high-profile hardware tier; it does not independently prove High10/10-bit decoding. The [hardware follow-up](hardware-followup.md) additionally establishes the fixed eight-bit DPB sample layout and proves both VP8 motion-vector-concealment entry calls are disabled under this reference capability state. The JPEG-extension bit is set, but JPEG decoding itself is absent from the effective report. A set extension bit is not sufficient to enable its parent codec.

PP is present. Its synthesis word advertises blending, deinterlacing, dithering, tiled 4×4 output, pixel-accurate output, blend cropping, configurable endian handling and tiled input. Scaling bits 27:26 are 3, selecting fast-scaling support mode 1 in `PPSelectOutputSize`; the PP fuse word does not remove these features. The software sets a separate maximum output height of 4096. These are capability/validation limits, not evidence that every combination is valid or has been executed.

`PPInitHW` enables a horizontal coefficient-rounding workaround only when `DWLReadAsicID() >> 3 == 216408617`, i.e. IDs `0x67311148..0x6731114F`. The supplied `0x67312398` does not meet this test.

## What the database cannot establish

The mapped hardware segments in this supplied database are zero-filled placeholders, including the ASIC ID, synthesis and fuse words. Their values must not be interpreted as captured hardware state. At the user’s instruction, the investigation now assumes a single console revision and uses the supplied GBATEK values as its target state. Revision-to-revision matching is therefore outside the remaining work; the assumption does not turn placeholder bytes into measured values.

The register-write allowlist matches the writable ranges of the supplied dump; see [platform-glue.md](platform-glue.md). No actual decoding, interrupt timing, cache-coherency or throughput measurements were performed. The [hardware-validation status](hardware-validation.md) records the remaining blockers, published Y2R numerical/timing evidence, and the observations required to settle actual High10, conversion-edge and peripheral-timing behavior.
