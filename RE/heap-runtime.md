# Process heap and allocation runtime

Heap analysis was the final family in the platform pass. The implementations are identified by their instructions and call graph. `MvdSdk` and `MvdExpandedHeap` are descriptive names, not a claim that an exact Nintendo SDK source release was recovered. The Hantro decoder wrappers use this allocator through `MvdHeapAlloc` and `MvdHeapFree`; the allocator itself is outside the supplied G1 implementation.

## Process memory and object layout

`MvdRuntimeInitializeProcessHeap` (`0x1000A8`) requests **1 MiB at `0x08000000`** before constructing the heap. `MvdSdkResizeProcessHeap` (`0x1000F0`) accepts only multiples of `0x1000`; other sizes return `0xE0E01BF2`. Growth commits the newly added tail with read/write permission. Shrinking frees the removed tail with permission zero. The cached committed size at `0x12108C` changes only after a successful SVC. Equal old/new sizes still take the free branch with zero length.

The constructor (`0x10013C`) aligns the object address upward to four bytes and reserves **100 bytes** for `MvdSdkHeapObject`. It initializes the expanded heap over the remainder, initializes the recursive lock and publishes `g_mvdHeap` and the small heap-interface object.

| Heap-object offset | Size | Recovered field |
|---|---:|---|
| `0x00` | 4 | Vtable pointer |
| `0x04` | 20 | Five zeroed words, original class roles unknown |
| `0x18` | 60 | Embedded expanded heap |
| `0x54` | 4 | Allocation count |
| `0x58` | 12 | Recursive lock |
| `0x64` | — | Beginning of managed block region |

The twelve-byte interface at `0x1213C4` contains a vtable pointer, heap pointer and three initialized flag bytes. The vtable-address regions referenced at `0x120640` and `0x120658` contain zero words in the supplied image; they do not identify additional callable virtual methods or source classes.

`MvdSdkInitializeHeapObject` (`0x1004CC`) panics if expanded-heap initialization fails. Expanded initialization (`0x10093C`) aligns the managed range inward to four-byte boundaries, rejects reversed ranges or fewer than **20 bytes**, installs signature `0x45585048`, initializes policy/group state and creates one free block. The reuse-padding byte is set by the allocation wrapper before allocation; the initializer does not independently initialize every byte of the state view.

## Heap header, registry and address arenas

The common header is 36 bytes:

| Offset | Size | Field |
|---|---:|---|
| `0x00` | 4 | Signature `0x45585048` for this expanded heap |
| `0x04` | 4 | Previous heap in registry list |
| `0x08` | 4 | Next heap in registry list |
| `0x0C` | 12 | Child-heap intrusive list |
| `0x18` | 4 | Managed begin address |
| `0x1C` | 4 | Managed end address, exclusive |
| `0x20` | 4 | Options; initializer takes an eight-bit value |

`MvdSdkRegisterHeapHeader` (`0x100B10`) initializes the child list with link offset 4 and lazily initializes the root list at `0x12186C`. `0x100D76` recursively searches half-open managed ranges and returns the deepest containing heap. Registration searches using the **header's own address**, then appends it to that containing heap's child list or to the root list.

The generic intrusive-list view is twelve bytes: first pointer, last pointer, 16-bit count and 16-bit embedded-link offset. List append uses that offset to access an object's previous/next fields. Unlike the notification list, these lists are null-terminated. `MvdSdkListNext(list, NULL)` selects the first object.

Two 24-byte address-arena records are also initialized during startup:

| Record | Managed bounds | Initialization |
|---|---|---|
| `0x121854` | `0x0E000000..0x10000000` | `0x100480` |
| `0x12183C` | `0x10000000..0x14000000` | `0x100498` |

Each contains begin/end words, an unnamed word and a recursive lock. These functions record bounds; they do **not** map those ranges with ControlMemory. The shared construction helper clears state, then installs bounds only when both existing endpoints are zero. The separate global lock at `0x121830` has an initializer; its existence is not evidence that every registry operation acquires it.

## Block layout and allocation policy

The expanded state immediately follows the common header:

| State offset | Size | Field |
|---|---:|---|
| `0x00` | 8 | Free-list first/last pointers |
| `0x08` | 8 | Used-list first/last pointers |
| `0x10` | 2 | Group ID |
| `0x12` | 2 | Allocation flags; bit 0 selects best fit |
| `0x14` | 1 | Reuse alignment padding |
| `0x15` | 3 | Unassigned padding/reserved bytes |

Every block begins with a **16-byte header**:

| Offset | Size | Field |
|---|---:|---|
| `0` | 2 | Signature: free `0x4652`, used `0x5544` |
| `2` | 2 | Used-block metadata |
| `4` | 4 | Payload size, excluding header |
| `8` | 4 | Previous block |
| `0xC` | 4 | Next block |

The block signatures above are numeric halfwords; their little-endian bytes should not be confused with a source-level multicharacter constant's spelling.

Used metadata contains:

