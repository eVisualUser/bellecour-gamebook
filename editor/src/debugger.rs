use eframe::egui;
use crate::file::TomlFile;

#[derive(Default)]
pub struct Debugger {
}

impl Debugger {
    pub fn find_missing_page(&self, file: &mut TomlFile, pages_path: String) -> Vec<(String, String)> {
        let mut result = Vec::<(String, String)>::new();

        let keys = file.get_all_keys_of("actions");
        for key in keys {
            let item = file.get_mut_item_of("actions", &key);
            let array = item.as_array().unwrap();
            for element in array.iter() {
                let as_str = element.as_str().unwrap();
                if as_str.starts_with("$") {
                    let mut path = String::new();
                    for i in as_str.chars() {
                        if i != '$' {
                            path.push(i);
                        }
                    }
                    let full_path = format!("{}/{}", pages_path, path);
                    if !std::path::Path::new(&full_path).exists() {
                        result.push((key.clone(), full_path));
                    }
                }
            }
        }

        result
    }

    pub fn variables_not_found(&self, file: &mut TomlFile, pages_path: String) -> Vec<(String, String)> {
        let mut result = Vec::<(String, String)>::new();

        let variables = file.get_all_keys_of("data");

        let mut operations = Vec::<(String, String)>::new();
        for key in file.get_all_keys_of("actions") {
            for element in file.get_mut_item_of("actions", &key).as_array().unwrap() {
                operations.push((format!("ACTION {}", key.clone()), element.as_str().unwrap().to_string()));
            }
        }

        for key in file.get_all_keys_of("conditions") {
            for element in file.get_mut_item_of("conditions", &key).as_array().unwrap() {
                operations.push((format!("CONDITION {}", key.clone()), element.as_str().unwrap().to_string()));
            }
        }

        for (key, process) in operations.iter() {
            let mut nodes = Vec::<String>::new();
            let mut buffer = String::new();
            for i in process.chars() {
                if i != ' ' {
                    buffer.push(i);
                } else {
                    if !buffer.contains("$") {
                        nodes.push(buffer.trim().to_string());
                        buffer = String::new();
                    }
                }
            }
            if !buffer.contains("$") && !buffer.trim().is_empty() {
                nodes.push(buffer.trim().to_string());
            }

            for node in nodes {
                if
                node != ">" &&
                node != "<" &&
                node != "+" &&
                node != "-" &&
                node != "/" &&
                node != "*" &&
                node != "=" &&
                node != "%" {
                    match node.parse::<i32>() {
                        Ok(_) => {}
                        Err(_) => {
                            let mut is_var = false;
                            for variable in variables.iter() {
                                if node == *variable {
                                    is_var = true;
                                }
                            }
                            if !is_var {
                                result.push((key.clone(), node));
                            }
                        }
                    }
                }
            }
        }

        return result;
    }
}

impl Debugger {
    pub fn update(&mut self, file: &mut TomlFile, ctx: &eframe::egui::Context, pages_path: String) {
        egui::Window::new("Debugger").show(ctx, |ui| {
            ui.heading("Files Missing");
            for (action, file) in self.find_missing_page(file, pages_path.clone()) {
                ui.colored_label(egui::ecolor::Color32::RED, format!("ACTION [{}] -> \"{}\"", action, file));
            }
            ui.heading("Variables Missing");
            for (key, file) in self.variables_not_found(file, pages_path) {
                ui.colored_label(egui::ecolor::Color32::RED, format!("VARIABLE [{}] -> \"{}\"", key, file));
            }
        });
    }
}
