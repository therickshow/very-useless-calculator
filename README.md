# very useless calculator

Type in a number of microseconds. Get back how long that actually is, in
months, weeks, days, hours, minutes and seconds.

It is useless in the sense that nobody has ever needed this. It is not useless
in the sense that it works.

```
Enter a time in microseconds: 99999999999
0 months, 0 weeks, 1 days, 3 hours, 46 minutes, 39 seconds
```

## What's here

| File | |
|---|---|
| `very-useless-calculator.c` | The console version. Reads from stdin, divides and modulos its way down through each unit. |
| `gui_calculator.c` | The same conversion behind a GTK window — event-driven instead of blocking on `scanf`, and it validates input with `strtoll` instead of trusting it. |

## A note on the maths

A "month" here is a flat **30 days**. Real months aren't, so anything a month
or longer is an approximation. That's deliberate — fixing it would mean
picking a calendar, and this is a very useless calculator.

## Build — console version

```powershell
python "..\library\scripts\build.py" very-useless-calculator.c --run
```

Or **Ctrl+Shift+B** in VS Code. Output goes to `build\`, which is gitignored.

## Build — GTK version

Same command as above:

```powershell
python "..\library\scripts\build.py" gui_calculator.c --run
```

`build.py` sees `#include <gtk/gtk.h>` and pulls GTK's compiler and linker flags
from `pkg-config` on its own — there are fourteen include directories and about
thirty libraries, so this is not a list worth typing. In VS Code, pick
**C: build active file (GTK / pkg-config)** from the build tasks; plain
Ctrl+Shift+B runs the default task, which has no GTK flags and will fail on this
file.

### If it can't find `gtk/gtk.h`

GTK's headers live in `include/gtk-3.0/`, which gcc doesn't search by default,
so a build without `pkg-config` flags stops at line 1. Both pieces come from an
MSYS2 MinGW64 shell:

```bash
pacman -S mingw-w64-x86_64-gtk3 mingw-w64-x86_64-pkgconf
```

Note `pkgconf`, not `pkg-config` — MSYS2 renamed the package, and the old name
no longer resolves.

## Licence

MIT — see [LICENSE](LICENSE).

---

Part of the `Github projz` workspace. Self-contained, with its own repository.
