Logic gate simulator in C++ using FOX.

![image](https://danieljon.es/media/foxlogicgates1.png)

Video of program: https://danieljon.es/posts/media/post115_3.mp4

Click on an icon and left-click anywhere on the canvas to place that gate (or input/output).

Select a gate by left-clicking on it and press delete to delete that gate and its links.

Hold shift and left-click and drag a link to another gate to connect them.

Click on a gates input to highlight that particular link, press the delete key to delete that link.

Right-click on an input to toggle it on and off.

You can save to an XML file by clicking the save button and selecting a name.

You can also load from an xML file by clicking load.

Some example circuits are provided in the examples/ directory.


# build
You need the FOX toolkit and pugixml installed. On arch install 'fox' and 'pugixml' packages.
```
cmake CmakeLists.txt
make
```
The binary is in bin/
