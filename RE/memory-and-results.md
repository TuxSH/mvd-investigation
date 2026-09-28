# Results, sizing and memory

## Result conversion

The library returns signed Hantro status codes. `MvdNormalizeLibraryStatus` (`0x112BC4`) and `MvdConvertLibraryResult` (`0x10B608`) turn these into 3DS results. Native success zero normally becomes `0x00017000`; several lifecycle and sizing wrappers instead return raw zero directly.

| Native decoder status | 3DS result | Meaning |
|---:|---|---|
| 0 | `0x00017000` | OK |
| 1 | `0x00017001` | Stream processed |
| 2 | `0x00017002` | Picture ready for output |
| 3 | `0x00017003` | Picture decoded |
| 4 | `0x00017004` | Headers ready |
| 5 | `0x00017005` | Advanced tools |
| 6, H.264 | `0x00017006` | Pending flush |
| 6, VP8 class | `0x00017038` | Slice ready |
| 7 | `0x00017007` | Nonreference picture skipped |

Not every decoder defines or emits every positive status. Picture-ready from next-picture is a successful dequeue, not a generic busy result. Headers-ready is an event at which the client can inspect dimensions and set up postprocessing.

Normalization special-cases PP timeout (-257 → 455) and PP system error (-259 → 457), and VP8-class slice-ready (6 → 56). Otherwise a negative status becomes `abs(status)+200` for magnitudes below 900, or `abs(status)-100` for magnitudes at least 900; the normalized value is truncated to 16 bits. Positive values pass through the same 16-bit return type. The subsequent conversion is a switch/decision tree, **not** unconditional addition to a base result.

| Native error | Result | Meaning |
|---:|---|---|
| -1 | `0xE16170C9` | Parameter error |
| -2 | `0xD96170CA` | Stream error |
| -3 | `0xD96170CB` | Not initialized |
| -4 | `0xD86170CC` | Memory allocation failure |
| -5 | `0xD96170CD` | Decoder initialization failure |
| -6 | `0xD96170CE` | Invalid headers |
| -8 | `0xD96170D0` | Unsupported stream |
| -254 | `0xD96171C6` | Hardware reservation failure |
| decoder -255 / PP -257 | `0xD96171C7` | Hardware timeout |
| -256 | `0xF96171C8` | Hardware bus error |
| decoder -257 / PP -259 | `0xD96171C9` | System error |
| -258 | `0xD96171CA` | DWL error |
| PP -512 | `0xD96172C8` | Combined-mode error |
| PP -513 | `0xD96172C9` | Decoder runtime error |
| -999 | `0xD9617383` | Evaluation limit |
| -1000 | `0xD9617384` | Unsupported format |

PP configuration errors -64 through -83 map consecutively to `0xD9617108` through `0xD961711B`: input size, input address, input format, crop, rotation, output size, output address, output format, video adjustment, RGB masks, framebuffer, mask 1, mask 2, deinterlace, input picture structure, input range mapping, alpha blending unsupported, deinterlacing unsupported, dithering unsupported, scaling unsupported. Thus `0xD961710F` specifically means invalid output format.

Several unmapped statuses fall through to **raw zero**. In particular PP_BUSY (-128) normalizes to 328, which has no conversion case. Raw zero therefore does not prove that every lower-level operation succeeded. This observation is about this converter, not a claim that every IPC path can reach PP_BUSY. The decision tree also has narrow default branches between enumerated cases, so the table must not be extrapolated to arbitrary native integers.

Initialize, shutdown, release and size-query wrappers have direct-zero success paths. Codec initialize/decode/info/picture and ordinary PP operations generally use the converter. Kernel failures and wrapper-specific failures need not follow the Hantro mapping.

## Work-buffer sizing (`03`)

Anchors: wrapper `0x11307C`, selection `0x102E84`, level calculation `0x1142F8`, macroblock calculation `0x10B3F0`, DPB calculation `0x119D78`, reference storage `0x10E9D4`. The request contains 12 normal words. Offsets below are relative to the payload, immediately after the IPC header:

