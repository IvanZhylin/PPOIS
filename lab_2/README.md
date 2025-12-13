# README: Computer Simulation (Симуляция компьютера)

- Классы: 51
- Поля: ~180
- Уникальные поведения: 120+
- Ассоциации: 45
- Исключения: 7

## Исключения (7)
Находятся в директории `include/exceptions/`

- **HardwareException** — базовое исключение для аппаратных ошибок
- **CPUOverheatException** — перегрев процессора
- **GPUOverheatException** — перегрев видеокарты
- **MemoryOverflowException** — переполнение оперативной памяти
- **DiskFullException** — переполнение дискового пространства
- **PowerSupplyFailureException** — отказ блока питания
- **NetworkConnectionException** — ошибка сетевого подключения

***

## Классы
Формат: Имя_класса Поля Методы -> Ассоциации (связанные классы)

### CPU_classes

**CPU** 12 15 -> CPUCore, CPUCache, ControlUnit, Component
- Поля: name, manufacturer, price, cores, caches, baseFrequency, maxFrequency, threadCount, architecture, tdp, powered, temperature
- Методы: powerOn, powerOff, processInstruction, getTemperature, cool, overclock, resetAllCores, getCoreCount, getThreadCount, getFrequency, getArchitecture, getTDP, getDiagnostics, isPowered, init

**CPUCore** 7 10 -> ALU, RegisterFile, ControlUnit
- Поля: coreId, frequency, temperature, registerFile, alu, controlUnit, active
- Методы: executeInstruction, getTemperature, cool, getId, getFrequency, setActive, isActive, reset, init, str

**ALU** 5 8 -> CPUCore
- Поля: bitWidth, zeroFlag, carryFlag, overflowFlag, signFlag
- Методы: add, subtract, multiply, divide, bitwiseAnd, bitwiseOr, getZeroFlag, init

**ControlUnit** 4 5 -> RegisterFile, ALU
- Поля: programCounter, instructionRegister, currentInstruction, state
- Методы: fetch, decode, execute, reset, init

**RegisterFile** 3 5
- Поля: registerCount, registers, generalPurposeRegisters
- Методы: setRegister, getRegister, clear, getCount, init

**CPUCache** 6 10
- Поля: level, sizeKB, cache, hitCount, missCount, latency
- Методы: read, write, invalidate, flush, getHitRate, getLevel, getSize, getHitCount, getMissCount, init

***

### Graphics_classes

**GPU** 7 9 -> VRAM
- Поля: cores, frequency, vram, temperature, frameCount, shaderUnits, powered
- Методы: renderFrame, computeShaders, getTemperature, cool, getCores, getFrequency, getVRAM, powerOn, powerOff

**VRAM** 5 9
- Поля: capacityGB, frequencyMHz, usedMemory, textureMemory, bandwidthGBps
- Методы: allocateTexture, freeTexture, clear, getCapacity, getUsed, getFree, getUsagePercent, getBandwidth, init

**GraphicsCard** 7 9 -> GPU, Component
- Поля: name, manufacturer, price, gpu, displayPorts, hdmiPorts, powerConsumption, powered
- Методы: render3DScene, computeShaders, getTemperature, cool, getDisplayPorts, getHDMIPorts, getPowerConsumption, getDiagnostics, init

***

### Memory_classes

**RAM** 6 12 -> MemoryModule, Component
- Поля: name, manufacturer, price, capacityGB, frequencyMHz, usedMemory, memoryMap, powered
- Методы: allocate, deallocate, read, write, getTotalCapacity, getUsedMemory, getFreeMemory, getUsagePercent, clear, powerOn, powerOff, getDiagnostics

**MemoryModule** 5 7
- Поля: moduleId, capacityGB, frequencyMHz, type, manufacturer
- Методы: getCapacity, getFrequency, getType, getManufacturer, init, str, getDiagnostics

**MemoryController** 5 7
- Поля: channels, frequency, operationCount, memoryMap, bandwidth
- Методы: read, write, getChannels, getFrequency, getBandwidth, getOperationCount, init

***

### Storage_classes

**HardDrive** 8 13 -> StorageController, Partition, Component
- Поля: name, manufacturer, price, capacityGB, isSSD, usedSpace, files, partitions, rpm, powered
- Методы: read, write, format, getCapacity, getUsedSpace, getFreeSpace, isSSD, getRPM, createPartition, powerOn, powerOff, getDiagnostics, init

