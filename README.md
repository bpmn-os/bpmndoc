# bpmndoc

A tool to generate a documentation from a BPMN model. The tool includes BPMN documentation and extension elements` into the generated documentation. It includes the diagram for each (sub)process if available.

## Dependencies

bpmndoc requires Xerces-C++ 3.2.x. and bpmn++.

On Ubuntu Linux Xerces can be installed by
```sh
sudo apt install libxerces-c-dev
```

You can obtain `bpmn++` from https://github.com/bpmn-os/bpmnpp.

## Build the program

The program is built like a typical CMake project. A normal build will look something like this (output omitted):

```sh
 ~/bpmndoc$ mkdir build
 ~/bpmndoc$ cd build
 ~/bpmndoc/build$ cmake ..
 ~/bpmndoc/build$ make
 ```

This creates an executable `bpmndoc` in the project directory.

## Usage

You can use the program as running
```sh
./bpmndoc <fileName> [-o <outputDir>] [-i <imageDir>]
```
where
- `<fileName>` is the file name of the BPMN model file,
- `<outputDir>` is the path to the desired output directory,
- `<imageDir>` is the path a directory where the diagrams of the model can be found.

The main diagram is assumed to have the path `<imageDir>/<baseName>.svg` where `<baseName>` is the base name of the BPMN model file, i.e. if `some_path/mymodel.bpm`, the `mymodel` is the base name. For all subprocesses, the diagram is assumed to have the path `<imageDir>/<baseName>-<nodeId>.svg` where `<baseName>` is the base name of the BPMN model file and `<nodeId>` is the `id` of the subprocess.

## License

MIT

Copyright (C) 2024 Asvin Goel
