Logic gate simulator in C++ using FOX.

![image](https://danieljon.es/media/foxlogicgates1.png)

Click on an icon and left-click anywhere on the canvas to place that gate (or input/output).

Select a gate by left-clicking on it and press delete to delete that gate and its links.

Hold shift and left-click and drag a link to another gate to connect them.

Click on a gates input to highlight that particular link, press the delete key to delete that link.

Right-click on an input to toggle it on and off.

# install
You need the FOX toolkit installed. On arch install the 'fox' package.
```
cmake CmakeLists.txt
make
```
The binary is in bin/
