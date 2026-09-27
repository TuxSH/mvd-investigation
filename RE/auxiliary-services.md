# Auxiliary services

## Registration and dispatch

The service table at `0x11A02C` registers `mvd:STD`, `l2b:u`, `l2b2:u` and `y2r2:u`, each with a maximum of one session. The main loop has four active-session slots in total. The function/context table at `0x11A04C` selects:

| Service | Dispatcher | Context |
|---|---|---|
| mvd:STD | `0x1124E8` | `0x12100C` |
| l2b:u | `0x111E64` | `0x121220` |
| l2b2:u | `0x111E64` | `0x121270` |
| y2r2:u | `0x11328C` | `0x121014` |

The L2B contexts select engines zero and one. Their input/output DMA FIFO virtual addresses derive from the table at `0x11A008`: bases `0x1EB00000` and `0x1EB01000`, plus `0x330000` for input and `0x330200` for output. Separately, register initialization at `0x110284` stores `engine << 12` as the register offset: ordinary register accesses use windows `0x1EC30000` and `0x1EC31000`. The DMA endpoints and CPU register windows must not be conflated. These are module virtual addresses, not physical I/O addresses.

Both auxiliary dispatchers select the low eight command-ID bits. Scalar cases mostly do not check the full request header. Tables below describe bytes actually consumed/returned, rather than endorsing header counts inferred from request/reply captures. In particular, a getter's result and output words belong to its **reply**, not its request. DMA requests explicitly validate four normal words plus a two-word process-handle descriptor.

## l2b:u and l2b2:u

Each operation has an implicit engine context. A scalar setter consumes one word, reading only the stated low byte/halfword. A scalar getter returns result plus a word whose low byte/halfword is written. Unwritten upper bytes should not be treated as data. Ordinary actions return one result word.

| ID | Operation | Handler | Explicit handler arguments / data |
|---|---|---|---|
| `0x01` | SetInputFormat | `0x112290` | `u8 format` |
| `0x02` | GetInputFormat | `0x11227C` | `u8 *format` |
| `0x03` | SetOutputFormat | `0x1122BA` | `u8 format` |
| `0x04` | GetOutputFormat | `0x1122A4` | `u8 *format` |
| `0x05` | SetTransferEndInterrupt | `0x1124AE` | `s8 enable` |
| `0x06` | GetTransferEndInterrupt | `0x1124A4` | `u8 *enable` |
| `0x07` | GetTransferEndEvent | `0x112444` | `Handle *event` |
| `0x08` | SetSending | `0x11216C` | `Handle process, void *address, u32 size, s16 unit, s16 gap` |
| `0x09` | IsDoneSending | `0x1123DE` | `u8 *done` |
| `0x0A` | SetReceiving | `0x112242` | `Handle process, void *address, u32 size, s16 unit, s16 gap` |
| `0x0B` | IsDoneReceiving | `0x11244C` | `u8 *done` |
| `0x0C` | SetInputLineWidth | `0x1123EE` | `u16 width` |
| `0x0D` | GetInputLineWidth | `0x1123D4` | `u16 *width` |
| `0x0E` | SetInputLines | `0x112272` | `u16 lines` |
| `0x0F` | GetInputLines | `0x112268` | `u16 *lines` |
| `0x10` | SetAlpha | `0x1124C2` | `u16 alpha` |
| `0x11` | GetAlpha | `0x1124B8` | `u16 *alpha` |
| `0x12` | StartConversion | `0x1122C8` | — |
| `0x13` | StopConversion | `0x11229A` | — |
| `0x14` | IsBusyConversion | `0x11230C` | `u8 *busy` |
| `0x15` | SetPackageParameter | `0x11245C` | `MvdL2bParams *params` |
| `0x16` | GetPackageParameter | `0x1123F8` | `MvdL2bParams *params` |
| `0x17` | PingProcess | `0x112238` | `u8 *sessions` |

