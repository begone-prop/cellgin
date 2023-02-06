# A cellular automata engine on an infinite plane

![showcase](./media/showcase.mp4)

## Description

This project is intended to be a cellular automata engine capable of custom rules.
It provides an infinite plane using a specialized [hashing function](https://www.forceflow.be/2013/10/07/morton-encodingdecoding-through-bit-interleaving-implementations/),
based on a [space-filling](https://en.wikipedia.org/wiki/Z-order_curve) curve.

At the moment it simulates [Conway's Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life).
The project is still in early stages of development, and it may never be finished.
A similar project is [golly](https://golly.sourceforge.net/).

## Installation

```
git clone https://github.com/begone-prop/cellgin.git
cd cellgin
make
```

## Dependencies
```
GNU make
gcc
Raylib
```
