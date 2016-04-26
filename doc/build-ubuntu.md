# Build dc-viewer on Ubuntu/Linux

### Prerequisites

```
sudo apt-get install cmake-gui
```
---

### Get the source code

* clone the dc-viewer repository into a folder on your machine.

https://github.com/decentralised-project/dc-viewer

---

### Building Ogre

* clone the Ogre repository below into dc-viewer/dependencies/ogre

https://bitbucket.org/sinbad/ogre/

* install dependencies for Ogre with the command below:

```
sudo apt-get install libfreetype6-dev libboost-date-time-dev libboost-thread-dev nvidia-cg-toolkit libfreeimage-dev zlib1g-dev libzzip-dev libois-dev libcppunit-dev doxygen libxt-dev libxaw7-dev libxxf86vm-dev libxrandr-dev libglu-dev
```

* open a terminal and type ```cmake-gui```
* select the root ogre folder as the source, and the build folder as the target.
* click the configure button and select "Unix Makefiles" as the build system, and ok.
* click the generate button.
* back in the terminal, cd into the build folder and type ```make && make install```

---