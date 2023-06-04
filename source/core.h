#pragma once

#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <pdh.h>
#include <sstream>
#include <iostream>
#include "psapi.h"

#pragma comment(lib, "pdh.lib")

class SystemMonitor {
public:
    SystemMonitor();
    ~SystemMonitor();

    void init();
    void makeWindowOnTop(sf::RenderWindow& window);
    int getCurrentValue();
    std::string formatMemorySize(DWORDLONG size);
    void run();

private:
    PDH_HQUERY cpuQuery;
    PDH_HCOUNTER cpuTotal;
    PDH_HCOUNTER diskTotal;
    sf::RenderWindow window;
    sf::Font font;
    sf::Text cpuText;
    sf::Text tramText;
    sf::Text ramText;
    sf::Text diskText;
};

SystemMonitor::SystemMonitor()
    : cpuQuery(nullptr), cpuTotal(nullptr)
{
    window.create(sf::VideoMode(169, 100), "CPU", sf::Style::None);
    makeWindowOnTop(window);
    window.setFramerateLimit(4);
    window.setPosition(sf::Vector2i(0, 0));

    if (!font.loadFromFile("font.ttf")) {
        std::cout << "Failed to load font." << std::endl;
    }

    cpuText.setFont(font);
    cpuText.setCharacterSize(26);
    cpuText.setFillColor(sf::Color::White);
    cpuText.setPosition(18.f, 10.f);

    tramText.setFont(font);
    tramText.setCharacterSize(21);
    tramText.setFillColor(sf::Color::White);
    tramText.setPosition(18.f, 50.f);

    ramText.setFont(font);
    ramText.setCharacterSize(21);
    ramText.setFillColor(sf::Color::White);
    ramText.setPosition(18.f, 70.f);
}

SystemMonitor::~SystemMonitor() {
    if (cpuQuery) {
        PdhCloseQuery(cpuQuery);
        cpuQuery = nullptr;
    }
}

void SystemMonitor::init() {
    PDH_STATUS status = PdhOpenQuery(nullptr, 0, &cpuQuery);
    if (status != ERROR_SUCCESS) {
        std::cout << "Failed to open query. Error code: " << status << std::endl;
        return;
    }

    status = PdhAddCounter(cpuQuery, L"\\Processor(_Total)\\% Processor Time", 0, &cpuTotal);
    if (status != ERROR_SUCCESS) {
        std::cout << "Failed to add counter. Error code: " << status << std::endl;
        PdhCloseQuery(cpuQuery);
        cpuQuery = nullptr;
        return;
    }

    status = PdhAddCounter(cpuQuery, L"\\PhysicalDisk(_Total)\\% Disk Time", 0, &diskTotal);
    if (status != ERROR_SUCCESS) {
        std::cout << "Failed to add disk counter. Error code: " << status << std::endl;
        PdhCloseQuery(cpuQuery);
        cpuQuery = nullptr;
        return;
    }

    PdhCollectQueryData(cpuQuery);
}

void SystemMonitor::makeWindowOnTop(sf::RenderWindow& window) {
    HWND hwnd = window.getSystemHandle();
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
}

int SystemMonitor::getCurrentValue() {
    PDH_FMT_COUNTERVALUE counterVal;

    PdhCollectQueryData(cpuQuery);
    PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_LONG, nullptr, &counterVal);
    return static_cast<int>(counterVal.longValue);
}

std::string SystemMonitor::formatMemorySize(DWORDLONG size) {
    const int bufferSize = 20;
    char buffer[bufferSize];
    snprintf(buffer, bufferSize, "%.2f GB", static_cast<double>(size) / (1024 * 1024 * 1024));
    return buffer;
}

void SystemMonitor::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        int cpuValue = getCurrentValue();

        MEMORYSTATUSEX memInfo;
        memInfo.dwLength = sizeof(MEMORYSTATUSEX);
        GlobalMemoryStatusEx(&memInfo);
        DWORDLONG totalPhysMem = memInfo.ullTotalPhys;
        DWORDLONG physMemUsed = totalPhysMem - memInfo.ullAvailPhys;

        std::ostringstream oss;
        oss << "CPU: " << cpuValue << "%";
        cpuText.setString(oss.str());

        std::string totalRam = formatMemorySize(totalPhysMem);
        tramText.setString("RAM: " + totalRam);

        std::string formattedRam = formatMemorySize(physMemUsed);
        ramText.setString("Used: " + formattedRam);

        window.clear(sf::Color(33, 33, 33));
        window.draw(cpuText);
        window.draw(ramText);
        window.draw(tramText);
        window.display();
    }
}
