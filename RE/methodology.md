# Methodology, evidence and remaining work

## Scope and inputs

This investigation analyzes the supplied `mvd.i64` through IDA MCP and compares it with the local Hantro G1 and libctru trees identified in [external-code.md](external-code.md). The database initially had 797 function entries; a later [epilogue correction](control-flow-corrections.md) reduces that to 796. Its initial metadata described a GDB remote process, with no original executable checksum or identified firmware version. Existing useful SDK/service names were retained.

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
* The unused capability word and extra byte in VP6/VP8 info; the full lifecycle of H.264 `mvc` at `+0x39E0` (the adjacent `+0x39DC` is now [identified as `mvcEnabled`](h264-mvc-state.md)) and the exact silicon fault behind the frame-number workaround. Its bit-12 patch mechanism and G1 build gate are now established. Access-unit, macroblock, slice-command and VP8 parser/concealment regions are now recovered in the codec leaf pass
* L2B pixel-format ordering and hardware conversion details beyond the recovered register writes and DMA interface
* Exact origin and ABI of the remaining runtime/SDK/service routines; all 265 unnamed entries are now classified in [remaining-functions.md](remaining-functions.md), with exact platform attribution deferred
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

The latest notes distinguish a confirmed software mechanism and build gate from an unknown underlying silicon fault. A field-xref query for `unresolvedWord3703` returned no references; that alone was not treated as proof that the word is unused. Relevant pseudocode was refreshed, all 50 newly applied names plus the four existing PP callback names were checked against IDA, and the database was saved. At that checkpoint there were 296 `sub_*` names. Documentation links/counts and whitespace were checked. No firmware bytes or live MMIO were changed, and no decoder experiment was performed.

## Parser/support pass

This pass added 30 function names/prototypes (28 standalone Hantro counterparts and two descriptively named extracted setup sequences), bringing the inventory to 501. It also corrected `0x114B8E` from `h264bsdSarSize` to `h264GetSarInfo`; the actual SAR accessor is `0x1189C0`. The original function comment retains an explicit correction. Details and source paths are in [parser-support.md](parser-support.md).

The VUI and HRD layouts were reconstructed from source and binary field accesses, then imported at 952/412 bytes. SPS `+0x58` is now a typed VUI pointer; parent sizes remain unchanged. Six scaling/scan tables were compared across all 960 bytes and named/typed. Parser errors were followed into the SPS caller: several ignored errors are inherited source behavior, not proof of complete syntax validation. Source names were preserved in matched code; the common initializer received a descriptive argument/local name.

Disassembly was needed for an ambiguous enum argument in the shared register initializer. It confirms ordinal 19 at `0x10DD96`; the source-matched initialization sequence independently identifies data-discard semantics. The enum and register inventory now have 725 names, leaving 8, 10, 128, 282 and 598 unresolved. Ordinal 598 is also cleared here, but this does not identify its meaning. Composite enum renderings were explicitly marked as artifacts.

All 30 additions, the corrected SAR name and four PP callback names were read back. IDA accepted the final prototypes; an initial wrong type spelling was corrected before completion. HRD/VUI/SPS sizes read back as 412/952/708; H.264 storage/container, PP container, VP8 parser and VP6 container remain 14864/15860/1404/2612/2424. The decompiler cache was refreshed, documentation consistency checked and the database saved. There are 266 `sub_*` names; this includes unclassified SDK/runtime/service code, not just codec routines. No firmware bytes, MMIO values or reference-source files were changed. Service/hardware questions and remaining platform glue remain last.

## Codec-data and scratch-storage pass

The [codec-data pass](codec-data.md) adds 67 data names/types without changing the 501-function inventory. A bounded scan compared complete integer-literal arrays with all 28,672 bytes of `.rodata`, reading in checked 512-byte chunks to avoid tool-output truncation. Of 75 candidates, 69 arrays matched in full; seven were already named. Five additional H.264 register-selector arrays matched after translating source names to MVD enum ordinals. Consumers disambiguated repeated values, including the two AC quantizer arrays and three local reference-list initializers. The 67 newly attributed arrays cover 14,586 bytes.

