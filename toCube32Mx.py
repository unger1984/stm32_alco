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

free_heap = ["heap_4.c"];
free_port = ["port.c", "portmacro.h"];
cube_c = ["freertos.c", "gpio.c", "main.c", "i2c.c", "stm32f4xx_hal_msp.c", "stm32f4xx_hal_timebase_tim.c", "stm32f4xx_it.c", "syscalls.c", "sysmem.c", "system_stm32f4xx.c", "tim.c", "usart.c"];
cube_h = ["FreeRTOSConfig.h", "gpio.h", "i2c.h", "main.h", "stm32f4xx_hal_conf.h", "stm32f4xx_it.h", "tim.h", "usart.h"];


def ensure_dir(path):
    if not os.path.exists(path):
        os.makedirs(path)

# FreeRTOS
if os.path.exists(cube_freertos):
     shutil.rmtree(cube_freertos)
os.makedirs(os.path.join(cube_freertos, "portable", "GCC","ARM_CM4F"));
os.makedirs(os.path.join(cube_freertos, "portable", "MemMang"));
os.makedirs(os.path.join(cube_freertos, "include"));

for file in os.listdir(os.path.join(pio_lib, "FreeRTOS", "include")):
    if file in free_port:
        shutil.copy(os.path.join(pio_lib, "FreeRTOS", "include", file),  os.path.join(cube_freertos, "portable", "GCC","ARM_CM4F",file))
    else:
        shutil.copy(os.path.join(pio_lib, "FreeRTOS", "include", file), os.path.join(cube_freertos, "include", file))

for file in os.listdir(os.path.join(pio_lib, "FreeRTOS", "src")):
    if os.path.isdir(file):
        continue
    elif file in free_port:
        shutil.copy(os.path.join(pio_lib, "FreeRTOS", "src", file),  os.path.join(cube_freertos, "portable", "GCC","ARM_CM4F",file))
    elif file in free_heap:
        shutil.copy(os.path.join(pio_lib, "FreeRTOS", "src", file),  os.path.join(cube_freertos, "portable", "MemMang",file))
    else:
        shutil.copy(os.path.join(pio_lib, "FreeRTOS", "src", file), os.path.join(cube_freertos, file))

# CMSIS_V2
shutil.copytree(os.path.join(pio_lib, "CMSIS_RTOS_V2"), os.path.join(cube_freertos, "CMSIS_RTOS_V2"))

# Копирование
for file in cube_c:
    print(os.path.join(pio_src, file));
    shutil.copy(os.path.join(pio_src, file), os.path.join(cube_src, file))
for file in cube_h:
    shutil.copy(os.path.join(pio_inc, file), os.path.join(cube_inc, file))


print("✅ PlatformIO → CubeMX синхронизировано.")
