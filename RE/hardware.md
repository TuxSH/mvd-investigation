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

Ordered `(word, width, shift)` sequences were aligned against `source/common/8170table.h`, using matching runs of at least three triples. This yielded 698 transferred names in the database's `MvdHwIf` enum. Unmatched entries were not assigned speculative meanings. Repeated triples are common because fields overlap for different codecs; sequence context is part of the evidence, and equal triples alone do not establish a semantic match. See [register-fields.md](register-fields.md) for the exact transferred mapping and unresolved entries.

Register names inherited from other codec modes do **not** imply that their corresponding software decoders are compiled or accessible through IPC. The common register map spans more modes than this module exposes.

## Capability filtering

`DWLReadAsicConfig` first clears a 100-byte structure, reads synthesis words, conditionally reads fuse status for the applicable product IDs, and limits capabilities/dimensions to the fuse values. This differs from the local source's 84-byte capability structure. Applying that source structure wholesale would place several fields incorrectly.

The function extracts H.264, MPEG-4, VC-1, MPEG-2, JPEG, VP6, VP7, VP8, AVS, RealVideo, PP, reference-buffer and layout-related capabilities even though not all corresponding software codecs exist here. Decoder width combines low eleven bits of the first synthesis word with extension bits in the second. PP width uses thirteen bits of the PP synthesis word. PP presence gates both its configuration word and width.

Crucially, the MVC capability at structure word 20 is assigned **zero unconditionally** before fuse filtering, and is never enabled later. `H264DecSetMvc` checks it and returns format-not-supported. This is a concrete software restriction independent of unknown fuse values.

The product gates recognize legacy `0x8170` and special `0x6731` paths, with other version thresholds around `0x8190` and `0x9170`. Seeing those constants in branches does not identify the actual console ASIC ID. PP limits and feature workarounds vary by these branches.

`MvdDwlCreate` (`0x118CB0`) accepts client types 1 (H.264), 4 (PP), 7 (VP6) and 10 (VP8 family). `PPDecCombinedModeEnable` accepts only switch cases 1 (H.264), 6 (VP6), 9 (VP8) and 10 (WebP), although an initial range check admits 1..11. The two numbering schemes are different. They should not be conflated with either PP pixel formats or VP8DecFormat values.

`MvdBindDecoderInterrupt` (`0x114264`) creates the decoder event and binds interrupt `0x4F` once, guarded by a global initialized flag. L2B initialization reads interrupt IDs `0x45`/`0x46` from `0x11A000` for its two engines. These bindings are independent of whether the database contains a live register snapshot.

## What the database cannot establish

The mapped hardware segments in this supplied database are zero-filled placeholders, including the ASIC ID, synthesis and fuse words. Their values must not be interpreted as captured hardware state. The code establishes register layout, decoding logic and software restrictions; it does **not** establish which additional features Nintendo physically disabled or the exact decoder/PP width reported by a running console.

No actual decoding, interrupt timing, cache-coherency or throughput measurements were performed. A trustworthy live register capture would be needed to turn the capability-reader analysis into a concrete silicon feature matrix.
