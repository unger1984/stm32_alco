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

free_heap = [];
for file in os.listdir(os.path.join(cube_freertos,  "portable", "MemMang")):
    free_heap.append(file);

free_port = [];
for file in os.listdir(os.path.join(cube_freertos, "portable", "GCC","ARM_CM4F")):
    free_port.append(file);

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
     print(os.path.join(cube_freertos, "include", file)+ " -> "+os.path.join(pio_lib, "FreeRTOS", "include", file));
     shutil.copy(os.path.join(cube_freertos, "include", file), os.path.join(pio_lib, "FreeRTOS", "include", file))

for file in os.listdir(os.path.join(cube_freertos)):
     if file.endswith(".c") or file == "LICENSE":
        print(os.path.join(cube_freertos, file)+ " -> "+os.path.join(pio_lib, "FreeRTOS", "src", file));
        shutil.copy(os.path.join(cube_freertos, file), os.path.join(pio_lib, "FreeRTOS", "src", file))

for file in free_heap:
     if file.endswith(".c"):
          print(os.path.join(cube_freertos, "portable", "MemMang", file)+ " -> "+os.path.join(pio_lib, "FreeRTOS", "src", file));
          shutil.copy(os.path.join(cube_freertos, "portable", "MemMang", file),os.path.join(pio_lib, "FreeRTOS", "src", file))
     elif file.endswith(".h"):
          print(os.path.join(cube_freertos, "portable", "MemMang", file)+ " -> "+os.path.join(pio_lib, "FreeRTOS", "include", file));
          shutil.copy(os.path.join(cube_freertos, "portable", "MemMang", file),os.path.join(pio_lib, "FreeRTOS", "include", file))

for file in free_port:
     print(os.path.join(cube_freertos, "portable", "GCC","ARM_CM4F", file)+ " -> "+os.path.join(pio_lib, "FreeRTOS", "include", file));
     if file.endswith(".c"):
          shutil.copy(os.path.join(cube_freertos, "portable", "GCC","ARM_CM4F", file),os.path.join(pio_lib, "FreeRTOS", "src", file))
     elif file.endswith(".h"):
          shutil.copy(os.path.join(cube_freertos, "portable", "GCC","ARM_CM4F", file),os.path.join(pio_lib, "FreeRTOS", "include", file))


# Копирование .c файлов
for file in os.listdir(cube_src):
    if file.endswith(".c"):
            print(os.path.join(cube_src, file)+ " -> "+os.path.join(pio_src, file));
            shutil.copy(os.path.join(cube_src, file), pio_src)

# Копирование .h файлов
for file in os.listdir(cube_inc):
    if file.endswith(".h"):
        print(os.path.join(cube_src, file)+ " -> "+os.path.join(pio_inc, file));
        shutil.copy(os.path.join(cube_inc, file), pio_inc)

print("✅ CubeMX → PlatformIO синхронизировано.")
