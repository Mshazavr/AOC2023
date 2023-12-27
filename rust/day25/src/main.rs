use std::collections::HashMap;
use std::fs;
use std::cmp;
use itertools::Itertools;

static FILE_PATH: &str = "day25.in";

type NodeID = usize;

#[derive(Default)]
struct NodeIDCounter(Option<NodeID>);
impl NodeIDCounter {
    fn current(&self) -> NodeID {
        self.0.unwrap()
    }
    fn next(&mut self) -> NodeID {
        match &mut self.0 {
            Some(c) => {
                *c += 1;
                *c
            }
            None => {
                self.0 = Some(0);
                0
            }
        }
    }
}

#[derive(Default)]
struct NodeStore {
    name_to_id: HashMap<String, NodeID>,
    counter: NodeIDCounter,
}
impl NodeStore {
    fn get_node_id(&mut self, name: &str) -> NodeID {
        match self.name_to_id.get(name) {
            Some(id) => *id,
            None => {
                self.name_to_id
                    .insert(name.to_string(), self.counter.next());
                self.counter.current()
            }
        }
    }

    fn get_max_node_id(&self) -> NodeID {
        self.counter.current()
    }
}

#[derive(Clone, Default, Debug)]
struct NodeInfo {
    visited: bool,
    level: Option<i32>,
    parent_node: Option<NodeID>,
    subgraph_size: Option<i32>,
    num_up_edges: Option<i32>,
    num_down_edges: Option<i32>,
    num_up_edges_from_subgraph: Option<i32>,
}

fn compute_node_info(
    v: NodeID,
    p: Option<NodeID>,
    level: i32,
    node_infos: &mut Vec<NodeInfo>,
    graph: &Vec<Vec<NodeID>>,
) {
    node_infos[v].visited = true;
    node_infos[v].level = Some(level);
    node_infos[v].parent_node = p;

    let mut subgraph_size = 1;
    let mut num_up_edges = 0;
    let mut num_down_edges = 0;
    let mut num_up_edges_from_subgraph = 0;

    for w in &graph[v] {
        if p.is_some() && p.unwrap() == *w {
            continue;
        }

        if node_infos[*w].visited {
            if node_infos[*w].level < node_infos[v].level {
                num_up_edges += 1;
            } else {
                num_down_edges += 1;
            }
            continue;
        }

        compute_node_info(*w, Some(v), level + 1, node_infos, graph);
        subgraph_size += node_infos[*w].subgraph_size.unwrap();
        num_up_edges_from_subgraph +=
            node_infos[*w].num_up_edges_from_subgraph.unwrap()
    }
    num_up_edges_from_subgraph += num_up_edges - num_down_edges;

    node_infos[v].subgraph_size = Some(subgraph_size);
    node_infos[v].num_up_edges = Some(num_up_edges);
    node_infos[v].num_down_edges = Some(num_down_edges);
    node_infos[v].num_up_edges_from_subgraph = Some(num_up_edges_from_subgraph);
}

fn main() {
    let input_string: String =
        fs::read_to_string(FILE_PATH).expect("Failed reading input");

    let mut node_store: NodeStore = NodeStore::default();
    let edges: Vec<(NodeID, NodeID)> = input_string
        .trim()
        .split('\n')
        .map(|line| {
            let node_names: Vec<&str> = line.split_whitespace().collect();
            let first_name: &str = node_names[0];
            let first_name: &str = &first_name[..first_name.len() - 1];
            let left_node_id = node_store.get_node_id(first_name);
            node_names[1..]
                .into_iter()
                .map(|node_name| {
                    (left_node_id, node_store.get_node_id(node_name))
                })
                .collect::<Vec<(NodeID, NodeID)>>()
        })
        .flatten()
        .unique_by(|(left, right)| (cmp::min(*left, *right), cmp::max(*left, *right)))
        .collect();

    let n = node_store.get_max_node_id() + 1;
    let mut graph: Vec<Vec<NodeID>> = vec![Vec::new(); n];
    for (left, right) in edges {
        if left == right {
            panic!("left == right");
        }
        graph[left].push(right);
        graph[right].push(left);
    }
    //println!("Graph:");
    //graph.iter().enumerate().for_each(|(i, v)| println!("{}: {:?}", i, v));

    for root_id in 0..n {
        let mut node_infos: Vec<NodeInfo> = vec![
            NodeInfo {
                visited: false,
                ..NodeInfo::default()
            };
            n as usize
        ];
        compute_node_info(root_id, None, 0, &mut node_infos, &graph);
        for (i, node) in node_infos.iter().enumerate() {
            if node.num_up_edges_from_subgraph.unwrap() == 2 {
                println!("Found cut node: {}", i);
                println!("Subgraph size: {}", node.subgraph_size.unwrap());
                println!("Rest size: {}", (n as i32) - node.subgraph_size.unwrap());
                println!(
                    "Product: {}",
                    ((n as i32) - node.subgraph_size.unwrap()) * node.subgraph_size.unwrap()
                );
                return;
            }
        }
    }
}
