# Methodology, evidence and remaining work

## Scope and inputs

This pass analyzed the supplied `mvd.i64` through IDA MCP and compared it with the local Hantro G1 and libctru trees identified in [external-code.md](external-code.md). The database has 797 functions. Its initial metadata described a GDB remote process, with no original executable checksum or identified firmware version. Existing useful SDK/service names were retained.

The original database was copied to `/tmp/mvd-re/mvd.before.i64` before edits. That is a temporary recovery copy, not a tracked project artifact. The edited database is saved in place as `mvd.i64`. IDA's loose working files belong to the active database session and were not deleted. No executable instructions were patched, and no changes were made to either reference source tree.

## Analysis sequence

1. Inventory functions, segments, strings and existing symbols; locate service registration and the three dispatchers
2. Trace all `mvd:STD` cases to their wrappers, separate IPC transport from native library calls, and recover argument/result lengths
3. Match Hantro APIs using multiple fingerprints: parameter order, structure accesses, allocation sizes, error values, constants, callback structure and control flow
4. Compare layouts field-by-field. Import matching public structures, and create `Mvd*` types for branch-specific layouts instead of forcing the current Linux headers onto the binary
5. Align register-field triples as ordered sequences. Preserve binary enum ordinals and leave unmatched fields unresolved
6. Apply function names/prototypes and descriptive locals to service/platform code; retain upstream terminology for matched library functions
7. Inspect disassembly only where decompilation was insufficient: floating-point calling convention, packed work-size controls, configuration cache-size branches, output clearing and bounds, and a missing L2B receive branch
8. Document source correspondence, complete command-role maps, configuration fields, sizing/results, memory behavior and explicit uncertainty; verify important type sizes in IDA and save the database

The first pass applied 208 names/prototypes; the continuation adds 108, for 316 in the address inventory. This includes both identified Hantro routines and descriptively named Nintendo/platform wrappers, not 316 proven upstream-source functions. Two auxiliary dispatcher signatures were also refined while preserving their existing names. Eighty local-variable renames succeeded in the main wrapper/transport pass; one obsolete decompiler temporary disappeared after type propagation and was not forced into the stack frame. Additional local naming was adjusted when type propagation changed register reuse.

## Strength of evidence

**Direct binary observations** include command IDs and transport lengths, copies and pointer arithmetic, result conversion, the unconditional MVC capability clear, supported combined-mode switch cases, and the static wrapper defects. These observations have address anchors in the topic documents and database comments.

**High-confidence source attribution** combines structural and behavioral agreement across multiple functions and constants. It identifies Hantro family code. It does not prove an exact upstream commit, compiler version or unmodified source file. Platform DWL implementations are adapted to Nintendo services.

**Source-assisted field names** describe layouts that agree with binary accesses. The recovered public PP configuration is particularly strong: copy size, defaults, validation and hardware setup all corroborate the map. Register names transferred through aligned repeated triples have less independent semantic evidence than an API matched by full behavior; the exact transfer is exposed in the register inventory.

**Not established** includes successful hardware execution, complete codec/profile conformance, matching fuse settings across console revisions, timing, image quality, and exploitability of unchecked paths. Such claims are deliberately not inferred from a zero-filled hardware segment, a parser accepting headers, or the presence of common register names.

## Validation performed

IDA accepted the applied names and prototypes. Important recovered sizes were read back: `PPConfig` 284, `PPOutputBuffers` 140, PP container 1404, H.264 info/picture 64 each, VP8 info/picture 40/64, VP6 info/picture 36/36, capability structure 100, L2B parameters 8 and Y2R parameters 12 bytes. The annotated SPS structure is 708 bytes and the bitstream state is 28 bytes, matching the observed clear/copy sizes.

The documentation was checked for complete command coverage, internal links and consistency with the final type layouts. No build or unit tests apply to this documentation/database-only change. No decoding, malformed-input fuzzing or repeated empirical verification was performed.

## Remaining uncertainties

* Exact firmware/build identity and upstream Hantro release
* Whether a live console corresponding to this database matches the supplied GBATEK register reference; the conditional feature matrix is now decoded in hardware.md
* Runtime H.264 profile/bit-depth support, particularly the wiki's High10 claims; the SPS parser reads and discards depth fields
* The unused capability word and extra byte in VP6/VP8 info; H.264 access-unit-boundary storage, a storage extension word and opaque macroblock/slice substructures; VP8 parser/concealment extension words
* L2B pixel-format ordering and hardware conversion details beyond the recovered register writes and DMA interface
* Exact origin of every runtime/SDK/internal decoder routine; many functions outside the exposed API and traced helpers remain unnamed
* Some Hex-Rays artifacts: overlapping packed work-size locals and aliases inside the output mapping array; those were not hidden by inventing a cleaner but unsupported layout

There was no previous analysis file in the repository to reconcile. These files describe the current database evidence. The [3DBrew page](https://www.3dbrew.org/wiki/MVD_Services) provided the starting interface vocabulary; its experimental labels were corrected where source and binary agreed, and unverified hardware claims remain qualified. GBATEK retrieval did not initially yield usable page contents. The user subsequently supplied a register excerpt; its six read-only words are explicitly recorded as reference evidence and decoded without patching MMIO placeholders.

## Continuation sequence and validation

The continuation analyzed PP scaling and shared PP interfaces, then H.264/VP6/VP8 parser, DPB, ASIC-buffer and state-management helpers. Source record layouts were examined with ARM32/short-enum Clang output, adjusted to binary-observed offsets, and checked in IDA. Explicit padding corrected the local-type parser’s differing alignment of mixed byte/word fields. The PP scaling block is fully named; hardware-error-concealment and stride capability bits are identified by their VP8 consumers.

Register call sites resolved 24 branch-specific fields plus the previously isolated source IRQ bit: 723 names total, seven unresolved. Codec instance, buffer, interface and parser sizes were read back after typing. DWL/platform analysis came last, covering filtered MMIO writes, caller-work-buffer allocation, no-op frees, status/event waits and PP cache flush. Disassembly was used for preserved argument registers, timeout conversion, the address-helper OR condition and syscall identities. Eighteen more platform-local names and four global names were applied.

Important continuation sizes: linear descriptor 12, H.264/VP6/VP8 containers 15860/2424/4224, H.264 DPB 1680, VP6 parser 1480, VP8 parser 2612, VP8 ASIC state 812, reference-buffer controller 228, shared decoder-to-PP interface 104, fuse status 76. All 316 inventory names were checked against their database addresses. The decompiler cache was invalidated for all 797 functions; fresh pseudocode confirmed the register-preservation corrections in the MMIO writer and PP cache helper, and propagated stack types in PP initialization and capability decoding. Interior-pointer aliases and the packed-local artifacts noted above remain. No firmware instructions, MMIO contents or reference-source files were changed.
