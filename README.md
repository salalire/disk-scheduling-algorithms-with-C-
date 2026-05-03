# Disk Scheduling Algorithms (SCAN & C-SCAN)

## 📌 Project Overview
This project implements two disk scheduling algorithms: SCAN and C-SCAN. The system simulates disk head movement based on user input and compares the performance of both algorithms using total seek time and servicing sequence. It also includes a simple web-based visualization to demonstrate how each algorithm works.

---

## 🎯 Objectives
- Implement SCAN and C-SCAN algorithms in C++
- Simulate disk head movement
- Calculate and compare total seek time
- Provide a clear visualization of algorithm behavior

---

## 🛠 Technologies Used
- C++
- STL (Standard Template Library)
- HTML, CSS, JavaScript (for visualization)

---

## 👥 Team Roles

**Member 1 – SCAN Algorithm Developer**
- Implement SCAN algorithm
- Handle sorting and direction logic
- Output seek sequence

**Member 2 – C-SCAN Algorithm Developer**
- Implement C-SCAN algorithm
- Handle circular movement logic
- Output seek sequence

**Member 3 – Seek Time & Comparison**
- Calculate total seek time
- Compare SCAN vs C-SCAN
- Format comparison results

**Member 4 – Input/Output (CLI)**
- Design input format
- Validate inputs
- Display clean output

**Member 5 – Visualization Developer (Web Interface)**
- Build a simple web interface using HTML + JavaScript
- Visualize disk head movement step-by-step
- Display request sequence as a graph or line animation

**Visualization Requirements:**
- Create a simple webpage (index.html)
- Draw a horizontal line representing disk tracks (0 → max)
- Plot points for:
  - Initial head position
  - Each request
- Draw lines connecting the sequence (movement path)
- Differentiate:
  - SCAN → continuous back-and-forth
  - C-SCAN → jump from end to start (visually shown as a gap or dashed line)

**Data Input for Visualization:**
- Read sequence from a file (e.g., output.txt or JSON)

Example format:
```
SCAN: 50 82 140 170 190 43 24 16
CSCAN: 50 82 140 170 190 0 24 43
```

OR JSON:
```
{
  "scan": [50, 82, 140, 170, 190, 43, 24, 16],
  "cscan": [50, 82, 140, 170, 190, 0, 24, 43]
}
```

**Suggested Implementation:**
- Use `<canvas>` or simple DOM elements
- Use JavaScript to:
  - Parse data
  - Draw points and lines
  - Animate movement (optional using setTimeout)

---

**Member 6 – Integration & Documentation**
- Integrate all modules
- Manage data flow (C++ → file → visualization)
- Prepare report and presentation

---

## 🔁 Workflow
Input → Algorithm (SCAN / C-SCAN) → Seek Time → Save Output → Visualization → Presentation

---

## ⚙️ Features
- Custom disk size and request queue
- Supports left/right direction
- Displays seek sequence and total seek time
- Algorithm comparison
- Web-based visualization

---

## 📥 Input Format
- Disk size (e.g., 200)
- Number of requests
- Request queue (e.g., 98, 183, 41, 122)
- Initial head position
- Direction (left/right)

---

## 📤 Output Example
```
Algorithm: SCAN

Seek Sequence:
50 → 82 → 140 → 170 → 190 → 43 → 24 → 16

Total Seek Time:
208
```

---

## 🚀 How to Run

1. Compile:
```
g++ main.cpp -o disk_scheduler
```

2. Run:
```
./disk_scheduler
```

3. Open visualization:
- Open `index.html` in browser
- Load output file or use embedded data

---

## 📌 Notes
- Keep output format consistent for visualization
- Test integration early
- Visualization should reflect actual algorithm output

---

## 📚 References
- Operating System Concepts by Silberschatz
- Course materials

---

## 📄 License
Academic use only.