| Payload offset | Meaning |
|---|---|
| `0x00` | Unused leading byte |
| `0x01` | Enable level-based candidate |
| `0x02` | Level candidate flags |
| `0x03` | Double level candidate's image allocation |
| `0x04` | Level-table index |
| `0x05`, `0x06` | Enable reference candidate A, reference count A |
| `0x07`, `0x08` | Enable reference candidate B, reference count B |
| `0x09..0x27` | Not used by this calculation |
| `0x28`, `0x2C` | Width, height (u32) |

The internal structure reorders width/height ahead of the eight control bytes. The returned size is the **maximum** of the enabled candidates, not their sum. It is zero if no method is enabled or the 32-bit width×height product is zero.

Let `M = ceil(width/16) * ceil(height/16)` and `Y = 384*M`. The implementation uses floating-point conversion and `ceilf`, rather than a pure integer alignment expression; the formula describes ordinary image sizes. Extremely large inputs can overflow integer intermediates or lose float precision.

* Candidate A: `Y * clamp(countA, 2, 16) + 67584`
* Candidate B: `Y * clamp(countB, 2, 16) + 2136`
* Level candidate: if flags are zero, it contributes zero. Otherwise require `M <= maxFrameMbs`, then set `R = min(floor(maxDpbMbs/M), 16)`. If `R` is zero, contribute zero. Start with `S = Y*(R+1)`; if `flags & 6` is nonzero, add `floor(S/6)`. If the doubling byte is nonzero, double the resulting `S`. Return `S+4040`

These formulas describe the code rather than assigning unproven codec meanings to candidate A/B. The flag test enabling the level path is **any nonzero byte**, not only bit zero. The final fixed 4040-byte allowance is added after doubling.

The 17-entry table at `0x11A0D0`, now typed as `g_mvdH264LevelLimits`, stores index, maximum frame macroblocks, maximum DPB macroblocks in 12-byte `MvdH264LevelLimit` records. The index is not range-checked by the sizing helper.

| Index | maxFrameMbs | maxDpbMbs |
|---:|---:|---:|
| 0, 1 | 99 | 396 |
| 2 | 396 | 900 |
| 3, 4, 5 | 396 | 2376 |
| 6 | 792 | 4752 |
| 7, 8 | 1620 | 8100 |
| 9 | 3600 | 18000 |
| 10 | 5120 | 20480 |
| 11, 12 | 8192 | 32768 |
| 13 | 8704 | 34816 |
| 14 | 22080 | 110400 |
| 15, 16 | 36864 | 184320 |

`ceilf` at `0x119DDC` and `floorf` at `0x119E6C` take/return `s0`. Their recovered `__usercall` prototypes are necessary: treating them as integer-returning functions produced misleading pseudocode. The wrapper's overlapping stack-byte extraction was checked in disassembly.

## Image-size query (`04`)

`0x102D08` clamps each dimension to 1920. Formats `0x010001` and `0x040002` return two bytes per pixel; `0x041002` returns four. Other formats call an assertion routine; if that routine returns, control falls through to the four-byte calculation. This helper is not a general PP plane-size calculator, nor does its clamp define all decoder/PP limits.

## Work memory and address translation

The initialized client process and work-buffer range underpin decoder allocations. DWL memory helpers have Nintendo-specific heap, copy, cache and interrupt implementations. They are not the Linux DWL device-file implementation.

`MvdClientVirtualToBus` (`0x10B5D8`) uses `MvdTranslateLinearRange` (`0x10B9BE`). It translates either the `0x14000000..0x1C000000` region or `0x30000000..0x40000000` region to a bus region beginning at `0x20000000`, subtracting the corresponding virtual base. It checks the supplied start and computed end against the selected range. Arithmetic is 32-bit and the endpoint check admits the upper endpoint for a zero-sized range. This is fixed linear-window arithmetic, not arbitrary process page-table translation.

`MvdCopyMemory` (`0x10F474`) classifies an address at or above `0x30000000` as belonging to the currently selected client process, and lower addresses as belonging to the current process pseudo-handle. Local-to-local copying uses the runtime memcpy. Cross-process paths configure process DMA and cache maintenance. `MvdFillMemory` (`0x118D04`) similarly uses local memset for local memory and temporary filled storage plus copying for client memory. Callers temporarily install the process handle around library operations and reset it afterwards; this is global operation context.

