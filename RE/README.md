# MVD sysmodule reverse engineering

Analysis of the supplied `mvd.i64`, performed on 2026-09-28. Addresses throughout these notes are **IDA virtual addresses**, not file offsets. The database originated from a GDB remote process; it does not identify a firmware version or provide an input-binary hash. Do not generalize version-specific observations to every MVD release.

## Main findings

* The module contains Hantro decoder-library code, not merely a custom driver for compatible hardware. The H.264, VP6, VP8/WebP and postprocessor API implementations have identifiable counterparts in the supplied `hlibg1v6` source tree.
* The current inventory contains 598 inventoried function names/prototypes, and 725 of 730 register fields are named. Three data passes add 88 constant-table names/types. Control-flow corrections repair a falsely separated H.264 epilogue, two Thumb long branches and a switch branch misclassified as data. Storage `+0x39DC` is identified as `mvcEnabled`. The latest readability pass separates VP8 filter/reference-buffer lifetimes, corrects scratch-union views, resolves further H.264/VP8 pointer aliases and confirms a separate MVC DPB-size cap. The database has 796 functions, with 170 `sub_*` names after the auxiliary-driver pass.
* The auxiliary-driver pass adds 95 names, recovers L2B/Y2R contexts and the DMA ABI, and fixes the ARM cache-clean veneer and L2B receive-command decoding. Its command tables cover both L2B engines and all 45 Y2R2 commands; four more read-only arrays are typed separately from the codec-table count.
* All 33 `mvd:STD` commands can be assigned functional roles. The previously unclear command groups are H.264, VP8/VP7/WebP, VP6, and postprocessor operations.
* The service's 284-byte configuration is Hantro `PPConfig`. Its previously unknown areas contain RGB controls, masks, range mapping, rotation and deinterlacing settings.
* `0x00020001` in this configuration means **YCbCr 4:2:0 semiplanar**, not an H.264 codec selector. The initialization command selects the decoder; configuration selects its postprocessing pixel layout.
* Positive decoder results retain Hantro meanings. `0x17002` from next-picture means **picture ready**, while `0x17004` means **headers ready**. These need context and should not simply be called busy or incomplete processing.
* This build explicitly clears advertised MVC support and compiles postprocessor connections only for H.264, VP6 and VP8/WebP. The supplied GBATEK reference dump reports effective H.264, VP6 and VP8/WebP support with a 1920-pixel decoder/PP width; MPEG-4 and Sorenson synthesis support is masked off by fuses. This is separate reference evidence, not live state in the database.

## Reading order

1. [Sources, evidence and database changes](external-code.md)
2. [Complete mvd:STD command ABI](mvd-std.md)
3. [Postprocessor configuration and formats](postprocessor.md)
4. [Results, work-buffer sizing and memory handling](memory-and-results.md)
5. [Hardware interface and capability filtering](hardware.md)
6. [L2B and Y2R service dispatchers](auxiliary-services.md), [driver internals](auxiliary-drivers.md) and [IPC transport matrix](auxiliary-ipc.md)
7. [Methodology and remaining uncertainties](methodology.md)
8. [Address-to-name inventory](function-map.md)
9. [Recovered internal layouts](database-layouts.md) and [register-field inventory](register-fields.md)
10. [Decoder internals, state and source matches](decoder-internals.md)
11. [Codec leaf analysis, entropy and concealment](codec-leaf-analysis.md)
12. [H.264 DPB, prediction and workaround](h264-dpb-prediction.md)
13. [Parser metadata, scaling tables and support helpers](parser-support.md)
14. [Codec constant tables and VP8 scratch storage](codec-data.md)
15. [H.264 control-flow corrections](control-flow-corrections.md)
16. [H.264 MVC state and interior pointers](h264-mvc-state.md)
17. [Codec pointer lifetimes and scratch views](codec-readability.md)
18. [Attribution coverage and remaining data](attribution-coverage.md) and [170 remaining unnamed functions](remaining-functions.md)
19. [Nintendo DWL/platform adaptations](platform-glue.md)

The source tree is a matching **family/revision reference**, not proof that Nintendo compiled exactly that checkout. Names without an exact upstream counterpart use an `Mvd`/`MVDSTD_` prefix. Source-derived names preserve Hantro spelling. Details that remain uncertain are identified explicitly in the relevant document.
