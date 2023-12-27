use std::fs;

enum Part {
  Part1,
  Part2,
}

static FILE_PATH: &str = "day9.in";
static MODE: Part = Part::Part2;

fn extrapolated_value(value_history: &Vec<i32>) -> i32 {
  let all_zeroes = |values: &Vec<i32>| values.iter().all(|v| v == &0);
  let next_layer = |values: &Vec<i32>| -> Vec<i32> {
    (0..values.len() - 1)
      .map(|i| values[i + 1] - values[i])
      .collect()
  };

  let mut last_values: Vec<i32> = vec![value_history.last().unwrap().clone()];
  let mut first_values: Vec<i32> = vec![value_history.first().unwrap().clone()];
  let mut current_layer: Vec<i32> = value_history.clone();
  loop {
    current_layer = next_layer(&current_layer);
    last_values.push(current_layer.last().unwrap().clone());
    first_values.push(current_layer.first().unwrap().clone());
    if all_zeroes(&current_layer) {
      break;
    }
  }

  match MODE {
    Part::Part1 => last_values.into_iter().sum(),
    Part::Part2 => first_values
      .into_iter()
      .enumerate()
      .map(|(i, v)| if i % 2 == 0 { v } else { -v })
      .sum(),
  }
}

fn main() {
  let input_string: String =
    fs::read_to_string(FILE_PATH).expect("Failed reading input");

  let value_histories: Vec<Vec<i32>> = input_string
    .trim()
    .split('\n')
    .map(|line| {
      line
        .split(' ')
        .map(|x| x.trim().to_owned().parse().expect("Failed to parse number"))
        .collect()
    })
    .collect();

  let sum_extrapolated_values: i32 =
    value_histories.iter().map(extrapolated_value).sum();

  println!("{}", sum_extrapolated_values);
}
