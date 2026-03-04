# Minimap HUD - Arma Reforger Mod

A circular minimap HUD mod for Arma Reforger that displays a real-time map at the bottom-center of the screen with GTA-style rotation.

## Features

- **Circular Minimap** — Displays at the bottom-center of the screen
- **Map Texture** — Full terrain texture with roads, mountains, hills, and lakes visible (not just topography lines)
- **Map Markers** — Capture points, objectives, bases, and other markers shown on the minimap
- **GTA-Style Rotation** — Map rotates based on the player's heading direction
- **Player Icon** — Arrow icon at the center of the minimap showing player position
- **Player Tracking** — Map stays centered on the player and moves as the player moves
- **Adjustable Zoom** — Zoom level can be configured from 0.1x to 5.0x
- **Pause Menu Settings** — Access minimap settings (zoom, size, opacity, rotation) from the pause menu
- **Compass Indicator** — North direction indicator on the minimap

## Mod Structure

```
MinimapHud/
├── MinimapHud.gproj                              # Mod project file
├── Scripts/
│   └── Game/
│       └── UI/
│           └── HUD/
│               └── Minimap/
│                   ├── SCR_MinimapDisplay.c       # Main HUD display component
│                   ├── SCR_MinimapConfig.c        # Configuration singleton (zoom, opacity, etc.)
│                   ├── SCR_MinimapMarkerManager.c # Map marker rendering on minimap
│                   └── SCR_MinimapSettings.c      # Pause menu settings integration
├── UI/
│   └── Layouts/
│       └── HUD/
│           └── MinimapHUD.layout                  # Widget layout definition
├── Configs/
│   └── Map/
│       └── MinimapConfig.conf                     # Map display configuration
├── README.md
├── LICENSE
└── image.png                                      # Resource Browser reference
```

## Installation

1. Clone or download this repository
2. Open the mod in Arma Reforger Workbench
3. Register the `MinimapHud.gproj` as an addon
4. Build the mod through the Workbench
5. Enable the mod in your game/server

## Workbench Setup

### Layout Setup
The layout file `UI/Layouts/HUD/MinimapHUD.layout` defines the widget hierarchy:
- **MinimapFrame** — Main frame container (256×256 pixels, bottom-center anchored)
  - **MapContainer** — Rotatable container holding the map widget
    - **MapWidget** — The actual map rendering widget (uses `SCR_MapEntity` MINIMAP mode)
  - **CircleMask** — Circular mask overlay for clipping map to circle shape
  - **CircleBorder** — Decorative circular border
  - **MarkerOverlay** — Container for custom marker widgets
  - **PlayerIcon** — Player position arrow at center
  - **CompassText** — North indicator text

### Texture Assets Needed
Create these texture assets in the Workbench:
- `UI/Textures/HUD/MinimapCircleMask.edds` — Circle mask (white center, black edges with alpha)
- `UI/Textures/HUD/MinimapCircleBorder.edds` — Circle border ring
- `UI/Textures/HUD/MinimapPlayerArrow.edds` — Player arrow icon (16×16)

### HUD InfoDisplay Registration
The `SCR_MinimapDisplay` class extends `SCR_InfoDisplay` and must be registered in the HUD manager:
1. In Workbench, open the player controller prefab (`DefaultPlayerControllerMP.et`)
2. Find the `SCR_HUDManagerComponent`
3. Add a new `SCR_InfoDisplay` entry pointing to `SCR_MinimapDisplay`
4. Set the layout path to `{DE15F59659B8CF68}UI/Layouts/HUD/MinimapHUD.layout`

## Configuration

### Default Settings
| Setting | Default | Range | Description |
|---------|---------|-------|-------------|
| Zoom Level | 0.5 | 0.1 – 5.0 | Map zoom (higher = more detail) |
| Minimap Size | 256 | 128 – 512 | Diameter in pixels |
| Opacity | 0.85 | 0.1 – 1.0 | Minimap transparency |
| Rotate With Player | true | — | GTA-style heading rotation |
| Show Markers | true | — | Display capture points and markers |
| Enabled | true | — | Toggle minimap on/off |

### Settings Access
Settings can be changed through:
1. **Pause Menu** → Settings → Minimap settings (via `SCR_MinimapSettingsModule`)
2. **Keybinds** (if configured):
   - `MinimapZoomIn` — Increase zoom
   - `MinimapZoomOut` — Decrease zoom
   - `MinimapToggle` — Toggle minimap on/off

## Technical Details

### Map Rendering
The minimap uses `SCR_MapEntity` with `EMapEntityMode.MINIMAP` to render the actual game map with full texture support. This ensures:
- Satellite/terrain texture is displayed (not just contour lines)
- Roads, water bodies, and terrain features are visible
- Standard map markers and descriptors are rendered
- The map configuration re-enables character camera rendering after initialization

### Rotation System
The `MapContainer` widget rotates based on the player's yaw angle (`GetAngles()[0]`), providing the GTA-style minimap rotation where the map turns as the player turns.

### Performance
- Map position updates run via `CallQueue` with minimal delay
- Marker updates are throttled to reduce overhead
- The minimap pauses when the fullscreen map or any menu is open

## Dependencies

- Arma Reforger base game (`58D0FB3206B6F859`)

## License

MIT License — See [LICENSE](LICENSE) for details