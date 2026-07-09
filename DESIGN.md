# Mojira NX UI Design Requirements

The application should visually feel like a native Nintendo Switch system application.

Do not use generic SDL demo styling.
Do not use plain rectangles with default fonts.

Study the Nintendo Switch HOME menu and system settings UI patterns:

- clean dark background
- large rounded cards
- soft shadows
- generous spacing
- smooth transitions
- minimal text
- focus-based navigation
- selected items clearly highlighted

The UI should feel like a first-party Nintendo application.

---

# Visual Style

## Colors

Use a Switch-inspired palette:

Background:
- dark charcoal (#101014)

Panels:
- slightly lighter dark gray

Primary accent:
- Switch red (#e60012)

Text:
- white
- secondary gray

Selected items:
- bright accent outline
- slight scale increase
- glow/shadow

---

# Layout

The gallery should resemble the Switch HOME menu.

Main screen:

```

+------------------------------------------------+

Mojira Gallery

+-------+  +-------+  +-------+  +-------+
|       |  |       |  |       |  |       |
| IMG 1 |  | IMG 2 |  | IMG 3 |  | IMG 4 |
|       |  |       |  |       |  |       |
+-------+  +-------+  +-------+  +-------+

```
                ^
             selected
```

A Upload        X Refresh        + Exit

+------------------------------------------------+

```

---

# Gallery Cards

Images should:

- have rounded corners
- maintain aspect ratio
- have padding
- animate when selected

Selected image:

- scale to 110%
- brighter border
- slight shadow
- title appears underneath

Example:

```

Normal:

+---------+
|         |
|  IMG    |
|         |
+---------+

Selected:

+-----------+
|           |
|   IMG     |
|           |
+-----------+

Screenshot.png

```

---

# Typography

Use SDL_ttf.

Rules:

- large title font
- medium menu labels
- small metadata

Avoid:
- tiny text
- console/debug style text
- default SDL fonts

---

# Navigation

Use Switch-style focus navigation:

- D-pad moves focus
- Analog stick moves focus
- A confirms
- B cancels
- PLUS exits

Never require a mouse.

---

# Animations

Add simple animations:

Selection:
- smooth scale interpolation

Upload:
- spinner/progress indicator

Transitions:
- fade between screens

Avoid instant jumps.

---

# Screens

## Home Gallery

Shows:

- title
- image grid
- selected image
- upload button
- server status

---

## Upload Screen

Example:

```

Uploading...

[##########------]

my_screenshot.png

Server:
192.168.1.10:8000

```

---

## Success Screen

Example:

```

Upload Complete

URL:

[http://server/image/abc123](http://server/image/abc123)

A Copy
B Back

````

---

# Code Requirements

Create reusable UI components:

```c
ui_panel()
ui_button()
ui_card()
ui_text()
ui_image_card()
````

Do not hardcode drawing everywhere.

Keep rendering separate from application logic.

---

# Quality Bar

Before considering the UI complete:

* It should look like it belongs on a Switch.
* It should not look like a PC SDL sample.
* A user should understand it without instructions.

```

