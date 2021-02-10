# VHF-Sim

Copyright (c) 2021 Mario Konrad (mario.konrad@gmx.net)


Simulates a VHF radio for maritime navigation. The purpose of this software
to learn and practice the handling of radio infrastructure, usually found
on pleasure crafts.

It is also useful for SRC exams.

## Usage

The VHF simulator has one main window, showing the simulated VHF and a menu.
Use the buttons shown on the simulation to use it. If it is unclear where
usable areas are, use the menu `View / Show Buttons` to reveal all existing
buttons.

![vhf-sim](doc/vhf-sim.png)

### Features

- Simulation (partial) of an Simrad-RD68 VHF radio.
- All "buttons" displayed by the VHF front end are usable.
- Simulates an "empty" (factory reset) device as well.
- Contains a pseudo GPS unit to provide position, or enter it manually.
  Also simulates the loss of GPS signal.
- Use DSC function to do routine, safety, urgency calls, as
  well as distress alerts.
- Simulates DSC calls from a shore station.
- Communicate with other instances of the simulator, either on
  the same PC or over network, to simulate DSC calls and distress
  alerts.
- Fullscreen mode.
- Exam mode. This disables repeated distress alerts. Useful for exam
  situations (SRC).

### Menu

- `File / VHF Preferences`
  : general configuration of the simulator
- `File / GPS`
  : a primitive GPS simulation to feed static positional information
    to the VHF simulation
- `File / Exam Mode`
  : toggles between normal mode and exam mode. In exam mode, the VHF simulation
    behaves slighly different, for example distress alerts are not being sent
    every 5 minutes. This is particularly useful in an exam situation for the SRC.
- `Communication / Control Center`
  : simulates a communication peer, to receive and confirm DSC calls sent by the
    simulation. Also able to call the simulation. This is a very rudimentary
    simulation of a sea or shore station and is useful to learn how the VHF
    simlator behaves in DSC mode.
- `Communication / Connection Preferences`
  : configuration dialog for communication over TCP to a communications hub.
- `Communication / Open Connection`
  : Opens a configured communication to the communications hub.
- `Communication / Close Connection`
  : closes an open communication channel.
- `Communication / Communications Hub`
  : control of a communications hub. Useful if you like to use two or more VHF
    simulators at the same time, either local or over a network. The communication
    is TCP based. This is useful if you like to learn how the simulation behaves
    with DSC calls of all types in conjunction with other sea stations. Only one
    enabled communications hub is necessary to use this feature.
- `View / Fullscreen`
  : toggles between window mode and fullscreen
- `View / Show Buttons`
  : reveals all buttons of the simulation. All areas usable by the mouse will
    be highlighted in red color.
- `Help / About`
  : information about the simulation
- `Help / About Qt`
  : information about Qt

### Keyboard

It is possible to use the computer keyboard, following keys are defined:

| Key            | Function within the Simulator |
|----------------|-------------------------------|
| 0 ..9          | 0 .. 9                        |
| F1 .. F4       | Soft 1 .. Soft 4              |
| RETURN         | E                             |
| ENTER (numpad) | E                             |
| ESC            | C                             |

### Commandline Parameters

~~~
--mmsi MMSI : defines a non-modifyable MMSI for the VHF instance
~~~

## License

See file [LICENSE](License)

## Technical

**Note:** this is direct successor of the old implementation from 2007 (which was
  for free but not open source). The old version was implemented in
  `wxWidgets-2.8.12` and `OpenAL 1.15`. The current
  software is not a complete overhaul, but merely a port with minor refactoring.
  There is however, no guarantee of compatibility.

Tested build environments: Linux, Windows (mingw), currently the packaging is done
only for Windows.

Formatted using `clang-format-3.7`.

Language translations done using Qt's `linguist`, language files in directory `lang`.

The software is written in C++ (some C++11) and Lua 5.1.1


### Build on Linux

Tested on Ubuntu 16.04, Ubuntu 18.04, Ubuntu 20.04

#### Requirements

- C++ Compiler, current supported/tested: GCC 4.9, GCC 5.3, GCC 7, GCC 8, GCC 9, GCC 10
- Qt 5.5 (or newer)
- cmake 3.12
- git 2.6

#### Build

The environment variable `QTHOME` must be set properly.

~~~{.sh}
$ git clone https://github.com/mariokonrad/vhf-sim
$ mkdir build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Release ../vhf-sim
$ make -j8
~~~

#### Using Docker

There are docker files for various distributions and scripts to create and use
the containers.

Example: prepare Ubuntu 20.04 container and build the application:
~~~{.sh}
$ tools/docker/build.sh ubuntu2004
$ tools/build-vhf-docker.sh ubuntu2004
~~~

The result is built in the directory `build-ubuntu2004`.


### Build on Windows

Tested on Windows 7 Professional 32bit

#### Requirements

Packages used to build:

- Git 2.7.0
- CMake 3.12
- NSIS 2.50
- Qt 5.5 MinGW 4.9

#### Build

The environment variable `QTHOME` must be set properly.

On the console (`cmd.exe`, all necessary paths must be in order):

~~~{.bat}
> git clone https://github.com/mariokonrad/vhf-sim
> mkdir build
> cd build
> cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..\vhf-sim
> cmake --build .
> cpack
~~~


### Build on OSX

Tested on Sierra, Catalina, Big Sur

#### Requirements

Packages used to build:

- Git 2.19.0
- CMake 3.12
- Qt 5.10

#### Build

The environment variable `QTHOME` must be set properly.

On the console:

~~~{.sh}
> git clone https://github.com/mariokonrad/vhf-sim
> mkdir build
> cd build
> cmake -DCMAKE_BUILD_TYPE=Release ../vhf-sim
> cmake --build .
> cpack -G DragNDrop
~~~