`07` returns result and a shared event-handle descriptor/address. `08` and `0A` consume address, size, signed 16-bit transfer unit and gap in four separate normal words, followed by the process-handle descriptor. Receive's direct IPC wrapper is `0x112242`; it calls DMA setup at `0x112318` and closes the process handle on success. The sender closes its handle in `0x11216C` on the active setup path.

The DMA configuration uses devices 23/24 for engine zero and 25/26 for engine one. Its burst selection starts at 64 and halves until the transfer unit is divisible by that size. Source cache flush and destination invalidation precede DMA. The receive and send contexts are at offsets 12 and 40. The status commands query their DMA state; `09` is a completion query, not a general blocking wait command.

The line-width and line-count setters both require positive multiples of eight, at most 1024. The encoded register value for 1024 is zero. Alpha's low byte is written to a halfword at register offset `0x20`. The format setters clear input bits 0..1 or output bits 8..9 and OR in the supplied value (shifted for output), without a separate range-validation branch. Pixel-layout semantics for these L2B format values have not been independently established by this analysis.

`15` consumes this **eight-byte** block, spanning two words:

| Byte offset | Field |
|---|---|
| 0 | u8 input format |
| 1 | u8 output format |
| 2 | u16 line width |
| 4 | u16 line count |
| 6 | u16 alpha |

The setter applies fields sequentially and returns at the first failing operation, with no rollback. `16` fills this same block. Its reply header advertises **five normal words**, but the dispatcher writes only result plus the two data words. The remaining advertised words are not populated by that path. `17` returns the context's session-count byte, incremented during session acceptance.

## y2r2:u

The operation ordering matches libctru's Y2R client API through `2C`, with an additional `2D` getter. This is a comparison of interfaces, not evidence that Nintendo included libctru. Context arguments shown below are implicit and omitted from the table. Parameter blocks are packed as described afterwards.

| ID | Operation | Handler | Explicit handler arguments / data |
|---|---|---|---|
| `0x01` | SetInputFormat | `0x113BB8` | `u8 format` |
| `0x02` | GetInputFormat | `0x113BA0` | `u8 *format` |
| `0x03` | SetOutputFormat | `0x113BF4` | `u8 format` |
| `0x04` | GetOutputFormat | `0x113BD8` | `u8 *format` |
| `0x05` | SetRotation | `0x1138DC` | `u8 rotation` |
| `0x06` | GetRotation | `0x1138B0` | `u8 *rotation` |
| `0x07` | SetBlockAlignment | `0x113D74` | `u8 alignment` |
| `0x08` | GetBlockAlignment | `0x113D48` | `u8 *alignment` |
| `0x09` | SetSpacialDithering | `0x114044` | `s8 enable` |
| `0x0A` | GetSpacialDithering | `0x113F98` | `u8 *enable` |
| `0x0B` | SetTemporalDithering | `0x1140D4` | `s8 enable` |
| `0x0C` | GetTemporalDithering | `0x114064` | `u8 *enable` |
| `0x0D` | SetTransferEndInterrupt | `0x114104` | `s8 enable` |
| `0x0E` | GetTransferEndInterrupt | `0x1140F4` | `u8 *enable` |
| `0x0F` | GetTransferEndEvent | `0x113FA8` | `Handle *event` |
| `0x10` | SetSendingY | `0x113A70` | `Handle process, void *address, u32 size, s16 unit, s16 gap` |
| `0x11` | SetSendingU | `0x1138F0` | `Handle process, void *address, u32 size, s16 unit, s16 gap` |
| `0x12` | SetSendingV | `0x1139B0` | `Handle process, void *address, u32 size, s16 unit, s16 gap` |
| `0x13` | SetSendingYUYV | `0x113B78` | `Handle process, void *address, u32 size, s16 unit, s16 gap` |
| `0x14` | IsDoneSendingYUYV | `0x114074` | `u8 *done` |
| `0x15` | IsDoneSendingY | `0x113E8C` | `u8 *done` |
| `0x16` | IsDoneSendingU | `0x113E64` | `u8 *done` |
| `0x17` | IsDoneSendingV | `0x113E78` | `u8 *done` |
| `0x18` | SetReceiving | `0x113B30` | `Handle process, void *address, u32 size, s16 unit, s16 gap` |
| `0x19` | IsDoneReceiving | `0x113FB8` | `u8 *done` |
| `0x1A` | SetInputLineWidth | `0x113D88` | `u16 width` |
| `0x1B` | GetInputLineWidth | `0x113D64` | `u16 *width` |
| `0x1C` | SetInputLines | `0x113B68` | `u16 lines` |
| `0x1D` | GetInputLines | `0x113B58` | `u16 *lines` |
| `0x1E` | SetCoefficients | `0x114088` | `MvdY2rCoefficients coefficients` |
| `0x1F` | GetCoefficients | `0x114054` | `MvdY2rCoefficients *coefficients` |
| `0x20` | SetStandardCoefficient | `0x1140E4` | `u8 index` |
| `0x21` | GetStandardCoefficient | `0x11422C` | `MvdY2rCoefficients *coefficients, u8 index` |
| `0x22` | SetAlpha | `0x11424C` | `u16 alpha` |
| `0x23` | GetAlpha | `0x11423C` | `u16 *alpha` |
| `0x24` | SetDitheringWeightParams | `0x11418C` | `MvdY2rDitherWeights weights` |
| `0x25` | GetDitheringWeightParams | `0x114114` | `MvdY2rDitherWeights *weights` |
| `0x26` | StartConversion | `0x113C04` | — |
| `0x27` | StopConversion | `0x113BC8` | — |
| `0x28` | IsBusyConversion | `0x113C8C` | u8 output |
| `0x29` | SetConversionParams | `0x113FCC` | `MvdY2rParams *params` |
| `0x2A` | PingProcess | `0x1138CC` | `u8 *sessions` |
| `0x2B` | DriverInitialize | `0x113C68` | — |
| `0x2C` | DriverFinalize | `0x10FEF0` | — |
| `0x2D` | GetConversionParams | `0x113EA0` | `MvdY2rParams *params` |