**Partition** 6 10
- Поля: partitionId, sizeGB, usedSpace, fileSystem, label, files
- Методы: writeFile, readFile, deleteFile, getSize, getUsedSpace, getFreeSpace, getFileSystem, format, init, str

**StorageController** 3 4
- Поля: interface, maxDevices, connectedDevices
- Методы: getInterface, getMaxDevices, optimizeIO, init

***

### Motherboard_classes

**Motherboard** 10 9 -> BIOS, Chipset, PCIeSlot, Component
- Поля: name, manufacturer, price, socketType, formFactor, chipset, bios, pcieSlots, memorySlots, m2Slots, powered
- Методы: installComponent, updateBIOS, configureBIOS, getSocketType, getMemorySlots, powerOn, powerOff, getDiagnostics, init

**BIOS** 4 6
- Поля: version, manufacturer, settings, bootOrder
- Методы: updateFirmware, changeSetting, getSetting, getVersion, boot, init

**Chipset** 5 5
- Поля: model, manufacturer, pcieLanes, sataPorts, usb3Ports, supportsOverclocking
- Методы: getModel, getPCIeLanes, getSATAPorts, getUSBPorts, init

**PCIeSlot** 5 7
- Поля: slotNumber, lanes, generation, occupied, cardType
- Методы: insertCard, removeCard, isOccupied, getCardType, getLanes, getBandwidth, init

***

### Power_classes

**PowerSupply** 8 11 -> PowerRail, Component
- Поля: name, manufacturer, price, wattage, efficiency, currentLoad, rails, powered, certification
- Методы: powerOn, powerOff, supplyPower, getWattage, getEfficiency, getCurrentLoad, getLoadPercent, addRail, getDiagnostics, init, str

**PowerRail** 5 6
- Поля: railId, voltage, maxCurrent, currentLoad, connectors
- Методы: getVoltage, getMaxCurrent, getCurrentLoad, draw, init, str

***

### Cooling_classes

**CoolingSystem** 5 6 -> Fan, Component
- Поля: systemType, fans, maxRPM, noiseLevel, powered
- Методы: startCooling, stopCooling, adjustFanSpeed, getNoiseLevel, init, str

**CPUCooler** 6 6 -> Fan, Heatsink, Component
- Поля: name, manufacturer, coolerType, fanSize, fans, heatsink, tdp
- Методы: coolComponent, getFanSize, getType, getTDP, init, getDiagnostics

**Fan** 6 7
- Поля: fanId, size, maxRPM, currentRPM, noiseLevel, isRunning
- Методы: start, stop, setSpeed, getSpeed, getNoiseLevel, init, str

**Heatsink** 4 4
- Поля: material, surfaceArea, heatpipes, thermalConductivity
- Методы: getMaterial, getSurfaceArea, getHeatpipes, init

***

### Network_classes

**NetworkAdapter** 7 9 -> NetworkPacket, Component
- Поля: name, manufacturer, speedMbps, isWireless, macAddress, ipAddress, packetsReceived, packetsSent, powered
- Методы: sendPacket, receivePacket, connect, disconnect, getSpeed, getMacAddress, powerOn, powerOff, init

**NetworkPacket** 5 6
- Поля: sourceIP, destinationIP, data, size, timestamp
- Методы: getSource, getDestination, getData, getSize, init, str

***

### Peripherals_classes

**Monitor** 9 9 -> Component
- Поля: name, manufacturer, price, resolutionWidth, resolutionHeight, refreshRate, panelType, diagonalInches, powered
- Методы: displayImage, changeResolution, changeRefreshRate, getWidth, getHeight, powerOn, powerOff, getDiagnostics, init

**Keyboard** 5 5 -> USBDevice
- Поля: name, layout, isMechanical, hasBacklight, keyCount
- Методы: typeKey, connect, disconnect, init, str

**Mouse** 6 7 -> USBDevice
- Poля: name, dpi, buttonCount, wireless, isConnected, currentDPI
- Методы: click, setDPI, getDPI, connect, disconnect, init, str

**USBDevice** 4 4
- Поля: deviceName, usbVersion, transferSpeed, isConnected
- Методы: connect, disconnect, getTransferSpeed, init

***

### Audio_classes

**SoundCard** 6 8 -> AudioCodec, Component
- Поля: name, manufacturer, price, codec, channels, snrDB, powered
- Методы: playSound, recordSound, adjustVolume, getChannels, powerOn, powerOff, getDiagnostics, init

