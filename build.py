import os
import sys
import subprocess

CC = "gcc"
if sys.platform == 'win32':
    print()
    print("[WARNING]: Set MINGW path in envitemet variables")
    print("Default path: C:\\raylib\\w64devkit\\bin")
    print()
    CC = "gcc"

PROJECT_NAME = "splander"
PROJECT_DIR = os.path.dirname(os.path.realpath(__file__))
SRC_PATH = "src"
BUILD_PATH = "build"

if sys.platform == 'win32':
    EXT = '.exe'
else:
    EXT = ""

use_x11 = False

RAYLIB_SRC_PATH = "C:\\raylib\\raylib\\src"
RAYLIB_INCLUDE_PATH = RAYLIB_SRC_PATH
RAYLIB_LIB_PATH = RAYLIB_SRC_PATH

PROJECT_CUSTOM_FLAGS = []
CFLAGS = ["-Wall", "-std=c99", "-D_DEFAULT_SOURCE", "-Wno-missing-braces"]
CFLAGS += ["-Wno-unused-value", "-Wno-pointer-sign"]
CFLAGS += PROJECT_CUSTOM_FLAGS

INCLUDE_PATHS = ["-I.", "-Iexternal", f"-I{RAYLIB_INCLUDE_PATH}"]

LDFLAGS = ["-L.", f"-L{RAYLIB_LIB_PATH}"]
if sys.platform == 'win32':
    # LDFLAGS += f"{SRC_PATH}/{PROJECT_NAME}.rc.data"
    pass

if sys.platform == 'win32':
    LDLIBS = ["-lraylib", "-lopengl32", "-lgdi32", "-lwinmm", "-lcomdlg32"]
    LDLIBS += ["-static", "-lpthread", "-lole32"]
else:
    LDLIBS = ["-lraylib", "-lGL", "-lm", "-lpthread", "-ldl", "-lrt"]
    if use_x11:
        LDLIBS += "-lX11"
    else:
        LDLIBS += ["-lwayland-client", "-lwayland-cursor", "-lwayland-egl",
                   "-lxkbcommon"]

SRC_FILES = []
for root, dirs, files in os.walk(SRC_PATH):
    for file in files:
        if file.endswith(".c"):
            src = os.path.join(PROJECT_DIR, root, file)
            SRC_FILES.append(src)

TARGET_EXE = os.path.join(PROJECT_DIR, BUILD_PATH, f"{PROJECT_NAME}{EXT}")

# $(CC) -o $(PROJECT_BUILD_PATH)/$(PROJECT_NAME)$(EXT) $(OBJS) $(CFLAGS)
# $(INCLUDE_PATHS) $(LDFLAGS) $(LDLIBS) -D$(PLATFORM)
cmd = [CC] + SRC_FILES + ["-o", TARGET_EXE]
cmd += CFLAGS + INCLUDE_PATHS + LDFLAGS + LDLIBS + ["-DPLATFORM_DESKTOP"]

if not os.path.exists(BUILD_PATH):
    os.makedirs(BUILD_PATH)

print(cmd)
subprocess.run(cmd)
