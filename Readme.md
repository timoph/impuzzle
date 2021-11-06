# ImPuzzle 2

a version of 15-puzzle in which you can use your own photos as the puzzle. Originally written for the Nokia N900 phone.

Original version tagged as "legacy\_version"

## Compiling and running

Tested to build and run with Qt versions 5.12.8, 5.15.2 and 6.2

## Some info about the thing

The swedish translation is made with google translate so it most likely will provide
a few giggles for our swedish friends.

Translations are not set automatically. By default the game is in english. This can
be changed by command line parameter.

The application's own image browser only show the first 30 images it finds from
home path or current directory (including subdirectories). A specific image can
be set by using a command line option. 

Available command line options:

>  -s, --shuffles <count>            Define how many moves are made to shuffle
>                                    the pieces.

>  -d, --directory <directory path>  Define path where to look for images

>  -i, --image <file path>           Define image used to create the puzzle

>  -l, --language <en|fi|sv>         Define language.

>  -f, --fullscreen                  Start in fullscreen.

>  -h, --help                        Show help.

