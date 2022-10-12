# Memory Leak Detector

Small project which aims to build a tool allowing me to detect memory leaks in C.

## Get started

Just include the **ld.h** header, then use the **malloc** / **free** functions normally. Finally, call the **show_leaks** function when you want to display the leaks (be careful it will also free them).

![leaks](https://user-images.githubusercontent.com/50496792/195443092-e9374ae0-74c3-4110-84ae-b7dbe805dd5d.png)
