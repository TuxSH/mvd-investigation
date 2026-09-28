# H.264 shared epilogue and Thumb BL correction

## Finding

The ten bytes at `0x115CDE..0x115CE7`, previously named `sub_115CDE`, are a shared epilogue of `h264bsdDecode` (`0x115C28`). They are not a separately callable decoder helper. The instruction at `0x1165A4` uses Thumb `BL` as a long jump to this epilogue.

Before correction, the standalone epilogue decompiled as a call through an uninitialized stack function pointer. Its parent displayed an apparent call returning a pointer followed by further decoder work, and several ordinary returns appeared as `JUMPOUT(0x115CE0)`. Those expressions were analysis artifacts, not binary behavior.

## Disassembly evidence

The parent prologue pushes `R0-R7,LR` and subtracts `0x1FC`, `0x1FC` and `0x1F4` from SP. Total stack movement is `0x610` bytes. The shared error path is:

```asm
00115CDE  MOVS R0, #3
00115CE0  LDR  R4, =0x600
00115CE2  SUBS R4, R4, #4
00115CE4  ADD  SP, R4
00115CE6  POP  {R4-R7,PC}
```

Adding `0x5FC` and popping five words restores the same `0x610` bytes. The return PC comes from the parent's saved LR. A `BL` to this sequence therefore does not return to the instruction following that `BL`.

The branch at `0x1165A4` targets `0x115CDE`, a displacement of -2250 bytes from the Thumb PC base (`instruction address + 4`), beyond the short unconditional Thumb `B` range. Other parent branches target `0x115CE0` directly with a result already in R0. The failed-NAL-extraction path falls through into `0x115CDE`.

This use of `BL` and IDA's per-instruction correction are described by [Hex-Rays: ARM BL jumps](https://hex-rays.com/blog/igors-tip-of-the-week-134-arm-bl-jumps). The MVD classification rests on the observed branches and matching stack restoration, not just that general compiler convention.

## Corrected decoder behavior

The source counterpart is `source/h264high/h264hwd_decoder.c` in the local Hantro tree. Its `H264BSD_ERROR` value is 3, from `h264hwd_decoder.h`.

| Trigger | Binary path | Correct result |
|---|---|---|
| `h264bsdExtractNalUnit` fails | Branch decision at `0x115CDC`, followed by the error epilogue | Return 3 immediately |
| `h264bsdDecodeSliceData` fails | Call `h264bsdMarkSliceCorrupted` at `0x1165A0`, then long branch at `0x1165A4` | Return 3 immediately after marking corruption |

Both paths match explicit returns in the source. In particular, a failed slice-data decode does not resume at the following B-slice/profile checks. This is an annotation/control-flow correction, not discovery or repair of a firmware bug.

## Database changes and verification

The false function entry was removed; its ten-byte range was assigned to `h264bsdDecode`. Its stale function type was removed, and `h264bsdDecode_returnError` is now an internal code label. The branch at `0x1165A4` was marked with IDA's `force_bl_jump` setting. Merging the range alone was insufficient: Hex-Rays still treated the `BL` as a call until that setting was applied.

The installed IDA batch interface was used for the function-tail and processor-setting operations, which the exposed MCP tools do not provide. The MCP session was saved and closed first, then reopened afterward. A temporary pre-correction database copy was made at `/tmp/mvd-before-boundary7.i64`; it is not a tracked artifact. An initial IDAPython API spelling was corrected before the completed operation and read-back.

The epilogue and branch bytes were checked unchanged. Fresh pseudocode after reopening shows direct `return 3` statements on both error paths, without the false epilogue call or `JUMPOUT`. The database now contains 796 functions and 265 `sub_*` names, down from 797/266 because one entry was not a function. The 501-entry documented function inventory is unchanged; this correction is not counted as a newly attributed function.

Other interior-pointer and stack-lifetime artifacts remain in the large decoder decompilation. This correction does not assert that all such artifacts are resolved. The [codec-data continuation](codec-data.md) records the separate VP8 reused-pointer finding. Hardware behavior, runtime concealment-defect reachability and platform attribution remain deferred.
