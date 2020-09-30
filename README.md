# Snake-SFML
The snake is written in pure C++ using the SFML graphic library.

How to play? (Windows, Visual Studio)
1.	Download ZIP archive.
2.	Download from https://www.sfml-dev.org/download.php the newest version of the graphics library "SFML". This project uses version 2.5.1.
3.	Let's archive it in any directory convenient for you. Open the “SFML<version>/include” folder. We copy the address of this directory.
4 Open the *.sln file, compile it and click Project -> Properties-> C/C++. In the tab "Additional directories of included files" insert the copied address.
5.	Open the “SFML<version>/lib” folder. We copy the address of this directory. Go to Visual Studio, click on "Linker", select "Additional Library directories" and paste the copied address.
6.	Then Linker->Input. In the "Additional Dependencies" we add the library data:
•	sfml-system.lib
•	sfml-window.lib
•	sfml-graphics.lib
7.	Open the project folder, then open the Debug folder, where you need to put all dll files from the “SFML<version>/bin” folder.
8.	Set Debug x86 configuration
9.	Run and enjoy ^^
