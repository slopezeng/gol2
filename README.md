# Game of life

This repo contains an implementation of Game of life in 64-bit integer space.
It was written in Visual Studio 2019 (preview) and contains the following
projects:
 - gollib
	This is the main game (static) library where the implementation of the core of 
	game of life is written.
 - golcommandline
	Thie is a command line execulable that reads stdin and writes to stdout. It
	expects Life1.06 format. in powershell, the application can be invoked like this:
	  Get-Content .\test_input.life | .\x64\Debug\golcommandline.exe > output.life
	from the top level folder after building the solution.
 - golwrapper
	This is a dll project that wraps the gollib into a python module so that it can 
	be used by a python program
 - golui
	This python project provides a visual interface for the game (using pygame). It 
	creates a random list of points and does not use Life1.06 as input at the moment.
	Move the camera with 'wasd'.

To build the solution define a PYTHON_ROOT environment variable and point it to the 
python used by Visual Studion, e.g. in:
		C:\Program Files (x86)\Microsoft Visual Studio\2019\Preview\Common7\IDE\Extensions\Microsoft\Python\Miniconda\Miniconda3-x64

The python environment within the solution must have the following modules installed:
 - numpy
 - pip
 - pygame