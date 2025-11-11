Setting up a project in CubeMX is fairly simple, just go through the following options:

1. New project
2. Select the board you're working with
3. Go to the Project Manager tab and give your project a name, remember to change the ToolChain/IDE to MakeFile
4. Change whatever pins are needed (can vary a lot) and click on "GENERATE CODE"
5. Open the folder with the project just created using Visual Studio Code (the whole folder, don't go inside of it).

Possible issues and potential solutions:
-If the code didn't actually generate, it might be that only options were changed and not pins, so please if you haven't changed any, change a pin to some role and then and undo it, as that will register as a modification and make the code actually generate)
-If the debugger doesn't start, giving issue related to gdb, go in the launch.json file and add this line somewhere between line 4 and line 15 (remember the comma at the end!): "gdbPath": "gdb-multiarch",

Also, keep the following in mind: CubeMX was written in Java, so it's going to be a pain to work with.
If there's only one thing you're taking from this team at the end of your education, please, don't write software in Java.
