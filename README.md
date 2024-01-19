## Introduction

This program was made for my OOP course in UNI. It's a simple 2D game made in C++ with the use of the Standard Library and SDL. 

## Structure

This game displays multiple features of Object-oriented programming, the most notable being polymorphism with the Entities vecotr inside the Arena class. 
It also uses multiple design patterns such as singleton, state machine and observer. 
The update function for the AI also uses the A* algorithm and is made threading safe with the use of the [lock_guard](https://en.cppreference.com/w/cpp/thread/lock_guard) toa void destroying the timers and exploding the game.
It also has custom UI.

# Story concept

![image](https://github.com/lukapopovici/dropout/assets/128390767/3a04357f-cb5b-493d-9ac1-4de55279473f)


In this game you are a DROPOUT being hunted by academics.

# Compiling

Simply run  
```
"make"
```
 command inside the source folder and execute 
 ```
 ./my_program
 ```
Afterwards you can run
```
make clean

```
to remove the object files.
