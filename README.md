# C++ Console Mini Games Hub 🎮

โปรเจกต์รวบรวมมินิเกมบน Terminal / Console ในภาษา C++ (C++17) พร้อมระบบบันทึก High Score และเมนูเลือกเล่นที่ใช้งานง่าย

---

## ⚡ วิธีเริ่มเล่นเกมทันที (Quick Start)

สำหรับผู้ที่ดาวน์โหลดโปรเจกต์ (Download ZIP):
- **ดับเบิลคลิกที่ไฟล์ `game_hub.exe`** ในโฟลเดอร์หลัก เพื่อเริ่มเล่นได้ทันที (ไม่ต้องติดตั้งโปรแกรมคอมไพล์)

---

## 📁 โครงสร้างโปรเจกต์ (Project Structure)

```text
miniproject/
├── game_hub.exe                 # ไฟล์พร้อมเล่น (คลิกเล่นได้ทันที)
├── .vscode/
│   ├── launch.json              # คอนฟิกการรัน
│   └── tasks.json               # Tasks สำหรับคอมไพล์ใน VS Code
├── bin/
│   └── game_hub.exe             # ไฟล์ Executable สำรอง
├── data/                        # ไฟล์เก็บข้อมูลสถิติและ High Scores (.txt)
│   ├── 2048.txt
│   ├── hangman.txt
│   ├── number_guess.txt
│   ├── rps.txt
│   ├── snake.txt
│   ├── sudoku.txt
│   └── tic_tac.txt
├── include/                     # Header Files (.h)
│   ├── games/
│   │   ├── game_2048.h
│   │   ├── hangman.h
│   │   ├── number_guess.h
│   │   ├── rock_paper_scissors.h
│   │   ├── snake.h
│   │   ├── sudoku.h
│   │   └── tic_tac_toe.h
│   └── utils/
│       └── console_utils.h      # Helper จัดการหน้าจอและสีคอนโซล
├── src/                         # Source Files (.cpp)
│   ├── games/
│   │   ├── game_2048.cpp
│   │   ├── hangman.cpp
│   │   ├── number_guess.cpp
│   │   ├── rock_paper_scissors.cpp
│   │   ├── snake.cpp
│   │   ├── sudoku.cpp
│   │   └── tic_tac_toe.cpp
│   ├── utils/
│   │   └── console_utils.cpp
│   └── main.cpp                 # Main Menu Hub
├── compile_flags.txt
├── .gitignore
└── README.md
```

---

## 🕹️ รายชื่อเกมทั้งหมด (Games List)

1. **Number Guess Game**: ทายตัวเลขสุ่ม 1 - 100 พร้อมคำใบ้ Too High / Too Low
2. **Tic-Tac-Toe**: เกม XO สลับผู้เล่น 2 คน พร้อมระบบตรวจผลแพ้ชนะ/เสมอ
3. **Hangman**: เกมทายคำศัพท์ภาษาอังกฤษพร้อมคำใบ้และความหมาย
4. **Snake Game**: เกมงูกินผลไม้แบบ Real-time บน Console
5. **Rock, Paper, Scissors**: เกมเป่ายิ้งฉุบ (ค้อน กรรไกร กระดาษ) แข่งกับคอมพิวเตอร์
6. **Sudoku**: เกมกระดานซูโดกุ 9x9 พร้อมระบบจับเวลาและตรวจความถูกต้อง
7. **2048**: เกมเลื่อนรวมตัวเลข 2048 แบบคลาสสิก พร้อมสีสันแยกตามระดับตัวเลข

---

## 🛠️ วิธีการคอมไพล์โค้ดเอง (สำหรับนักพัฒนา)

### 1. คอมไพล์ผ่าน Terminal (g++ / MinGW)
```bash
g++ -std=c++17 -Iinclude src/main.cpp src/games/*.cpp src/utils/*.cpp -o game_hub.exe
```

### 2. Build ผ่าน VS Code
- กดปุ่ม **`Ctrl + Shift + B`** เพื่อรัน Build Task ตามที่ตั้งค่าไว้ใน `.vscode/tasks.json`
