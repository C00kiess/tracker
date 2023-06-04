# Tracker

Tracker is a simple system monitoring software written in C++ using the SFML library. It provides real-time information about CPU usage, RAM usage, and disk usage.

## Prerequisites
- SFML: The Simple and Fast Multimedia Library
- Windows.h: Windows API header file
- pdh.h: Performance Data Helper header file
- psapi.h: Process Status API header file

## How to Use
1. Include the necessary SFML and Windows headers in your project.
2. Add the `SystemMonitor` class definition to your code.
3. Instantiate an object of the `SystemMonitor` class.
4. Call the `init()` method to initialize the system monitor.
5. Call the `run()` method to start monitoring the system.

```cpp
#include "core.h"

int main()
{
    SystemMonitor tracker;
    tracker.init();
    tracker.run();

    return 0;
}
```
## Class Members

### Constructors and Destructor
- `SystemMonitor()`: Constructor. Initializes the window and sets up the text properties.
- `~SystemMonitor()`: Destructor. Closes the query and releases the allocated resources.

### Methods
- `void init()`: Initializes the system monitor by opening the query and adding the necessary counters.
- `void makeWindowOnTop(sf::RenderWindow& window)`: Makes the window stay on top of other windows.
- `int getCurrentValue()`: Retrieves the current CPU usage value.
- `std::string formatMemorySize(DWORDLONG size)`: Formats the given memory size in bytes to a human-readable string (e.g., "2.15 GB").
- `void run()`: Starts the system monitor's main loop, updating the window with the current system information.

### Private Members
- `PDH_HQUERY cpuQuery`: Handle for the CPU query.
- `PDH_HCOUNTER cpuTotal`: Handle for the CPU total counter.
- `PDH_HCOUNTER diskTotal`: Handle for the disk total counter.
- `sf::RenderWindow window`: SFML window object.
- `sf::Font font`: SFML font object.
- `sf::Text cpuText`: SFML text object for CPU usage.
- `sf::Text tramText`: SFML text object for total RAM.
- `sf::Text ramText`: SFML text object for used RAM.
- `sf::Text diskText`: SFML text object for disk usage.

Feel free to modify the code to suit your specific requirements.

## License
This software is released under the [MIT License](LICENSE).
