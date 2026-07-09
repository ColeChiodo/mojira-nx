# Mojira NX - Development Plan

## Project Overview

Mojira NX is a Nintendo Switch homebrew application built with:

- C
- libnx
- SDL2
- SDL2_image
- SDL2_ttf
- libcurl (or native HTTP client if needed)

The goal is to create a gallery application that displays album images and allows the user to upload selected images to a local network server.

The original desktop workflow:

```bash
grim screenshot.png

curl \
  -X POST \
  http://server:8000/captures \
  -F "image=@screenshot.png"
```

will become:

```
Switch Gallery
      |
      v
Browse images
      |
      v
Press A
      |
      v
Upload image
      |
      v
Show returned URL/status
```

---

# Architecture

## Directory Structure

```
mojira-nx/

├── source/
│   ├── main.c          # Application entry point
│   ├── app.c           # Application state and lifecycle
│   ├── gallery.c       # Gallery UI and image selection
│   ├── image.c         # Image loading and textures
│   ├── filesystem.c    # File discovery
│   ├── upload.c        # HTTP upload
│   ├── config.c        # Server configuration
│   └── ui.c            # UI rendering helpers
│
├── include/
│   ├── app.h
│   ├── gallery.h
│   ├── image.h
│   ├── filesystem.h
│   ├── upload.h
│   ├── config.h
│   └── ui.h
│
├── romfs/
│   └── data/
│       ├── fonts/
│       └── images/
│
└── Makefile
```

---

# Application Design

## Main Loop

The application should follow this pattern:

```
initialize

while appletMainLoop():

    read controller input

    update application state

    render frame

cleanup
```

---

# Core Features

## Gallery

The gallery should:

* Scan image directories
* Load image thumbnails
* Display images in a grid
* Support scrolling
* Show selected image
* Handle missing/corrupt images gracefully

Controls:

```
D-pad / Analog stick:
    Move selection

A:
    Upload selected image

B:
    Back

PLUS:
    Exit application
```

---

## Upload

The upload system should:

POST:

```
POST /captures
Content-Type: multipart/form-data
```

with:

```
image=@selected_file
```

Expected server responses:

Redirect:

```
302 Location: http://server/image/url
```

or JSON:

```json
{
    "url": "http://server/image/url"
}
```

Display:

```
Upload successful

URL:
http://server/image/url
```

---

# Configuration

Server address should not be hardcoded.

user inputs address for their server.

on start, call http://server/health
if healthy, continue, else reprompt for address

---

# Milestones

---

# Milestone 1 - Basic SDL Application

Goal:

A working Switch application with SDL rendering.

Tasks:

* [x] Build libnx project
* [x] Initialize SDL
* [x] Create window
* [x] Render loop
* [x] Controller input
* [x] Exit with PLUS button

Success criteria:

```
Application launches on Switch
SDL renders correctly
```

---

# Milestone 2 - UI Foundation

Goal:

Create reusable UI system. UI should look like a native switch app.

Tasks:

* Create text rendering
* Load TTF font
* Create buttons
* Create status messages
* Add application state

Success criteria:

Screen displays:

```
Mojira Gallery

Images: 0

A Upload
+ Exit
```

---

# Milestone 3 - Image Loading

Goal:

Display images.

Tasks:

* Add SDL_image
* Load PNG/JPEG
* Create textures
* Render texture
* Handle invalid files

Success criteria:

```
One image displays correctly
```

---

# Milestone 4 - Gallery Grid

Goal:

Create image browser.

Tasks:

* Scan image directory
* Store image metadata
* Generate thumbnails
* Render grid
* Selection cursor
* Scrolling

Example:

```
+----------------------+

[img] [img] [img]

[img] [img] [img]

        ^
     selected

A Upload

+----------------------+
```

Success criteria:

User can browse many images.

---

# Milestone 5 - Filesystem Integration

Goal:

Load images from SD card.

Tasks:

* Define image location
* Scan directories
* Filter extensions:

```
.jpg
.jpeg
.png
.webp
```

* Sort by date/name

Success criteria:

Application loads real images.

---

# Milestone 6 - HTTP Upload

Goal:

Upload selected image.

Tasks:

* Add HTTP client
* Create multipart request
* Upload file
* Handle errors
* Display progress

Success criteria:

Pressing A uploads an image.

---

# Milestone 7 - Server Configuration

Goal:

Allow changing server.

Tasks:

* Read config file
* Add settings screen
* Validate URL

Success criteria:

User can change server without rebuilding.

---

# Milestone 8 - Polish

Tasks:

* Loading indicators
* Better error messages
* Image caching
* Memory cleanup
* Performance testing
* Controller UX improvements

---

# Coding Guidelines

## Language

Use C only.

Do not introduce C++.

---

## Memory

Always:

* Free SDL surfaces
* Destroy textures
* Close files
* Handle allocation failures

---

## Error Handling

Every subsystem should return errors.

Example:

```c
bool gallery_init(void);

bool upload_file(const char *path);
```

Do not crash on:

* Missing files
* Bad images
* Network failures

---

# Future Features

Possible additions:
* Upload history

---

# Current Status

Environment:

* [x] devkitPro installed
* [x] devkitA64 working
* [x] libnx working
* [x] SDL2 working
* [x] SDL2_image working
* [x] SDL2_ttf working
* [x] NRO builds successfully

Next task:

Implement Milestone 2:
SDL UI foundation and text rendering.


