# Y2R2 and L2B drivers

This pass follows the exposed `y2r2:u`, `l2b:u` and `l2b2:u` IPC handlers into register programming, DMA and service lifetime. All addresses are IDA virtual addresses in this module. Names beginning `Mvd` describe observed behavior; these drivers have not been attributed to matching Hantro or libctru implementation source. The local libctru tree supplies a matching kernel DMA ABI and Y2R client vocabulary.

The pass adds **95 function names**, refines 74 existing prototypes and makes 99 local name/type edits. The function inventory now contains 598 entries, including explicit rows for the two previously named dispatchers; 170 database functions remain unnamed.

The existing [command descriptions](auxiliary-services.md) are supplemented by a [complete transport matrix](auxiliary-ipc.md). There are 23 shared L2B cases and 45 Y2R2 cases: 91 exposed service/command combinations across the three services.

## Engines and address spaces

| Service | State | CPU register bank | DMA input | DMA output | Interrupt ID |
|---|---|---|---|---|---|
| `l2b:u` | `g_l2bContexts[0]`, `0x121220` | `0x1EC30000` | `0x1EE30000` | `0x1EE30200` | `0x45` |
| `l2b2:u` | `g_l2bContexts[1]`, `0x121270` | `0x1EC31000` | `0x1EE31000` | `0x1EE31200` | `0x46` |
| `y2r2:u` | `g_y2rContext`, `0x121014` | `0x1EC32000` | Y/U/V/YUYV ports below | `0x1EE32200` | `0x4E` |

`MvdL2bRegistersInitialize` (`0x110284`) accepts engine 0 or 1 and stores `engine << 12`. `MvdY2rRegistersInitialize` (`0x10FFF4`) also accepts 0 or 1 but stores `engine * 0x30000` relative to `0x1EC02000`. This service always requests Y2R engine 1. Presence of the engine-0 branch does not expose another service here.

The two words at `g_l2bDmaBaseAddresses` (`0x11A008`) are `0x1EB00000` and `0x1EB01000`; the driver adds `0x330000` for input and `0x330200` for output. These FIFO virtual addresses differ from CPU register addresses. The bytes at `g_l2bInterruptIds` (`0x11A000`) are **interrupt IDs**, verified at the BindInterrupt/UnbindInterrupt calls (`0x110430`, `0x110364`). The earlier attribution queue incorrectly described them as DMA request IDs; DMA device IDs are different constants.

The older [3DBrew Y2R register map](https://www.3dbrew.org/wiki/Y2R_Registers) supplies comparison vocabulary for control, dimensions, coefficients and alpha. The bank selection and addresses above come from this binary, not an assumption that the wiki's physical addresses are module virtual addresses.

## Recovered contexts

`MvdL2bContext` occupies 80 bytes, matching the spacing of the two service objects and their constructors. Its register subobject is eight bytes: register offset at 0 and initialized byte at 4. A DMA channel is a four-byte handle followed by a 24-byte configuration.

| L2B offset | Type/member | Evidence |
|---|---|---|
| `0x00` | `u32 engine` | Initializer and DMA/IRQ selection |
| `0x04` | `MvdL2bRegisterContext registers` | All register helpers |
| `0x0C` | `MvdDmaChannel receive` | Destination DMA, config at `0x10` |
| `0x28` | `MvdDmaChannel send` | Source DMA, config at `0x2C` |
| `0x44` | `u8 interruptInitialized` | Interrupt initialization/finalization gate |
| `0x45` | `u8 sessionOpen` | Open/close-session gate |
| `0x46..47` | Padding | No semantic member assigned |
| `0x48` | `Handle transferEndEvent` | Bind/unbind and event getter |
| `0x4C` | `u8 conversionBlocked` | Rejects conversion start; suppresses DMA start |
| `0x4D` | `u8 sessionCount` | Main-loop acceptance and close; PingProcess |
| `0x4E` | `u8 terminating` | Termination callback; skips DMA setup |
| `0x4F` | Padding | No semantic member assigned |

`MvdY2rContext` is a 36-byte analysis view of the global service state, starting at the registered context pointer. Most Y2R handlers ignore that explicit pointer and access the singleton instead. Its register subobject has the reverse order from L2B: initialized byte at 0 and register offset at 4.

| Y2R context offset | Member |
|---|---|
| `0x00` | `unresolvedWord0`; not assigned a vtable/object identity |
| `0x04` | `interruptInitialized` |
| `0x05` | `conversionBlocked` |
| `0x06` | `sessionCount` |
| `0x07` | `terminating` |
| `0x08` | `receiveDma` |
| `0x0C` | `sendYDma`, also used for YUYV |
| `0x10` | `sendUDma` |
| `0x14` | `sendVDma` |
| `0x18` | `transferEndEvent` |
| `0x1C` | `MvdY2rRegisterContext registers` |

Four separate `MvdDmaConfig` objects at `g_y2rDmaConfigs` (`0x1212C0`) describe receive, Y/YUYV, U and V respectively. The context types describe observed storage and accesses, not recovered original C++ class names. `conversionBlocked` names the branch behavior; no normal setter of that flag to one was established in this pass.

## Lifetime and session behavior

At startup, `main` (`0x1001A8`) initializes both L2B interrupt objects and the Y2R2 singleton. L2B interrupt initialization (`0x1103D8`) initializes registers, creates a reset-type-0 event, and binds the selected interrupt at priority 8 with manual-clear argument zero. Y2R initialization (`0x100724`) follows the same pattern for IRQ `0x4E`. IRQ lifetime is longer than a client session.

On L2B session acceptance, `MvdL2bOpenSession` (`0x110398`) requires the interrupt object to exist and the session-open flag to be clear. It calls register initialization and sets session-open. The register initializer's return is ignored: the first open after process startup may encounter already-initialized register state without producing an IPC failure. The main loop increments the session-count byte. `CloseClientHandle` (`0x100808`) decrements it and calls `MvdL2bCloseSession` (`0x10FF54`), which stops/closes both DMA handles and finalizes register state while retaining the interrupt event.

Y2R session acceptance increments the count. Client command `2B` requires the interrupt object to exist, invokes register initialization for engine 1, and returns success without forwarding that initializer's result. Command `2C` stops/closes all four DMA handles and finalizes registers; its register-finalization result is also discarded. Session closure automatically invokes the same command-`2C` implementation. Neither command destroys the bound interrupt event; module shutdown does that through `MvdY2rFinalizeInterrupt` (`0x100790`). Most scalar register accessors do not test either initialization byte, so these lifecycle flags do not form a general per-command authorization or readiness gate.

`MvdY2rInitializeStaticState` (`0x11475C`) clears the singleton DMA/event handles and register-initialized byte. The termination callback `MvdBeginAuxiliaryServiceTermination` at `0x1197B8` sets the module termination flag, closes the L2B sessions, finalizes Y2R register/DMA state, then sets each driver's `terminating` byte. The main loop continues until its client sessions are gone before unregistering services and destroying interrupt resources. Commands already present in this teardown interval can therefore see the early-return paths described below.

## Register programming

Offsets below are relative to the selected CPU register bank. The binary performs read/modify/write operations directly; the database's zero-filled hardware segments are not live register evidence.

| Offset | Access in this code | Meaning / behavior |
|---|---|---|
| `0x00` | 32-bit control; some byte reads | Format, conversion and interrupt controls |
| `0x04` | 16-bit | Input width |
| `0x06` | 16-bit | Input line count |
| `0x08` | Write 1 | Reset operation; Y2R writes a halfword, L2B a word |
| `0x10..1E` | Eight halfwords, Y2R only | Conversion coefficients |
| `0x20` | Halfword read/write | Alpha; setter retains only low eight input bits |
| `0x100/108/110/118` | Halfword read/write, Y2R only | Four groups of dithering weights |

Control fields established by setters/getters:

| Bits | L2B | Y2R |
|---|---|---|
| `0..1` / `0..2` | Input format uses bits 0..1 | Input format uses bits 0..2 |
| `8..9` | Output format | Output format |
| `10..11` | No interpretation assigned | Rotation |
| `12` | No interpretation assigned | Block alignment |
| `16` | No interpretation assigned | Spatial dithering |
| `17` | No interpretation assigned | Temporal dithering |
| `22`, `23` | Input/output DMA enable; enabled during initialization, disabled during finalization | Same roles and lifecycle |
| `24..25` | Conditional write-back helpers | Part of Y2R's larger group |
| `26..28` | No interpretation assigned | Conditional write-back helpers |
| `29` | Cleared during initialization | Cleared during initialization |
| `30` | Transfer-end interrupt enable | Transfer-end interrupt enable |
| `31` | Start/stop and busy read | Start/stop and busy read |

The [hardware follow-up](hardware-followup.md) corroborates bits 22/23 as input/output DMA enables, L2B bits 24/25 as input/output DRQ, and Y2R bits 27/28 as YUYV input/output DRQ. Their helpers now have semantic names. Bit 29 and Y2R bits 24..26 retain numeric names because the hardware reference is tentative. Write-back helpers read the entire control word and, when their bit is set, write the word ORed with that same bit. Other set status bits are also included in that write. Exact acknowledgement/clear effects remain unconfirmed.

Format writes clear the documented field mask and OR the supplied value, without a separate enum-range check. L2B output and Y2R output use an input byte shifted by 8. Y2R rotation/alignment wrappers shift by 10/12 and truncate to 16 bits; malformed enum values can set neighboring control bits. Valid Y2R enum vocabulary matches local libctru (`include/3ds/services/y2r.h`): input 0..4, output 0..3, rotation 0..3 and alignment 0..1. Published hardware findings now corroborate L2B input/output IDs 0=RGBA8888, 1=RGB888, 2=RGBA5551, 3=RGB565. RGBA8888 memory order is `AA BB GG RR`; L2B replaces incoming alpha. See the [evidence chain and limits](hardware-followup.md). A one-byte `MvdRgbFormat` now types both L2B package format fields and the Y2R output field; the corresponding seven getter/setter prototypes preserve byte-sized values and outputs. Raw register writers retain their integer/shifted-bit interfaces.

Both drivers require width to be positive, at most 1024, and divisible by eight. L2B applies the same rule to line count. The value 1024 is stored as zero; getters return raw halfwords and do not expand zero back to 1024. Y2R line count is different: zero and signed values above 1024 fail, exactly 1024 succeeds **without writing**, and negative values reach `lines & 0x3FF`. Both scalar dispatchers use `LDRSH` for width/line arguments; the package readers also sign-extend those fields. Types now preserve `s16` width/line inputs and packed fields. The alpha path keeps its low byte regardless of the sign-extended transport load.

StartConversion checks `conversionBlocked`, reads busy, conditionally performs the status write-back sequence when idle, and sets bit 31. It does not wait for completion. StopConversion clears bit 31; DMA handles are stopped/closed through the separate lifecycle/setup routines. The L2B start helper's apparent address-symbol arithmetic is a constant construction: `0x1EC30000 << 15`, truncated to 32 bits, is `0x80000000`.

### Coefficients and dithering

Two identical 64-byte preset arrays now have types: `g_y2rStandardCoefficientPresets` (`0x11A1A4`) and `g_y2rStandardCoefficientMatchTable` (`0x11A08C`). Each contains four 16-byte `MvdY2rCoefficients` records. Raw halfword values are:

| Index | Eight coefficient words |
|---|---|
| 0 | 256, 358, 182, 88, 453, 59793, 4334, 58277 |
| 1 | 256, 403, 119, 47, 475, 59085, 2684, 57935 |
| 2 | 298, 408, 208, 100, 516, 58402, 4338, 56677 |
| 3 | 298, 458, 136, 54, 540, 57596, 2460, 56287 |

The first five words are masked to ten bits when written; the last three are stored intact. Their signed-offset interpretation and preset vocabulary are described in libctru's header, but the numbers above are binary observations. The two source-like stack copies are now typed as four records, replacing misleading 80-byte integer arrays. GetConversionParams compares every word and returns index 4 if no preset matches. GetStandardCoefficient rejects index 4 or above without filling its output.

The dither writer takes a **32-byte record by value**, split across R1–R3 and the stack; the coefficient writer takes a **16-byte record by value**. Correcting these prototypes removes false duplicated arguments. For each dither group `i`, the stored halfword is the low 16 bits of `((w[4*i]&3)<<2) | ((w[4*i+1]&3)<<6) | ((w[4*i+2]&3)<<10) | (w[4*i+3]<<14)`. The reader extracts two bits at shifts 2, 6, 10 and 14. Initialization supplies groups `[1,2,3,0]`, `[3,0,1,2]`, `[0,3,2,1]`, `[2,1,0,3]`. It also clears coefficients and alpha and disables spatial/temporal dithering.

## DMA ABI and flow

The kernel configuration agrees with local libctru's `DmaDeviceConfig` and `DmaConfig` declarations in `include/3ds/svc.h`. Applying that layout does not attribute Nintendo's wrapper implementation to libctru.

| Configuration offset | Field |
|---|---|
| `0x00` | s8 channel ID, set to -1 |
| `0x01` | s8 endian-swap size, set to 0 |
| `0x02` | Flags: sending 6, receiving 5 |
| `0x03` | Padding |
| `0x04` | Ten-byte source-device configuration |
| `0x0E` | Ten-byte destination-device configuration |

Each device record contains signed-byte device ID/alignment mask followed by signed-halfword burst size, transfer size, burst stride and transfer stride. Sending uses destination-is-device plus wait-available; receiving uses source-is-device plus wait-available. The selected device record receives alignment mask 4, burst size `B`, transfer size `unit`, burst stride `B`, and transfer stride `unit+gap` truncated to 16 bits. The unselected device record is not populated by each setup call.

`B` begins at 64 and is repeatedly halved until the **signed division remainder** of `unit / B` is zero. A zero unit therefore selects 64; this layer does not reject zero/negative units or gaps. Actual kernel acceptance and resulting transfers were not tested.

| Transfer | Device ID | Peripheral VA | Handle/config reuse |
|---|---:|---|---|
| L2B0 send | 23 | `0x1EE30000` | Context send |
| L2B0 receive | 24 | `0x1EE30200` | Context receive |
| L2B1 send | 25 | `0x1EE31000` | Context send |
| L2B1 receive | 26 | `0x1EE31200` | Context receive |
| Y2R2 Y | 18 | `0x1EE32000` | Y handle, config 1 |
| Y2R2 U | 19 | `0x1EE32080` | U handle, config 2 |
| Y2R2 V | 20 | `0x1EE32100` | V handle, config 3 |
| Y2R2 YUYV | 21 | `0x1EE32180` | Reuses Y handle/config 1; stops and closes Y/U/V handles first |
| Y2R2 RGB receive | 22 | `0x1EE32200` | Receive handle, config 0 |

Sending first calls **StoreProcessDataCache**, SVC `0x53`, on the source process/range. Receiving calls InvalidateProcessDataCache, SVC `0x52`, on the destination. The earlier description of sending as a cache *flush* was inaccurate. Cache-operation results are discarded in these paths. The peripheral side uses the current-process pseudo-handle `0xFFFF8001`. Its literal copies at `0x11A004` and `0x11A088` are now typed/named for L2B and Y2R respectively.

The common helpers are now `MvdDmaStop`, `MvdDmaStopAndClose`, `MvdDmaTryStart`, `MvdDmaGetState` and `MvdDmaIsDone`. Setup stops an existing transfer; when conversion is not blocked it also closes the old handle, starts the replacement, then polls state. Sending waits for a state other than 0; receiving waits for a state at least 2. The libctru state vocabulary is starting=0, waiting-for-destination=1, waiting-for-source=2, running=3, done=4. These loops wait for setup progress, not completion, and contain no software timeout.

`MvdDmaTryStart` returns false for result description `0x3F0`; other negative results panic. Driver callers ignore the Boolean and proceed to poll. On that false-return path, the destination handle remains zero and `MvdDmaGetState` returns zero, so the static control flow loops indefinitely. This is a conditional failure path, not a claim that a normal workload causes it. `IsDone*` instead performs a zero-time wait on the DMA handle; a zero handle counts as done, and description `0x3FE` is treated as timeout after SDK result handling.

Process-handle ownership differs during termination. L2B sending skips both setup **and handle close** when its terminating byte is already set. L2B receiving still returns success through a wrapper that closes the process handle. Y2R Y/U/V senders always close the received handle, including their termination early-out; YUYV and RGB wrappers close it on successful helper return, which includes the termination early-out. Thus the L2B sending path can retain the received process handle during teardown. No live resource-leak experiment was performed.

## Results and IPC limits

These services return platform result words directly, without Hantro status conversion.

| Condition | L2B result | Y2R result |
|---|---|---|
| Already initialized/open | `0xD8216FF9` | `0xD82053F9` |
| Not initialized/open | `0xD8216FF8` | `0xD82053F8` |
| Invalid engine | `0xE0E16C02` | `0xE0E05002` |
| Invalid dimensions | `0xE0E16FFD` | `0xE0E053FD` |
| Invalid coefficient index | — | `0xE0E053ED` |
| Conversion blocked | `0xC9416C01` | `0xC9405001` |

Some initialization/finalization results above are discarded by higher-level wrappers; their presence in a low-level routine does not imply every command returns them. Package setters apply fields in order and stop at the first reported failure, without rollback. Scalar commands generally do not require a full matching request header. DMA commands do. Reply padding, advertised-but-unwritten words, and invalid coefficient-getter output are detailed in [auxiliary-ipc.md](auxiliary-ipc.md).

## Analysis corrections and validation

The old `sub_10B97C` was eight bytes `53 00 00 EF 1E FF 2F E1`, decoded in Thumb mode as unrelated instructions and a jump into VP8 code. ARM decoding yields exactly `SVC 0x53; BX LR`. The T-bit analysis setting and instruction definitions were corrected, and the routine is now `svcStoreProcessDataCache`; no bytes changed.

L2B receive case `0x0A` starts at `0x111FAE` with bytes `02 23`, a Thumb `MOVS R3,#2` that was marked as data. Restoring code alone did not remove the decompiler jump-out; restoring the switch reference and reanalyzing the case made the full header/descriptor checks and receive call visible. The typed context and seven parent-relative locals expose state flags and DMA fields instead of misleading offsets into unrelated members.

The signed division helper returns quotient in R0 and remainder in R1. A register-pair result type makes driver callers display `.remainder`. Its own decompilation, and the tiny Y2R rotation/alignment wrappers, still report local-allocation warnings; the relevant behavior was established from instructions rather than treating those bodies as clean C.

Names, function types, structure sizes, data extents and representative regenerated callers were checked after reopening. Context sizes are L2B 80 and Y2R 36; DMA device/config/channel sizes are 10/24/28; public package sizes remain 8/12. Both coefficient arrays are 64 bytes and equal in full. No firmware bytes, reference-source files or live MMIO were modified. The later [hardware follow-up](hardware-followup.md) resolves reference pixel packing and several DMA/DRQ roles; tentative status/IRQ/strobe semantics and live DMA timing remain unresolved.
