POSIX Shell
=================
This is a from-scratch implementation of the POSIX Shell in C using only system calls and basic C libraries except one command which uses curses.

This supports most Unix commands with some having custom implementations while others are called directly in Unix. A few custom commands have also been added.

# Setting Up
Please note this is for Ubuntu/debian
```
git clone https://github.com/manangoel99/Shell.git
cd Shell
sudo apt-get install libncurses5-dev
make
./shell
```

Use ```make clean``` to remove all files.