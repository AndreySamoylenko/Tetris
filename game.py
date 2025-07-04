import random
import time
import threading
import os
import pygame

WIDTH = 10
HEIGHT = 20

CELL_SIZE = 30
WINDOW_WIDTH = WIDTH * CELL_SIZE
WINDOW_HEIGHT = HEIGHT * CELL_SIZE



tetromino = [[0, 0] for _ in range(4)]
tetromino_type = 0
map_field = [[0 for _ in range(HEIGHT)] for _ in range(WIDTH)]
input_key = 5
next_tetromino = 0
tetrominos_count = 0
score = 0
level = 0
lines_total = 0
can_rotate = 1


curent_tetromino = 13
tetromino_line = [0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6]




def get_fall_delay(level):
    delays = [800, 700, 600, 500, 400, 350, 300, 250, 200, 150, 100]
    return delays[min(level, 10)]

def show_start_screen():
    screen.fill((0, 0, 0))
    font_title = pygame.font.SysFont("Arial", 48, bold=True)
    font_text = pygame.font.SysFont("Arial", 28)

    text1 = font_title.render("ТЕТРИС", True, (255, 255, 255))
    text2 = font_text.render("Нажмите ENTER для начала", True, (200, 200, 200))

    screen.blit(text1, ((WINDOW_WIDTH + 150) // 2 - text1.get_width() // 2, 180))
    screen.blit(text2, ((WINDOW_WIDTH + 150) // 2 - text2.get_width() // 2, 250))
    pygame.display.flip()
    wait_for_enter()

def show_game_over_screen():
    screen.fill((0, 0, 0))
    font_title = pygame.font.SysFont("Arial", 48, bold=True)
    font_text = pygame.font.SysFont("Arial", 28)
    font_small = pygame.font.SysFont("Arial", 24)

    text1 = font_title.render("ИГРА ОКОНЧЕНА", True, (255, 0, 0))
    text2 = font_text.render(f"Score: {score} ", True, (255, 255, 255))
    text3 = font_small.render("Нажмите ENTER для выхода", True, (200, 200, 200))

    screen.blit(text1, ((WINDOW_WIDTH + 150) // 2 - text1.get_width() // 2, 160))
    screen.blit(text2, ((WINDOW_WIDTH + 150) // 2 - text2.get_width() // 2, 230))
    screen.blit(text3, ((WINDOW_WIDTH + 150) // 2 - text3.get_width() // 2, 280))
    pygame.display.flip()
    wait_for_enter()

def wait_for_enter():
    waiting = True
    while waiting:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                exit()
            if event.type == pygame.KEYDOWN and event.key == pygame.K_RETURN:
                waiting = False
        time.sleep(0.05)

def draw_next_tetromino():
    base_x = WIDTH * CELL_SIZE + 20
    base_y = 50
    preview_size = 120
    block_size = 20

    pygame.draw.rect(screen, (255, 255, 255), (base_x - 10, base_y - 10, preview_size, preview_size), 2)

    types = [
        [[0, 0], [1, 0], [0, 1], [1, 1]],      # O
        [[-1, 0], [0, 0], [1, 0], [2, 0]],     # I
        [[0, 0], [1, 0], [2, 0], [2, 1]],      # L
        [[0, 0], [1, 0], [2, 0], [0, 1]],      # J
        [[1, 0], [2, 0], [0, 1], [1, 1]],      # S
        [[0, 0], [1, 0], [1, 1], [2, 1]],      # Z
        [[0, 0], [1, 0], [2, 0], [1, 1]],      # T
    ]

    if 0 <= next_tetromino <= 6:
        shape = types[next_tetromino]
        offset_x = base_x + (preview_size - 4 * block_size) // 2
        offset_y = base_y + (preview_size - 4 * block_size) // 2
        for x, y in shape:
            rect = pygame.Rect(offset_x + x * block_size, offset_y + y * block_size, block_size, block_size)
            pygame.draw.rect(screen, (217, 217, 217), rect)
            pygame.draw.rect(screen, (84, 82, 91), rect, 2)

def draw_ui():
    font = pygame.font.SysFont("Arial", 20)
    text_score = font.render(f"Score: {score}", True, (255, 255, 255))
    text_lines = font.render(f"Lines: {lines_total}", True, (255, 255, 255))
    screen.blit(text_score, (WIDTH * CELL_SIZE + 20, 200))
    screen.blit(text_lines, (WIDTH * CELL_SIZE + 20, 230))

def out():
    screen.fill((0, 0, 0))
    
    for x in range(WIDTH):
        for y in range(HEIGHT):
            if map_field[x][y]:
                rect = pygame.Rect(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE)
                pygame.draw.rect(screen, (217, 217, 217), rect)
                pygame.draw.rect(screen, (84, 82, 91), rect, 3)

    pygame.draw.rect(screen, (255, 255, 255), (0, 0, WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE), 2)

    draw_next_tetromino()
    draw_ui()
    pygame.display.flip()

def clear_lines():
    global lines_total, score
    lines_cleared = 0
    for y in range(HEIGHT - 1, -1, -1):
        line_full = 1
        for x in range(WIDTH):
            if not map_field[x][y]:
                line_full = 0
                break

        if line_full:
            lines_cleared += 1
            for x in range(WIDTH):
                map_field[x][y] = 0
        elif lines_cleared > 0:
            for x in range(WIDTH):
                map_field[x][y + lines_cleared] = map_field[x][y]
                map_field[x][y] = 0

    out()
    lines_total += lines_cleared
    if lines_cleared == 4:
        score += 400
    return lines_cleared * lines_cleared * 100

def is_valid_position(tetromino_pos):
    for i in range(4):
        x, y = tetromino_pos[i]
        if x < 0 or x >= WIDTH or y >= HEIGHT:
            return 0
        if y >= 0 and map_field[x][y]:
            return 0
    return 1

def fall(tetromino_pos):
    for i in range(4):
        x, y = tetromino_pos[i]
        if y >= 0:
            map_field[x][y] = 0

    new_pos = [[tetromino_pos[i][0], tetromino_pos[i][1] + 1] for i in range(4)]

    if is_valid_position(new_pos):
        for i in range(4):
            tetromino_pos[i][1] += 1
            map_field[tetromino_pos[i][0]][tetromino_pos[i][1]] = 1
        out()
        return 1
    else:
        for i in range(4):
            if tetromino_pos[i][1] >= 0:
                map_field[tetromino_pos[i][0]][tetromino_pos[i][1]] = 1
        return 0

def choose_tetromino():
    global curent_tetromino, tetromino_line
    
    if curent_tetromino == 13:
        random.shuffle(tetromino_line)
        curent_tetromino = -1
    
    curent_tetromino += 1

    return tetromino_line[curent_tetromino]
    
def add_tetromino(type_, tetromino_pos):
    global tetromino_type, tetrominos_count
    tetromino_type = type_
    for i in range(4):
        tetromino_pos[i][0] = 0
        tetromino_pos[i][1] = 0

    if type_ == 0:  # O
        tetromino_pos[0] = [4, 0]
        tetromino_pos[1] = [5, 0]
        tetromino_pos[2] = [4, 1]
        tetromino_pos[3] = [5, 1]
    elif type_ == 1:  # I
        tetromino_pos[0] = [3, 0]
        tetromino_pos[1] = [4, 0]
        tetromino_pos[2] = [5, 0]
        tetromino_pos[3] = [6, 0]
    elif type_ == 2:  # L
        tetromino_pos[0] = [4, 0]
        tetromino_pos[1] = [6, 0]
        tetromino_pos[2] = [5, 0]
        tetromino_pos[3] = [6, 1]
    elif type_ == 3:  # J
        tetromino_pos[0] = [4, 0]
        tetromino_pos[1] = [5, 0]
        tetromino_pos[2] = [6, 0]
        tetromino_pos[3] = [4, 1]
    elif type_ == 4:  # S
        tetromino_pos[0] = [5, 0]
        tetromino_pos[1] = [5, 1]
        tetromino_pos[2] = [4, 1]
        tetromino_pos[3] = [6, 0]
    elif type_ == 5:  # Z
        tetromino_pos[0] = [4, 0]
        tetromino_pos[1] = [5, 0]
        tetromino_pos[2] = [5, 1]
        tetromino_pos[3] = [6, 1]
    elif type_ == 6:  # T
        tetromino_pos[0] = [4, 0]
        tetromino_pos[1] = [5, 0]
        tetromino_pos[2] = [6, 0]
        tetromino_pos[3] = [5, 1]

    if not is_valid_position(tetromino_pos):
        return 0

    for i in range(4):
        x, y = tetromino_pos[i]
        map_field[x][y] = 1

    out()
    tetrominos_count += 1
    return 1

def move(tetromino_pos, dx):
    for i in range(4):
        x, y = tetromino_pos[i]
        if y >= 0:
            map_field[x][y] = 0

    new_pos = [[tetromino_pos[i][0] + dx, tetromino_pos[i][1]] for i in range(4)]

    if is_valid_position(new_pos):
        for i in range(4):
            tetromino_pos[i][0] += dx
            x, y = tetromino_pos[i]
            map_field[x][y] = 1
        out()
        return 1
    else:
        for i in range(4):
            x, y = tetromino_pos[i]
            if y >= 0:
                map_field[x][y] = 1
        return 0

def rotate(tetromino_pos, type_):
    global can_rotate
    if type_ == 0:
        return 1 

    for i in range(4):
        x, y = tetromino_pos[i]
        if y >= 0:
            map_field[x][y] = 0

    cx, cy = tetromino_pos[1]

    new_pos = []
    for i in range(4):
        dx = tetromino_pos[i][0] - cx
        dy = tetromino_pos[i][1] - cy
        new_x = cx - dy
        new_y = cy + dx
        new_pos.append([new_x, new_y])


    wall_kicks = [
        (0, 0), (-1, 0), (1, 0), (-2, 0), (2, 0),
        (0, 1), (0, 2),  
        (-1, 1), (1, 1), (-2, 1), (2, 1)  
    ]

    for dx, dy in wall_kicks:
        try_pos = [[x + dx, y + dy] for x, y in new_pos]

    
        if all(0 <= x < WIDTH and 0 <= y < HEIGHT and not map_field[x][y] for x, y in try_pos):
            for i in range(4):
                tetromino_pos[i] = try_pos[i]
                x, y = tetromino_pos[i]
                map_field[x][y] = 1
            out()
            return 1

    for i in range(4):
        x, y = tetromino_pos[i]
        if y >= 0:
            map_field[x][y] = 1
    return 0

def inputs(input_val):
    if input_val == 1:
        return fall(tetromino)
    elif input_val == 2:
        return move(tetromino, -1)
    elif input_val == 3:
        return move(tetromino, 1)
    elif input_val == 4:
        return rotate(tetromino, tetromino_type)
    else:
        return 1

def update_input():
    global input_key, can_rotate
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            input_key = 0
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_w and can_rotate:
                input_key = 4
                can_rotate = 0
            elif event.key == pygame.K_ESCAPE:
                input_key = 0
        elif event.type == pygame.KEYUP:
            if event.key == pygame.K_w:
                can_rotate = 1

    keys = pygame.key.get_pressed()
    if keys[pygame.K_s]:
        input_key = 1
    elif keys[pygame.K_a]:
        input_key = 2
    elif keys[pygame.K_d]:
        input_key = 3

def get_time_ms():
    return int(time.time() * 1000)

def process():
    global input_key, next_tetromino, level, score

    input_thread = threading.Thread(target=update_input)
    input_thread.daemon = True
    input_thread.start()

    next_time = get_time_ms()
    game = True
    next_tetromino = choose_tetromino()
    if not add_tetromino(next_tetromino, tetromino):
        return 0
    next_tetromino = choose_tetromino()

    while game:
        update_input()

        if input_key == 0:
            game = False
            break

        inputs(input_key)
        input_key = 5

        now = get_time_ms()
        if now >= next_time:
            next_time = now + get_fall_delay(level)

            if not fall(tetromino):
                score += clear_lines()
                level = lines_total // 10

                if not add_tetromino(next_tetromino, tetromino):
                    game = False
                else:
                    next_tetromino = choose_tetromino()

        time.sleep(0.05)

    return score

pygame.init()
screen = pygame.display.set_mode((WIDTH * CELL_SIZE + 150, HEIGHT * CELL_SIZE))
pygame.display.set_caption("Tetris")


if __name__ == "__main__":
    random.seed(time.time())
    show_start_screen()
    process()
    show_game_over_screen()
    pygame.quit()