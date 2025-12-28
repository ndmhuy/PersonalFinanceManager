# Personal Finance Manager (C++)

A simple console-based personal finance manager written in C++.

- **Purpose:** Track wallets, incomes, expenses and recurring transactions via a clear, testable command-line UI.  
- **Status:** Student project (Milestone M3: UI & Controllers implemented).

---

## 📁 Project layout

Top-level tree (important files/folders):

```
PersonalFinanceManager/
  ├─ include/           # public headers (Views, Models, Controllers, Utils)
  ├─ src/               # source files (implementations)
  ├─ data/              # sample and runtime data
  ├─ Report/            # LaTeX report and images
  │   ├─ content/       # sections (including M3.tex)
  │   └─ img/           # images / screenshots
  ├─ CMakeLists.txt
  ├─ makefile
  ├─ run_windows.bat
  └─ run_mac.command
```

Key source areas:
- `include/Views` & `src/Views` — Console drawing, menus, dashboard, input validators
- `include/Controllers` & `src/Controllers` — Navigation and UI flows (Nav* files) and `AppController` for business rules
- `src/Models` — Transaction, Wallet, Category, IncomeSource, RecurringTransaction

---

## 🛠 Requirements

- C++ toolchain (C++17+), CMake
- For building the PDF report: TeX distribution (TeX Live / MiKTeX) with `tikz` and `svg` packages; `latexmk` recommended
- Optional: Python + Pillow for automated screenshot generation
- Optional: Inkscape (if using `svg` package and converting SVGs during LaTeX builds via `--shell-escape`)

---

## 🔧 Build & Run

### Windows (quick)
- Use the provided batch: `run_windows.bat` — this script builds and runs the application (if present and configured for your system).

### Generic (CMake)
```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
# then run the executable produced (name may be PersonalFinanceManager or similar)
```

### Running
- Run the produced executable from the build directory, or use `run_windows.bat` on Windows.

---

## ✅ Tests

- There is a `UnitTest.cpp` file in `src/` (unit test skeletons). Build the tests using the same CMake build and run the generated test executable (check `build/` for test binary).
- Suggested tests: `InputValidator` (money/date/index validation) and basic controller flows (add/view/delete flows).

---

## 📄 Report (LaTeX)

- The project report is in `Report/`. The main file is `Report/main.tex` and M3 content is in `Report/content/M3.tex`.
- To build the PDF (recommended with `latexmk`):
```bash
cd Report
latexmk -pdf -shell-escape main.tex
```
- If you prefer `pdflatex` directly and the document uses `svg` conversion, run with `--shell-escape` and ensure `inkscape` is available:
```bash
pdflatex --shell-escape main.tex
pdflatex --shell-escape main.tex
```

---

## 🧭 Useful file references

- UI / Views: `include/Views/*`, `src/Views/*`
- Controllers: `src/Controllers/Nav*.cpp`, `src/Controllers/AppController.cpp`
- Models: `src/Models/*`
- Utilities: `src/Utils/*`
- Report: `Report/main.tex`, `Report/content/M3.tex`

---

## 🤝 Contributing

Feel free to open issues / PRs to:
- Add unit tests
- Enhance UI flows (better pagination, multi-line inputs)

---

## ⚖️ License

No license is specified. Add a `LICENSE` file if you want to set an explicit license (MIT/Apache/etc.).

---