# Assembler (C) — Pre-Assembler + Two-Pass Assembler (MAMAN 23)

This project implements an **assembler** for the custom assembly language defined in **MAMAN 23** (Open University 2023A).
It is written in **C** and built for **Ubuntu/Linux** using **gcc** (ANSI C, pedantic, Wall).

The assembler pipeline includes:
1. **Pre-Assembler** stage: expands macros and generates an expanded source file (`.am`)
2. **Assembler** stage (Two Passes): builds symbol tables, encodes instructions/data, and generates output files:
   - **Object file** (`.ob`)
   - **Entries file** (`.ent`) — only if `.entry` exists
   - **Externals file** (`.ext`) — only if `.extern` exists

---

## Demo Screenshot

![Demo / Run Example](docs/images/demo.png)

---

## Key Features

### ✅ Pre-Assembler (Macro Expansion)
- Supports macro definitions:
  - `mcr <name>`
  - `endmcr`
- Supports macro calls by writing the macro name alone in a line
- Produces expanded output file: `filename.am`
- Assumptions (per assignment):
  - No nested macros
  - Macro names cannot be instruction/directive/register names
  - Each `mcr` has a matching `endmcr`

### ✅ Two-Pass Assembler
- **Pass 1**
  - Parses lines, builds **symbol table**
  - Computes **IC/DC**
  - Encodes:
    - first word of each instruction (opcode + addressing)
    - data image from `.data` / `.string`
- **Pass 2**
  - Completes encoding using resolved symbol addresses
  - Marks `.entry` symbols
  - Collects `.extern` references for `.ext`

### ✅ Outputs
For every `file.as`, the assembler may generate:

| Output | Meaning |
|---|---|
| `file.am` | Expanded source after macro expansion |
| `file.ob` | Machine code (instructions + data), with custom base-2 encoding |
| `file.ent` | Entry symbols and their addresses (only if `.entry` exists) |
| `file.ext` | External symbols usage addresses (only if `.extern` exists) |

### ✅ Error Handling
- Detects and reports syntax/semantic errors (does not stop on first error)
- Prints errors to **stdout** with **line numbers**
- If any errors exist → output files are not produced (or are not considered valid)

---

## Assembly Language Summary (MAMAN 23)

### Directives
- `.data n1, n2, ...`  (integers, comma-separated)
- `.string "text"`     (ASCII chars + terminating `\0`)
- `.entry LABEL`
- `.extern LABEL`

### Instructions (Opcodes)
`mov, cmp, add, sub, not, clr, lea, inc, dec, jmp, bne, red, prn, jsr, rts, stop`

### Addressing Modes
| Mode | Name | Example |
|---:|---|---|
| 0 | Immediate | `#-1` |
| 1 | Direct (Label) | `LABEL` |
| 2 | Jump with params (jmp/bne/jsr only) | `jmp L1(#5,N)` |
| 3 | Register direct | `r3` |

---

## Machine Code Encoding Notes

- Word size: **14 bits**
- Memory addresses: **0–255**, but output starts at address **100** (decimal)
- Uses a **special base-2 text encoding**:
  - `0` is represented by `.` (dot)
  - `1` is represented by `/` (slash)

---

## Project Structure (Recommended)

> Your repo structure may differ — adjust this section to match your folders/files.

Example layout:
