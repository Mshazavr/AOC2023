use std::fs;

static FILE_PATH: &str = "day10.in";

fn within_bounds(pos: (i32, i32), bounds: (usize, usize)) -> bool {
    return pos.0 >= 0
        && (pos.0 as usize) < bounds.0
        && pos.1 >= 0
        && (pos.1 as usize) < bounds.1;
}

#[derive(Clone, Copy, Debug, PartialEq)]
enum Direction {
    North,
    South,
    West,
    East,
}

impl Direction {
    fn from_tuple(tuple: (i32, i32)) -> Option<Direction> {
        match tuple {
            (1, 0) => Some(Direction::South),
            (-1, 0) => Some(Direction::North),
            (0, 1) => Some(Direction::East),
            (0, -1) => Some(Direction::West),
            _ => None,
        }
    }

    fn to_tuple(&self) -> (i32, i32) {
        match self {
            Direction::South => (1, 0),
            Direction::North => (-1, 0),
            Direction::East => (0, 1),
            Direction::West => (0, -1),
        }
    }

    fn opposite(&self) -> Self {
        match self {
            Direction::North => Direction::South,
            Direction::South => Direction::North,
            Direction::West => Direction::East,
            Direction::East => Direction::West,
        }
    }

    fn all_directions() -> [Self; 4] {
        return [
            Direction::North,
            Direction::South,
            Direction::East,
            Direction::West,
        ];
    }

    fn move_coordinate(
        &self,
        pos: (usize, usize),
        bounds: (usize, usize),
    ) -> Option<(usize, usize)> {
        let coordinates = match self {
            Direction::North => (pos.0 as i32 - 1, pos.1 as i32),
            Direction::South => (pos.0 as i32 + 1, pos.1 as i32),
            Direction::West => (pos.0 as i32, pos.1 as i32 - 1),
            Direction::East => (pos.0 as i32, pos.1 as i32 + 1),
        };

        if within_bounds(coordinates, bounds) {
            return Some((coordinates.0 as usize, coordinates.1 as usize));
        }

        return None;
    }
}

#[derive(PartialEq)]
enum Tile {
    Empty,
    Start,
    Vertical,
    Horizontal,
    NorthEast,
    NorthWest,
    SouthEast,
    SouthWest,
}

impl Tile {
    fn touches_direction(&self, direction: &Direction) -> bool {
        match direction {
            Direction::North => {
                [Tile::Vertical, Tile::NorthEast, Tile::NorthWest]
                    .contains(self)
            }
            Direction::South => {
                [Tile::Vertical, Tile::SouthEast, Tile::SouthWest]
                    .contains(self)
            }
            Direction::West => {
                [Tile::Horizontal, Tile::NorthWest, Tile::SouthWest]
                    .contains(self)
            }
            Direction::East => {
                [Tile::Horizontal, Tile::NorthEast, Tile::SouthEast]
                    .contains(self)
            }
        }
    }
}

type Grid = Vec<Vec<Tile>>;

struct Arrow {
    direction: Direction,
    rotations_c_90deg: i32,
}

impl Arrow {
    fn relative_c_position(&self) -> (i32, i32) {
        match self.direction {
            Direction::North => (0, 1),
            Direction::South => (0, -1),
            Direction::East => (1, 0),
            Direction::West => (-1, 0),
        }
    }

    fn relative_cc_position(&self) -> (i32, i32) {
        let res = self.relative_c_position();
        (res.0 * -1, res.1 * -1)
    }

    fn c_position(
        &self,
        pos: (usize, usize),
        bounds: (usize, usize),
    ) -> Option<(usize, usize)> {
        let rel_pos = self.relative_c_position();
        let res = ((pos.0 as i32) + rel_pos.0, (pos.1 as i32) + rel_pos.1);
        if within_bounds(res, bounds) {
            return Some((res.0 as usize, res.1 as usize));
        }

        return None;
    }

    fn cc_position(
        &self,
        pos: (usize, usize),
        bounds: (usize, usize),
    ) -> Option<(usize, usize)> {
        let rel_pos = self.relative_cc_position();
        let res = ((pos.0 as i32) + rel_pos.0, (pos.1 as i32) + rel_pos.1);
        if within_bounds(res, bounds) {
            return Some((res.0 as usize, res.1 as usize));
        }

        return None;
    }

