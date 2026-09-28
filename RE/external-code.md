# External-code identification

## Reference material

The source used for comparison is `/Users/elouan/Documents/git_dl/buildroot-ltc/system/hlibg1v6`, in buildroot-ltc commit `31cf5593a5bb4de4608425886e93f4be628f87f4`. Client-side comparison uses `/Users/elouan/Documents/git/libctru`, commit `0a376398d19505df3c236342827a3a72a42dd41e`, especially `libctru/source/services/mvd.c` and `libctru/include/3ds/services/mvd.h`.

[3DBrew MVD Services](https://www.3dbrew.org/wiki/MVD_Services) was consulted as a baseline, including its indexed revision 21607 when direct access returned HTTP 403. The findings below are independently grounded in the database and local source; the wiki's field names and experimental support claims are not treated as authoritative symbol information.

## Why this is shared code

Several independent fingerprints agree:

| Binary anchor | Source counterpart | Evidence |
|---|---|---|
| `0x107670` | `source/pp/ppapi.c: PPInit` | Same hardware product gate (`0x8170` / `0x6731`), DWL client 4, allocation/zero/init/configure/release sequence |
| `0x107A44` | `PPSetConfig` | Same multibuffer address substitution, previous/current configuration copies, BT.601/709 coefficient branches, format selection and hardware setup |
| `0x10635C` | `source/pp/ppinternal.c: PPCheckConfig` | Same sequence of input, crop, rotation, output, scaling, framebuffer, RGB, masks, deinterlace and range-map checks; same negative error vocabulary |
| `0x106AF0` | `PPDecCombinedModeEnable` | Same idle/already-linked checks and PP callback registration, with compiled codec cases reduced to 1, 6, 9 and 10 |
| `0x103BE0` | `source/h264high/h264decapi.c: H264DecInit` | Same no-reorder/freeze/smoothing/reference-format arguments, self-pointer validation design, capability gate and reference-filter setup |
| `0x102F2C` | `H264DecDecode` | Same 20-byte input, 12-byte output, parser/ASIC state machine, status values and stream-position accounting |
| `0x103AD4` | `H264DecGetInfo` | Same dimensions, range, matrix, crop, SAR, monochrome/interlace and DPB information, with an extra word in this build |
| `0x109628` | `source/vp6/vp6hwd_api.c: VP6DecInit` | Same DWL client 7, reference count clamped to 3..16, concealment and tiled-reference handling |
| `0x110EA0` | `source/vp8/vp8decapi.c: VP8DecInit` | Same VP7/VP8/WebP selector, DWL client 10, buffer minima 3/4/1 and mode-specific branches |
| `0x10F324`, `0x10E0FC` | `source/common/regdrv.c: SetDecRegister`, `GetDecRegister` | Identical table-driven word/width/shift insertion and extraction algorithm |
| `0x11A38C` | `source/common/8170table.h` | Ordered register triples align across long runs; 698 ordered source transfers initially; later one IRQ name and 25 call-site-derived names, retaining six unnamed entries |

Together these establish Hantro software ancestry at function and data-layout level. A generic Hantro-compatible register map alone would not establish that ancestry.

## Differences that affect type recovery

* The local Linux APIs have an added `mmuEnable` argument. The observed MVD H.264/VP6/VP8 initialization entry points do not take that argument; PP initialization takes only an instance-output pointer.
* `PPConfig` is exactly `0x11C` bytes and `PPOutputBuffers` is `0x8C` bytes in both the observed ABI and source.
* MVD H.264 info is `0x40` bytes, versus the reference header's `0x3C`. The additional word follows `interlacedSequence` and carries the DPB storage-mode state.
* MVD VP8 info is `0x28` bytes, versus source `0x24`; VP6 info is `0x24`, versus source `0x20`. Each has a zeroed byte plus padding before the final format word. Its semantic name remains unresolved.
* MVD VP8 picture is `0x40` bytes, with luma/chroma stride words before the output pointers. The source header lacks those two words. Format is stored with a byte write at the end.
* The recovered PP container is `0x57C` bytes. Its frame bookkeeping includes two bottom-field addresses per buffer, and its stored combined result is a signed halfword. Importing the reference container wholesale would mislabel later members.
* The capability structure is 100 bytes with reordered members and extra fields, versus the reference `DWLHwConfig_t`'s 84 bytes. A separate `MvdDwlHwConfig` preserves the observed ordering and unresolved words.
* The MVD register table contains 730 meaningful entries including the two aggregate IRQ fields; the local table has 701. Aligned names were transferred by ordered triple matching, not by copying enum ordinals.

* MVD linear memory descriptors are 12 bytes; the Linux `uid` word is absent. H.264 DPB is 1680 bytes, and decoder-to-PP interface is 104 bytes with two extra stride words. Internal container allocations are H.264 15860, VP6 2424 and VP8 4224 bytes.
* VP8 adds separate chroma buffers, stride controls and hardware-concealment state; the tiled-reference helper adds a DPB-mode argument. See [decoder-internals.md](decoder-internals.md).

* MVD access-unit-boundary state is 76 bytes, with an extra masked previous frame number. VP8 adds coefficient-probability progress and a fully recovered concealment tail. Six concealment helpers have descriptive names because no exact counterpart was found in the local revision; this is not an attribution of authorship. VP6’s 4544-byte Huffman workspace is source-matched. See [codec-leaf-analysis.md](codec-leaf-analysis.md).

These differences indicate a related Hantro branch, not a byte-identical build of the provided source revision. The exact upstream release and Nintendo's patch history are not established.

## Database treatment

Known library functions retain upstream API/internal names. Platform adaptations are explicitly distinguished: the DWL API wraps MVD heap allocation, cross-process copying and interrupt handling rather than Linux device nodes or `mmap`. The existence of `DWLInit` does **not** mean the Linux DWL implementation was included.

Public data types were imported where layouts match. `Mvd*` structures describe observed ABI variants. Unresolved container fields remain reserved instead of importing plausible but incompatible members. Original named service-dispatch functions were preserved; the old generic `ValidateConfig` and `ConvertErrorCode` names were refined.

[The function inventory](function-map.md) records applied names and prototypes. This is an inventory of identified functions, not a claim that every runtime, SDK, or internal decoder routine has been attributed to an exact source function.
