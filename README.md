# 🌍 CG Project – The World of the Four Elements
> An interactive 2D scene built with **C++ | OpenGL | GLUT**

---

## 📖 Overview

A graphical computing project where the princes of the four elements — **Fire, Water, Earth, and Air** — come together to restore color to the world. The project features animated scenes with particles, special effects, and dynamic backgrounds.

---

## 💻 System Requirements

| OS | Support |
|---|---|
| Windows 10/11 | ✅ Fully supported |
| macOS 10.14+ | ✅ Built-in OpenGL/GLUT |
| Linux (Ubuntu/Debian) | ✅ via FreeGLUT |

**Required Libraries:**
- OpenGL
- GLUT / FreeGLUT
- GLEW (Windows/Linux)
- C++11 compiler or later (GCC, MSVC, or Clang)

---

## 🔧 Build Instructions

### Windows – Visual Studio

1. Download FreeGLUT from https://freeglut.sourceforge.net
2. Copy include files → `C:\Program Files\freeglut\include`
3. Copy lib files → `C:\Program Files\freeglut\lib`
4. Copy `freeglut.dll` to the project folder
5. In Visual Studio: **New Project → Empty C++ Project** → Add `main.cpp`
6. Set in Project Properties:
   - `C/C++ → Additional Include Directories` → FreeGLUT include path
   - `Linker → Additional Library Directories` → lib path
   - `Linker → Input → Additional Dependencies`:
     ```
     freeglut.lib opengl32.lib glu32.lib
     ```
7. Edit image paths in the code:
   ```cpp
   char roadpath[] = "C:/Users/YourName/cg/stone.BMP";
   char brickpath[] = "C:/Users/YourName/cg/red brick.bmp";
   ```
8. Build: `Ctrl + Shift + B` → Run: `F5`

### Windows – MinGW/G++

```bash
g++ main.cpp -o project.exe -lfreeglut -lopengl32 -lglu32 -lglew32
project.exe
```

### macOS

```bash
g++ main.cpp -o project -framework OpenGL -framework GLUT
./project
```

### Linux (Ubuntu/Debian)

```bash
sudo apt-get update
sudo apt-get install freeglut3-dev libgl1-mesa-dev libglu1-mesa-dev libglew-dev

g++ main.cpp -o project -lGL -lGLU -lglut -lGLEW
./project
```

---

## 🎮 Controls

| Key | Action |
|---|---|
| `SPACE` | Skip intro → go to element scenes |
| `1` | Return to opening scene |
| `2` | Jump to Scene 2 (Elements) |
| `3` | Jump to Scene 3 (Realization) |
| `4` | Jump to Scene 4 (Union & Restoration) |
| `7` / `Ctrl+G` | Exit program |

---

## 🎬 Scene Descriptions

### Scene 1 – Introduction
Text slowly reveals the story of the four elements. Character icons are shown. Press `SPACE` to continue.

### Scene 2 – Elements Phase
Each element appears with its unique visual effect:
- 🔥 Fire → flame particles
- 💧 Water → rain effect
- 🌿 Earth → growing plants
- 💨 Air → wind rings

Each element tries to restore color alone — and fails.

### Scene 3 – Realization
All four characters meet. A dialog box shows their conversation. Auto-transitions after **8 seconds**.

### Scene 4 – Union & Restoration
The four unite and restore color to the world. Colored rays burst outward with a glowing light circle at the center.

---

## 🗂️ Code Structure

| Function | Description |
|---|---|
| `DrawFirePrinceAt()` | Fire Prince with flame crown |
| `DrawWaterCharacterAt()` | Water Prince with buoyancy effect |
| `DrawEarthCharacterAt()` | Earth Prince character |
| `DrawAirCharacterAt()` | Air Prince character |
| `DrawFireEffect / DrawRainEffect / ...` | Element visual effects |
| `DrawScene3Realization()` | Full realization scene |
| `DrawScene4UnionRestoration()` | Full union scene |
| `display()` | Main render function (every frame) |
| `timer()` | Timer — triggers every 16ms (60fps) |
| `keyboard()` | Keyboard input handler |
| `LoadTexture() / LoadTextureMod()` | Load BMP textures |
| `init()` | OpenGL, texture, and particle setup |

---

## ✨ Particle System

80 particles simulate flames around the Fire Prince's head.

| Function | Description |
|---|---|
| `resetParticle()` | Spawns particle at head edge |
| `updateParticle()` | Updates position and lifetime |
| `drawParticlesmini()` | Renders particles (yellow → red) |

---

## 🖼️ Textures

Both BMP files must be exactly **512×512 pixels** (raw BMP, no header read).

| Variable | File | Usage |
|---|---|---|
| `textureprojectroad` | `stone.BMP` | Road texture (`GL_REPLACE`) |
| `textureprojectbrick` | `red brick.bmp` | Castle roof texture (`GL_MODULATE`) |

---

## 🛠️ Troubleshooting

| Problem | Solution |
|---|---|
| Can't open image file | Update paths of `stone.BMP` and `red brick.bmp` in code |
| Program closes immediately | Ensure `freeglut.dll` is in the same folder as the `.exe` |
| Black screen | Update GPU drivers; OpenGL 2.0+ required |
| Linker error | Add `opengl32.lib`, `glu32.lib`, `freeglut.lib` to Linker |
| Slow performance | Designed for 60fps — check VSync settings |

---

## 📝 Additional Notes

- Coordinate system: X from `-1.0` to `+1.0`, Y from `-1.0` to `+1.25`
- All characters are drawn with custom functions — no external 3D models
- Transparency enabled via `GL_BLEND` throughout runtime
- Window resolution: `1280×720` (modifiable via `width` / `height` variables)
- Scene transitions can be automatic (timed) or manual via keys `1–4`
