# Build dc-viewer on Windows

### Prerequisites

* Visual Studio 2013
* [CMake](https://cmake.org/)
* [DirectX 9 SDK June 2010](http://www.microsoft.com/en-us/download/details.aspx?id=6812)
* [Windows 8.1 SDK](http://msdn.microsoft.com/en-us/windows/desktop/bg162891.aspx) (for DirectX 11)

---

### Get the source code

* clone the dc-viewer repository into a folder on your machine.

https://github.com/decentralised-project/dc-viewer

---
### Building Boost

* get the latest boost source package from the [Boost Website](http://www.boost.org/) as a ZIP file.
* unzip the package to a folder with a short path. (eg. c:\boost_1_60_0)
* double click bootstrap.bat in the root boost folder, to generate b2 and bjam files.
* open a Visual Studio command prompt as an Administrator. (not the standard cmd console)
* cd into the root boost folder containing bootstrap.bat and execute the command below:

```
b2 --build-dir="C:\boost-vc12" toolset=msvc-12.0 --build-type=complete --with-date_time --with-thread --with-atomic
```
* copy the contents of c:\boost_1_60_0\stage\lib to c:\boost_1_60_0\lib
* delete the stage folder, and the c:\boost-vc12 folder.
* set the required environment variables with the commands below:
```
setx BOOST_ROOT c:\boost_1_60_0 /m
setx BOOST_INCLUDEDIR c:\boost_1_60_0\boost /m
setx BOOST_LIBRARYDIR c:\boost_1_60_0\lib /m
```

---
### Building OgreDeps

* clone the OgreDeps repository below into dc-viewer/dependencies/ogredeps

https://bitbucket.org/cabalistic/ogredeps

* open a console (cmd) and type `cmake-gui`
* in "where is the source code" box, select the full path to the cloned ogredeps folder.
* in "where to build the binaries" box, enter the path to the output folder (eg. c:\dc-viewer\build\ogredeps)
* click the configure button, and then when it's finished, the generate button.
* go to the folder dc-viewer/build/ogredeps and open OGREDEPS.sln in Visual Studio.
* build the solution in Release and Debug configurations.
* right click the INSTALL project and build, in Release and Debug.

---

### Building Ogre

* clone the Ogre repository below into dc-viewer/dependencies/ogre

https://bitbucket.org/sinbad/ogre/

* open a console (cmd) and type `cmake-gui`
* in "where is the source code" box, select the full path to the cloned ogre folder.
* in "where to build the binaries" box, enter the path to the output folder (eg. c:\dc-viewer\build\ogre)
* click the configure button.
* Tick OGRE_STATIC, and set OGRE_DEPENDENCIES_DIR to c:/dc-viewer/build/ogredeps/ogredeps (where ogredeps installed itself to)
* 

