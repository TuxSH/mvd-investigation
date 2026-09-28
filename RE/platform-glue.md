# MVD adaptations of the Hantro DWL boundary

This part was analyzed after the codec/layout work, as requested. Hantro DWL entry-point names describe the library-facing contract; their Nintendo implementations use descriptive `Mvd*` names. They are not Linux DWL source matches.

## Register access

`DWLReadReg` (`0x10D7C4`) forwards to `MvdDwlReadRegister` (`0x118D8C`), which reads a word at `0x1ED07000 + byteOffset`. The DWL instance is unused by this primitive.

`DWLWriteReg` (`0x10DA50`) forwards to `MvdDwlWriteRegister` (`0x118DFC`). Its `MvdIsWritableG1Register` predicate (`0x101416`) permits only these word indices:

```text
1..49, 51, 55, 59..74, 79..95
```

For aligned accesses this is exactly offsets `004..0C4`, `0CC`, `0DC`, `0EC..128`, and `13C..17C`. These agree with the writable ranges in the supplied GBATEK excerpt. Writes to ID, synthesis/fuse words and the other holes are silently omitted. The predicate shifts the offset by two and does not separately enforce alignment; normal library callers supply word-aligned offsets.

The disassembly preserves the byte offset in R3 and value in R2 across the predicate call. Hex-Rays initially displayed these as undefined temporaries because of its conservative call-clobber model. Register-spoil information and comments were applied. After invalidating the decompiler cache, fresh pseudocode correctly uses the original offset and value; the actual store was also checked in disassembly.

Enable forwards through a PP cache-maintenance helper before writing the control register; disable just uses the filtered register writer.

## Allocation lifetime

`MvdDwlCreate` allocates 44 bytes, accepts client types 1/4/7/10, stores the type, clears the last linear/reference allocation sizes, initializes a word at offset 20 to 400 and clears the global next-allocation-kind flag. The purpose of the 400-valued word is not established. `MvdDwlDestroy` frees this local instance.

`DWLMallocLinear` calls `MvdDwlAllocateLinear` (`0x10F56C`). It fills a 12-byte descriptor with virtual address, bus address and requested byte size. Allocations consume the global client work-buffer cursor and remaining count. `DWLMallocRefFrm` first sets a flag, allowing the shared allocator to record the request as a reference-frame allocation.

The allocator adds the exact requested size; it does not round or align it here. Callers must arrange any required alignment through their sizes and initial buffer address. `DWLFreeLinear` (`0x10CD24`) and `DWLFreeRefFrm` (`0x10C918`) are empty functions, so individual library frees do not restore this cursor. Local `DWLmalloc`/`DWLfree` still use the module heap and are separate from hardware-buffer allocation.

The range predicate requires a nonempty client range starting at or above `0x30000000` and ending no later than `0x3FFFFFD8`, with wraparound rejected by the end-greater-than-start test. The cursor is advanced before this validation and is not rolled back on validation failure. After bus translation the return path checks the virtual pointer, rather than independently checking for a zero bus address. These are direct control-flow observations, not claims of reachable exploitation.

## Interrupt wait behavior

`DWLWaitHwReady` dispatches on DWL client type. H.264, VP6 and VP8 use `MvdWaitDecoderStatus` (`0x111A4C`); PP uses `MvdWaitPostprocessorStatus` (`0x111A9C`). Null/unrecognized instances return -1.

The supplied timeout argument is ignored. Each loop:

1. Uses the shared hardware event at `0x121050`
2. Checks status bits 19:12 of the decoder control word or PP control word
3. If no status is present, waits with a newly constructed one-second timeout and clears the event before checking again

The PP loop also stops when decoder status bit 18 is set. A status-driven completion returns 0; a timed-out event wait returns 1. Event signals without relevant status can cause another full one-second wait, so this is not an overall one-second deadline.

The time conversion is confirmed in disassembly: 1000 milliseconds multiplied by 1,000,000 gives nanoseconds for SVC 0x24. SVC 0x19 clears the event. The SDK result wrapper recognizes timeout by the low ten-bit description value 1022 and calls a separate SDK error handler for negative results; that handler's full policy is not yet attributed.

`MvdDwlReserveHardware` selects the PP lock at `0x12132C` or decoder lock at `0x121320`, then maps lock success to DWL result 0/-1. Full lock ownership/release and thread-lifecycle attribution remain outside this pass.

## PP cache flush before enable

`MvdFlushPpOutputBeforeStart` (`0x101374`) acts for PP client type 4. It reads output width, height and hardware format from bank word 85 (`0x154`). For RGB hardware format 0, word 79 bit 28 selects two versus four bytes per pixel. For hardware format 3 it uses two bytes per pixel. Other format cases return without a flush in this helper.

The helper uses the low eleven-bit width and height fields; it does not combine the later extension fields recovered in the register table. It reads the output bus address at `0x108`, converts it to a client address and invokes `svcFlushProcessDataCache` (SVC 0x54). Its return value is ignored. The byte count remains live in R3 across the address helper, which was verified directly in disassembly.

`MvdBusToClientVirtualForCache` at `0x101768` adds `0x10000000` when either `address >= 0x20000000` **or** the unsigned sum `address + size <= 0x30000000`; otherwise it returns zero. The OR is present in the branch instructions. This helper is therefore not a strict validation of the expected bus-address interval, and its behavior should not be rewritten as an AND in reconstructed code.

Other client-copy, DMA and wrapper cache-maintenance paths are described in [memory-and-results.md](memory-and-results.md). Their existence should not be confused with this additional flush immediately before PP enable.
