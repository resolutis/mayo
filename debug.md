# Mayo Project Debugging Guide (VS Code)

This document provides comprehensive instructions for debugging the Mayo 3D CAD viewer and converter project using Visual Studio Code.

## 📋 Table of Contents

- [Prerequisites](#prerequisites)
- [VS Code Configuration Files](#vscode-configuration-files)
- [Debug Configurations](#debug-configurations)
- [How to Debug](#how-to-debug)
- [Key Debugging Points](#key-debugging-points)
- [Sample Test Files](#sample-test-files)
- [Debugging Tips](#debugging-tips)
- [Troubleshooting](#troubleshooting)

## Prerequisites

- Visual Studio Code with C/C++ extension
- CMake Tools extension (optional but recommended)
- Project built with debug symbols (`CMAKE_BUILD_TYPE=Debug`)

## VS Code Configuration Files

The following configuration files have been created in `.vscode/` directory:

### `.vscode/launch.json`

Contains debug launch configurations for both GUI and CLI applications.

### `.vscode/tasks.json`

Contains build tasks for compiling the project with debug symbols.

### `.vscode/settings.json`

Contains VS Code settings for C++ IntelliSense and CMake integration.

## Debug Configurations

### 1. Debug Mayo GUI Application

- **Target**: Main GUI application (`mayo.app`)
- **Entry Point**: `src/app/main.cpp:449`
- **Use Case**: Debug the full GUI application with window interface
- **Arguments**: None (opens empty application)

### 2. Debug Mayo CLI Application

- **Target**: CLI converter (`mayo-conv`)
- **Entry Point**: `src/cli/main.cpp:496`
- **Use Case**: Debug command-line interface with help output
- **Arguments**: `["--help"]`

### 3. Debug Mayo CLI with File Input

- **Target**: CLI converter with file processing
- **Entry Point**: `src/cli/main.cpp:496`
- **Use Case**: Debug file import/export operations
- **Arguments**:
  ```json
  [
    "${workspaceFolder}/tests/inputs/test.brep",
    "-e",
    "${workspaceFolder}/tests/outputs/test.stp"
  ]
  ```

### 4. Debug Mayo GUI with File Input

- **Target**: GUI application with file loading
- **Entry Point**: `src/app/main.cpp:449`
- **Use Case**: Debug file loading in GUI interface
- **Arguments**: `["${workspaceFolder}/tests/inputs/test.brep"]`

## How to Debug

### Method 1: VS Code Debug Panel (Recommended)

1. **Open VS Code** in the project root directory
2. **Navigate to Debug Panel**:
   - Press `Ctrl+Shift+D` (Windows/Linux) or `Cmd+Shift+D` (macOS)
   - Or click the Debug icon in the Activity Bar
3. **Select Debug Configuration**:
   - Use the dropdown at the top of the Debug panel
   - Choose from the available configurations listed above
4. **Set Breakpoints**:
   - Click in the left margin of any source file
   - Red dots indicate active breakpoints
5. **Start Debugging**:
   - Press `F5` or click the green play button
   - The debugger will start and stop at your breakpoints

### Method 2: Command Line Debugging

```bash
# Debug CLI application
cd build-mayo
lldb ./mayo-conv
(lldb) breakpoint set --file src/cli/main.cpp --line 496
(lldb) run --help

# Debug GUI application
lldb ./mayo.app/Contents/MacOS/mayo
(lldb) breakpoint set --file src/app/main.cpp --line 449
(lldb) run
```

## Key Debugging Points

### GUI Application (`src/app/main.cpp`)

| Line | Function               | Purpose                      |
| ---- | ---------------------- | ---------------------------- |
| 449  | `main()`               | Application entry point      |
| 90   | `processCommandLine()` | Parse command-line arguments |
| 311  | `runApp()`             | Main application logic       |
| 277  | `initGui()`            | Initialize GUI components    |
| 430  | `MainWindow` creation  | Create main window           |

### CLI Application (`src/cli/main.cpp`)

| Line | Function                     | Purpose                      |
| ---- | ---------------------------- | ---------------------------- |
| 496  | `main()`                     | Application entry point      |
| 174  | `processCommandLine()`       | Parse command-line arguments |
| 352  | `runApp()`                   | Main application logic       |
| 336  | `initGui()`                  | Initialize GUI for CLI       |
| 471  | `cli_asyncExportDocuments()` | File export operations       |

### Common Entry Points for Both Applications

- **Application Initialization**: Both apps initialize `AppModule`, `GuiApplication`, and I/O systems
- **Command Line Processing**: Both parse arguments using `QCommandLineParser`
- **Settings Loading**: Both load application settings from INI files or registry
- **I/O System Registration**: Both register file format readers and writers

## Sample Test Files

The project includes test files in `tests/inputs/` directory:

| File        | Format           | Description         |
| ----------- | ---------------- | ------------------- |
| `test.brep` | OpenCascade BREP | 3D CAD geometry     |
| `test.off`  | OFF              | Mesh format         |
| `test.ply`  | PLY              | Point cloud format  |
| `test.bin`  | Binary           | Generic binary file |

## Debugging Tips

### 1. Start Simple

- Begin with the CLI application (`mayo-conv`) as it's easier to debug
- Use the `--help` option to see available commands

### 2. Breakpoint Strategy

- Set breakpoints in `main()` functions first
- Add breakpoints in `processCommandLine()` to see argument parsing
- Use breakpoints in `runApp()` to trace main application flow

### 3. Navigation Commands

- **F10**: Step Over (execute current line)
- **F11**: Step Into (enter function calls)
- **Shift+F11**: Step Out (exit current function)
- **F5**: Continue execution
- **Shift+F5**: Stop debugging

### 4. Variable Inspection

- Use the **Variables** panel to inspect local and global variables
- Use the **Watch** panel to monitor specific expressions
- Use the **Call Stack** panel to see function call hierarchy

### 5. Debug Console

- Use the Debug Console to evaluate expressions
- Type variable names to see their current values
- Execute function calls to test behavior

## Build Tasks

The following build tasks are available via `Ctrl+Shift+P` → "Tasks: Run Task":

### build-mayo-debug

- **Purpose**: Build the project with debug symbols
- **Command**: `make -j8` in build directory
- **Use**: Run before debugging to ensure latest changes are compiled

### clean-mayo

- **Purpose**: Clean build directory
- **Command**: `make clean` in build directory
- **Use**: Remove all compiled objects

### rebuild-mayo

- **Purpose**: Clean and rebuild project
- **Command**: `make clean && make -j8`
- **Use**: Full rebuild when configuration changes

### cmake-configure

- **Purpose**: Configure CMake with debug settings
- **Command**: `cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..`
- **Use**: Reconfigure build system

## macOS Debug Tools and Verification

### Debug Symbol Verification Tools

When setting up debugging on macOS, several tools are used to verify that debug symbols are properly embedded in the executables:

#### 1. `file` Command

**Purpose**: Identifies the type and architecture of executable files.

```bash
# Check executable type and architecture
file mayo-conv
# Output: mayo-conv: Mach-O 64-bit executable arm64
```

**What it tells us**:

- The file is a valid Mach-O executable
- Built for 64-bit ARM64 architecture (Apple Silicon)
- Confirms the executable is properly linked

#### 2. `otool` Command

**Purpose**: Displays object file information, including UUID for debug symbol matching.

```bash
# Get the UUID of the executable (used for debug symbol matching)
otool -l mayo-conv | grep -A 5 LC_UUID
# Output:
#     cmd LC_UUID
#  cmdsize 24
#     uuid 5CCB7273-6E7A-3C06-9466-E2C435332A28
```

**What it tells us**:

- The executable has a unique identifier (UUID)
- This UUID is used to match debug symbols with the executable
- Essential for debuggers to locate the correct debug information

#### 3. `nm` Command

**Purpose**: Lists symbols from object files, including debug-related symbols.

```bash
# Check for debug-related symbols in the executable
nm mayo-conv | grep -i debug | head -5
# Output:
# 0000000100061ee8 T __ZN4Mayo17LogMessageHandler15enableDebugLogsEb
# 00000001000241f8 T __ZN6QDebug10maybeSpaceEv
# 00000001000420c4 T __ZN6QDebug7noquoteEv
```

**What it tells us**:

- The executable contains debug-related function symbols
- These are mangled C++ function names (demangled: `Mayo::LogMessageHandler::enableDebugLogs(bool)`)
- Indicates debug functionality is present in the binary

#### 4. `dsymutil` Command

**Purpose**: Creates and manages debug symbol files (.dSYM bundles) on macOS.

```bash
# Extract debug symbols into a .dSYM bundle
dsymutil mayo-conv
# Output (success): (no output means success)
# Output (no debug symbols): warning: no debug symbols in executable (-arch arm64)
```

**What it tells us**:

- **Success (no output)**: Debug symbols were successfully extracted to `mayo-conv.dSYM`
- **Warning**: No debug symbols found in the executable
- The .dSYM bundle contains all debug information needed by debuggers

#### 5. `.dSYM Bundle Verification`

**Purpose**: Verify that debug symbol bundle was created successfully.

```bash
# Check if debug symbol bundle exists
ls -la mayo-conv.dSYM
# Output:
# total 0
# drwxr-xr-x@  3 john  staff   96  7 Sep 15:12 .
# drwxr-xr-x  18 john  staff  576  7 Sep 15:13 ..
# drwxr-xr-x@  4 john  staff  128  7 Sep 15:12 Contents
```

**What it tells us**:

- The `.dSYM` bundle was created successfully
- Contains a `Contents` directory with debug information
- This bundle is what debuggers (LLDB, Xcode, VS Code) use for debugging

### Debug Symbol Workflow

Here's the complete workflow used to verify and set up debugging:

#### Step 1: Initial Build Check

```bash
# Check if executable exists and is valid
file mayo-conv
```

#### Step 2: Debug Symbol Extraction

```bash
# Try to extract debug symbols
dsymutil mayo-conv
```

#### Step 3: Rebuild with Debug Symbols (if needed)

```bash
# If no debug symbols found, rebuild with debug configuration
cd /Users/john/Developer/Kioko/mayo
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -B build-mayo
cd build-mayo
make -j8
```

#### Step 4: Verify Debug Symbols

```bash
# Extract debug symbols again
dsymutil mayo-conv
# Check that .dSYM bundle was created
ls -la mayo-conv.dSYM
```

### Understanding Debug Symbols on macOS

#### What are Debug Symbols?

Debug symbols contain information that debuggers need to:

- Map machine code back to source code lines
- Display variable names and values
- Show function call stacks
- Enable breakpoint setting by line number

#### Debug Symbol Formats on macOS

1. **Embedded Symbols**: Debug info stored within the executable (larger file size)
2. **Separate .dSYM Bundle**: Debug info in separate file (recommended for release builds)
3. **Stripped Executable**: No debug symbols (smaller file size, no debugging capability)

#### CMake Debug Build Configuration

```cmake
# Key CMake settings for debug builds
set(CMAKE_BUILD_TYPE Debug)                    # Enable debug symbols
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)          # Generate compile_commands.json
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")           # Debug flags (if not set by CMAKE_BUILD_TYPE)
```

### Debug Symbol Best Practices

#### For Development

- Always build with `CMAKE_BUILD_TYPE=Debug`
- Keep .dSYM bundles alongside executables
- Use `dsymutil` to verify debug symbols are available

#### For Release

- Strip debug symbols from release executables
- Keep .dSYM bundles for crash analysis
- Use separate debug and release build directories

#### For CI/CD

- Build debug and release versions separately
- Archive .dSYM bundles for crash symbolication
- Test debug builds in development environment

## Troubleshooting

### Debug Symbols Not Found

```bash
# Rebuild with debug symbols
cd build-mayo
make clean
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j8

# Verify debug symbols are now available
dsymutil mayo-conv
ls -la mayo-conv.dSYM
```

### Breakpoints Not Hit

- Ensure the project is built with debug symbols (`CMAKE_BUILD_TYPE=Debug`)
- Check that you're debugging the correct executable
- Verify the source file paths match the debug symbols

### VS Code IntelliSense Issues

- Reload VS Code window (`Ctrl+Shift+P` → "Developer: Reload Window")
- Check that `compile_commands.json` exists in build directory
- Ensure C/C++ extension is installed and enabled

### Application Crashes on Startup

- Check the Debug Console for error messages
- Verify all dependencies are properly linked
- Try running the application from terminal first to see error output

## Additional Resources

- [VS Code C++ Debugging Documentation](https://code.visualstudio.com/docs/cpp/cpp-debug)
- [LLDB Debugger Documentation](https://lldb.llvm.org/)
- [CMake Debug Build Documentation](https://cmake.org/cmake/help/latest/variable/CMAKE_BUILD_TYPE.html)

---

**Note**: This debugging setup is specifically configured for Visual Studio Code on macOS. For other IDEs or platforms, similar concepts apply but configuration details may differ.
