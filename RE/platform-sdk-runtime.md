# Platform, SDK and runtime analysis

This pass follows the codec and auxiliary-driver work. It resolves the remaining platform families, then the [heap and allocation family](heap-runtime.md). Addresses refer to this database and the single supplied console revision.

The names beginning `MvdSdk` and `MvdRuntime` are descriptive analysis names. They do not establish an exact Nintendo SDK release, ARM compiler release, or original C++ class name. The supplied Hantro tree does not provide these Nintendo kernel/service implementations. The local libctru `source/svc.s`, `include/3ds/svc.h`, `source/srv.c`, `source/errf.c` and associated headers provide kernel and IPC ABI comparisons; **libctru inclusion is not established**. Exact source attribution remains distinct from behavioral identification.

## Coverage and database corrections

The pass names all 170 entries in the previous unnamed-function queue and recovers four additional functions. The database now contains **800 functions and zero `sub_*` names**. This closes that queue, not every possible source-identity, padding-field or runtime-observation question.

Four instruction sequences had no function definitions:

| Address | End, exclusive | Evidence and meaning |
|---|---|---|
| `0x100B94` | `0x100BB0` | Direct Thumb `BL` from `0x100B06`; invokes the per-thread termination handler and panics on return |
| `0x1144C8` | `0x1144EA` | Self-relative initializer entry; initializes the fatal-error port and lock |
| `0x1147E8` | `0x1147EA` | Initializer target containing only `BX LR` |
| `0x1147EA` | `0x1147EC` | Separate initializer target containing only `BX LR` |

The former `dword_100B94` was executable code, **not an indirect function-pointer array**. Its instruction boundaries and branch destinations were checked before replacing the data classification. No instruction bytes were patched.

Two other inherited names needed correction:

* `GetThreadCommandBuffer` at `0x1103D0` became `MvdSdkGetTlsBase`: it returns the CP15 thread pointer without adding `0x80`.
* `RecursiveLock_Lock` at `0x11018A` became `MvdSdkConstructScopedRecursiveLockAlternate`: it stores a lock pointer in a guard object and calls the actual lock method at `0x10FED0`.

Several SVC-output registers, stack-save slots and incidental register values had been inferred as C arguments. Explicit ABIs and scalar stack types remove those false arguments from SDK callers. The function inventory records the applied declarations.

## Startup and initializer tables

`MvdRuntimeProcessEntry` (`0x100000`) executes:

1. Clear linked BSS through exclusive end `0x1219CC` (`0x100024`). The output-mapping table is only the first object in this range.
2. Initialize built-in locale pointers (`0x100070`).
3. Initialize SDK state (`0x1000C8`): address arbiter, cached TLS base, address-arena records, initial thread runtime state, process-info cache, and an SRV reference.
4. Initialize the 1 MiB process heap (`0x1000A8`).
5. Run the self-relative initializer table, then the absolute initializer table.
6. Enter `main` through `0x100048`, then call `svcExitProcess`.

The absolute table walker at `0x100054` has equal begin/end addresses, `0x1207E8`, so it invokes nothing in this image. The relative walker at `0x10128C` traverses thirteen signed 32-bit offsets. Each destination is `entryAddress + signedEntry`; the low bit selects Thumb.

| Entry | Target | Role |
|---|---|---|
| `0x1207B4` | `0x114500` | L2B/service contexts, handle array and notification callback objects |
| `0x1207B8` | `0x11475C` | Y2R static state |
| `0x1207BC` | `0x114818` | DWL handles and decoder/PP locks |
| `0x1207C0` | `0x1144C8` | Fatal-error service state |
| `0x1207C4` | `0x1147EA` | Empty initializer |
| `0x1207C8` | `0x1147EC` | Object with process-exit vtable |
| `0x1207CC` | `0x1147BC` | Event object |
| `0x1207D0` | `0x11473C` | Owned current-thread handle |
| `0x1207D4` | `0x1148E0` | Clear static word `0x1210AC` |
| `0x1207D8` | `0x1147E8` | Empty initializer |
| `0x1207DC` | `0x114890` | Global heap-registry lock |
| `0x1207E0` | `0x1148B8` | Resource-slot finalizer placeholder |
| `0x1207E4` | `0x114884` | Clear static word `0x1210C4` |

The static guard at `0x10B36C` is a plain test/store of a word, without an exclusive operation. Several initializer sequences load object, destructor and process-entry addresses and then execute **NOPs**, not calls to a finalizer-registration routine. In particular, `0x1148B8` performs no object write or destructor registration. References to the process entry from these sequences are address references, not recursive startup calls.

The two clear-only static words have no other recovered direct consumers. Their source class identities remain unknown. The exit-object and event-object labels describe their vtable targets and observed fields; they do not claim recovered SDK class names.

## TLS, locale and termination

Both `0x1103D0` and `0x10F824` read `MRC p15,0,R0,c13,c0,3`. The latter result is also used as the thread identity in recursive locks. `0x1004BC` caches the initial TLS base at `0x1210A0`.

| TLS offset | Recovered use |
|---|---|
| `0x00..0x3F` | Sixteen words cleared by `0x1008BC` |
| `0x5C` | Pointer to the 32-byte runtime state |
| `0x60..0x7F` | Initial thread's inline runtime state |
| `0x80` | IPC command-buffer start, added explicitly by callers |

`MvdSdkTlsPrefix` describes the first `0x80` bytes only; it does not assert the size of the entire TLS or command-buffer area. Clearing the prefix does **not** clear the IPC command buffer.

The runtime state has a termination entry at `+4`, a default termination callback at `+8`, a byte flag at `+0xC`, a one-shot termination handler at `+0x10`, and an optional emergency-buffer pointer at `+0x1C`. The remaining words retain neutral names. Initial-thread setup installs `MvdRuntimeTerminate` and `MvdRuntimeDefaultTerminate`. Lazy setup at `0x100C60` allocates 32 bytes if the TLS slot is empty, installs the alternate default callback, and panics if allocation fails.

`MvdRuntimeInvokeTerminateHandler` (`0x100B94`) loads the `+0x10` handler, clears it before calling it, otherwise calls `+8`, and panics if the selected callback returns. The optional 136-byte emergency-buffer branch is constant-disabled in both observed initialization paths. Its acquire/release helpers nevertheless exist: a request of at most 128 bytes gets `buffer+8` only when the busy byte is clear; release accepts exactly that pointer.

The locale getters accept null, an empty string or `"C"`, rejecting any other nonempty name. The character-classification block starts at `0x120694`; startup installs the pointer one byte past its start. The numeric locale block at `0x1207A4` contains relative data including the decimal-point string. `0x119A84` writes `0x03000000` to FPSCR. These observations do not identify an exact runtime source checkout.

## SVC veneer ABIs

These signatures follow the actual save/load sequences and match the local kernel ABI reference. `Result` remains in returned R0. Hex-Rays' inline `SVC` rendering does not itself model output-register writes; its apparent assignment of the input register value to an output pointer is not the syscall's meaning.

| Address | SVC | Recovered ABI |
|---|---|---|
| `0x110150` | `0x2D` | `svcConnectToPort(Handle *out, const char *name)`; returned R1 is stored through saved R0 |
| `0x119930` | `0x01` | `svcControlMemory(u32 *out, address0, address1, size, operation, permission)`; stack operation to R0, permission to R4, returned R1 to `*out` |
| `0x119A6C` | `0x21` | `svcCreateAddressArbiter(Handle *out)` |
| `0x119A90` | `0x2B` | `svcGetProcessInfo(s64 *out, Handle process, u32 type)`; returned R1:R2 stored as 64 bits |
| `0x119AB4` | `0x0A` | `svcSleepThread(s64 timeoutNs)` consumes both R0 and R1 |
| `0x119AC4` | `0x35` | `svcGetProcessId(u32 *out, Handle process)` |
| `0x119B78` | `0x22` | `svcArbitrateAddress(Handle, address, action, value, s64 timeoutNs)`; stack timeout to R4:R5, preserving caller R4:R5 |
| `0x119CC8` | `0x27` | `svcDuplicateHandle(Handle *out, Handle original)` |

