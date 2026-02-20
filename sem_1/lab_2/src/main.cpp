#include "../include/Computer.h"
#include <iostream>

int main() {
    try {
        std::cout << "=== COMPUTER SIMULATION PROJECT ===" << std::endl << std::endl;

        Computer myPC("Gaming PC");

        std::cout << "Assembling computer..." << std::endl;
        myPC.assembleParts();

        std::cout << "\nPowering on..." << std::endl;
        myPC.powerOn();

        std::cout << "\nPerforming CPU tasks..." << std::endl;
        myPC.performCPUTask();

        std::cout << "\nAllocating memory..." << std::endl;
        myPC.allocateMemory(8);

        std::cout << "\nSaving data to storage..." << std::endl;
        myPC.saveToStorage("Important data");

        std::cout << "\nRendering graphics..." << std::endl;
        myPC.renderGraphics();

        std::cout << "\nConnecting to network..." << std::endl;
        myPC.connectToNetwork();

        std::cout << std::endl;
        myPC.runDiagnostics();

        std::cout << "\nShutting down..." << std::endl;
        myPC.powerOff();

        std::cout << "\n=== SIMULATION COMPLETE ===" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