All 74 selected table names, addresses and sizes were read back. An interior auto-generated pointer interpretation initially prevented the Y2 AC table from becoming a full array despite a successful tool response; undefining its verified 256-byte data span and recreating the array fixed it. Fresh pseudocode confirms the named CABAC initializer and its 3680-byte copy, while later writes confirm the 136-byte POC and 224-byte scaling-list tails. These are analysis-data edits, not firmware patches.

Disassembly of `VP8DecDecode` showed that the apparent capability-field uses were aliases of saved pointers. A 128-byte union now models two overlapping 100-byte configs at `SP+0x20` and `SP+0x3C` plus the pointer lifetime. Local type application alone could not enlarge the previous inferred variable; an explicit stack declaration succeeded. The union and frame span were read back as 128 bytes. Hex-Rays still chooses incorrect member views at some sites, so comments and the documentation identify those expressions rather than presenting them as genuine capability semantics.

At this checkpoint the unknown capability word, extra VP6/VP8 info byte, H.264 extension word and five register ordinals remained unresolved. The later [MVC state pass](h264-mvc-state.md) identifies the H.264 extension word through its API writer and aliased parser read. Bounded searches and zero stores do not justify semantic names. Remaining functions sampled in this pass were platform/service/runtime helpers and were left for the requested final phase. Relevant pseudocode was refreshed, documentation links and whitespace checked, and the database saved. No decoder execution, live hardware changes or reference-source edits were performed.

## Small tables and control-flow correction

The next continuation extends constant attribution to 19 more arrays (980 bytes): eight small CAVLC tables, three VP6 probability defaults, two structured arrays and six VP6/VP8 register-selector arrays. Consumers distinguish ambiguous two-byte patterns and identical selector copies. Source record declarations establish the signed eight-byte `LINE_EQ` and existing twelve-byte memory-cost layout. Partially initialized automatic arrays are excluded rather than misrepresented as small read-only tables. Final table names/sizes and both record layouts were read back; fresh pseudocode displays the VP6 linear equations and separate VP8 partition base/bit selectors correctly. See [codec-data.md](codec-data.md).

A targeted review of unnamed entries embedded among codec functions found runtime/SDK helpers and one false function: `sub_115CDE` is the shared error epilogue of `h264bsdDecode`. Stack restoration, branch range and the source's error returns establish the correction. After merging the range into its parent, the Thumb `BL` at `0x1165A4` also needed an explicit jump interpretation. The installed IDA batch interface supplied those two operations; the MCP session was closed before editing and reopened for verification. No instruction bytes changed. The corrected pseudocode returns 3 after NAL-extraction failure and after marking a failed slice corrupt, with no false continuation. Details and the IDA reference are in [control-flow-corrections.md](control-flow-corrections.md).

Current counts are 796 functions, 265 `sub_*` names and 501 inventoried names/prototypes. The two constant-data passes add 86 arrays totaling 15,566 bytes. A separate merged-lifetime artifact in `VP8HwdAsicInitPicture` is now commented: one stack pointer first addresses a filter row and later the reference-buffer controller. Unresolved field/register meanings remain unassigned. Platform attribution and deferred hardware questions retain their requested order.

## MVC state, interior pointers and API control flow

The next static-analysis pass follows `H264DecSetMvc` and the NAL acceptance gate, identifying storage `+0x39DC` as `mvcEnabled`. Earlier member-xref and listing searches missed the gate's biased-pointer access. The adjacent `mvc` word remains separate: prefix processing copies the enable flag into it. The later readability pass also establishes its DPB-size cap of 8. Two shifted pointer types now expose named stream/MVC members in `h264bsdDecode`; at that checkpoint a third pointer rename persisted but its attempted shifted type still rendered as a plain indexed pointer. The following readability pass confirms that even a frame-member type repair is lost after reopening; that item remains unfinished. [The MVC state note](h264-mvc-state.md) records the exact offsets, writer, consumer and remaining display limits. The normal API still fails its capability gate.

