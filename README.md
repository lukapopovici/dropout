## Introduction

This program was made for my OOP course in UNI. It's a simple 2D game made in C++ with the use of the Standard Library and SDL. 

## Structure

This game displays multiple features of Object-oriented programming, the most notable being polymorphism with the Entities vecotr inside the Arena class. 
It also uses multiple design patterns such as singleton, state machine and observer. 
The update function for the AI also uses the A* algorithm and is made threading safe with the use of the [lock_guard](https://en.cppreference.com/w/cpp/thread/lock_guard) toa void destroying the timers and exploding the game.
It also has custom UI.

## Video

I've recorded the project milestones of making the game [here](https://youtu.be/T90d7CCsZw0). Feel free to like and subscrive! 

## UML Diagram

Below, there is an abstractized UML diagram representation of the relationship between the classes.

The diagram itself is not wholly accuracte as I made it for the purpose of a short presentation.
![Screenshot from 2024-01-21 13-48-18](https://github.com/lukapopovici/dropout/assets/128390767/face8445-5434-4bf9-85d1-607db0cc140a)

# Story concept

![image](https://github.com/lukapopovici/dropout/assets/128390767/3a04357f-cb5b-493d-9ac1-4de55279473f)


In this game you are a DROPOUT being hunted by academics.

# Compiling

Simply run the
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
to remove the object files and the compiled executable.