* Bits 0–7: low eight bits of the current group ID, even though the setter stores sixteen bits.
* Bits 8–14: absorbed prefix length between the consumed-range start and the used header, limited to seven bits.
* Bit 15: allocation direction, set for allocation from the back.

`MvdHeapGetBlockEnd` (`0x10F790`) returns `header + 16 + payloadSize`. `MvdHeapGetBlockRange` (`0x10125C`) subtracts the seven-bit absorbed-prefix count to reconstruct the full consumed interval. This is why free must recover the range rather than merely inserting the nominal user buffer.

## Allocation search and splitting

`MvdExpandedHeapAllocate` (`0x100FF4`) changes a size of zero to one, then rounds upward to a multiple of four. The signed alignment selects the search direction:

* Nonnegative alignment: scan the free list from the first block and align the user pointer upward.
* Negative alignment: scan backward from the last block, using the magnitude to align the user pointer downward from the available tail.

Policy bit 0 clear chooses the first suitable block in that direction. Set chooses the smallest fitting block by its payload size; an exact-size match terminates the search. Equal-size candidates do not replace the first selected candidate. The code uses the usual `alignment-1` masks but does not validate a nonzero power-of-two alignment. The ordinary MVD allocation wrapper supplies the valid fixed value 4.

`MvdExpandedHeapCarveBlock` (`0x101124`) removes the chosen free block and forms prefix/suffix intervals around the user allocation and its header. A reusable fragment must be at least **20 bytes**: a 16-byte header plus four payload bytes.

| Direction | Prefix fragment | Suffix fragment |
|---|---|---|
| From front | Split only if at least 20 bytes and reuse-padding is enabled; otherwise absorb | Split if at least 20 bytes; otherwise absorb |
| From back | Split if at least 20 bytes; otherwise absorb | Split only if at least 20 bytes and reuse-padding is enabled; otherwise absorb |

The consumed interval is optionally cleared when common-header options bit 0 is set. The helper then initializes a used header at `userPointer - 16`, records metadata, and appends it to the used list. Clearing occurs **before** writing the used header and covers the consumed range, including absorbed padding. An unsplit tail contributes to the resulting block's payload size.

`MvdSdkHeapAllocateLocked` (`0x100DAC`) holds the heap object's recursive lock while setting group/best-fit/reuse-padding policy, allocating, and incrementing the allocation count on success. `MvdHeapAlloc` (`0x10F82C`) always requests:

```
size = caller's size
alignment = 4
groupId = 0
bestFit = 0
reuseAlignmentPadding = 0
```

The process heap is created with options zero. Ordinary allocations therefore use forward first fit, group zero and no allocator-driven clearing. The separate calloc wrapper clears explicitly.

## Free and coalescing

`MvdHeapFree` (`0x10F500`) accepts null as a no-op. Otherwise `0x119840` holds the object lock around `0x111DB6`, which frees the block and decrements the allocation count.

`MvdExpandedHeapFree` (`0x111DC8`) reads the header at `allocation - 16`, recovers its consumed interval, removes it from the used list, and passes the interval to the coalescer at `0x111DEC`.

The coalescer maintains the free list in increasing address order. It merges a successor exactly when `freedEnd == successorHeader`, and a predecessor exactly when `predecessorEnd == freedBegin`. It removes merged headers, writes one free header and inserts the merged block after the remaining predecessor. A final range shorter than 16 bytes is rejected; this lower-level threshold differs from the allocation splitter's 20-byte minimum.

`MvdHeapBlockListRemove` (`0x10F7BC`) returns the **previous block pointer in R0**. R1 happens to hold the next pointer. The former 64-bit return inferred from an LDM pair was incorrect for these callers and made list manipulation misleading.

The inspected free path does not independently validate the used signature, allocation ownership, or duplicate frees. The count decrements without checking the coalescer's boolean. These are observed preconditions of internal allocator use, not claims that malformed allocations were exercised.

## Calloc and emergency buffer

`MvdRuntimeCalloc` (`0x1148EC`), reached through the veneer at `0x10189C`, multiplies count and element size in 32 bits, calls `MvdHeapAlloc`, then clears the computed number of bytes on success. There is no multiplication-overflow check. Because allocation converts zero to one before rounding, a zero product can still produce a minimum allocation, while calloc clears zero bytes.

The runtime emergency-buffer allocator at `0x100C18` requests 136 bytes and clears only its busy byte. The payload begins at offset 8 and is at most 128 bytes. Both normal runtime-state initialization paths keep this feature disabled with a constant; its presence does not imply ordinary allocations use it as a fallback.

## Validation and limits

Names, arguments, structure offsets and scalar stack slots were checked by regenerating the important allocator callers after application. The recovered structures are sized explicitly: common header 36, expanded state 24, expanded heap 60, heap object 100, block 16 and range 8 bytes. Allocation and free loops were checked against their instructions for list return values, metadata bit positions and split thresholds.

No allocator was executed against the host or console, no malformed input testing was needed, and no reference-source files were changed. Analysis does not establish an original SDK class name or prove that the allocator accepts arbitrary alignment, size overflow, or invalid pointers safely.
