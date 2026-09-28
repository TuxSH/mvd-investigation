# Completed libctru clients for the MVD module

This directory contains **altered local copies of libctru**, extended from the recovered MVD service ABI. The upstream checkout is unchanged. These clients have been compiled and tested against mocked IPC, **not run on a console**.

## Contents and provenance

| Files | Origin and scope |
|---|---|
| `include/3ds/services/mvd.h`, `source/services/mvd.c` | Copies of libctru's MVD client, completed for all 33 `mvd:STD` commands |
| `include/3ds/services/y2r2.h`, `source/services/y2r2.c` | Copies of libctru's Y2R client, renamed and adapted to `y2r2:u`, including command `0x2D`; all 45 commands |
| `include/3ds/services/l2b.h`, `source/services/l2b.c` | New clients for all 23 commands of **each** of `l2b:u` and `l2b2:u`, selected by an engine argument |
| `upstream/` | Unmodified copies of the original `mvd.c`, `mvd.h`, `y2r.c` and `y2r.h` for comparison |
| `upstream/manifest.json` | Upstream commit, original paths and SHA-256 hashes |
| `tests/` | ARM C/C++ structure checks and host IPC/ownership/error-path tests |

The baseline is local libctru commit `0a376398d19505df3c236342827a3a72a42dd41e` from [devkitPro/libctru](https://github.com/devkitPro/libctru/tree/0a376398d19505df3c236342827a3a72a42dd41e). Its license is retained in [LICENSE.libctru](LICENSE.libctru). The original Y2R client talks to `y2r:u`, a different module: it is retained unchanged under `upstream/`; the completed derivative uses independent `y2r2Init`/`y2r2Exit`, `Y2R2U_*` names and `Y2R2_*` enum constants. It can coexist with ordinary libctru Y2R.

There are 101 distinct command implementations, covering 124 service/command pairs because the two L2B services share the same 23-command implementation. No Hantro decoder implementation or sysmodule binary is incorporated into these clients.

Primary evidence: [completed MVD service reference](../MVD_Services.md), [STD ABI](../RE/mvd-std.md), [PP configuration](../RE/postprocessor.md), [result conversion](../RE/memory-and-results.md), [auxiliary transport](../RE/auxiliary-ipc.md), [auxiliary drivers](../RE/auxiliary-drivers.md), and [pixel packing](../RE/hardware-followup.md). The original libctru prototypes alone were not used to infer missing wire fields.

## Build and integration

With devkitARM and libctru installed:

```sh
make -C libctru-mvd
make -C libctru-mvd check
```

`DEVKITARM` and `LIBCTRU` can be overridden; `LIBCTRU` names the directory containing `include/3ds`. For the reference checkout:

```sh
make -C libctru-mvd LIBCTRU=/Users/elouan/Documents/git/libctru/libctru
make -C libctru-mvd check LIBCTRU=/Users/elouan/Documents/git/libctru/libctru
```

The archive is `build/libmvd-re.a`. Put this directory's `include` **before** libctru's include directory, and link `-lmvd-re` **before** `-lctru`, with this directory's `build` on the library search path. This supplies the replacement MVD object before the linker considers upstream `mvd.o`. Alternatively compile the three completed C files directly with the application. Do not compile files under `upstream/` or link both MVD objects with `--whole-archive`.

Include `<3ds/services/mvd.h>`, `<3ds/services/l2b.h>`, and `<3ds/services/y2r2.h>` explicitly as needed; this copy does not replace the umbrella `3ds.h`. C uses GNU11 anonymous unions/structures to retain legacy field aliases; GNU C++11 header compilation is also checked. Wire structures use explicit bytes for packed enum fields, and codec address fields use `u32` rather than host-sized pointers.

The service must be available and permitted by the application's service access configuration. These clients do not alter service permissions. The APIs use global service handles like libctru: callers must serialize initialization, shutdown, and each complete decode/conversion sequence. Reference counts are not a lock for concurrent reconfiguration.

## MVD STD API

### Compatibility and corrections

The original public MVD functions and legacy field names remain available. `MVDSTD_Config` is still exactly `0x11C` bytes. Named aliases now describe picture structure, video range, chroma/bottom-field bus addresses, inherited VC-1 controls, rotation, RGB adjustments/coefficients/masks, output masks, framebuffer placement and deinterlacing. Signed fields such as brightness and framebuffer X/Y have signed types. `MVDSTD_InitStruct` also has descriptive aliases for all initialization arguments; ignored work-size words have `ignored_*` aliases.

The canonical events are:

| Result | New name | Meaning |
|---|---|---|
| `0x17000` | `MVD_STATUS_OK` | Native operation succeeded |
| `0x17001` | `MVD_STATUS_STREAM_PROCESSED` | Input processed without an output-picture event |
| `0x17002` | `MVD_STATUS_PICTURE_READY` | A next-picture/peek operation returned a picture |
| `0x17003` | `MVD_STATUS_PICTURE_DECODED` | A picture was decoded |
| `0x17004` | `MVD_STATUS_HEADERS_READY` | Headers available; query dimensions/configure output |
| `0x17005` | `MVD_STATUS_ADVANCED_TOOLS` | H.264 advanced-tools event |
| `0x17006` | `MVD_STATUS_PENDING_FLUSH` | H.264 pending flush |
| `0x17007` | `MVD_STATUS_NONREF_SKIPPED` | Nonreference picture skipped |
| `0x17038` | `MVD_STATUS_SLICE_READY` | VP8-family/WebP slice output event |

Legacy `BUSY`, `FRAMEREADY`, `INCOMPLETEPROCESSING`, `PARAMSET` and `NALUPROCFLAG` names retain their numeric values, but are misleading; use the new names. `MVD_CHECKNALUPROC_SUCCESS` retains its previous accepted set and now evaluates its argument once. It is not a complete codec event state machine.

Lifecycle/release/size calls can return raw zero. Some unmapped native PP statuses also become zero, including native `PP_BUSY`. Consequently `R_SUCCEEDED(result)` alone does not establish that a picture or info structure is valid. These wrappers preserve actual service results rather than treating every nonnegative reply as a populated record.

The convenience API remains standalone PP or **H.264 plus PP**. It does not select VP8 based on `MVDSTD_InputFormat`: `MVD_INPUT_H264` is the historical name of **decoded YCbCr 4:2:0 semiplanar pixels entering PP**. The new alias is `MVD_INPUT_YUV420SP`.

Changes to the convenience implementation:

- Repeated `mvdstdInit` calls retain the first configuration rather than overwriting active globals
- Cleanup releases only successfully initialized decoder/PP components; standalone conversion never invokes H.264 dequeue/release
- The legacy 4096-byte work-buffer margin is retained and checked for arithmetic overflow
- H.264 input uses the new LINEAR address alias, consistent with work-buffer setup
- `mvdstdRenderVideoFrame(NULL, ...)` honors the original documented optional configuration
- Rendering with `wait=true` drains available pictures until the result is no longer picture-ready; it is **not a hardware-busy wait**
- Output-buffer count is checked as 1..17 before IPC, protecting against the server's unchecked translation loop; unused table entries are encoded as zero
- Only defined successful outputs are copied, and wire padding is cleared

### Explicit codec lifecycle

Use `mvdstdOpen()` / `mvdstdClose()` for explicit access without automatically allocating memory or choosing H.264. Do not mix these with `mvdstdInit()` / `mvdstdExit()` in the same session. A raw session has one decoder slot; H.264, VP6 and VP8/WebP lifetimes are mutually exclusive.

1. Open, allocate and retain an appropriately sized LINEAR work buffer, then call `MVDSTD_Initialize` with its **new LINEAR VA** and size
2. Initialize the selected codec. VP8 format values are `MVD_VP7=1`, `MVD_VP8=2`, `MVD_WEBP=3`; VP6 has a separate initializer
3. When PP output is needed, initialize PP and attach it with `MVD_PP_H264=1`, `MVD_PP_VP6=6`, `MVD_PP_VP8=9`, or `MVD_PP_WEBP=10`. There is no separate VP7 attachment case
4. Submit compressed frame/NAL payloads. The client does not demux IVF, RIFF/WebP, MP4, or FLV containers. Observe codec-specific returned events; query info and configure PP after headers are available
5. Retrieve pictures with the codec's `NextPicture`; nonzero `end_of_stream` flushes pending output. `Peek` does not dequeue
6. Detach PP before releasing PP and the codec, call `MVDSTD_Shutdown`, then close and free the work buffer

The sizing command and `MVD_DEFAULT_WORKBUF_SIZE` originate in the H.264 path; no general VP6/VP8/WebP size guarantee is added. Raw codec calls do not allocate buffers, translate stream aliases automatically, retry, or repair caller lifecycle ordering. The optional VP8/WebP user-picture VAs are forwarded by the service rather than remapped to module pointers; their mere presence in the structure does not establish a working arbitrary-client-buffer mode.

The frame-ID cycle `0..17` in the original high-level H.264 helper remains. This is separate from the internal G1 frame-number bit-12 workaround discussed in [the writeup](../writeup.md), which is disabled on the assumed console revision. The C clients do not patch compressed bitstreams or reimplement concealment.

### Command coverage

| ID | Public function |
|---|---|
| `01` | `MVDSTD_Initialize` |
| `02` | `MVDSTD_Shutdown` |
| `03` | `MVDSTD_CalculateWorkBufSize` |
| `04` | `MVDSTD_CalculateImageSize` |
| `05` | `MVDSTD_H264Initialize` |
| `06` | `MVDSTD_H264EnableMvc` |
| `07` | `MVDSTD_H264Release` |
| `08` | `MVDSTD_ProcessNALUnit` |
| `09` | `MVDSTD_H264NextPicture` |
| `0A` | `MVDSTD_H264GetInfo` |
| `0B` | `MVDSTD_H264Peek` |
| `0C` | `MVDSTD_Vp8Initialize` |
| `0D` | `MVDSTD_Vp8Release` |
| `0E` | `MVDSTD_Vp8Decode` |
| `0F` | `MVDSTD_Vp8NextPicture` |
| `10` | `MVDSTD_Vp8GetInfo` |
| `11` | `MVDSTD_Vp8Peek` |
| `12` | `MVDSTD_Vp6Initialize` |
| `13` | `MVDSTD_Vp6Release` |
| `14` | `MVDSTD_Vp6Decode` |
| `15` | `MVDSTD_Vp6NextPicture` |
| `16` | `MVDSTD_Vp6GetInfo` |
| `17` | `MVDSTD_Vp6Peek` |
| `18` | `MVDSTD_PpInitialize` |
| `19` | `MVDSTD_PpRelease` |
| `1A` | `MVDSTD_PpGetResult` |
| `1B` | `MVDSTD_PpEnableCombinedMode` |
| `1C` | `MVDSTD_PpDisableCombinedMode` |
| `1D` | `MVDSTD_GetConfig` |
| `1E` | `MVDSTD_SetConfig` |
| `1F` | `mvdstdSetupOutputBuffers` |
| `20` | `MVDSTD_GetNextOutput` |
| `21` | `mvdstdOverrideOutputBuffers` |

### Record validity and remaining service defects

- Info records are copied only for `MVD_STATUS_OK`. Picture records are copied only for `MVD_STATUS_PICTURE_READY`. Other results leave those outputs zeroed, even if the service copied its stack scratch into the reply
- In the normal work-buffer allocation path, decoder picture VA members identify client memory within the supplied LINEAR work buffer; paired bus addresses identify the same storage to hardware. These remain explicit `u32` wire fields. Observe picture validity, layout/stride, hardware completion, cache coherence and decoder buffer reuse when accessing pixels. With PP multibuffering, `MVDSTD_GetNextOutput` retrieves the separately registered PP output client VAs
- H.264 decode progress is copied only for the recognized `0x17000..0x17007` events. Its `end_vaddr` points into module scratch freed before the reply; use `remaining_size` to advance the original input instead
- VP6/VP8's additional decode output word is named `unused` in the G1 source and is intentionally not exposed as meaningful progress
- VP6/VP8 info `constant_zero` has established zero-only behavior but an unknown original meaning; it is not assigned a guessed codec feature name
- `MVDSTD_GetConfig` uses the exact `0x11C` mapped writable buffer; `SetConfig` preserves the service's readable mapped descriptor and process handle. The server does not implement structure-size negotiation and may overwrite output addresses in the submitted config in multibuffer mode
- Hantro pixel format constants are exposed for interpretation/configuration, **not as a promise of support**. The service initializes its PP cache-maintenance length only for `0x10001`, `0x40002`, and `0x41002`. Other formats can reach undefined cache-length behavior even when Hantro validation accepts them. `CalculateImageSize` rejects unsupported formats and arithmetic overflow rather than invoking the server's assertion path
- Legacy libctru `MVD_OUTPUT_BGR565=0x40002` and `MVD_OUTPUT_RGB565=0x40004` use opposite names to Hantro's corresponding RGB565/BGR565 constants. Both naming conventions remain explicit
- `GetNextOutput` can leave its server-side VA pair unwritten if no saved mapping matches. Even an OK result should be checked against the application's registered buffer pool. The client cannot recover missing server output by clearing its own destination first
- Only the first extra replacement VA mapping is saved by the server's override path. Repeated replacements are not a general mapping-table update API
- MVC initialization is exposed for ABI completeness but is capability-disabled on the analyzed console; native High10 support is not claimed

A focused read-only IDA check while implementing this client established the following successful-`Peek` omissions:

| Function | Binary location | Unwritten fields | Client behavior |
|---|---|---|---|
| `H264DecPeek` | `0x104328` | `viewId` | `view_id=0`, a placeholder rather than observed output |
| `VP6DecPeek` | `0x109874` | `outputFormat` and padding | `output_layout=MVD_LAYOUT_UNAVAILABLE` (`0xFF`), padding zero |
| `VP8DecPeek` | `0x1113F0` | `numSliceRows`, `outputFormat` and padding | `slice_rows=0` placeholder, layout unavailable, padding zero |

Dequeue records retain the returned layout. `MVD_LAYOUT_RASTER=0` and `MVD_LAYOUT_TILED_8X4=1` are distinct from the `0xFF` client sentinel. No database or binary edits were made for these checks.

## L2B/L2B2 and Y2R2

`l2bInit(L2B_ENGINE_0)` selects `l2b:u`; `L2B_ENGINE_1` selects `l2b2:u`. Each has its own reference count and handle. Every `L2BU_*` function takes the engine explicitly. The header exposes all setters/getters, DMA setup/completion, event, conversion, package and session-count operations. Session closure performs the server-side DMA cleanup; `StopConversion` alone does not stop/close DMA handles.

The Y2R2 derivative preserves the complete copied Y2R API shape under a separate namespace. `y2r2Init` acquires `y2r2:u` and invokes `DriverInitialize`; exit invokes `DriverFinalize`. In addition to commands `01..2C`, `Y2R2U_GetConversionParams` implements `2D`.

Recovered behavior reflected in the clients:

- RGBA8888 memory bytes are **AA BB GG RR**, RGB888 bytes are **BB GG RR**. L2B replaces input alpha with the alpha register. These layouts come from the existing hardware-reference analysis, not new console tests
- L2B packages are eight bytes, Y2R2 packages twelve. Byte enum fields have fixed sizes without enum bitfields. Y2R2 `SetConversionParams` sends three normal words rather than upstream Y2R's seven-word header
- Getters copy only initialized byte/halfword widths and record lengths; L2B's two excess advertised reply words and Y2R2's four excess words are ignored. Y2R2 package byte 9 is cleared on both input and output
- Y2R2 `GetConversionParams` returns preset coefficient index `0..3`, or `4` for a custom matrix. The setter cannot accept `4`; use `SetCoefficients` for custom matrices
- Formats, coefficient indices, rotation/alignment, package geometry, positive DMA units and nonempty buffers are checked in the client. These checks are stricter than the server's raw register-writing paths. Local validation failures return `L2B_ERROR_INVALID_ARGUMENT` or `Y2R2_ERROR_INVALID_ARGUMENT` (`-1`); named platform result constants remain separate
- Width is a positive multiple of eight through 1024. L2B imposes the same rule on line count. Getters return the raw zero encoding written for 1024. Y2R2 line count `1024` succeeds **without changing the register**; ordinary positive line counts `1..1023` write it
- DMA packets use four normal words plus one shared current-process handle. Units and gaps occupy separate sign-extended words with signed 16-bit semantics. The server handles source cache clean and destination invalidation; callers retain buffers through actual DMA completion
- Event getters return a newly shared handle owned by the caller. Close it with `svcCloseHandle`; no preexisting handle in the output variable is closed
- Conversion events and busy/DRQ queries do not prove destination DMA completion. Query `IsDoneReceiving` after starting a transfer; a missing DMA handle also reports done. The server's DMA-start failure path can loop without a timeout; these clients do not claim to fix that module defect
- The historic misspelling `SpacialDithering` remains available, with correctly spelled `SpatialDithering` aliases

Inherited Y2R advice about rotation and transfer-unit tuning is identified as such where applicable. Y2R2 edge behavior, silicon rounding and FIFO timing remain subject to the limits recorded in [hardware validation](../RE/hardware-validation.md).

## Validation

`make check` compiles the completed C clients with the real libctru headers for ARM11, checks wire sizes/offsets in C and GNU C++11 (including short-enum mode), then runs the C implementations against mocked kernel/service calls with AddressSanitizer and UndefinedBehaviorSanitizer. The mock uses libctru's actual IPC descriptor/header helpers.

Tests cover every STD opcode, translated process/mapped-buffer descriptors, signed byte and DMA arguments, output count bounds, event ownership, native-event output gating, transport failures, padding/overlong replies, auxiliary engine routing, and partial-init cleanup. Auxiliary tests focus on distinct transport and failure cases rather than exhaustively repeating every scalar setter. The archive is also compiled with optimization and warnings as errors.

These checks validate client encoding, layouts and local control flow. They do not demonstrate successful real video decoding, DMA timing, pixel accuracy or hardware capability beyond the prior static/reference findings.

## Source style and API documentation

The completed clients and C checks follow the local `.clang-format`: tabs for block indentation, spaces for alignment, and a four-column tab width. Format only the maintained files, leaving the `upstream/` provenance snapshots intact:

```sh
cd libctru-mvd
clang-format -i include/3ds/services/*.h source/services/*.c tests/*.c
```

Public function contracts live in the headers, with `@brief`, direction-qualified `@param` entries for every argument, and return semantics for non-void functions. Types, individual fields (including legacy aliases), enum values and macros have their own descriptions. Private IPC helpers are documented at their definitions. The documentation records units, address spaces, ownership and result-dependent output validity without changing the wire ABI.
