# Methodology, evidence and remaining work

## Scope and inputs

This pass analyzed the supplied `mvd.i64` through IDA MCP and compared it with the local Hantro G1 and libctru trees identified in [external-code.md](external-code.md). The database has 797 functions. Its initial metadata described a GDB remote process, with no original executable checksum or identified firmware version. Existing useful SDK/service names were retained.

The first pass recorded a temporary recovery copy at `/tmp/mvd-re/mvd.before.i64`. It was not a tracked project artifact and was no longer present when the latest codec pass began. The latest pass started from the repository’s saved database. The edited database is saved in place as `mvd.i64`. IDA's loose working files belong to the active database session and were not deleted. No executable instructions were patched, and no changes were made to either reference source tree.

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
* The unused capability word and extra byte in VP6/VP8 info; the H.264 storage extension word at `0x39DC` and the exact silicon fault behind the frame-number workaround. Its bit-12 patch mechanism and G1 build gate are now established. Access-unit, macroblock, slice-command and VP8 parser/concealment regions are now recovered in the codec leaf pass
* L2B pixel-format ordering and hardware conversion details beyond the recovered register writes and DMA interface
* Exact origin of every runtime/SDK/internal decoder routine; many functions outside the exposed API and traced helpers remain unnamed
* Some Hex-Rays artifacts: overlapping packed work-size locals and aliases inside the output mapping array; those were not hidden by inventing a cleaner but unsupported layout

There was no previous analysis file in the repository to reconcile. These files describe the current database evidence. The [3DBrew page](https://www.3dbrew.org/wiki/MVD_Services) provided the starting interface vocabulary; its experimental labels were corrected where source and binary agreed, and unverified hardware claims remain qualified. GBATEK retrieval did not initially yield usable page contents. The user subsequently supplied a register excerpt; its six read-only words are explicitly recorded as reference evidence and decoded without patching MMIO placeholders.

## Continuation sequence and validation

The continuation analyzed PP scaling and shared PP interfaces, then H.264/VP6/VP8 parser, DPB, ASIC-buffer and state-management helpers. Source record layouts were examined with ARM32/short-enum Clang output, adjusted to binary-observed offsets, and checked in IDA. Explicit padding corrected the local-type parser’s differing alignment of mixed byte/word fields. The PP scaling block is fully named; hardware-error-concealment and stride capability bits are identified by their VP8 consumers.

Register call sites resolved 24 branch-specific fields plus the previously isolated source IRQ bit: 723 names total, seven unresolved. Codec instance, buffer, interface and parser sizes were read back after typing. DWL/platform analysis came last, covering filtered MMIO writes, caller-work-buffer allocation, no-op frees, status/event waits and PP cache flush. Disassembly was used for preserved argument registers, timeout conversion, the address-helper OR condition and syscall identities. Eighteen more platform-local names and four global names were applied.

Important continuation sizes: linear descriptor 12, H.264/VP6/VP8 containers 15860/2424/4224, H.264 DPB 1680, VP6 parser 1480, VP8 parser 2612, VP8 ASIC state 812, reference-buffer controller 228, shared decoder-to-PP interface 104, fuse status 76. All 316 inventory names were checked against their database addresses. The decompiler cache was invalidated for all 797 functions; fresh pseudocode confirmed the register-preservation corrections in the MMIO writer and PP cache helper, and propagated stack types in PP initialization and capability decoding. Interior-pointer aliases and the packed-local artifacts noted above remain. No firmware instructions, MMIO contents or reference-source files were changed.

## Codec leaf pass

The next pass kept service/hardware unknowns and platform glue last, as requested. It added 105 names/prototypes, bringing the inventory to 421. Six are descriptive VP8 concealment names; the remaining additions have source counterparts, with ABI/layout differences recorded in [codec-leaf-analysis.md](codec-leaf-analysis.md). Seventy-two additional local-variable renames were applied across the custom VP8 concealment code. Source-matched routines retain upstream terminology.

H.264 access-unit state, macroblock state/payload and nested slice commands were recovered from consumers and source layouts. The additional AUB frame-number word was retained, not collapsed into the 72-byte reference structure. VP8 parser/concealment state and VP6 Huffman structures were typed. Read-back sizes were AUB 76, reordering 276, marking 716, macroblock layer/storage 1124/160, slice header 1364, VP8 accumulator/state 36/16 and VP6 Huffman workspace 4544. Parent sizes remained unchanged: H.264 storage/container 14864/15860, VP8 parser/container 2612/4224 and VP6 parser 1480.

Disassembly was used only for ambiguous signed motion-vector extraction, fractional-neighbor conditions, misleading null-pointer pseudocode and a missing lower-left boundary guard. The collector's eight-word histogram remains split in Hex-Rays despite attempted local typing; names/comments record the correct span. A temporary stack-argument rename during this attempt was restored to `mbX`. No analysis result relies on an unaccepted type edit.

Register field 579 now has an observed normal/concealment interpretation, bringing the enum and register inventory to 724 names. Its other two encodings remain unknown; six other fields remain unnamed. Equal triples for ordinals 19/20 and 281/282 were not treated as proof of equal semantics.

All 421 inventory names and important type layouts were read back from IDA. Four PP callback names had become generic callback labels during type propagation; their documented names were restored and checked. The decompiler cache was refreshed for all 797 functions, documentation links/counts checked and the database saved. At that checkpoint there were 346 `sub_*` function names; this differs from the inventory remainder because the database also contains useful names retained from before this investigation. No decoder execution or fuzzing was needed for the source attribution and layout changes. Hardware behavior and the newly documented conditional boundary defect remain statically analyzed observations.

## DPB/prediction pass

This pass added 50 names/prototypes, for 471 inventory entries: 49 Hantro counterparts and one descriptive `MvdH264PatchFrameNumBit12` helper. It followed DPB allocation, reference marking, output rings and reference-list construction, then intra-prediction/neighbor helpers and the previously unresolved frame-number mask. Service/hardware questions and remaining platform glue remained deferred.

Disassembly resolved missing predicate arguments, the by-value 52-byte `IsReference` argument, MMCO 6's unsigned branch, and the bit-patching helper's byte write and return semantics. Three 48-byte neighbor tables were compared with source initializers and given names/types. Ten locals were renamed in the unmatched helper; source-derived routines retain source terminology. A two-byte neighbor type and 16-byte workaround union were added; parent sizes were read back unchanged.

The latest notes distinguish a confirmed software mechanism and build gate from an unknown underlying silicon fault. A field-xref query for `unresolvedWord3703` returned no references; that alone was not treated as proof that the word is unused. Relevant pseudocode was refreshed, all 50 newly applied names plus the four existing PP callback names were checked against IDA, and the database was saved. There are now 296 `sub_*` names. Documentation links/counts and whitespace were checked. No firmware bytes or live MMIO were changed, and no decoder experiment was performed.
