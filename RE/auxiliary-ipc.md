# Auxiliary IPC transport matrix

This supplements [auxiliary-services.md](auxiliary-services.md) with per-command transport sizes. The two L2B services share all 23 cases; Y2R2 has 45 cases. `Req N/T` gives consumed normal words and translated words for a conventional request; **only DMA rows enforce the complete header and descriptor**. Other rows describe bytes read, not accepted-header constraints. `Reply N/T` is the header actually emitted. Output byte counts exclude the four-byte result and the header. Scalar outputs are only partially written within their reply word.

For DMA rows the exact header is `(id << 16) | 0x102`, word 5 must be zero (one shared process handle), and word 6 holds that handle. Normal words 1..4 contain address, size, signed low-halfword unit and signed low-halfword gap. Event replies emit a zero shared-handle descriptor followed by one event handle.

Unknown IDs return header `0x40` and result `0xD900182F`; malformed DMA requests return header `0x40` and `0xD9001830`. Scalar dispatch selects `(header >> 16) & 0xFF`, ignoring the high command-ID byte and transport counts. Thus non-DMA command aliases can reach a case, while DMA aliases fail exact-header validation.

## l2b:u / l2b2:u

| ID | Operation | Req N/T | Reply N/T | Written output |
|---|---|---|---|---|
| `01` | SetInputFormat | 1/0 | 1/0 | — |
| `02` | GetInputFormat | 0/0 | 2/0 | 1 byte |
| `03` | SetOutputFormat | 1/0 | 1/0 | — |
| `04` | GetOutputFormat | 0/0 | 2/0 | 1 byte |
| `05` | SetTransferEndInterrupt | 1/0 | 1/0 | — |
| `06` | GetTransferEndInterrupt | 0/0 | 2/0 | 1 byte |
| `07` | GetTransferEndEvent | 0/0 | 1/2 | shared event handle |
| `08` | SetSending | 4/2 (exact) | 1/0 | — |
| `09` | IsDoneSending | 0/0 | 2/0 | 1 byte |
| `0A` | SetReceiving | 4/2 (exact) | 1/0 | — |
| `0B` | IsDoneReceiving | 0/0 | 2/0 | 1 byte |
| `0C` | SetInputLineWidth | 1/0 | 1/0 | — |
| `0D` | GetInputLineWidth | 0/0 | 2/0 | 2 bytes |
| `0E` | SetInputLines | 1/0 | 1/0 | — |
| `0F` | GetInputLines | 0/0 | 2/0 | 2 bytes |
| `10` | SetAlpha | 1/0 | 1/0 | — |
| `11` | GetAlpha | 0/0 | 2/0 | 2 bytes |
| `12` | StartConversion | 0/0 | 1/0 | — |
| `13` | StopConversion | 0/0 | 1/0 | — |
| `14` | IsBusyConversion | 0/0 | 2/0 | 1 byte |
| `15` | SetPackageParameter | 2/0 | 1/0 | — |
| `16` | GetPackageParameter | 0/0 | 5/0 | 8 bytes; 2 advertised words unwritten |
| `17` | PingProcess | 0/0 | 2/0 | 1 byte |

## y2r2:u

| ID | Operation | Req N/T | Reply N/T | Written output |
|---|---|---|---|---|
| `01` | SetInputFormat | 1/0 | 1/0 | — |
| `02` | GetInputFormat | 0/0 | 2/0 | 1 byte |
| `03` | SetOutputFormat | 1/0 | 1/0 | — |
| `04` | GetOutputFormat | 0/0 | 2/0 | 1 byte |
| `05` | SetRotation | 1/0 | 1/0 | — |
| `06` | GetRotation | 0/0 | 2/0 | 1 byte |
| `07` | SetBlockAlignment | 1/0 | 1/0 | — |
| `08` | GetBlockAlignment | 0/0 | 2/0 | 1 byte |
| `09` | SetSpacialDithering | 1/0 | 1/0 | — |
| `0A` | GetSpacialDithering | 0/0 | 2/0 | 1 byte |
| `0B` | SetTemporalDithering | 1/0 | 1/0 | — |
| `0C` | GetTemporalDithering | 0/0 | 2/0 | 1 byte |
| `0D` | SetTransferEndInterrupt | 1/0 | 1/0 | — |
| `0E` | GetTransferEndInterrupt | 0/0 | 2/0 | 1 byte |
| `0F` | GetTransferEndEvent | 0/0 | 1/2 | shared event handle |
| `10` | SetSendingY | 4/2 (exact) | 1/0 | — |
| `11` | SetSendingU | 4/2 (exact) | 1/0 | — |
| `12` | SetSendingV | 4/2 (exact) | 1/0 | — |
| `13` | SetSendingYUYV | 4/2 (exact) | 1/0 | — |
| `14` | IsDoneSendingYUYV | 0/0 | 2/0 | 1 byte |
| `15` | IsDoneSendingY | 0/0 | 2/0 | 1 byte |
| `16` | IsDoneSendingU | 0/0 | 2/0 | 1 byte |
| `17` | IsDoneSendingV | 0/0 | 2/0 | 1 byte |
| `18` | SetReceiving | 4/2 (exact) | 1/0 | — |
| `19` | IsDoneReceiving | 0/0 | 2/0 | 1 byte |
| `1A` | SetInputLineWidth | 1/0 | 1/0 | — |
| `1B` | GetInputLineWidth | 0/0 | 2/0 | 2 bytes |
| `1C` | SetInputLines | 1/0 | 1/0 | — |
| `1D` | GetInputLines | 0/0 | 2/0 | 2 bytes |
| `1E` | SetCoefficients | 4/0 | 1/0 | — |
| `1F` | GetCoefficients | 0/0 | 5/0 | 16 bytes |
| `20` | SetStandardCoefficient | 1/0 | 1/0 | — |
| `21` | GetStandardCoefficient | 1/0 | 5/0 | 16 bytes |
| `22` | SetAlpha | 1/0 | 1/0 | — |
| `23` | GetAlpha | 0/0 | 2/0 | 2 bytes |
| `24` | SetDitheringWeightParams | 8/0 | 1/0 | — |
| `25` | GetDitheringWeightParams | 0/0 | 9/0 | 32 bytes |
| `26` | StartConversion | 0/0 | 1/0 | — |
| `27` | StopConversion | 0/0 | 1/0 | — |
| `28` | IsBusyConversion | 0/0 | 2/0 | 1 byte |
| `29` | SetConversionParams | 3/0 | 1/0 | — |
| `2A` | PingProcess | 0/0 | 2/0 | 1 byte |
| `2B` | DriverInitialize | 0/0 | 1/0 | — |
| `2C` | DriverFinalize | 0/0 | 1/0 | — |
| `2D` | GetConversionParams | 0/0 | 8/0 | 12 bytes; 4 advertised words unwritten |

## Output initialization limits

The byte and halfword getter paths write only one or two bytes at word 2. They do not clear the unused bytes. L2B `16` and Y2R2 `2D` advertise more reply words than they fill. Y2R2 `2D` also copies the unwritten padding byte at offset 9 in its 12-byte parameter scratch record. These are static stores, not a live capture of the bytes delivered by the kernel.

Y2R2 `21` always copies the 16-byte coefficient scratch record into the reply after calling the getter, including when an index of 4 or greater makes that getter return `0xE0E053ED` without writing the record. Consequently those output bytes are undefined on that error path. Event scratch handles, in contrast, are initialized to zero before their getters.

The exposed command matrices and driver semantics are derived from this binary. Matching libctru command ordering does not establish that libctru code is linked.
