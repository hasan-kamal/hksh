# hksh

### Introduction
- `hksh` is a simple custom shell implemented in C
- `hksh` supports following basic shell features:
    1. `help`, `cd`, `history`, `exit` built-in commands
    2. Piping with `|`
    3. Redirecting stdin and stdout using `<` and `>`
    4. Handling erroneous commands
    
### Repository structure
-  `src/` contains the source code
- `build.sh` bash shell-script which can be used to build `hksh` from source

### Build
To build `hksh`,
- `cd` into the root of this repository and run command  `./build.sh`

### Example run
```bash
Hasans-MacBook-Pro:hksh hasankamal$ ./build.sh
hksh: hksh> pwd
/Users/hasankamal/Desktop/hksh
hksh: hksh> cd bin
hksh: bin> pwd
/Users/hasankamal/Desktop/hksh/bin
hksh: bin> ls
a.out
hksh: bin> cd ..
hksh: hksh> pwd
/Users/hasankamal/Desktop/hksh
hksh: hksh> history
1: pwd
2: cd bin
3: pwd
4: ls
5: cd ..
6: pwd
7: history
hksh: hksh> ls
README.md        history_hksh.txt
bin            src
build.sh
hksh: hksh> ls | head -2
README.md
bin
hksh: hksh> echo "Hello world" > output.txt
hksh: hksh> cat output.txt
"Hello world"
hksh: hksh> wc < output.txt         
1       2      14
hksh: hksh> unknown_command
-hksh command not executable: unknown_command
hksh: hksh> help

help--
1-help : lists commands available in this shell
2-cd : change current directory
3-history : show history of previously entered commands
4-exit : exit the shell

hksh: hksh> exit
```
This example run demonstrates the features mentioned in the _Introduction_ section above
