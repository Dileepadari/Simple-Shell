[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/76mHqLr5)
# Description

0. This is final submission

1. This is a basic shell with commands like cd and ls renamed as warp and peek etc

2. The shell prints a prompt with username and system name and present working directory.

3. The home directory is shown as ~

4. The shell contains commands like warp (cd), peek (ls), seek (find) ,proclore (ps), 
 pastevents (history) and system commands using execvp.

5. The commands like peek, seek takes flags and executes according to them as like as ls command in termainal.

6. The I/O redirection is done. You can use < to give input to a command and > to store output to a file and >> to append the output to the file. It will also work with pipes.

7. pipes are impleented such that it the output of previous command be the input of next command

8. activities spawn by shell are shown when activities is given as input

9. signals are handled including ctrl + z, ctrl + c, ctrl + d.

10. fg and bg processes shifting is done

11. neonate is implemented to get latest process id continuously

12. iMan is implemented to get the manual of the given command

# How to Use

1. In the main directory, execute make command which basically compiles all the files.

2. An executable a.out will be created. execute it in the terminal

3. A proompt will be displayed and your shell is ready to Use.

4. click on ctrl + d or type exit to exit the shell

5. clcik on ctrl + c to stop the running foreground process

6. click on ctrl + z to send the running foreground to background and stop it.

# Assumptions

1. Assumed that the given flags are limited .

2. Assumed that the file format will be same as now

3. Assumed the required permissions for the folders provided

4. Assumed that seek , fg, warp, activities will not be given in piping as it cant run in child process

5. Assumed that the inputs are as given in the file not tricky.

6. Assumed internet is available while using iMan.