Scalar setters/getters and event return use the same transport conventions as L2B. DMA setup commands `10..13` and `18` consume four normal words (VA, size, signed 16-bit unit, signed 16-bit gap) and a shared process handle. Coefficients are eight 16-bit values (16 bytes), passed in four words for `1E` and returned after result by `1F`/`21`. Dithering weights are sixteen 16-bit values (32 bytes), passed in eight words by `24` and returned after result by `25`.

`29` reads exactly three words holding a **12-byte** conversion-parameter structure:

| Byte offset | Field |
|---|---|
| 0, 1, 2, 3 | u8 input format, output format, rotation, block alignment |
| 4, 6 | 16-bit line width, line count |
| 8 | u8 standard-coefficient index |
| 9 | Unused byte |
| 10 | u16 alpha |

This matches the packed libctru structure; its client nevertheless advertises seven normal words in the command header. The module's scalar dispatcher does not use that count to determine how many bytes to copy here. Do not replace the packed byte fields with four C enum-sized words.

`2D` reads current settings into the same structure. It compares all eight current coefficients against each of four standard tables, returning an index 0..3 on a match and **4 when none matches**. The padding byte at offset 9 is not written. The dispatcher advertises **eight normal reply words** while explicitly writing result plus only three data words; the remaining four words are not populated by this branch.

Y2R's line width requires a positive multiple of eight up to 1024, encoded as zero for 1024. Its line-count setter is different: it rejects zero and values above 1024, leaves the register unchanged for exactly 1024, and otherwise writes the low ten bits. The signed dispatcher load and the setter's absence of a negative check are relevant for malformed values. This is the observed code, not a recommendation to pass such values. Format, rotation and alignment setters clear the corresponding register fields then OR in shifted input, without independently validating every enum value.

These auxiliary interfaces use direct platform results, not the Hantro decoder/PP result converter. Detailed silicon behavior, L2B format ordering and DMA timing remain outside what the static call mapping proves.