The arithmetic translator recognizing the old `0x14*` linear window therefore does **not** imply that client pointers in that window work throughout the service. The copying layer treats them as module-local addresses. This distinction explains why client-facing usage should use the `0x30*` window despite the translation helper containing both ranges.

For normal decoded-picture allocations, **the returned virtual addresses belong to the client**, not to private module memory. `MvdDwlAllocateLinear` (`0x10F56C`) stores the client work-buffer cursor directly in `MvdLinearMem.virtualAddress` and derives `busAddress` separately. For example, `VP8DecPeek` (`0x1113F0`) copies `prevOutBuffer->virtualAddress` into the returned luma pointer; the chroma pointer comes from its allocation descriptor or an offset into the same picture allocation. H.264/VP6 `output_vaddr` and VP8 `luma_vaddr`/`chroma_vaddr` describe decoded storage through this normal allocation path. The decoder manages the allocations and reference-picture lifetime within memory supplied by the client.

Conceptually, the picture VA is `client_work_buffer + allocation_offset`, while the paired bus address lets G1 access the same bytes. The address returned through IPC locates the pixels without copying the image into the reply. MVD's software copy layer accesses client memory through process DMA and the selected client process handle; storing a client VA in a decoder descriptor does not make it a module-local pointer.

Client CPU access requires a valid picture result, hardware completion, the appropriate layout and plane strides, and cache coherence. Retaining an address does not prevent the decoder from reusing that picture buffer. PP is useful for conversion/resizing and output into separately registered buffers; it is not required merely to make the normal decoded-picture VA accessible. Optional WebP user-picture buffers follow a separate path, and these allocator findings do not establish that every such mode works. The H.264 compressed-stream progress pointer discussed below is a different case: it points into freed module scratch.

The decode wrappers copy compressed input to module-owned scratch memory for software parsing. The hardware stream bus address remains the caller's supplied address. H.264 frees the scratch after decoding, but returns the library's current-stream pointer into that scratch along with the bus position and bytes left. The returned virtual pointer is therefore not a durable client pointer; use the bus/remaining-byte progress information to reason about consumption. This behavior also explains why both stream VA and bus address are supplied.

## Static implementation defects and sharp edges

These were observed in code; no malformed-input or hardware experiments were run.

* `0x112EF4` (`1F`) translates and writes entries using the caller count **before** the later 17-entry clamp and lower PP validation. Its stack output table and global mapping table are finite; the loop index is narrowed to u8. Excess counts can exceed those tables, and very large counts can prevent index progress beyond 255. The later check does not protect the first loop
* `0x112DE8` (`20`) allocates an eight-byte output object without a null check, and scans mappings even if `PPGetNextOutput` fails. Matching compares only luma bus address; the last duplicate match wins. A missing match leaves the caller output untouched. It scans `count+1` entries to include the saved override mapping
* `0x112FE0` (`21`) saves the extra VA mapping only on the first override. Subsequent replacements do not refresh that extra mapping
* `0x113248` (`19`) has a clearing loop whose stores repeatedly use the fixed current count as the index, rather than the loop counter. It then resets the count. This was checked in disassembly; it does not clear all previous entries
* `0x112D24` (`1E`) sets cache size only for `0x010001`, `0x040002` and `0x041002`. Other formats reach cache invalidation with an uninitialized R5 size. These branches were checked in disassembly
* `1D`/`1E` do not use the explicit size word to bound the `0x11C` configuration access
* Info/picture IPC replies can copy full stack structures on native paths that did not populate all fields. A nonnegative IPC result alone is insufficient to assume all picture fields are valid; interpret the specific native event and function contract

These findings do not establish exploitability, kernel mapping behavior, or effects on a running console. They do constrain which API sequences and results can safely be interpreted from the static code.

## DWL allocation and pre-enable cache continuation

Hardware buffers are allocated from a global cursor in the caller-provided work buffer. Individual `DWLFreeLinear` and `DWLFreeRefFrm` calls are no-ops. The allocator advances by the exact requested byte count, without alignment or rollback on later range-check failure. This differs from local heap allocation. The PP enable path also has a separate cache flush based on programmed output dimensions and format; its bus-to-client helper uses a permissive OR condition verified in disassembly. See [platform-glue.md](platform-glue.md) for the exact predicates, addresses and wait behavior.
