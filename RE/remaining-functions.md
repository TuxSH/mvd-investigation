# Remaining function inventory

The current database contains **170 `sub_*` entries** among 796 functions. The previous complete triage covered 265 entries; the [auxiliary-driver pass](auxiliary-drivers.md) names 95 of them. The table below is the remaining queue, not a claim of exact source provenance or accepted prototypes. SDK labels describe observed behavior and reference context.

The graph included pointer references and branches as well as calls. A reference is therefore not automatically a direct call, and an entry with no incoming reference is not proved unreachable. Existing function boundaries and successful decompilation do not prove complete code coverage.

| Family | Entries |
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

| Address / current name | Family | Evidence / next review |
|---|---|---|
| `0x100000` / `sub_100000` | Startup / static state | Process-entry initialization and exit sequence |
| `0x100024` / `sub_100024` | Startup / static state | Zeroes the BSS/output-mapping region |
| `0x100048` / `sub_100048` | Startup / static state | Veneer to main |
| `0x100054` / `sub_100054` | Startup / static state | Walks function-pointer initializer table |
| `0x100070` / `sub_100070` | Startup / static state | Startup/static-object initialization or teardown chain |
| `0x1000A8` / `sub_1000A8` | Startup / static state | Startup/static-object initialization or teardown chain |
| `0x1000C8` / `sub_1000C8` | Startup / static state | Startup/static-object initialization or teardown chain |
| `0x1000D4` / `sub_1000D4` | Startup / static state | Startup/static-object initialization or teardown chain |
| `0x1000F0` / `sub_1000F0` | Startup / static state | Startup/static-object initialization or teardown chain |
| `0x10013C` / `sub_10013C` | Heap / allocation | Heap headers, intrusive lists, allocation/free or containing-heap lookup |
| `0x100454` / `sub_100454` | SDK service / TLS / errors | Service connection/IPC, TLS scratch storage, result packing or failure handling |
| `0x100478` / `sub_100478` | SDK service / TLS / errors | Service connection/IPC, TLS scratch storage, result packing or failure handling |
| `0x100480` / `sub_100480` | Startup / static state | Startup/static-object initialization or teardown chain |
| `0x100498` / `sub_100498` | Startup / static state | Startup/static-object initialization or teardown chain |
| `0x1004AC` / `sub_1004AC` | Startup / static state | Startup/static-object initialization or teardown chain |
| `0x1004BC` / `sub_1004BC` | Startup / static state | Startup/static-object initialization or teardown chain |
| `0x1004CC` / `sub_1004CC` | Heap / allocation | Heap headers, intrusive lists, allocation/free or containing-heap lookup |
| `0x1004E8` / `sub_1004E8` | SDK service / TLS / errors | Service connection/IPC, TLS scratch storage, result packing or failure handling |
| `0x1005D4` / `sub_1005D4` | SDK service / TLS / errors | Service connection/IPC, TLS scratch storage, result packing or failure handling |
| `0x1006C8` / `sub_1006C8` | SDK service / TLS / errors | Service connection/IPC, TLS scratch storage, result packing or failure handling |
| `0x1008BC` / `sub_1008BC` | SDK service / TLS / errors | Service connection/IPC, TLS scratch storage, result packing or failure handling |
| `0x1008D4` / `sub_1008D4` | SDK service / TLS / errors | Service connection/IPC, TLS scratch storage, result packing or failure handling |
| `0x100914` / `sub_100914` | SDK service / TLS / errors | Service connection/IPC, TLS scratch storage, result packing or failure handling |
| `0x10093C` / `sub_10093C` | Heap / allocation | Heap headers, intrusive lists, allocation/free or containing-heap lookup |
| `0x100994` / `sub_100994` | SDK service / TLS / errors | Service connection/IPC, TLS scratch storage, result packing or failure handling |
| `0x1009B8` / `sub_1009B8` | SDK service / TLS / errors | Service connection/IPC, TLS scratch storage, result packing or failure handling |
| `0x1009FC` / `sub_1009FC` | SDK service / TLS / errors | Service connection/IPC, TLS scratch storage, result packing or failure handling |
| `0x100A70` / `sub_100A70` | SDK service / TLS / errors | Service connection/IPC, TLS scratch storage, result packing or failure handling |
| `0x100AA8` / `sub_100AA8` | SDK service / TLS / errors | Service connection/IPC, TLS scratch storage, result packing or failure handling |
| `0x100AC2` / `sub_100AC2` | Heap / allocation | Heap headers, intrusive lists, allocation/free or containing-heap lookup |
| `0x100ADE` / `sub_100ADE` | Heap / allocation | Heap headers, intrusive lists, allocation/free or containing-heap lookup |
| `0x100B02` / `sub_100B02` | SDK service / TLS / errors | Service connection/IPC, TLS scratch storage, result packing or failure handling |
| `0x100B06` / `sub_100B06` | Startup / static state | Indirect call through pointer at 0x100B94; target identity unresolved |
| `0x100B10` / `sub_100B10` | Heap / allocation | Heap headers, intrusive lists, allocation/free or containing-heap lookup |
| `0x100BB0` / `sub_100BB0` | SDK service / TLS / errors | Service connection/IPC, TLS scratch storage, result packing or failure handling |
| `0x100BB8` / `sub_100BB8` | Heap / allocation | Heap headers, intrusive lists, allocation/free or containing-heap lookup |
| `0x100BE4` / `sub_100BE4` | Heap / allocation | Heap headers, intrusive lists, allocation/free or containing-heap lookup |
| `0x100C00` / `sub_100C00` | Heap / allocation | Heap headers, intrusive lists, allocation/free or containing-heap lookup |
| `0x100C0C` / `sub_100C0C` | SDK service / TLS / errors | Service connection/IPC, TLS scratch storage, result packing or failure handling |
| `0x100C18` / `sub_100C18` | SDK service / TLS / errors | Service connection/IPC, TLS scratch storage, result packing or failure handling |
| `0x100C2A` / `sub_100C2A` | SDK service / TLS / errors | Service connection/IPC, TLS scratch storage, result packing or failure handling |
| `0x100C44` / `sub_100C44` | SDK service / TLS / errors | Service connection/IPC, TLS scratch storage, result packing or failure handling |
| `0x100C5C` / `sub_100C5C` | SDK service / TLS / errors | Service connection/IPC, TLS scratch storage, result packing or failure handling |
| `0x100C60` / `sub_100C60` | SDK service / TLS / errors | Service connection/IPC, TLS scratch storage, result packing or failure handling |
| `0x100CB0` / `sub_100CB0` | Synchronization / atomics | Lock/arbitration state, thread identity, or exclusive-update callback |
| `0x100CC0` / `sub_100CC0` | Synchronization / atomics | Lock/arbitration state, thread identity, or exclusive-update callback |
| `0x100CD4` / `sub_100CD4` | SDK service / TLS / errors | Service connection/IPC, TLS scratch storage, result packing or failure handling |
| `0x100D60` / `sub_100D60` | Heap / allocation | Heap headers, intrusive lists, allocation/free or containing-heap lookup |
| `0x100D76` / `sub_100D76` | Heap / allocation | Heap headers, intrusive lists, allocation/free or containing-heap lookup |
| `0x100DAC` / `sub_100DAC` | Heap / allocation | Heap headers, intrusive lists, allocation/free or containing-heap lookup |
| `0x100DE2` / `sub_100DE2` | Synchronization / atomics | Lock/arbitration state, thread identity, or exclusive-update callback |
| `0x100DEA` / `sub_100DEA` | Synchronization / atomics | Lock/arbitration state, thread identity, or exclusive-update callback |
| `0x100E0C` / `sub_100E0C` | Synchronization / atomics | Lock/arbitration state, thread identity, or exclusive-update callback |
| `0x100E4A` / `sub_100E4A` | Synchronization / atomics | Lock/arbitration state, thread identity, or exclusive-update callback |
| `0x100E5E` / `sub_100E5E` | Synchronization / atomics | Lock/arbitration state, thread identity, or exclusive-update callback |
| `0x100E7C` / `sub_100E7C` | SDK service / TLS / errors | Service connection/IPC, TLS scratch storage, result packing or failure handling |
| `0x100E90` / `sub_100E90` | SDK service / TLS / errors | Service connection/IPC, TLS scratch storage, result packing or failure handling |
| `0x100EB0` / `sub_100EB0` | SDK service / TLS / errors | Service connection/IPC, TLS scratch storage, result packing or failure handling |
| `0x100ED8` / `sub_100ED8` | SDK service / TLS / errors | Service connection/IPC, TLS scratch storage, result packing or failure handling |
| `0x100F12` / `sub_100F12` | Heap / allocation | Heap headers, intrusive lists, allocation/free or containing-heap lookup |
| `0x100F4C` / `sub_100F4C` | Heap / allocation | Heap headers, intrusive lists, allocation/free or containing-heap lookup |
| `0x100F5C` / `sub_100F5C` | Heap / allocation | Heap headers, intrusive lists, allocation/free or containing-heap lookup |
| `0x100F6C` / `sub_100F6C` | Synchronization / atomics | Lock/arbitration state, thread identity, or exclusive-update callback |
| `0x100F82` / `sub_100F82` | SDK service / TLS / errors | Service connection/IPC, TLS scratch storage, result packing or failure handling |
| `0x100F8C` / `sub_100F8C` | Synchronization / atomics | Lock/arbitration state, thread identity, or exclusive-update callback |
| `0x100FC4` / `sub_100FC4` | Synchronization / atomics | Lock/arbitration state, thread identity, or exclusive-update callback |
| `0x100FD0` / `sub_100FD0` | SDK service / TLS / errors | Service connection/IPC, TLS scratch storage, result packing or failure handling |
| `0x100FF4` / `sub_100FF4` | Heap / allocation | Heap headers, intrusive lists, allocation/free or containing-heap lookup |
| `0x1010B6` / `sub_1010B6` | Heap / allocation | Heap headers, intrusive lists, allocation/free or containing-heap lookup |
| `0x1010C0` / `sub_1010C0` | Heap / allocation | Heap headers, intrusive lists, allocation/free or containing-heap lookup |
| `0x1010D2` / `sub_1010D2` | Heap / allocation | Heap headers, intrusive lists, allocation/free or containing-heap lookup |
| `0x1010DE` / `sub_1010DE` | Synchronization / atomics | Lock/arbitration state, thread identity, or exclusive-update callback |
| `0x1010F0` / `sub_1010F0` | Synchronization / atomics | Lock/arbitration state, thread identity, or exclusive-update callback |
| `0x1010FE` / `sub_1010FE` | Synchronization / atomics | Lock/arbitration state, thread identity, or exclusive-update callback |
| `0x10110A` / `sub_10110A` | Heap / allocation | Heap headers, intrusive lists, allocation/free or containing-heap lookup |
| `0x101124` / `sub_101124` | Heap / allocation | Heap headers, intrusive lists, allocation/free or containing-heap lookup |
| `0x101208` / `sub_101208` | Synchronization / atomics | Lock/arbitration state, thread identity, or exclusive-update callback |
| `0x10121A` / `sub_10121A` | Synchronization / atomics | Lock/arbitration state, thread identity, or exclusive-update callback |
| `0x10122C` / `sub_10122C` | Synchronization / atomics | Lock/arbitration state, thread identity, or exclusive-update callback |
| `0x101240` / `sub_101240` | Heap / allocation | Heap headers, intrusive lists, allocation/free or containing-heap lookup |
| `0x10125C` / `sub_10125C` | Heap / allocation | Heap headers, intrusive lists, allocation/free or containing-heap lookup |
| `0x101274` / `sub_101274` | Synchronization / atomics | Lock/arbitration state, thread identity, or exclusive-update callback |
| `0x10128C` / `sub_10128C` | Startup / static state | Walks another initializer table |
| `0x10189C` / `sub_10189C` | Runtime / compiler support | Veneer to allocation-and-clear helper 0x1148EC |
| `0x10ADD4` / `sub_10ADD4` | Runtime / compiler support | Byte/aligned clearing path; shared fill entry at 0x10ADD8 |
| `0x10B348` / `sub_10B348` | Startup / static state | Startup/static-object initialization or teardown chain |
| `0x10B36C` / `sub_10B36C` | Startup / static state | Startup/static-object initialization or teardown chain |
| `0x10B838` / `sub_10B838` | IPC representation | IPC header/descriptor bits or word-indexed buffer access |
| `0x10F518` / `sub_10F518` | Runtime / compiler support | Aligned memory-clearing path |
| `0x10F668` / `sub_10F668` | Runtime / compiler support | Unsigned division-family entry; quotient/remainder ABI needs review |
| `0x10F790` / `sub_10F790` | Heap / allocation | Heap headers, intrusive lists, allocation/free or containing-heap lookup |
| `0x10F79A` / `sub_10F79A` | Heap / allocation | Heap headers, intrusive lists, allocation/free or containing-heap lookup |
| `0x10F7BC` / `sub_10F7BC` | Heap / allocation | Heap headers, intrusive lists, allocation/free or containing-heap lookup |
| `0x10F7D8` / `sub_10F7D8` | Synchronization / atomics | Lock/arbitration state, thread identity, or exclusive-update callback |
| `0x10F7F8` / `sub_10F7F8` | Synchronization / atomics | Lock/arbitration state, thread identity, or exclusive-update callback |
| `0x10F804` / `sub_10F804` | Synchronization / atomics | Lock/arbitration state, thread identity, or exclusive-update callback |
| `0x10F812` / `sub_10F812` | Synchronization / atomics | Lock/arbitration state, thread identity, or exclusive-update callback |
| `0x10F824` / `sub_10F824` | Synchronization / atomics | Reads CP15 thread-pointer register |
| `0x10F848` / `sub_10F848` | SDK service / TLS / errors | Service connection/IPC, TLS scratch storage, result packing or failure handling |
| `0x10F884` / `sub_10F884` | SDK service / TLS / errors | Service connection/IPC, TLS scratch storage, result packing or failure handling |
| `0x10F88C` / `sub_10F88C` | SDK service / TLS / errors | Service connection/IPC, TLS scratch storage, result packing or failure handling |
| `0x10FE14` / `sub_10FE14` | Heap / allocation | Heap headers, intrusive lists, allocation/free or containing-heap lookup |
| `0x10FEAC` / `sub_10FEAC` | SDK service / TLS / errors | Service connection/IPC, TLS scratch storage, result packing or failure handling |
| `0x10FED0` / `sub_10FED0` | Synchronization / atomics | Lock/arbitration state, thread identity, or exclusive-update callback |
| `0x110124` / `sub_110124` | IPC representation | IPC header/descriptor bits or word-indexed buffer access |
| `0x110150` / `sub_110150` | SVC veneer | SVC 0x2D; exact register ABI deferred |
| `0x110168` / `sub_110168` | Synchronization / atomics | Lock/arbitration state, thread identity, or exclusive-update callback |
| `0x110176` / `sub_110176` | SDK service / TLS / errors | Service connection/IPC, TLS scratch storage, result packing or failure handling |
| `0x110220` / `sub_110220` | SDK service / TLS / errors | Service connection/IPC, TLS scratch storage, result packing or failure handling |
| `0x1103C8` / `sub_1103C8` | SDK service / TLS / errors | Returns service-not-initialized result on SRV registration paths; not a L2B hardware helper |
| `0x110474` / `sub_110474` | Synchronization / atomics | Lock/arbitration state, thread identity, or exclusive-update callback |
| `0x110484` / `sub_110484` | SDK service / TLS / errors | Service connection/IPC, TLS scratch storage, result packing or failure handling |
| `0x1104CC` / `sub_1104CC` | SDK service / TLS / errors | Service connection/IPC, TLS scratch storage, result packing or failure handling |
| `0x111C58` / `sub_111C58` | DMA / event / handle wrappers | DMA/cache-operation veneer, event wait or handle lifetime |
| `0x111C62` / `sub_111C62` | DMA / event / handle wrappers | DMA/cache-operation veneer, event wait or handle lifetime |
| `0x111C6C` / `sub_111C6C` | DMA / event / handle wrappers | DMA/cache-operation veneer, event wait or handle lifetime |
| `0x111C88` / `sub_111C88` | DMA / event / handle wrappers | DMA/cache-operation veneer, event wait or handle lifetime |
| `0x111C9E` / `sub_111C9E` | Synchronization / atomics | Lock/arbitration state, thread identity, or exclusive-update callback |
| `0x111CC8` / `sub_111CC8` | SDK/service object helpers | Clears resource-bitset entry |
| `0x111CE8` / `sub_111CE8` | SDK/service object helpers | Resource bitset, owned-handle destruction, or small service-state reset |
| `0x111D00` / `sub_111D00` | SDK/service object helpers | Resource bitset, owned-handle destruction, or small service-state reset |
| `0x111D28` / `sub_111D28` | SDK/service object helpers | Tests index below 16 in resource bitset |
| `0x111D40` / `sub_111D40` | SDK/service object helpers | Resource bitset, owned-handle destruction, or small service-state reset |
| `0x111D4C` / `sub_111D4C` | SDK/service object helpers | Resource bitset, owned-handle destruction, or small service-state reset |
| `0x111D64` / `sub_111D64` | SDK/service object helpers | Resource bitset, owned-handle destruction, or small service-state reset |
| `0x111D90` / `sub_111D90` | SDK/service object helpers | Resource bitset, owned-handle destruction, or small service-state reset |
| `0x111DAA` / `sub_111DAA` | SDK/service object helpers | Resource bitset, owned-handle destruction, or small service-state reset |
| `0x111DB6` / `sub_111DB6` | Heap / allocation | Heap headers, intrusive lists, allocation/free or containing-heap lookup |
| `0x111DC8` / `sub_111DC8` | Heap / allocation | Heap headers, intrusive lists, allocation/free or containing-heap lookup |
| `0x111DEC` / `sub_111DEC` | Heap / allocation | Heap headers, intrusive lists, allocation/free or containing-heap lookup |
| `0x11425C` / `sub_11425C` | SDK/service object helpers | Single-word zero initializer; class identity unresolved |
| `0x1142BC` / `sub_1142BC` | DMA / event / handle wrappers | DMA/cache-operation veneer, event wait or handle lifetime |
| `0x1142E4` / `sub_1142E4` | SDK/service object helpers | Clears service shutdown flag byte at 0x12103C |
| `0x11448C` / `sub_11448C` | Runtime / compiler support | Process-exit veneer |
| `0x114496` / `sub_114496` | Runtime / compiler support | Array-element constructor iteration |
| `0x1144B8` / `sub_1144B8` | Runtime / compiler support | Division-zero target; empty-looking decompilation is not a verified ABI |
| `0x114500` / `sub_114500` | Startup / static state | Startup/static-object initialization or teardown chain |
| `0x11473C` / `sub_11473C` | Startup / static state | Startup/static-object initialization or teardown chain |
| `0x1147BC` / `sub_1147BC` | Startup / static state | Startup/static-object initialization or teardown chain |
| `0x1147EC` / `sub_1147EC` | Startup / static state | Startup/static-object initialization or teardown chain |
| `0x114818` / `sub_114818` | Startup / static state | Startup/static-object initialization or teardown chain |
| `0x114884` / `sub_114884` | Startup / static state | Startup/static-object initialization or teardown chain |
| `0x114890` / `sub_114890` | Startup / static state | Startup/static-object initialization or teardown chain |
| `0x1148B8` / `sub_1148B8` | Startup / static state | Returns static object at 0x1210B4; ownership details deferred |
| `0x1148E0` / `sub_1148E0` | Startup / static state | Startup/static-object initialization or teardown chain |
| `0x1148EC` / `sub_1148EC` | Runtime / compiler support | Multiplies counts, allocates through MvdHeapAlloc, then clears |
| `0x119840` / `sub_119840` | Heap / allocation | Heap headers, intrusive lists, allocation/free or containing-heap lookup |
| `0x119860` / `sub_119860` | Runtime / compiler support | Indirect member-function-style dispatch |
| `0x11987C` / `sub_11987C` | Startup / static state | Startup/static-object initialization or teardown chain |
| `0x119888` / `sub_119888` | Startup / static state | Startup/static-object initialization or teardown chain |
| `0x1198D0` / `sub_1198D0` | Startup / static state | Startup/static-object initialization or teardown chain |
| `0x119930` / `sub_119930` | SVC veneer | SVC 1; exact register ABI deferred |
| `0x1199CC` / `sub_1199CC` | Runtime / compiler support | Bytewise string-comparison family |
| `0x119A6C` / `sub_119A6C` | SVC veneer | SVC 0x21; exact register ABI deferred |
| `0x119A84` / `sub_119A84` | Startup / static state | Startup/static-object initialization or teardown chain |
| `0x119A90` / `sub_119A90` | SVC veneer | SVC 0x2B; exact register ABI deferred |
| `0x119AB4` / `sub_119AB4` | SVC veneer | SVC 0xA; exact register ABI deferred |
| `0x119AC4` / `sub_119AC4` | SVC veneer | SVC 0x35; exact register ABI deferred |
| `0x119AE0` / `sub_119AE0` | Synchronization / atomics | Lock/arbitration state, thread identity, or exclusive-update callback |
| `0x119B2C` / `sub_119B2C` | Synchronization / atomics | Lock/arbitration state, thread identity, or exclusive-update callback |
| `0x119B78` / `sub_119B78` | SVC veneer | SVC 0x22; exact register ABI deferred |
| `0x119B90` / `sub_119B90` | Synchronization / atomics | Lock/arbitration state, thread identity, or exclusive-update callback |
| `0x119BDC` / `sub_119BDC` | Synchronization / atomics | Lock/arbitration state, thread identity, or exclusive-update callback |
| `0x119C28` / `sub_119C28` | Synchronization / atomics | Lock/arbitration state, thread identity, or exclusive-update callback |
| `0x119C74` / `sub_119C74` | Synchronization / atomics | Lock/arbitration state, thread identity, or exclusive-update callback |
| `0x119CC8` / `sub_119CC8` | SVC veneer | SVC 0x27; exact register ABI deferred |
| `0x119D28` / `sub_119D28` | Runtime / compiler support | 64-bit multiplication |
| `0x119D40` / `sub_119D40` | Runtime / compiler support | Veneer to memcpy |
| `0x119D4C` / `sub_119D4C` | Runtime / compiler support | Fill wrapper branching into 0x10ADD8 |
| `0x119D68` / `sub_119D68` | Runtime / compiler support | Fill ABI veneer into 0x10ADD8 |

See [attribution-coverage.md](attribution-coverage.md) for the original audit scope and [auxiliary-drivers.md](auxiliary-drivers.md) for the subsequent driver and IPC analysis. General SDK/runtime attribution remains deferred.
