# Doubly Linked List Visualizer (Raylib, C)

## Overview

This project is a GUI-based application that visualizes operations on a **Doubly Linked List (DLL)**.
It is implemented in **C** using the **raylib graphics library**, combining data structures with interactive UI.

---

## Features

###  Core DLL Operations

- Insert:

  - Beginning
  - Random position
  - End
  - Before / After a value
- Delete:

  - Beginning
  - Random position
  - End
  - First occurrence
  - All occurrences
  - Duplicates
- Update values
- Sort list
- Reverse list

---

### 🔍 Information Operations

- Search element
- Get element at position
- Count total nodes
- Visual representation of list

---

###  GUI Features

- Menu-driven interface
- Mouse interaction
- Dynamic rendering of nodes
- Real-time updates of list state

---

## Tech Stack

- Language: C
- Graphics Library: raylib

---

## Project Structure

```
.
├── main.c      # GUI and interaction logic
├── dll.c       # Doubly Linked List implementation
├── dll.h       # Header file
├── .gitignore
```

---

## How to Run

### Requirements

- raylib installed

### Compile

```bash
gcc main.c dll.c -o app -lraylib -lopengl32 -lgdi32 -lwinmm
```

### Run

```bash
./app
```

---

## Known Issues

- Limited error handling (some invalid operations may cause crashes)
- GUI does not handle large lists (overflow outside display area)
- No scrolling support yet
- Edge cases not fully handled

---

## Future Improvements

- Add robust error handling
- Implement scrollable UI for large lists
- Improve GUI layout and responsiveness
- Add animations for operations
- Optimize memory safety

---

## What I Learned

- Implementation of Doubly Linked Lists in C
- Pointer manipulation and memory management
- Integrating data structures with GUI (raylib)
- Event-driven programming
- Basic software structuring and modularization

---

## Status

Work in Progress — will be improved after semester exams

---

## Author

Shaurya Garg
