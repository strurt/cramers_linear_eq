The C++ program to solve algebric equations >> output.cpp
The C++ program with the Emscripten bindings >> presentation.cpp
The code in text format is stored in output_text and presentation_text respectively.

The compiled javascript module >> presentation.js

To Download the Emscripten SDK >>
Clone or Download Zip from the emsdk Github Page >> https://github.com/emscripten-core/emsdk
Enter inside the emsdk
exectute the following command: emsdk install latest
To activate the latest SDK execute : emsdk activate latest
Please refer to the following website for more installation details: https://emscripten.org/docs/getting_started/downloads.html

after activation, to compile the C++ code into javascript and webassembly module, open the emsdk terminal and execute the following 
command : 
emcc -s WASM=1 -o presentation.js presentation.cpp -s "EXPORTED_RUNTIME_METHODS=['ccall','writeStringToMemory']"


//please make sure that you place the presentation.cpp file within your Emscripten folder; if not please specify the file path to the code.


