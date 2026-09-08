# USB Architecture Tree Explorer in C

This utility scans the system's USB bus topology directly from the kernel memory using `libusb-1.0`. It maps the physical parent-child layout (Buses, Hubs, and Ports) and stores the structural data inside a custom C `struct` array before filtering and rendering it to the user.

---

## Prerequisites

Before compiling the application, ensure your Linux package database is updated and the `libusb` development headers are installed.

```bash
sudo apt update
sudo apt install libusb-1.0-0-dev gcc
```

---

## Repository Contents

* `usb_tree.c` - Core C application containing memory mapping logic and `struct` management.
* `README.md` - Documentation for application usage.

---

## Compilation & Build

To compile the C source file, use the GNU Compiler Collection (`gcc`) and link the execution path to the structural `libusb-1.0` dynamic library flag (`-lusb-1.0`).

```bash
gcc usb_tree.c -o usb_tree -lusb-1.0
```

---

## Execution & Running

Because querying underlying physical address layouts requires raw memory device communication abstractions inside the Linux Kernel (`/dev/bus/usb/`), **administrative root access is mandatory**.

Run the compiled executable with superuser privileges:

```bash
sudo ./usb_tree
```

### Expected Output Example
```text
--- Displaying Saved USB Tree Structure ---
Bus 002: Device 001 | Path: Root | ID 1d6b:0003
Bus 001: Device 001 | Path: Root | ID 1d6b:0002
Bus 001: Device 002 | Path: Root -> Port 1 | ID 093a:2510 
Bus 001: Device 003 | Path: Root -> Port 3 | ID 0781:5580 [★ Flash Drive]
```

---

## Architectural Concept

The program acts as a localized memory abstraction map:
1. **`libusb_init`** sets a volatile runtime memory sandbox environment.
2. **`libusb_get_device_list`** polls active hardware descriptors from the OS kernel namespace.
3. **`parse_device_layout`** serializes volatile descriptor data, capturing topological path arrays and hardware IDs into an array of isolated local structures (`struct USBDeviceNode`).
4. The structural nodes decouple the target hardware interaction layer from logic handlers, ensuring predictable memory cycles.