    fn rotate(&mut self, next_direction: Direction) {
        let start_tuple = self.direction.to_tuple();
        let end_tuple = next_direction.to_tuple();

        // cross product
        self.rotations_c_90deg +=
            -start_tuple.0 * end_tuple.1 + start_tuple.1 * end_tuple.0;
        self.direction = next_direction;
    }
}

fn tiles_connected(
    pos1: (usize, usize),
    pos2: (usize, usize),
    grid: &Grid,
) -> bool {
    let relative_position = Direction::from_tuple((
        pos2.0 as i32 - pos1.0 as i32,
        pos2.1 as i32 - pos1.1 as i32,
    ));

    if let None = relative_position {
        return false;
    }

    let relative_position = relative_position.unwrap();

    grid[pos1.0][pos1.1].touches_direction(&relative_position)
        && grid[pos2.0][pos2.1].touches_direction(&relative_position.opposite())
}

fn solve_part_1_and_2() {
    let input_string: String =
        fs::read_to_string(FILE_PATH).expect("Failed reading input");

    let grid: Grid = input_string
        .trim()
        .split('\n')
        .map(|line| {
            line.trim()
                .chars()
                .map(|c| match c {
                    '.' => Tile::Empty,
                    'S' => Tile::Start,
                    '-' => Tile::Horizontal,
                    '|' => Tile::Vertical,
                    'L' => Tile::NorthEast,
                    'J' => Tile::NorthWest,
                    'F' => Tile::SouthEast,
                    '7' => Tile::SouthWest,
                    x => panic!("Unrecognized tile: {}", x),
                })
                .collect()
        })
        .collect();

    let bounds: (usize, usize) = (grid.len(), grid[0].len());
    let mut annotated_grid: Vec<Vec<usize>> =
        vec![vec![0; grid[0].len()]; grid.len()];
    let mut group_index = 0;
    let mut start_coordinates: Option<(usize, usize)> = None;
    let mut group_counts: Vec<usize> = vec![0];

    let mut dfs_stack: Vec<(usize, usize)> = Vec::new();

    for (i, row) in grid.iter().enumerate() {
        for (j, tile) in row.iter().enumerate() {
            if tile == &Tile::Start {
                start_coordinates = Some((i, j));
            }

            if annotated_grid[i][j] == 0
                && tile != &Tile::Empty
                && tile != &Tile::Start
            {
                group_index += 1;
                annotated_grid[i][j] = group_index;
                group_counts.push(1);
                dfs_stack.push((i, j));
                while !dfs_stack.is_empty() {
                    let pos = dfs_stack.pop().unwrap();
                    for d in Direction::all_directions() {
                        match d.move_coordinate(pos, bounds) {
                            Some(new_pos) => {
                                if annotated_grid[new_pos.0][new_pos.1] == 0
                                    && tiles_connected(pos, new_pos, &grid)
                                {
                                    annotated_grid[new_pos.0][new_pos.1] =
                                        annotated_grid[pos.0][pos.1];
                                    group_counts
                                        [annotated_grid[pos.0][pos.1]] += 1;
                                    dfs_stack.push(new_pos);
                                }
                            }
                            None => (),
                        }
                    }
                }
            }
        }
    }

    let mut cycle_size = 0;
    let start_coordinates = start_coordinates.expect("Didn't find S");
    let mut main_group: Option<usize> = None;

    'outer: for d1 in Direction::all_directions() {
        for d2 in Direction::all_directions() {
            if d1 == d2 {
                continue;
            }
            let pos1 = d1.move_coordinate(start_coordinates, bounds);
            let pos2 = d2.move_coordinate(start_coordinates, bounds);

            if let None = pos1 {
                continue;
            }
            if let None = pos2 {
                continue;
            }

            let pos1 = pos1.unwrap();
            let pos2 = pos2.unwrap();

            if annotated_grid[pos1.0][pos1.1] == annotated_grid[pos2.0][pos2.1]
                && annotated_grid[pos1.0][pos1.1] > 0
            {
                cycle_size = group_counts[annotated_grid[pos2.0][pos2.1]] + 1;
                main_group = Some(annotated_grid[pos2.0][pos2.1]);
                break 'outer;
            }
        }
    }
    let main_group = main_group.expect("Couldn't find the main cycle");
    annotated_grid[start_coordinates.0][start_coordinates.1] = main_group;

    println!("Cycle size: {}", cycle_size);
    println!("Max Distance: {}", cycle_size / 2);

    // Part2
    let mut inside_or_outside: Vec<Vec<u8>> =
        vec![vec![0; grid[0].len()]; grid.len()];
    let mut print_annotation: Vec<Vec<char>> =
        vec![vec!['.'; grid[0].len()]; grid.len()];
    let mut visited: Vec<Vec<bool>> =
        vec![vec![false; grid[0].len()]; grid.len()];
    let mut initial_direction: Option<Direction> = None;

    //initial move
    for d in Direction::all_directions() {
        let pos_box = d.move_coordinate(start_coordinates, bounds);
        if let Some(pos) = pos_box {
            if annotated_grid[pos.0][pos.1] == main_group {
                initial_direction = Some(d);
                break;
            }
        }
    }

    let initial_direction =
        initial_direction.expect("Couldn't perform initial step");
    let mut arrow = Arrow {
        direction: initial_direction.clone(),
        rotations_c_90deg: 0,
    };
    let mut current_position = initial_direction
        .move_coordinate(start_coordinates, bounds)
        .unwrap();
    visited[current_position.0][current_position.1] = true;
    let mut previous_position = start_coordinates;

    loop {
        if let Some(pos) = arrow.c_position(current_position, bounds) {
            inside_or_outside[pos.0][pos.1] = 1;
        }
        if let Some(pos) = arrow.c_position(previous_position, bounds) {
            inside_or_outside[pos.0][pos.1] = 1;
        }
        if let Some(pos) = arrow.cc_position(current_position, bounds) {
            inside_or_outside[pos.0][pos.1] = 2;
        }
        if let Some(pos) = arrow.cc_position(previous_position, bounds) {
            inside_or_outside[pos.0][pos.1] = 2;
        }

        print_annotation[current_position.0][current_position.1] =
            match arrow.direction {
                Direction::North => '^',
                Direction::South => 'v',
                Direction::West => '<',
                Direction::East => '>',
            };

        previous_position = current_position;
        let mut direction_to_start: Option<Direction> = None;
        let mut found_next = false;
        for d in Direction::all_directions() {
            let pos_box = d.move_coordinate(current_position, bounds);
            if let Some(pos) = pos_box {
                if !visited[pos.0][pos.1]
                    && annotated_grid[pos.0][pos.1] == main_group
                    && tiles_connected(current_position, pos, &grid)
                {
                    arrow.rotate(d);
                    current_position = pos;
                    visited[pos.0][pos.1] = true;
                    found_next = true;
                    break;
                }
                if pos == start_coordinates {
                    direction_to_start = Some(d);
                }
            }
        }

        // close the loop
        if !found_next {
            let direction_to_start = direction_to_start
                .expect("Neither next tile nor start found when stepping.");
            arrow.rotate(direction_to_start);
            break;
        }
    }
    arrow.rotate(initial_direction);

    println!("Total Rotation: {}", arrow.rotations_c_90deg);

    visited = vec![vec![false; grid[0].len()]; grid.len()];
    let target_indicator = if arrow.rotations_c_90deg == 4 { 1 } else { 2 };
    let mut num_tiles = 0;

    for (i, row) in inside_or_outside.iter().enumerate() {
        for (j, tile) in row.iter().enumerate() {
            if tile == &target_indicator
                && !visited[i][j]
                && annotated_grid[i][j] != main_group
            {
                dfs_stack.push((i, j));
                visited[i][j] = true;
                num_tiles += 1;
                while !dfs_stack.is_empty() {
                    let pos = dfs_stack.pop().unwrap();
                    for d in Direction::all_directions() {
                        match d.move_coordinate(pos, bounds) {
                            Some(new_pos) => {
                                if annotated_grid[new_pos.0][new_pos.1]
                                    != main_group
                                    && !visited[new_pos.0][new_pos.1]
                                {
                                    visited[new_pos.0][new_pos.1] = true;
                                    num_tiles += 1;
                                    dfs_stack.push(new_pos);
                                }
                            }
                            None => (),
                        }
                    }
                }
            }
        }
    }

    println!("Total tiles: {}", num_tiles);

    /*
    for i in 0..grid.len() {
        for j in 0..grid[i].len() {
            if annotated_grid[i][j] == main_group {
                print!("{}", print_annotation[i][j]);
            } else {
                print!("{}", inside_or_outside[i][j]);
            }
        }
        println!("");
    }*/
}

fn main() {
    solve_part_1();
}
