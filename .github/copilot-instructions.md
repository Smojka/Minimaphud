# Copilot Instructions for Minimaphud

## Project Overview

Minimaphud is a minimap HUD (Heads-Up Display) project. It is a C/C++ based repository.

## Code Standards

### Language & Style
- This project uses C/C++. Follow standard C/C++ best practices and idiomatic patterns.
- Keep code readable, well-structured, and maintainable.
- Use descriptive variable and function names.

### Development Flow
- Build: use the project's build system (e.g., `make` or `cmake`) when available.
- Test: run tests if a test suite is available.
- Always validate changes compile without errors or warnings before submitting.

## Repository Structure
- Source files belong in appropriate subdirectories (e.g., `src/`).
- Header files belong in `include/` or alongside their source files.
- Keep the root directory clean — only configuration and top-level files should live there.

## Key Guidelines
1. Write clean, well-commented code where logic is non-obvious.
2. Prefer existing libraries and utilities over reinventing the wheel.
3. Avoid introducing memory leaks — manage resources carefully in C/C++.
4. When adding new features, update the `README.md` to reflect those changes.
5. Do not commit build artifacts (`.o`, `.a`, `.so`, `.exe`, etc.) — these are covered by `.gitignore`.