`svcBreak` (`0x119ABC`, SVC `0x3C`) consumes the reason in R0; its caller clears R1/R2. Existing AcceptSession, ReplyAndReceive, SendSyncRequest, CloseHandle and process-cache veneers retain their kernel roles.

`0x100914` queries current-process information type `20`, applies the SDK result policy on failure and caches the low word at `0x12107C`. Its high-word use and the original SDK name of the cached property are not inferred from the syscall number alone.

## SRV lifetime and command construction

`MvdSdkAcquireSrvReference` (`0x110484`) initializes SRV globals once and serializes access with the reference lock. If the count is nonpositive, `0x1005D4` connects to `srv:` and registers the client. Otherwise it increments the count and returns a level-1 status with summary 1, module 25, description `0x3F9`.

ConnectToPort retries only when the decoded result has level `-5`, summary `4`, and description `0x3FA`, sleeping **500,000 ns** between attempts. The module field is not part of that predicate. After a successful connection, RegisterClient runs and the count increments **even if the registration reply fails**; the registration result is returned to the caller.

Final release at `0x1006C8` closes the cached handle. Close failure enters the default panic path. Successful close decrements the reference count and clears the handle. A nonfinal release decrements the count and returns level 1 / summary 1 / module 25 / description `0x3F0`. The low-level close routine changes the count only after successful kernel close.

| SRV command | Builder | Request | Successful-transport handling |
|---|---|---|---|
| `1` RegisterClient | `0x1009FC` | 0 normal words, 2 translation words; PID descriptor `0x20` | Return reply word 1 |
| `2` EnableNotification | `0x100A34` | No payload | Copy handle from word 3, return word 1 |
| `3` RegisterService | `0x100630` | Exactly 8 name bytes, name length, maximum sessions | Copy handle from word 3, return word 1 |
| `4` UnregisterService | `0x100684` | Exactly 8 name bytes and name length | Return word 1 |
| `0xB` ReceiveNotification | `0x100A70` | No payload | Copy notification ID from word 2, return word 1 |

The builders test the transport result first. Handle/ID outputs listed above are copied before any separate check of the server result. `EnableNotification` at `0x100520` adds such a check before publishing its temporary handle. The RegisterService session limit is a scalar, not the pointer inferred by its previous prototype.

## Notification objects and service loop

A `NotificationEntry` prefix is 16 bytes: vtable at `+0`, circular-list `prev` and `next` at `+4/+8`, ID at `+0xC`. Registration (`0x100578`) appends under the notification lock. Lookup (`0x1009B8`) returns the first matching entry; duplicate IDs are not rejected. Receive/dispatch (`0x10053C`) invokes the callback while still holding the recursive lock, and returns success when no callback matches.

The 28-byte `MvdSdkMemberCallback` embeds this prefix and adds object pointer `+0x10`, function word `+0x14`, and encoded adjustment `+0x18`. Invocation (`0x119860`) is:

```
adjustedThis = object + (signedEncodedAdjustment >> 1)
if encodedAdjustment & 1:
    target = *(functionPointer *)(*(u32 *)adjustedThis + functionWord)
else:
    target = functionWord
branch to target with R0 = adjustedThis
```

This is the observed member-function dispatch representation. It does not establish the original class name or compiler version. The termination notification uses a callback that ignores its object argument; the stack word copied into that object field is not initialized as a meaningful object by `main`.

`main` (`0x1001A8`) registers four service ports and multiplexes **nine handle slots**: notification semaphore at index 0, ports at 1–4, and at most four active sessions at 5–8. `g_mvdServiceDispatchTable` (`0x11A04C`) contains four dispatch function pointers followed by four **context pointers**, not eight function pointers. The contexts are the STD state, the two L2B contexts and the Y2R context. The mutable session table at `0x1218C4` stores `{serviceIndex, context}` pairs.

After dispatch, the selected session becomes the next reply target. Without a pending reply, the loop writes `0xFFFF0000` to the command header before waiting. Shutdown waits for the termination flag and a handle count of five, meaning all client sessions have drained. It then unregisters ports and finalizes the auxiliary interrupt state.