**AudioCodec** 4 5
- Поля: codecName, bitDepth, sampleRate, supportedFormats
- Методы: encode, decode, getBitDepth, getSampleRate, init

***

### Case_classes

**ComputerCase** 7 7 -> CaseFan, Component
- Поля: name, manufacturer, price, formFactor, maxGPULength, fans, usb3Ports
- Методы: installFan, getFormFactor, getMaxGPULength, getUSBPorts, powerOn, powerOff, init

**CaseFan** 5 5 -> Fan
- Поля: fanId, size, maxRPM, currentRPM, position
- Методы: start, stop, setSpeed, getPosition, init

***

### OS_classes

**OperatingSystem** 5 7 -> Process, FileSystem
- Поля: osName, version, filesystem, processes, running
- Методы: boot, shutdown, startProcess, killProcess, getName, isRunning, init

**FileSystem** 3 6
- Поля: type, files, totalSpace
- Методы: createFile, deleteFile, getFileSize, getType, getTotalSpace, init

**Process** 5 7
- Поля: pid, name, memoryUsage, cpuUsage, state
- Методы: start, terminate, getPID, getName, getMemoryUsage, init, str

***

### Computer (Main Class)

**Computer** 20 15 -> CPU, RAM, HardDrive, GraphicsCard, Motherboard, PowerSupply, CPUCooler, NetworkAdapter, SoundCard, ComputerCase, Monitor, Keyboard, Mouse, OperatingSystem
- Поля: computerName, systemOn, totalPowerConsumption, cpu, ram, storage, gpu, motherboard, psu, cooler, network, soundCard, chassisCase, display, keyboard, mouse, os, components
- Методы: assembleParts, powerOn, powerOff, restart, performCPUTask, renderGraphics, allocateMemory, saveToStorage, connectToNetwork, getSystemInfo, runDiagnostics, isOn, getName, init, destructor

***

### Component (Base Class)

**Component** 4 6
- Поля: name, manufacturer, price, powered
- Методы: powerOn, powerOff, isPowered, getName, getManufacturer, getPrice, getDiagnostics

***

## Диаграмма связей (основные ассоциации)

```
Computer
├── CPU (contains) ──> CPUCore, CPUCache
├── RAM (contains)
├── GPU (contains) ──> VRAM
├── GraphicsCard (contains) ──> GPU
├── Motherboard (contains) ──> BIOS, Chipset, PCIeSlot
├── PowerSupply (contains) ──> PowerRail
├── CPUCooler (contains) ──> Fan, Heatsink
├── HardDrive (contains) ──> Partition, StorageController
├── NetworkAdapter (uses) ──> NetworkPacket
├── SoundCard (contains) ──> AudioCodec
├── OperatingSystem (manages) ──> Process, FileSystem
├── Monitor (peripheral)
├── Keyboard (peripheral)
└── Mouse (peripheral)
```

***

## Статистика проекта

- **Всего классов**: 51
- **Базовых классов**: 2 (Component, HardwareException)
- **Наследников**: 20+
- **Композиций**: 15+
- **Агрегаций**: 10+
- **Тестов**: 420+
- **Покрытие кода**: 90%+

***

## Структура директорий

```
project/
├── include/
│   ├── cpu/           # CPU, CPUCore, ALU, ControlUnit, RegisterFile, CPUCache
│   ├── graphics/      # GPU, VRAM, GraphicsCard
│   ├── memory/        # RAM, MemoryModule, MemoryController
│   ├── storage/       # HardDrive, Partition, StorageController
│   ├── motherboard/   # Motherboard, BIOS, Chipset, PCIeSlot
│   ├── power/         # PowerSupply, PowerRail
│   ├── cooling/       # CoolingSystem, CPUCooler, Fan, Heatsink
│   ├── network/       # NetworkAdapter, NetworkPacket
│   ├── peripherals/   # Monitor, Keyboard, Mouse, USBDevice
│   ├── audio/         # SoundCard, AudioCodec
│   ├── case/          # ComputerCase, CaseFan
│   ├── os/            # OperatingSystem, FileSystem, Process
│   ├── exceptions/    # All exceptions
│   └── Computer.h     # Main computer class
├── src/               # Implementation files (.cpp)
├── tests/             # Google Test unit tests
└── CMakeLists.txt     # Build configuration
```