A bounded scan of existing function entries and intra-function call references finds another Thumb long jump in `H264DecDecode`, at `0x103A44`. Source comparison and the remaining-length test establish its loop role. Correcting its IDA interpretation removes a false function call; refreshed output then exposes a two-byte switch-default branch at `0x1032D0` marked as data. Reclassifying those bytes as code restores stream accounting and removes that `JUMPOUT`. [Control-flow corrections](control-flow-corrections.md) records both changes and the scan's limits.

Read-back confirms `mvcEnabled`, the capability-gated write, named shifted-pointer accesses, the corrected loop/default path and unchanged 14864/15860-byte H.264 storage/container sizes. Database counts remain 796 functions and 265 unnamed `sub_*` entries; the documented attribution inventory remains 501. Previous notes that called `+0x39DC` unknown now link to the resolution. No firmware bytes, live MMIO or reference files were modified. Unresolved register/capability fields, remaining constants and custom-code readability precede the deferred hardware questions; remaining platform attribution is still last.

## Codec lifetime and union-view cleanup

The next pass repairs two reused VP8 stack slots, separating filter/tap pointers from reference-buffer/reference-ID locals. It saves 35 instruction-specific scratch-union selections in `VP8DecDecode`, gives seven saved-pointer members their parent-relative types, and improves eleven other VP8 interior-pointer locals. The H.264 picture-pointer frame type briefly rendered correctly but failed the reopen check and remains unresolved as a display issue; allocation and info-query helpers gain three persistent parent-relative locals. Fresh pseudocode, rather than successful mutation returns alone, verifies the final result. See [codec-readability.md](codec-readability.md).

A scan of named H.264 member accesses identifies the enable flag's effect on reported PP buffer count and the neighboring state's DPB cap. Disassembly confirms a read of storage `+0x39E0`, followed by a cap of 8 before DPB reset; the supplied allocation source lacks this condition. This adds a branch difference without claiming MVC execution or a complete state-lifecycle proof.

The database was saved, closed before batch edits and reopened for verification. Parent sizes and 796/265/501 function counts remain unchanged. Documentation was reconciled with the repaired aliases, and local links/whitespace checked. Remaining work includes SPS/PPS stack overlays, other unclassified functions/data and unresolved fields, followed by the deferred hardware questions and platform glue last. No executable bytes, reference trees or live hardware were changed.

## Full unnamed-function triage and remaining data references

The next continuation prioritizes function/constant attribution. Every one of the 265 remaining `sub_*` entries was decompiled and reviewed with reference context. The resulting [complete deferred inventory](remaining-functions.md) groups them by observed service/driver, SDK, startup and runtime behavior without applying speculative source names or ordinary C prototypes to unfamiliar runtime ABIs. No additional confident Hantro function match emerged. This replaces earlier sampled assessments with a complete triage of existing unnamed entries, while retaining the limitations of IDA's current boundaries and reference graph.

A separate data-reference audit covers all 501 documented functions, resolving interior references to containing items. Of nine remaining auto-named read-only heads, two are now typed arrays and seven remain platform/service data. The additions are a source-matched scaling-default pointer initializer and the already documented MVD level-limit records. The stack copy of the pointer initializer is corrected from signed integers to pointers. All source-table nonmatches were revisited for active consumers; conditional or absent consumers explain why a nonmatch need not imply a missing decoder feature. Details and exact remaining scope are in [attribution-coverage.md](attribution-coverage.md).

Both table extents and fresh pseudocode were verified, including reopening to confirm persistence. Counts remain 796 functions, 265 `sub_*` names and 501 inventoried functions; the three data passes now total 88 new arrays and 15,802 bytes. The broad remaining-function triage is complete, with exact platform attribution deferred. Unresolved codec field semantics and display artifacts remain before deferred hardware questions and platform glue last. No firmware bytes, live hardware or source trees were changed.
