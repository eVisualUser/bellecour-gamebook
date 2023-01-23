use eframe::egui;
use crate::{file::TomlFile, directory::Directory};

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

    pub fn variables_not_found(&self, file: &mut TomlFile) -> Vec<(String, String)> {
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

    pub fn choice_conditions_not_found(&self, file: &mut TomlFile, pages_path: String) -> Vec<String> {
        let mut result = Vec::<String>::new();

        let conditions = file.get_all_keys_of("conditions");

        let in_file_conditions = self.search_condition_in_dir(&pages_path);

        for in_file_condition in in_file_conditions.iter() {
            let mut exist = false;
            for condition in conditions.iter() {
                if in_file_condition.2 == *condition {
                    exist = true;
                }
            }
            if !exist {
                result.push(format!("In \"{}\" choice [{}] condition \"{}\" not found", in_file_condition.0, in_file_condition.1, in_file_condition.2));
            }
        }

        result
    }

    fn search_condition_in_dir(&self, dir: &str) -> Vec<(String, String, String)> {
        let mut result = Vec::<(String, String, String)>::new();

        let mut directory = Directory::default();
        directory.set_relative_path(dir);
        directory.update_file_list();
        for file in directory.get_file_list() {
            if std::path::Path::new(&file).is_dir() {
                return self.search_condition_in_dir(&file);
            } else {
                for condition in self.search_condition_in_file(&file).iter() {
                    result.push(condition.clone());
                }
            }
        }

        result
    }

    fn search_condition_in_file(&self, file: &str) -> Vec<(String, String, String)> {
        let mut result = Vec::<(String, String, String)>::new();

        let mut toml_file = TomlFile::new(file);
        toml_file.load();
        for key in toml_file.get_all_keys_of("choices").iter() {
            let content = toml_file.get_mut_item_of("choices", &key).as_array().unwrap();
            match content.get(1) {
                Some(value) => {
                    result.push((file.to_string(), key.to_string(), value.as_str().unwrap().to_string()));
                }
                None => {}
            }
        }
        result
    }

    pub fn choice_actions_not_found(&self, file: &mut TomlFile, pages_path: String) -> Vec<String> {
        let mut result = Vec::<String>::new();

        let conditions = file.get_all_keys_of("actions");

        let in_file_conditions = self.search_actions_in_dir(&pages_path);

        for in_file_condition in in_file_conditions.iter() {
            let mut exist = false;
            for condition in conditions.iter() {
                if in_file_condition.2 == *condition {
                    exist = true;
                }
            }
            if !exist {
                result.push(format!("In \"{}\" choice [{}] action \"{}\" not found", in_file_condition.0, in_file_condition.1, in_file_condition.2));
            }
        }

        result
    }

    fn search_actions_in_dir(&self, dir: &str) -> Vec<(String, String, String)> {
        let mut result = Vec::<(String, String, String)>::new();

        let mut directory = Directory::default();
        directory.set_relative_path(dir);
        directory.update_file_list();
        for file in directory.get_file_list() {
            if std::path::Path::new(&file).is_dir() {
                return self.search_actions_in_dir(&file);
            } else {
                for condition in self.search_action_in_file(&file).iter() {
                    result.push(condition.clone());
                }
            }
        }

        result
    }

    fn search_action_in_file(&self, file: &str) -> Vec<(String, String, String)> {
        let mut result = Vec::<(String, String, String)>::new();

        let mut toml_file = TomlFile::new(file);
        toml_file.load();
        for key in toml_file.get_all_keys_of("choices").iter() {
            let content = toml_file.get_mut_item_of("choices", &key).as_array().unwrap();
            match content.get(2) {
                Some(value) => {
                    result.push((file.to_string(), key.to_string(), value.as_str().unwrap().to_string()));
                }
                None => {}
            }
        }
        result
    }
}

impl Debugger {
    pub fn update(&mut self, file: &mut TomlFile, ctx: &eframe::egui::Context, pages_path: String) {
        egui::Window::new("Debugger").show(ctx, |ui| {
            ui.heading("Files Not Found");
            for (action, file) in self.find_missing_page(file, pages_path.clone()) {
                ui.colored_label(egui::ecolor::Color32::RED, format!("ACTION [{}] -> \"{}\"", action, file));
            }
            ui.heading("Variables Not Found");
            for (key, file) in self.variables_not_found(file) {
                ui.colored_label(egui::ecolor::Color32::RED, format!("VARIABLE [{}] -> \"{}\"", key, file));
            }
            ui.heading("Choice Conditions Not Found");
            for error in self.choice_conditions_not_found(file, pages_path.clone()).iter() {
                ui.colored_label(egui::ecolor::Color32::RED, error);
            }
            ui.heading("Choice Actions Not Found");
            for error in self.choice_actions_not_found(file, pages_path.clone()).iter() {
                ui.colored_label(egui::ecolor::Color32::RED, error);
            }
        });
    }
}
