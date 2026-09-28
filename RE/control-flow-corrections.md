# H.264 control-flow corrections

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

## API decode loop and switch default

A subsequent audit found another Thumb `BL` used as a long jump, this time entirely inside `H264DecDecode` (`0x102F2C`):

```asm
00103A3E  LDR  R0, [SP, ...length]
00103A40  CMP  R0, #0
00103A42  BEQ  loc_103A48
00103A44  BL   loc_10309E
```

`0x10309E` is the loop head: it clears `readBytes`, checks decoder state, calls `h264bsdDecode` when appropriate, and updates stream position and remaining length. It has no independent function prologue. The displacement from the Thumb PC base is -2474 bytes, beyond the short unconditional branch range. The source counterpart in `h264decapi.c` has a `do { ... } while(strmLen)` covering exactly this work. If length reaches zero, execution instead proceeds at `0x103A48` to populate the API output.

Marking `0x103A44` with `force_bl_jump` removes the false `loc_10309E` function call. Fresh pseudocode now tests `length`, exits for zero and continues the enclosing loop otherwise. The instruction bytes were checked unchanged; no function boundary or count changes were needed.

That refreshed output exposed a second issue in the same function: the default switch target at `0x1032D0` was a two-byte data item, producing `JUMPOUT(0x1032D0)`. Its bytes `E2 E3` encode Thumb `B 0x103A98`. This sits beside the other case-branch stubs and is the target for cases 0, 3, 9 and default in the existing switch metadata. The two bytes were reclassified as an instruction, without patching them.

The restored default path subtracts `readBytes` from `hwLength`, updates the stream bus/CPU pointers, then reaches the same remaining-length test. Source cases `H264BSD_RDY` and `H264BSD_ERROR` use this common stream-accounting path; the skipped-picture case sets its return value before joining it. Fresh pseudocode shows this accounting in the default branch, with no `JUMPOUT` there. An internal `H264BSD_ERROR` is therefore not itself an immediate API error return at this switch; the surrounding decode loop and final result handling matter.

### Audit scope

The audit visited the 796 current function entries and examined call-instruction code references back into the same function. There were 24 unique instruction sites: 20 switch-helper sites with intra-function case references, two recursive `ProcessTreeNode` calls, one recursive call in deferred `sub_100D76`, and the misclassified loop branch above. The genuine calls were left unchanged. A separate check found no remaining `sub_*` entry with an existing incoming jump/fallthrough reference. These bounded metadata checks do not prove that every function boundary, unclassified byte or control-flow edge is correct.

Current totals remain 796 functions, 265 `sub_*` names and 501 documented names/prototypes. The simultaneous [MVC state pass](h264-mvc-state.md) improves two interior-pointer types and resolves storage `+0x39DC`; other stack-lifetime artifacts remain.

## Auxiliary-driver continuation

The [driver pass](auxiliary-drivers.md) repairs two additional analysis errors. Bytes `53 00 00 EF 1E FF 2F E1` at `0x10B97C` were wrongly decoded as Thumb; ARM interpretation yields `SVC 0x53; BX LR`, now named `svcStoreProcessDataCache`. This removes a false edge into VP8 code and corrects the sender cache operation from flush to clean. Bytes `02 23` at `0x111FAE` are a Thumb `MOVS R3,#2` at L2B command `0x0A`, previously marked as data. Code restoration plus the switch reference from `0x111E7A` and case reanalysis restores the complete receive path in Hex-Rays. No bytes or function counts changed.
