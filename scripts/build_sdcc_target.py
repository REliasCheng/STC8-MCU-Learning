"""Build and verify the STC8H8K64U UART foundation with SDCC MCS-51.

Outputs must be placed outside the repository in a new directory. The build
proves compilation, linkage and Intel HEX structure, not board operation.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import re
import shutil
import subprocess
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
SOURCES = (
    "src/core/ring_buffer.c",
    "src/core/uart_frame.c",
    "src/core/uart_frame_drain.c",
    "platform/stc8/stc8_uart.c",
    "platform/stc8/stc8_uart_sdcc.c",
    "target/sdcc/main.c",
)


def run(command: list[str], log: list[str]) -> str:
    result = subprocess.run(
        command, cwd=ROOT, text=True, stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT, check=False,
    )
    log.append("COMMAND: " + " ".join(command))
    log.append(result.stdout)
    log.append(f"EXIT: {result.returncode}")
    if result.returncode:
        raise RuntimeError(f"Command failed ({result.returncode}): {command[0]}")
    return result.stdout


def read_hex(path: Path) -> dict[int, int]:
    memory: dict[int, int] = {}
    eof = False
    for number, line in enumerate(path.read_text(encoding="ascii").splitlines(), 1):
        if eof or not line.startswith(":"):
            raise ValueError(f"Invalid Intel HEX record at line {number}")
        record = bytes.fromhex(line[1:])
        if len(record) < 5 or len(record) != record[0] + 5 or sum(record) % 256:
            raise ValueError(f"Invalid Intel HEX length/checksum at line {number}")
        address = (record[1] << 8) | record[2]
        kind = record[3]
        if kind == 0:
            for offset, value in enumerate(record[4:-1]):
                position = address + offset
                if position >= 65536 or position in memory:
                    raise ValueError(f"Overlapping/out-of-range HEX data at {position:#x}")
                memory[position] = value
        elif kind == 1 and record[0] == 0 and address == 0:
            eof = True
        else:
            raise ValueError(f"Unexpected Intel HEX record type {kind} at line {number}")
    if not eof or not memory:
        raise ValueError("Intel HEX lacks data or EOF")
    return memory


def parse_memory(path: Path) -> dict[str, int]:
    content = path.read_text(encoding="ascii")
    stack = re.search(r"Stack starts at: 0x([0-9a-f]+).*?with (\d+) bytes available", content)
    xdata = re.search(r"^\s*EXTERNAL RAM\s+0x[0-9a-f]+\s+0x[0-9a-f]+\s+(\d+)\s+(\d+)", content, re.M)
    code = re.search(r"^\s*ROM/EPROM/FLASH\s+0x[0-9a-f]+\s+0x[0-9a-f]+\s+(\d+)\s+(\d+)", content, re.M)
    if not stack or not xdata or not code:
        raise ValueError("SDCC memory report could not be parsed")
    values = {
        "stack_start": int(stack[1], 16),
        "stack_available_bytes": int(stack[2]),
        "xdata_bytes": int(xdata[1]),
        "xdata_limit_bytes": int(xdata[2]),
        "code_bytes": int(code[1]),
        "code_limit_bytes": int(code[2]),
    }
    if values["xdata_bytes"] > values["xdata_limit_bytes"] or values["code_bytes"] > values["code_limit_bytes"]:
        raise ValueError("Memory report exceeds configured target limits")
    return values


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--sdcc", default="sdcc", help="SDCC executable path")
    parser.add_argument("--output", required=True, type=Path, help="new output directory outside this repository")
    args = parser.parse_args()

    sdcc_name = shutil.which(args.sdcc)
    if not sdcc_name:
        parser.error("SDCC executable not found")
    sdcc = Path(sdcc_name).resolve()
    packihx = sdcc.with_name("packihx.exe" if sdcc.suffix.lower() == ".exe" else "packihx")
    if not packihx.is_file():
        found = shutil.which("packihx")
        if not found:
            parser.error("packihx executable not found")
        packihx = Path(found).resolve()

    output = args.output.resolve()
    if output == ROOT or ROOT in output.parents:
        parser.error("Output must be outside the repository")
    if output.exists():
        parser.error("Output directory already exists; use a fresh directory")
    output.mkdir(parents=True)
    log: list[str] = []
    try:
        version = run([str(sdcc), "-v"], log).strip()
        if "mcs51" not in version.lower():
            raise RuntimeError("This SDCC build does not advertise the MCS-51 backend")
        objects: list[str] = []
        for source in SOURCES:
            object_path = output / (Path(source).stem + ".rel")
            run([
                str(sdcc), "-mmcs51", "--std-c89", "--model-small",
                "--stack-auto", "--Werror", "-Iinclude", "-Iplatform/stc8",
                "-c", source, "-o", str(object_path),
            ], log)
            objects.append(str(object_path))
        ihx = output / "stc8_uart_foundation.ihx"
        run([
            str(sdcc), "-mmcs51", "--model-small", "--stack-auto",
            "--iram-size", "256", "--xram-size", "8192",
            "--code-size", "65536", "--out-fmt-ihx", "-o", str(ihx),
            *objects,
        ], log)

        packed = subprocess.run(
            [str(packihx), str(ihx)], cwd=ROOT, stdout=subprocess.PIPE,
            stderr=subprocess.PIPE, check=False,
        )
        log.extend(["COMMAND: " + str(packihx) + " " + str(ihx),
                    packed.stderr.decode("utf-8", errors="replace"),
                    f"EXIT: {packed.returncode}"])
        if packed.returncode:
            raise RuntimeError("packihx failed")
        hex_path = output / "stc8_uart_foundation.hex"
        hex_path.write_bytes(packed.stdout)
        ihx_data = read_hex(ihx)
        hex_data = read_hex(hex_path)
        if ihx_data != hex_data:
            raise ValueError("Packed HEX differs from linked IHX")

        map_text = (output / "stc8_uart_foundation.map").read_text(encoding="ascii")
        symbol = re.search(r"C:\s+([0-9A-Fa-f]{8})\s+_stc8_uart1_isr\b", map_text)
        if not symbol:
            raise ValueError("UART1 ISR missing from linked map")
        isr_address = int(symbol[1], 16)
        vector_address = (hex_data.get(0x24, -1) << 8) | hex_data.get(0x25, -1)
        if hex_data.get(0x23) != 0x02 or vector_address != isr_address:
            raise ValueError("UART1 interrupt vector does not jump to linked ISR")

        diagnostics = "\n".join(log)
        if re.search(r"(?im)^.*\b(?:warning|error)\s*[:0-9]", diagnostics):
            raise ValueError("Compiler/linker emitted a warning or error; inspect build.log")

        report = {
            "target": "STC8H8K64U register adapter on SDCC MCS-51",
            "board_configuration_verified": False,
            "compiler": version,
            "sources": list(SOURCES),
            "memory_model": "small, stack-auto; 256-byte IRAM, 8192-byte XDATA, 65536-byte CODE limits",
            "memory": parse_memory(output / "stc8_uart_foundation.mem"),
            "uart1_vector": f"0x23 -> 0x{isr_address:04X}",
            "hex_size_bytes": hex_path.stat().st_size,
            "hex_sha256": hashlib.sha256(hex_path.read_bytes()).hexdigest(),
            "compile_link_warnings": 0,
            "compile_link_errors": 0,
        }
        (output / "build-report.json").write_text(
            json.dumps(report, indent=2) + "\n", encoding="utf-8",
        )
        print(json.dumps(report, indent=2))
    finally:
        (output / "build.log").write_text("\n".join(log) + "\n", encoding="utf-8")


if __name__ == "__main__":
    main()
