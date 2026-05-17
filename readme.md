# C Disk Scanner

A simple terminal-based disk usage visualizer written in C, using ncurses.

It scans a directory and generates a treemap to help you visualize what is taking up space on your disk.

Still a very experimental project with lots to improve on!

## Demo

![Project Demo](https://i.imgur.com/f6EMoNJ.gif)

## Installation

```bash
make
```

## Usage

```bash
./scanner [path]
```

If no path is provided, it defaults to the home directory.
