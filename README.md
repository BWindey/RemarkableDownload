# RemarkableDownload
My tool to easily download files from the Remarkable tablet over USB.
First I made a bash-script, which worked, but then I wanted to do recursive downloading.
That's not something I wanted to do in bash anymore, so I started on a C++ project.

If you want a simple bash-script that downloads all the files under a single folder, by typing its name,
use the bash-script. If you want recursive behaviour, then you'll need to use the C++ program.

- Collections of other Remarkable projects: https://github.com/reHackable/awesome-reMarkable
- Comparable project in Python: https://github.com/cherti/remarkable-cli-tooling

## Table of contents
- [How to use bash script](#how-to-use-bash-script)
- [How to use C++ binary](#how-to-use-c-binary)
- [How to install bash script](#how-to-install-bash-script)
- [How to install C++ binary](#how-to-install-c-binary)
- [How it works](#how-it-works)


## How to use (bash-script)
Connect your tablet via USB to your computer, and make sure to have installed ssh-keys: 
https://remarkable.guide/guide/access/ssh.html
On your Remarkable, go to `settings > storage` and enable the USB web interface.

Now you can use the command on your pc as follows
```bash
rmDown [-h] [-v] <folder>
```
where you substitute `<folder>` by the actual name you want to download.
This will create a new folder called `<folder>_rm/`, and it will put all documents that are in the `<folder>` on your Remarkable, inside that new folder, all converted to pdf's.

If the `<folder>_rm/` directory already exists in your current working directory, then the script will ask for 
confirmation, as it is possible that the script would overwrite already existing files.

The `-f` flag downloads single files matching the given name\
The `-h` flag prints a help-message and exits without actually downloading.\
The `-v` flag turns on verbose mode, which prints each file's UUID alongside its name.
The `-r` flag will enable recursive downloading of
subdirectories.


## How to use (C++ binary)
For ease of use I renamed the binary to `rmDown` in my shell (see [#how-to-install](#how-to-install-c-binary)).

Now you can follow the bash-instructions, as they are the same.
The C++ binary does have an extra `-r` flag, which downloads all the subfolders recursively as well.
This recursive mode will create a traditional file-structure on your pc, 
where each sub-folder will also have the '_rm' postfix.

There is also an extra '-f' flag, which will download single files instead of entire folders.


## How to install (bash-script)
There are 2 options for now, either you clone this repo on your system, and make a sym-link for rmDown.sh to somewhere in your path (like `~/.local/bin`), 
or you only download the rmDown.sh script, and put it somewhere in your path.


## How to install (C++ binary)
Clone this repository and `cd` into it:
```shell
git clone git@github.com:BWindey/RemarkableDownload.git
cd RemarkableDownload
```

Now make a directory called `build` and `cd` into it, here the build-files will be placed:
```shell
mkdir build
cd build
```

Now use cmake to generate the build-files:
```shell
cmake ..
```
It is possible that you still have to download cmake, then please do that first. \
It is also possible that you get an error about your cmake-version being too low. 
If that is the case, open `../CMakeLists.txt` and edit the `cmake_minimum_required` to your version. 
I tested v3.22, which worked for me, probably even older versions will work too.

When cmake is done, you then run make to create the `RemarkableDownload` binary:
```shell
make
```

You can execute it with `./RemarkableDownload`, and of course you can rename and move the binary as you wish.


## How it works
Apparently you can just make an HTTP request to your remarkable: `http://<ip>/download/<file-UUID>/placeholder`, and it will send a pdf. 
For some more smartiness, this script also ssh's in your remarkable to know which UUIDs it should download, using grep.
