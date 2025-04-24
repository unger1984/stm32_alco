import shutil
import os

# Пути
cube = "cube32mx"

cube_lib = os.path.join(cube, "Drivers")
cube_start = os.path.join(cube, "Core", "Startup")
cube_src = os.path.join(cube, "Core", "Src")
cube_inc = os.path.join(cube, "Core", "Inc")
cube_freertos = os.path.join(cube, "Middlewares", "Third_Party",'FreeRTOS','Source')

pio_lib = "lib"
pio_src = "src"
pio_inc = "include"

def ensure_dir(path):
    if not os.path.exists(path):
        os.makedirs(path)

# CMSIS_V2
if os.path.exists(os.path.join(pio_lib, "CMSIS_RTOS_V2")):
     shutil.rmtree(os.path.join(pio_lib, "CMSIS_RTOS_V2"))
shutil.copytree(os.path.join(cube_freertos, "CMSIS_RTOS_V2"), os.path.join(pio_lib, "CMSIS_RTOS_V2"))

# FreeRTOS
if os.path.exists(os.path.join(pio_lib, "FreeRTOS")):
     shutil.rmtree(os.path.join(pio_lib, "FreeRTOS"))
os.makedirs(os.path.join(pio_lib, "FreeRTOS", "src"));
os.makedirs(os.path.join(pio_lib, "FreeRTOS", "include"));

for file in os.listdir(os.path.join(cube_freertos, "include")):
     shutil.copy(os.path.join(cube_freertos, "include", file), os.path.join(pio_lib, "FreeRTOS", "include"))

for file in os.listdir(os.path.join(cube_freertos)):
     if file.endswith(".c") or file == "LICENSE":
        shutil.copy(os.path.join(cube_freertos, file), os.path.join(pio_lib, "FreeRTOS", "src"))

shutil.copy(os.path.join(cube_freertos, "portable", "GCC","ARM_CM4F", "port.c"), os.path.join(pio_lib, "FreeRTOS", "src", "port.c"))
shutil.copy(os.path.join(cube_freertos, "portable", "GCC","ARM_CM4F", "portmacro.h"), os.path.join(pio_lib, "FreeRTOS", "include", "portmacro.h"))
shutil.copy(os.path.join(cube_freertos, "portable", "MemMang", "heap_4.c"), os.path.join(pio_lib, "FreeRTOS", "src", "heap_4.c"))


# Копирование .c файлов
for file in os.listdir(cube_src):
    if file.endswith(".c"):
            shutil.copy(os.path.join(cube_src, file), pio_src)

# Копирование .h файлов
for file in os.listdir(cube_inc):
    if file.endswith(".h"):
        shutil.copy(os.path.join(cube_inc, file), pio_inc)

print("✅ CubeMX → PlatformIO синхронизировано.")