On kernel result `0xC920181A`, `MvdCloseClientSession` (`0x100808`) uses the returned index, or searches for the previous reply handle when the index is `-1`. It closes the session, performs service-specific L2B/Y2R cleanup, and fills its slot from the final active session. The helper assumes the kernel-reported index or reply handle identifies a current session; it has no independent not-found recovery path.

## Recursive locks and exclusive updates

`MvdSdkRecursiveLock` is twelve bytes:

| Offset | Field | Meaning |
|---|---|---|
| `0` | signed counter | Free value 1, held value -1, more-negative values account for queued contenders |
| `4` | owner TLS pointer | Current owner identity, or zero after final release |
| `8` | recursion depth | Number of acquisitions by the current owner |

`0x10FED0` performs a recursive blocking acquisition; `0x111C9E` is a **nonblocking try-lock**. DWL reservation uses the latter, so an unavailable hardware lock fails immediately rather than waiting in this helper. Scoped guards contain only a pointer to this lock.

The counter transitions are recoverable without assigning a source-library class:

| Operation | Predicate | Replacement |
|---|---|---|
| Fast acquire | `counter > 0` | `-counter` |
| Enqueue | `counter < 0` | `counter - 1` |
| Wake handoff | `counter > 0` | `1 - counter` |
| Release | unconditional | `-counter` |

Final recursive release clears the owner and atomically negates the counter. A resulting value above 1 signals one waiter with arbiter action 0/value 1. A contender enqueues, waits using action 1/value 0, and retries the handoff transition after wakeup. The wrappers pass zero timeout words and ignore arbitration results. No additional memory-barrier behavior is inferred beyond the inspected instructions.

Six ARM helpers at `0x119AE0`, `0x119B2C`, `0x119B90`, `0x119BDC`, `0x119C28`, and `0x119C74` implement LDREX/callback/STREX loops. A failed exclusive store retries; a rejected callback executes CLREX and returns zero. The previous extra C parameters were register-save artifacts. Compare/exchange uses a 12-byte `{expected, desired, observed}` context; its public wrapper returns the old value, not a boolean.

`MvdSdkAddressWaitState` is a separate eight-byte object. Initialization stores auxiliary word 1 and value -2 or -1 according to mode. Waiting on -2 makes **one** arbitration call and returns. State -1 and unrecognized states wait and recheck; state 0 is consumed atomically to -1; state 1 returns immediately. Kernel errors are ignored. This explains why the fatal-report wait must not be represented as an unconditional infinite C loop.

## Result policy and fatal-error reporting

The result builder (`0x100AA8`) packs signed level at bits 27–31, six summary bits at 21–26, module shifted by 10, and ten description bits. It does not separately mask the module argument. The level accessor sign-extends five bits.

`MvdSdkHandleUnexpectedResult` (`0x110220`) reads the configuration byte at `0x1FF80014` and the signed policy byte at `0x121080`. Its strict branch is equivalent to both low bits being clear:

* Strict branch: level -1 first reports a generic fatal error; then the default panic path is reached if reporting returns. Other levels go directly to that panic path.
* Other branch: levels -7 and +1 return; every other level is reported.

The original semantic names of those configuration/policy bits are not established here. The reporting PC comes from saved LR. `panicIfFailed` (`0x110468`) independently captures a PC with `MOV R1,PC` before reporting a negative result.

`MvdSdkFatalErrorInfo` is exactly 128 bytes, matching the local err:f wire-layout reference:

| Offset | Size | Field |
|---|---:|---|
| `0` | 1 | Type |
| `1` | 1 | Revision high |
| `2` | 2 | Revision low |
| `4` | 4 | Result |
| `8` | 4 | Caller PC |
| `0xC` | 4 | Process ID |
| `0x10` | 8 | Title ID |
| `0x18` | 8 | Application title ID |
| `0x20` | 96 | Type-specific payload |

