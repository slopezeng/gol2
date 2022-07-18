from golwrapper import init
from golwrapper import dest
from golwrapper import gen
import random
import pygame
import sys
import time
import math
import numpy as np

SCREEN_WIDTH = 800
SCREEN_HEIGHT = SCREEN_WIDTH 
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
GREY = (40, 40, 40)
BLOCK_SIZE = 15
SCREEN = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
GRIDLINE_THICKNESS = 1
X_OFFSET = 0 # +1 is going right
Y_OFFSET = 0 # +1 is going up

# will shorten num by converting it to scientific notation
# if needed
def shorten(num: int) -> str:
    if len(str(num)) >= 7:
        return "{:.1e}".format(num)
    return str(num)

def drawGrid():
    # 'clear' screen on every draw - necessary to do for offset stuff
    SCREEN.fill(BLACK)

    # boilerplate font stuff for rendering text
    myFont = pygame.font.SysFont('Comic Sans MS', BLOCK_SIZE // 4)
    myFont.set_bold(True)

    # draw vertical lines
    x = 0 
    numLinesDrawn_x = SCREEN_WIDTH // BLOCK_SIZE - 1
    farLeft = int(math.ceil(numLinesDrawn_x / 2)) * -1 + X_OFFSET
    while x < SCREEN_WIDTH:
        if x > 0:
            pygame.draw.line(SCREEN, GREY, (x,0), (x, SCREEN_HEIGHT), GRIDLINE_THICKNESS)
        textSurface = myFont.render(shorten(farLeft), False, WHITE)
        if x > 0:
            SCREEN.blit(textSurface, (x, 0))
        x += BLOCK_SIZE
        farLeft += 1

    # draw horizontal lines
    y = 0 
    numLinesDrawn_y = SCREEN_HEIGHT // BLOCK_SIZE - 1
    top = int(math.ceil(numLinesDrawn_y / 2)) + Y_OFFSET
    while y < SCREEN_HEIGHT:
        if y > 0:
            pygame.draw.line(SCREEN, GREY, (0,y), (SCREEN_WIDTH, y), GRIDLINE_THICKNESS)
        textSurface = myFont.render(shorten(top), False, WHITE)
        if y > 0:
            SCREEN.blit(textSurface, (0, y))
        y += BLOCK_SIZE
        top -= 1

# here we pass in an x,y coordinate assuming the origin is
# (SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2)
def translate(x, y):
    numLinesDrawn = (SCREEN_WIDTH / BLOCK_SIZE) - 1
    temp = int(math.ceil(numLinesDrawn / 2)) * BLOCK_SIZE
    origin = (temp, temp)
    realXCoord = origin[0] + BLOCK_SIZE * x
    realYCoord = origin[1] - BLOCK_SIZE * y
    return (realXCoord, realYCoord)
  

def drawPoint(x, y, color):
    trans = translate(x - X_OFFSET, y - Y_OFFSET)
    trans_x = trans[0]
    trans_y = trans[1]
    # pygame.draw.circle(SCREEN, WHITE, trans, math.ceil(BLOCK_SIZE / 3))
    rect = pygame.Rect(trans_x - BLOCK_SIZE + GRIDLINE_THICKNESS, \
                       trans_y - BLOCK_SIZE + GRIDLINE_THICKNESS, \
                       BLOCK_SIZE - GRIDLINE_THICKNESS, \
                       BLOCK_SIZE - GRIDLINE_THICKNESS)
    SCREEN.fill(color, rect)

# takes in list of tuples representing coords
def updateGameCells(prev, cur):
    prevSet = set(prev)
    curSet = set(cur)
    deadCells = []
    for tup in prevSet:
        if tup not in curSet:
            deadCells.append(tup)
    
    for tup in deadCells:
        drawPoint(tup[0], tup[1], BLACK)
    for tup in cur:
        drawPoint(tup[0], tup[1], WHITE)
    

def camUp(offset=1):
    global Y_OFFSET
    Y_OFFSET += offset
 
def camDown(offset=1):
    global Y_OFFSET
    Y_OFFSET -= offset

def camRight(offset=1):
    global X_OFFSET
    X_OFFSET += offset

def camLeft(offset=1):
    global X_OFFSET
    X_OFFSET -= offset


def moveCamera(moveSize):
  key_state = pygame.key.get_pressed()
  if key_state[pygame.K_w]:
    camUp(moveSize)
  elif key_state[pygame.K_s]:
    camDown(moveSize)
  elif key_state[pygame.K_d]:
    camRight(moveSize)
  elif key_state[pygame.K_a]:
    camLeft(moveSize)

# takes in the coord of a cell and centers it in the screen
def centerCell(x, y):
  # all that has to happen is offset must change
  # (0, 0) is centered right now
  global X_OFFSET, Y_OFFSET
  X_OFFSET += x
  Y_OFFSET += y


def run():
    N = 30

    # get random number in 64bit range, which will be our center point
    # Some unknown issues with using full 64bit range so use PETAByte for now
    PETABYTE = 1024*1024*1024*1024*1024
    MAXRANGE = PETABYTE
    
    randomValInRange = random.sample(range(-MAXRANGE+1, MAXRANGE),1)
    centerPoint = randomValInRange[0]    

    R = 6

    # random
    listOfTuples = []
    for i in range(0,N):
        listOfTuples.append(tuple(random.sample(range(centerPoint-R, centerPoint+R),2)))

    # my test sample
    #listOfTuples = [(0,0),(1,-1),(2,0),(0,-2),(0,1),(-1,0),(1,1),(-1,-1)]

    # glider
    #listOfTuples = [(0,0),(1,0),(-1,0),(1,1),(0,2)]

    # center screen
    npa = np.array(listOfTuples)
    floattuple = tuple(npa.mean(axis=0))
    center = tuple(map(int, floattuple))
    x,y = center
    centerCell(x,y)
    
    prev = []
    cur = init(listOfTuples)
    pygame.font.init()
    SCREEN.fill(BLACK)
    moveSize = 3
    MAX_GAME_LOOP_ITERATIONS = 1000000

    # main game loop
    for i in range(0,MAX_GAME_LOOP_ITERATIONS):
        # process events
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                dest()
                sys.exit()

        # draw base gridlines
        drawGrid()

        # update alive and dead cells
        updateGameCells(prev, cur)

        # get next generation
        prev = cur
        cur = gen()

        # tell pygame to update display
        pygame.display.flip()

        # update camera if necessary
        moveCamera(moveSize)

        # sleep a bit to allow frames to be seen (and reduce cpu load)
        time.sleep(.1)

run()
while True:
    time.sleep(10)


