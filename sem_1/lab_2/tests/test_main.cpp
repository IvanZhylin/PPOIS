#include <gtest/gtest.h>

#include "graphics/test_GPU.cpp"
#include "graphics/test_VRAM.cpp"
#include "graphics/test_GraphicsCard.cpp"

#include "cpu/test_ALU.cpp"
#include "cpu/test_CPU.cpp"
#include "cpu/test_CPUCache.cpp"
#include "cpu/test_CPUCore.cpp"

#include "integration/test_Computer.cpp"

#include "memory/test_RAM.cpp"
#include "memory/test_MemoryModule.cpp"

#include "storage/test_HardDrive.cpp"
#include "storage/test_Partition.cpp"
#include "storage/test_StorageController.cpp"

#include "motherboard/test_Motherboard.cpp"

#include "power/test_PowerSupply.cpp"

#include "os/test_FileSystem.cpp"
#include "os/test_OperatingSystem.cpp"

#include "audio/test_SoundCard.cpp"

#include "peripherals/test_Mouse.cpp"
#include "peripherals/test_Monitor.cpp"

//#include "../include/exceptions/Exceptions.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
