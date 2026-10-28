# STC8H8K64U SDCC Target Build

This repository builds an original UART1 receive foundation for the STC8H8K64U with the open-source SDCC MCS-51 toolchain. The evidence covers translation, linkage, an interrupt vector, memory report, valid Intel HEX, and two identical fresh builds. It does not establish a working board configuration or hardware behavior.

## Toolchain and source boundary

- Local verification used the official [SDCC Windows snapshot](https://sourceforge.net/projects/sdcc/files/snapshot_builds/x86_64-w64-mingw32/sdcc-snapshot-x86_64-w64-mingw32-20261026-17092-cf-x86.zip/download), `SDCC 4.6.4 #17092 (MINGW64)`, MCS-51 backend. The downloaded ZIP SHA-256 was `88b5b308662517e2d9643b1103d9f5d748b44a934c75ba6f61d8ee0c3e965efd`, matching the publisher's listing. SDCC is GPL licensed; its compiler, runtime and startup files are external build dependencies, not distributed by this repository.
- CI installs the Ubuntu 24.04 `sdcc` package, then runs the same build script twice in fresh temporary directories. CI's actual compiler version and results are recorded in its job log.
- The six source files are `src/core/ring_buffer.c`, `src/core/uart_frame.c`, `src/core/uart_frame_drain.c`, `platform/stc8/stc8_uart.c`, `platform/stc8/stc8_uart_sdcc.c`, and `target/sdcc/main.c`. The SDCC adapter and application are repository-authored. The small `platform/stc8/stc8h8k64u_sdcc.h` declares only registers and bits used by this application from the [STC8H technical reference](https://www.stcmicro.com/datasheet/STC8H-en.pdf); it is not a copied vendor device header. No course code, proprietary startup, RTX51, Keil binary, or generated firmware is committed.
- The existing `stc8_uart_c51.c` is a separate Keil path and is excluded from the SDCC build. [Its Keil gate](keil-target-build.md) remains unverified.

## Rebuild

Use a fresh output directory outside the repository for every run:

```powershell
py -3.10 scripts/build_sdcc_target.py --sdcc C:\path\to\sdcc.exe --output C:\path\to\fresh-build-1
py -3.10 scripts/build_sdcc_target.py --sdcc C:\path\to\sdcc.exe --output C:\path\to\fresh-build-2
```

On Linux, use `python3` and `--sdcc sdcc`. Compare the two `stc8_uart_foundation.hex` files byte for byte or by SHA-256. The script refuses an existing output directory and any output under this repository. Each output contains the generated HEX and IHX, compiler artifacts, `build.log`, and `build-report.json`; keep these outside the source tree. A successful run checks the Intel HEX record checksums and the UART1 vector at `0x23` against the linked ISR address in the map file.

All translation units use `-mmcs51 --std-c89 --model-small --stack-auto --Werror`; linking uses 256 bytes IRAM, 8192 bytes XDATA and 65536 bytes CODE. `--stack-auto` is applied consistently to support calls from the interrupt context; see the [SDCC manual](https://sdcc.sourceforge.net/doc/sdccman.pdf). The build uses SDCC's own startup and runtime. The script does not claim that available stack space equals measured worst-case stack consumption.

## Local result, 2026-10-28

| Check | Result |
| --- | --- |
| SDCC MCS-51 compile and link | PASS, six source files, 0 warnings, 0 errors |
| Intel HEX validation | PASS, 11,536 bytes, SHA-256 `5c90447ddb8d64b7dfa9d3e47c018b57d8e54e1182187c2c85e655c90d06ff54` |
| UART1 vector | `0x23` jumps to linked ISR at `0x0DEB` |
| CODE | 4,069 of 65,536 bytes |
| XDATA | 138 of 8,192 bytes |
| Stack | Starts at `0x21`; 223 address bytes available per linker report, not a measured high-water mark |
| Clean rebuild | PASS, two separate new output directories produced byte-identical HEX |
| Host tests | PASS, 26 ring-buffer, 10 frame-collector, 12 adapter tests (48 total) with GCC C89 strict warnings |

## Configuration assumptions and next evidence

The minimal application assumes a 24 MHz system clock, 9,600 baud UART1, and the chip route P3.0/P3.1. The adapter's Timer2 1T formula yields `24,000,000 / 4 / 9,600 = 625` ticks, reload `0xFD8F`, and an arithmetic baud of 9,600. This calculation does not measure the oscillator or serial waveform. The actual board, package, clock setup, pin routing, electrical connection, stack high-water mark, ISR latency, and time spent with `ES=0` during frame draining have not been verified. The counters in `main.c` are diagnostic state only; there is no command parser, transmit path, flashing result, UART log, or hardware runtime evidence.

Before claiming board support, identify a specific board and package, obtain its schematic and clock configuration, confirm UART route wiring and pin modes, measure baud/interrupt timing and stack use, flash a known build, and retain a reproducible receive/overflow/recovery log. The [STC8H8K64U product page](https://www.stcmicro.com/stc/stc8h8k64u.html) establishes device capacities, not this board's configuration.