The generic builder at `0x10FEAC` writes type, revision high 0, revision low `0xE56D`, result and caller PC into the global packet at `0x121338`. Reporting fills the process ID using the current-process pseudo-handle. The builder does not populate title IDs or the payload. The stored revision value is not sufficient to identify this firmware image's provenance.

`MvdSdkReportFatalError` (`0x100CD4`) serializes err:f handling with a recursive lock. On connection success it sends command 1 with 32 normal words, then closes the port; process-ID and report results are ignored. On connection result `0xD0401834`, **type 2 only** sleeps 5 ms while holding the lock, releases it and retries. Other connection failures leave the retry path. Types 2 and 5 may return. Other types wait on a private state initialized to -2; see the one-call/error caveat above. The libctru wire reference labels types 2 and 5 as card removal and log-only respectively.

The optional global abort callback (`0x121038`) is cleared before invocation. `0x100FD0` forwards R0–R3 and **two** stack words: six words total. `0x100E7C` supplies reason, four zero words, and incoming R1 as the final word. Some panic callers do not initialize that auxiliary register; its original API meaning is unresolved. Callback ABI forwarding is established, not the semantics of all six fields.

## Runtime memory and arithmetic helpers

The former `memcpy` label at `0x10FE24` and `__aeabi_memcpy` label at `0x10FABC` obscured their return behavior. Both advance R0 as bytes are copied and return **destination + size**. They are now `MvdRuntimeCopyBytesAdvance` and `MvdRuntimeCopyAlignedBytesAdvance`. The wrapper at `0x119D40` saves R0 before the call and restores it afterward, providing the original-destination return. This also repairs the undefined-looking local-copy return in `MvdCopyMemory` caused by the wrapper's former void prototype.

The fill wrapper at `0x119D4C` uses `(destination, value, size)` and preserves the original destination. `0x119D68` accepts `(destination, size, value)`, replicates the low byte, and enters the shared body at `0x10ADD8`. Zeroing entries at `0x10ADD4` and `0x10F518` share byte/bulk fill bodies. Shared-body jumps can remain visible in decompilation; they are not additional unresolved external calls.

`MvdUnsignedDivideWithRemainder` (`0x10F668`) takes dividend in R0 and divisor in R1, returning quotient in R0 and remainder in R1. Its arithmetic chunks are shared with the signed helper. Division by zero calls the return-only default handler (`0x1144B8`) with zero and restores the original dividend as remainder. The applied eight-byte return view describes both registers. **Hex-Rays still reports local-allocation failure for this register-pair/shared-chunk body**; its pseudocode is not reliable evidence for the carry algorithm. Alternative scalar/scattered return representations did not resolve that limitation, so the verified register-pair declaration was retained.

`0x119D28` returns the low 64 bits of a 64-bit product. `0x1199CC` implements unsigned-byte string comparison with an aligned zero-detection path; the aligned RRX path preserves the comparison sign despite an apparently reversed intermediate subtraction. `0x114496` iterates array constructors. The inherited `__ARM_common_switch8_thumb` helper uses R3 as index and the LR-relative byte table to branch to the selected target, rather than behaving like an ordinary four-argument C function. These roles do not prove exact runtime-source provenance.

## Handles and residual limits

Owned-handle destructors close and clear their handle word. Thread objects are eight bytes: handle, joined byte, auxiliary byte and padding. Construction duplicates current-thread pseudo-handle `0xFFFF8000`; join occurs once before destruction. The twelve-byte event object has a vtable, handle and state byte; its final unconditional close sees zero after a preceding conditional close.

The resource-slot helpers access a 16-bit bitmap only for indices below 16. They do not establish the original resource class. The zero process-handle constants at `0x11A014` and `0x11A1A0`, and current-process pseudo-handle `0xFFFF8001` at `0x11A19C`, now have separate four-byte definitions. Adjacent data is not absorbed into those definitions.

Remaining uncertainty is concentrated in exact SDK/runtime source identities, unused/static class and padding fields, the auxiliary abort-callback word, and the explicitly noted decompiler limitations. Hardware timing and codec interpretation limits remain in [hardware-validation.md](hardware-validation.md) and [codec-semantics.md](codec-semantics.md); naming every function does not resolve those independently.
