## saturn-texture-demo
Test project to figure out how out how to read textures into the saturn environment.

## Setup
This project depends on https://github.com/robertoduarte/yaul-win64 specifically version yaul-0.3.1-win64-20240125. Checkout this project dir such that it sits side-by-side with yaul-0.3.1-win64-20240125.

```
  some directory
    |
    +- ...
     - saturn-texture-demo
     - yaul-0.3.1-win64-20240125
     - ...
```

## Building

### Windows
Open a `cmd` shell in and run `yaul-0.3.1-win64-20240125\scripts\setenv.bat`. Then start VS Code using `code` on the samd `cmd` shell. (This will inherited the Yaul environment in VS Code). Once inside VS Code you can open the `texture_test.code-workspace`.

**To build the Windows project:**
```
shift+ctrl+p 
Tasks: Run build task
TextureTest - Debug
```

**To build the Saturn project:**
```
[using VS Code's terminal]\saturn-texture-demo> make
```
