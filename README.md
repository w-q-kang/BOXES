# BOXES

## Introduction
BOXES is a mildly esoteric/weird programming language. It is of course not intended for mainstream programming. Trying to solve a program using it should rather be viewed as a kind of brain-teaser. 
The main idea of Boxes is that a program consists of a set of small, independent units that communicate with each other via messages. I call these units ‘boxes‘.

So a program basically consists of a number of boxes, where each box has:
- a name
- a type specification
- one or more message responses
- a variable or a set of variables (depending on its type specification)

send:
- a send call specifies a tag followed by a parameter list (which may be empty)
- if a tag is sent, each box that specifies a message response for that tag executes the message actions associated with the tag
- if the send call specifies too many params, excess params are simply ignored

program execution is governed by a message queue.
In the beginning the message queue only contains the send call ‘start’ and sends it. Sending it may cause boxes to send other tags + params where each tag + params is regarded as one entry in the queue.
If all messages have been processed and the queue is empty the send call ‘final’ is added to the (empty) queue and sent and the appropriate messages (if any) are processed. ’final’ is only added once, so execution definitely stops after all actions/messages evoked (directly or indirectly) by ‘final’ have been processed.

To get acquainted with the programming style needed for BOXES programs you should look at some sample programs - see remarks below - and read the documentation (in PDF format).

The repository contains an interpreter written in C++ as well as sample programs.


## Build
You should compile the source code with C++17 or later. Note that I still prefer to include debug code and suggest you do the same when compiling the code. 

### Windows
You should have GCC (including the compiler g++) installed first. Then run the build script:

```batch
make.bat
```


After successfully compiling you should be all set. Try executing a sample program like so:

```batch
bin/Debug/BOXES.exe samples/hello_world.box
```

### macOS/Linux
This requires `clang++` to be installed. Run the build script:

```shell
./make.sh
```


After successfully compiling you should be all set. Try executing a sample program like so:

```shell
bin/Debug/boxes samples/hello_world.box
```

## Samples
The 'samples' directory lists the solutions to a few 'standard' tasks. Might be extended in the future... 

There is also a 'features' directory which contains small programs that mostly do not have a meaning in themselves but simply demonstrate the features of the language. You should make yourself familiar with the examples listed there and try to understand the results by running them to get a better understanding of the language.

## Issues
If you find that something is wrong or should be changed for whatever reason please submit an issue.
