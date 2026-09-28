# H.264 MVC enable state and interior pointers

## Resolved storage word

`MvdH264Storage+0x39DC`, formerly `unresolvedWord3703`, is now named **`mvcEnabled`**. This is a descriptive name for the MVC request/NAL-acceptance flag in this branch. The identification comes from a writer and a parser consumer, not from its position beside an existing MVC field.

| Address | Function | Evidence |
|---|---|---|
| `0x10451C` | `H264DecSetMvc` (`0x1044E0`) | Writes 1 after instance validation and the MVC capability check |
| `0x115D4C` | `h264bsdDecode` (`0x115C28`) | Reads the word through a base at storage `+0x39C0`, seven words later |
| `0x11660C` | `h264bsdDecode`, prefix NAL case | Copies this flag into the distinct word at storage `+0x39E0` |

The local reference `source/h264high/h264decapi.c`, function `H264DecSetMvc`, has the same validation/capability-check sequence followed by `pDecCont->storage.mvc = HANTRO_TRUE`. Its `h264hwd_decoder.c` uses `storage.mvc` in the corresponding NAL gate. These two matches establish the role of the MVD word.

The parser discards NAL type 0. For types at least 13, it only proceeds when `mvcEnabled` is nonzero **and** the type is 14, 15 or 20. Those types are prefix, subset SPS and coded-slice extension respectively. This identifies a software parsing gate; it does not establish successful MVC picture decoding.

## Two adjacent flags, not a layout shift

The neighboring word already named `mvc` is distinct. Prefix-NAL handling sets `view` to zero, copies `mvcEnabled` into `mvc`, then stores `!nal.interViewFlag` in `nonInterViewRef`. The current evidence supports retaining both fields and the following offsets:

| Storage offset | Member | Meaning established here |
|---|---|---|
| `0x39D8` | `currentMarked` | Existing picture-marking state |
| `0x39DC` | `mvcEnabled` | API request flag and MVC NAL acceptance gate |
| `0x39E0` | `mvc` | Separate state word receiving the enable flag on a prefix NAL |
| `0x39E4` | `view` | Current view; set to zero for the prefix path |
| `0x39E8` | `viewId[2]` | Existing two-view identifiers |
| `0x39F0` | `outView` | Existing output-view selector |
| `0x39F4` | `numViews` | Existing view-count state |
| `0x39F8` | `baseOppositeFieldPic` | Existing base-view field state |
| `0x39FC` | `nonInterViewRef` | Receives the inverse of the prefix's inter-view flag |

The supplied source has one `mvc` member between `currentMarked` and `view`; MVD has two words in that interval. Importing that source tail without accounting for this difference would move `view` and every later member to the wrong offset. The full lifecycle of the `+0x39E0` word remains to be traced; this pass does not rename it to a stronger interpretation such as “MVC picture active.” Storage/container sizes remain **14864/15860 bytes**.

## Service consequence

[`mvd:STD` command `0x06`](mvd-std.md) calls `H264DecSetMvc` through `MVDSTD_H264EnableMvc` (`0x112CA8`). The library function returns `H264DEC_FORMAT_NOT_SUPPORTED` before the new flag write if `mvcSupport` is zero. [The capability reader](hardware.md) clears that capability unconditionally in this build. Consequently, identifying the dormant flag and parser paths does not change the documented result of the normal enable command.

No register dump was treated as live state, and no capability checks or firmware instructions were patched.

## Database readability and method

Earlier member-xref and bounded listing searches missed the read because the compiler forms an interior pointer to `storage.pictureBroken`, then loads at displacement `0x1C`. The absence of a named-member reference was therefore insufficient evidence that the word lacked consumers. Following `H264DecSetMvc` and the source-assisted parser gate resolved the alias.

Three locals in `h264bsdDecode` now have descriptive base names:

| Local | Storage-relative base | Fresh pseudocode |
|---|---|---|
| `mvcStateBase` | `0x39C0` | `u32 *__shifted(MvdH264Storage,0x39C0)`; named MVC/view fields through `ADJ` |
| `streamStateBase` | `0x3500` | `u32 *__shifted(MvdH264Storage,0x3500)`; named previous-buffer flag, pointer and byte count |
| `pictureStateBase` | `0x1F80` | Still a plain `s32 *`; `[6]` is `aub.newPicture`, `[7]` is `currImage.data` |

The first two shifted types were applied through IDAPython pointer-type metadata after the MCP local-type parser rejected the shifted declarations. The third rename persists, but attempted shifted typing did not survive into regenerated pseudocode; its indexed expressions remain annotated rather than being presented as fixed. Other merged lifetimes and SPS/PPS stack overlays also remain in this large function.

The database was saved before switching between MCP and installed batch IDA, and reopened for read-back. The final member name, API assignment and parser uses were checked, along with unchanged parent sizes. A temporary pre-edit recovery copy is `/tmp/mvd-before-alias8.i64`, outside the tracked output. The same pass repairs the API decoder's loop branch and default switch target, documented in [control-flow-corrections.md](control-flow-corrections.md). Function attribution counts remain 501 documented entries, 796 database functions and 265 `sub_*` names.
