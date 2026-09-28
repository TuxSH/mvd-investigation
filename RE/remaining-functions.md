# Remaining scope and function-queue closure

The [platform/SDK/runtime pass](platform-sdk-runtime.md), with [heap analysis last](heap-runtime.md), closes the previous **170-entry unnamed-function queue**. The database now contains **800 functions and zero `sub_*` names**. Four functions were recovered from direct-call or relative-initializer targets that IDA had left as data or undefined code. All current applied names and prototypes are in [function-map.md](function-map.md).

## Closed queue

| Previous family | Entries resolved |
|---|---:|
| Startup / static state | 30 |
| Heap / allocation | 31 |
| SDK service / TLS / errors | 37 |
| Synchronization / atomics | 32 |
| Runtime / compiler support | 14 |
| IPC representation | 2 |
| SVC veneer | 8 |
| DMA / event / handle wrappers | 5 |
| SDK/service object helpers | 11 |
| **Total previously unnamed** | **170** |

The recovered functions at `0x100B94`, `0x1144C8`, `0x1147E8` and `0x1147EA` are additional to those 170. Existing names were also corrected where they obscured the actual ABI, including the TLS-base helper, scoped-lock constructor, advancing-copy cores and service session helper. Descriptive names do not imply exact upstream symbols.

## What remains uncertain

* **Source identity:** exact Nintendo SDK/runtime/compiler versions and original symbols for semantic matches have not been established. This is separate from the demonstrated Hantro inclusion.
* **Unused or lightly used fields:** source class roles of clear-only static words, padding/unused SDK state, and the abort callback's auxiliary word remain unknown. The callback's six-word forwarding ABI is established.
* **Decompiler presentation:** the unsigned division helper's register-pair return and shared arithmetic chunks still trigger a Hex-Rays local-allocation warning. Its ABI and division-by-zero behavior are instruction-verified. Some memory-fill entries retain jumps into shared bodies, and SVC pseudocode does not model output registers by itself.
* **Codec definitions:** four unassigned register ordinals (8, 128, 282, 598) and the original meanings of the VP6/VP8 constant-zero info bytes remain blocked as detailed in [the definition follow-up](codec-definition-followup.md). Capability `+0x18` and abort-control ordinal 10 are now identified. A zero-only or unused slot is not a recovered original API definition.
* **Hardware evidence:** outstanding numerical-edge, IRQ/strobe, dimension-zero and DMA timing behavior needs the observations listed in [hardware-validation.md](hardware-validation.md). Historical workaround causes need an appropriate erratum or evidence, not another console revision assumption.

Zero auto-names is not proof that every instruction, indirect target or source structure has been completely attributed. The analysis preserves that distinction; no new broad family of unreviewed unnamed functions remains in the existing database.
