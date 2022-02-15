# TeamProject
Team software project for college.
A platformer game made using raylib and C++.

# Instructions for Ubunutu 
Once Ubunutu is setup VM or otherwise:
1. install VS Code
2. get C/C++ extenstions 
3. pull the repo 
4. run the below command in terminal
- sudo apt install build-essential git
5. run the below command in terminal
- sudo apt install libasound2-dev mesa-common-dev libx11-dev libxrandr-dev libxi-dev xorg-dev libgl1-mesa-dev libglu1-mesa-dev
6. run the below command in terminal (Note: You can use Vim as well)
- nano ~/.bashrc

7. It should open a file. Add the below command to the bottom of the file replacing (your_path_to_the_project) with the path to TeamProject from home 
- export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/(your_path_to_the_project)/TeamProject/lib/
    
8. Restart all terminals(Including VSCode).
- You can now build the project. Just enter the command "make" from within the directory TeamProject. The client will crash after a few seconds with no server.
- You can run ./server.exe to start a server 
# ALSO DO NOT COMMIT THE FILES FROM THE WINDOWS BUILD FILES
- stuff to do
- terminate client when game ends
- add classes 
- score
- without no server crashes