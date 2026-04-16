# Lost in Forest

**Author:** Yaroslav Suprun

A 2D OpenGL game built in C++ for COMP 2501 at Carleton University.

You play as a cat lost in a cursed forest. Find two shells to break the curse and escape before nightfall — or the sacred beast will find you first. Fight off sea creatures with claw attacks and thrown fish as you search.

## Build

Originally built on Windows 11. On Linux/Mac, minor adjustments to `CMakeLists.txt` may be required.

**Required Libraries:**
- GLFW
- GLEW
- GLM
- SOIL

Archive containing necessary libraries is included. Build via CMake.

## Controls

| Input | Action |
|-------|--------|
| W/A/S/D | Move |
| Space | Claw attack |
| LMB | Throw fish (shooting) |
| Mouse | Aim direction |
| Arrow keys | Move (not recommended) |

## Technical Details

UI: Health, ammo, and score are shown via sprite icons (clownfish, squid, shell) with digit rendering from a 320x32 spritesheet using UV offsets. Other UI elements include NPC dialogue, a gun unlock popup, gatekeeper dialogue, and a countdown timer.

## Credits

Credited as requested by each author.

- **Cat sprite** (paid) — [ToffeeCraft](https://toffeecraft.itch.io/)
- **Sea creatures and shells sprites** (paid) — [Shubibubi](https://shubibubi.itch.io/)
- **Claw attack sprite**, frame ripped from animation 1120 (paid) — [BDragon1727](https://bdragon1727.itch.io/)
- **Background and grass tile sprite** — [Brullov](https://brullov.itch.io/)
- **Numbers and text sprites** — Hand drawn in Aseprite by myself
