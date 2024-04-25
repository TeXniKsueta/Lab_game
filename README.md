# PING PONG game, the essence of the game is to hit the ball with a platform.


To implement this game, you need to install STM32CubeIDE and STM32CubeMX. After that, open the project in STM32CubeMX. It is also necessary to study the principle of operation of the LED matrix and the principle of operation of the SPI, this information is freely available.
The panel used in the project has a dimension of 16x32 , the name of the matrix is P10

![image](https://github.com/TeXniKsueta/Lab_game/assets/152018745/4e949407-0c58-4f5e-a4e2-e9675a240862)

                                                     
![image](https://github.com/TeXniKsueta/Lab_game/assets/152018745/f7c2b986-829f-447a-9102-71f0d8ff357d)

A window will appear, which is shown in the picture above

Next, you need to connect the matrix to the STM32F429ZIT6U board, the connection will be shown below
![image](https://github.com/TeXniKsueta/Lab_game/assets/152018745/83182b65-b8ee-458c-8381-946b3c0b37bb)

Example of connecting an LED panel to the STM32F429ZIT6U

![image](https://github.com/TeXniKsueta/Lab_game/assets/152018745/b033dc79-a5de-4985-9d34-03389b2ed969)


After connecting the board to the panel correctly, it is necessary to run the project in the main.c file and upload it to the board, and check the robotability. An example of working on a video in Google drive at the 

link:https://drive.google.com/drive/folders/1WTH3njzC6gQlhZ72-iNvsB3M3VQNmIr5?usp=drive_link
