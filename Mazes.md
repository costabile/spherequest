How the Maze Works

# Introduction #

Hi! I got a basic maze and maze reader going. This wiki will explain basically how it all works (as of now)


# Details #

Okay so, the maze should be read from a file but we don't have such luxuries at the moment so as of now I made a generateMaze() function. If you can't understand the function it basically creates the following matrix

1 1 0 1 1 1,
1 0 0 1 0 1,
1 0 1 0 0 1,
1 0 0 0 1 1,
1 0 1 0 0 1,
1 1 1 1 2 1,
(I hate wiki auto-formating so much >:( this matrix should be on separate lines )

Where 1 = a wall (very primitive right now) and 2 = a wise man.

The wall and wise men primitives are defined in drawHorizontalWall() and drawWiseMen() respectively (you'll note I started some other draw functions which I hope to fill in later, also the wall is far too boring as it stands right now).

Moving onwards. In the renderScene() command there are the for loops which read through the matrix, check what needs to be drawn and then calls that draw command.

That's All! Pretty simple now to write your own draw commands and add extra stuff to the maze. Also note that the renderScene() is set up so that we start in the middle of this maze, but that can easily be changed!

Have fun and send me any questions/concerns!

# Reading from a File #

The actual code that reads the maze from a file is in maze.cpp.  Calling readMaze() is done in the initScene() method in the main game file.

I'm working on making reading mazes from a file a little more flexible so that additional levels can be added, with varying sizes.