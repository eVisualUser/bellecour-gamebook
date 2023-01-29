use crate::editor;
use crate::file::TomlFile;

#[derive(Default)]
pub struct Project {
    pub path: String,
    pub var_name_buffer: String,
    pub var_value_buffer: String,
    pub condition_name_buffer: String,
    pub action_name_buffer: String,
    pub page_editor: crate::page_editor::PageEditor,
    pub pages_dir: crate::directory::Directory,
    pub debugger: crate::debugger::Debugger,
    pub searcher: crate::searcher::Searcher,
    actions_min_range: usize,
    actions_max_range: usize,
    conditions_min_range: usize,
    conditions_max_range: usize,
    actions_filter_buffer: String,
    conditions_filter_buffer: String,
    data_filter_buffer: String,
    instant_save: bool,
    file: Option<TomlFile>,
    debug_activated: bool,
    file_explorer_filter_buffer: String,
}

impl Project {
    pub fn get_dir_pages(&self) -> String {
        format!("{}/pages", self.path)
    }

    pub fn get_project_config_path(&self) -> String {
        format!("{}/config.toml", self.path)
    }

    pub fn prepare_project_dir(&self) {
        self.create_if_missing(&self.path);
        self.create_if_missing(&self.get_dir_pages());
    }

    pub fn create_if_missing(&self, path: &str) {
        let path = std::path::Path::new(path);
        if !std::path::Path::new(path).exists() {
            std::fs::create_dir(path).unwrap();
        }
    }
}

impl crate::editor::Editor for Project {
    fn update(&mut self, ctx: &eframe::egui::Context, app: &mut crate::app::App) {
        app.path = self.path.clone();

        self.pages_dir.set_relative_path(&self.get_dir_pages());

        eframe::egui::SidePanel::new(eframe::egui::panel::Side::Left, "Project Settings").show(
            ctx,
            |ui| {
                ui.heading("Project Directory");

                let mut path_buffer = self.path.clone();
                ui.text_edit_singleline(&mut path_buffer);
                self.path = path_buffer.to_owned();
                if ui.button("Prepare").clicked() {
                    self.prepare_project_dir();
                }

                let pages = self.get_dir_pages().to_string();
                if !self.path.is_empty() && std::path::Path::new(&self.path).exists() {
                    let debug_activated = self.debug_activated;

                    match &mut self.file {
                        Some(file) => {
                            if debug_activated || ui.input().key_down(eframe::egui::Key::F5) {
                                self.debugger.update(file, ctx, pages.clone());
                            }
                            self.searcher.update(ctx, pages.clone());

                            ui.heading("Config");

                            ui.label("Name");
                            let item = file.get_mut_item_of("config", "name");
                            editor::edit_toml_string(ui, item);

                            ui.label("Author");
                            let item = file.get_mut_item_of("config", "author");
                            editor::edit_toml_string(ui, item);

                            ui.label("Tags");
                            let item = file.get_mut_item_of("config", "tags");
                            editor::edit_toml_string_array(ui, item, None);

                            ui.label("Default Frame Size (X;Y)");
                            let item = file.get_mut_item_of("client", "frameSize_x");
                            editor::edit_toml_int(ui, item);
                            let item = file.get_mut_item_of("client", "frameSize_y");
                            editor::edit_toml_int(ui, item);

                            let client_keys = file.get_all_keys_of("client");
                            let mut have_min_frame_size_x = false;
                            let mut have_min_frame_size_y = false;
                            for key in client_keys {
                                if key == "frame_size_min_x" {
                                    have_min_frame_size_x = true;
                                } else if key == "frame_size_min_y" {
                                    have_min_frame_size_y = true;
                                }
                            }

                            if have_min_frame_size_x && have_min_frame_size_y {
                                ui.label("Minimum Frame Size (X;Y)");
                                let item = file.get_mut_item_of("client", "frame_size_min_x");
                                editor::edit_toml_int(ui, item);
                                let item = file.get_mut_item_of("client", "frame_size_min_y");
                                editor::edit_toml_int(ui, item);
                            } else {
                                file.add_toml("client", "frame_size_min_x", 25);
                                file.add_toml("client", "frame_size_min_y", 25);
                            }

                            if ui.button("Save").clicked() {
                                file.save();
                            }

                            if !self.instant_save
                                && (ui.button("Active Instant Save").clicked()
                                    || ui.input().key_pressed(eframe::egui::Key::F2))
                            {
                                self.instant_save = true;
                            } else if self.instant_save
                                && (ui.button("Desactivate Instant Save").clicked()
                                    || ui.input().key_pressed(eframe::egui::Key::F2))
                            {
                                self.instant_save = false;
                            }

                            if !self.debug_activated
                                && (ui.button("Activate Debug").clicked()
                                    || ui.input().key_pressed(eframe::egui::Key::F6))
                            {
                                self.debug_activated = true;
                            } else if self.debug_activated
                                && (ui.button("Desactivate Debug").clicked()
                                    || ui.input().key_pressed(eframe::egui::Key::F6))
                            {
                                self.debug_activated = false;
                            }

                            eframe::egui::Window::new("Data").show(ctx, |ui| {
                                ui.heading("Data");
                                ui.heading("Configure");

                                ui.label("Name");
                                ui.text_edit_singleline(&mut self.var_name_buffer);
                                ui.label("Value (Must be an int)");
                                ui.text_edit_singleline(&mut self.var_value_buffer);

                                if ui.button("Add").clicked() {
                                    let value = match self.var_value_buffer.parse::<i64>() {
                                        Ok(number) => number,
                                        Err(_) => 0_i64,
                                    };

                                    file.add_toml("data", &self.var_name_buffer, value);
                                } else if ui.button("Remove").clicked() {
                                    file.remove_toml("data", &self.var_name_buffer);
                                }

                                ui.heading("List");
                                ui.label("Filter");
                                ui.text_edit_singleline(&mut self.data_filter_buffer);

                                for var in file.get_all_keys_of("data") {
                                    if self.data_filter_buffer.is_empty() | var.contains(&self.data_filter_buffer) {
                                        let item = file.get_mut_item_of("data", &var);
                                        ui.label(&var);
                                        editor::edit_toml_int(ui, item);
                                    }
                                }
                            });

                            eframe::egui::Window::new("Conditions").show(&ctx, |ui| {
                                ui.heading("Configure");

                                ui.label("Name");
                                ui.text_edit_singleline(&mut self.condition_name_buffer);

                                if ui.button("Add").clicked() {
                                    file.add_toml_array("conditions", &self.condition_name_buffer);
                                } else if ui.button("Remove").clicked() {
                                    file.remove_toml("conditions", &self.condition_name_buffer);
                                }

                                ui.heading("List");
                                let keys = file.get_all_keys_of("conditions");
                                ui.label("Range Min/Max");

                                let mut buffer = self.conditions_min_range.to_string();
                                ui.text_edit_singleline(&mut buffer);
                                match buffer.parse::<usize>() {
                                    Ok(number) => self.conditions_min_range = number,
                                    Err(_) => self.conditions_min_range = 0,
                                };
                                if self.conditions_min_range > keys.len() - 1 {
                                    self.conditions_min_range = keys.len() - 1;
                                }

                                let mut buffer = self.conditions_max_range.to_string();
                                ui.text_edit_singleline(&mut buffer);
                                match buffer.parse::<usize>() {
                                    Ok(number) => self.conditions_max_range = number,
                                    Err(_) => self.conditions_max_range = 0,
                                };
                                if self.conditions_max_range > keys.len() - 1 {
                                    self.conditions_max_range = keys.len() - 1;
                                }

                                ui.label("Filter");
                                ui.text_edit_singleline(&mut self.conditions_filter_buffer);

                                if self.conditions_max_range != 0 {
                                    let mut index = self.conditions_max_range;
                                    #[allow(unused_comparisons)]
                                    while index >= self.conditions_min_range {
                                        if self.conditions_filter_buffer.is_empty() | keys[index].contains(&self.conditions_filter_buffer) {
                                            ui.label(&keys[index]);
                                            let item = file.get_mut_item_of("conditions", &keys[index]);
                                            editor::edit_toml_string_array(ui, item, None);
                                        }
                                        if index > 0 {
                                            index -= 1;
                                        } else {
                                            break;
                                        }
                                    }
                                }
                            });

                            eframe::egui::Window::new("Actions").show(&ctx, |ui| {
                                ui.heading("Configure");

                                ui.label("Name");
                                ui.text_edit_singleline(&mut self.action_name_buffer);

                                if ui.button("Add").clicked() {
                                    file.add_toml_array("actions", &self.action_name_buffer);
                                } else if ui.button("Remove").clicked() {
                                    file.remove_toml("actions", &self.action_name_buffer);
                                }

                                ui.heading("List");
                                let keys = file.get_all_keys_of("actions");

                                ui.label("Range Min/Max");

                                let mut buffer = self.actions_min_range.to_string();
                                ui.text_edit_singleline(&mut buffer);
                                match buffer.parse::<usize>() {
                                    Ok(number) => self.actions_min_range = number,
                                    Err(_) => self.actions_min_range = 0,
                                };
                                if self.actions_min_range > keys.len() - 1 {
                                    self.actions_min_range = keys.len() - 1;
                                }

                                let mut buffer = self.actions_max_range.to_string();
                                ui.text_edit_singleline(&mut buffer);
                                match buffer.parse::<usize>() {
                                    Ok(number) => self.actions_max_range = number,
                                    Err(_) => self.actions_max_range = 0,
                                };
                                if self.actions_max_range > keys.len() - 1 {
                                    self.actions_max_range = keys.len() - 1;
                                }

                                ui.label("Filter");
                                ui.text_edit_singleline(&mut self.actions_filter_buffer);

                                if self.actions_max_range != 0 {
                                    let mut index = self.actions_max_range;
                                    #[allow(unused_comparisons)]
                                    while index >= self.actions_min_range {
                                        if self.actions_filter_buffer.is_empty() | keys[index].contains(&self.actions_filter_buffer) {
                                            ui.label(&keys[index]);
                                            let item = file.get_mut_item_of("actions", &keys[index]);
                                            editor::edit_toml_string_array(ui, item, None);
                                        }
                                        if index > 0 {
                                            index -= 1;
                                        } else {
                                            break;
                                        }
                                    }
                                }
                            });

                            eframe::egui::Window::new("Page Editor").show(&ctx, |ui| {
                                eframe::egui::Window::new("File Explorer").show(&ctx, |ui| {
                                    ui.text_edit_singleline(&mut self.file_explorer_filter_buffer);
                                    for file in get_all_files_in_dir(pages) {
                                        if self.file_explorer_filter_buffer.is_empty() || file.contains(&self.file_explorer_filter_buffer) {
                                            ui.label(&file);
                                            if ui.button("Open").clicked() {
                                                let mut buffer = String::new();
                                                for i in file.rfind("pages\\").unwrap()..file.len() {
                                                    buffer.push(file.chars().nth(i).unwrap());
                                                }
                                                self.page_editor.current_file = std::path::Path::new(&buffer).file_name().unwrap().to_str().unwrap().to_owned();
                                            }
                                        }
                                    }
                                });

                                self.page_editor.update(ui, file, &mut self.pages_dir);
                            });

                            if self.instant_save {
                                file.save();
                            }
                        }
                        None => {
                            let mut file = TomlFile::new(&self.get_project_config_path());
                            if file.create() {
                                file.init(vec![
                                    "[config]",
                                    "name = \"projectName\"",
                                    "author = \"unknow\"",
                                    "tags = [\"edited\"]",
                                    "[data]",
                                    "[conditions]",
                                    "[actions]",
                                    "[client]",
                                    "page = \"pages/index.toml\"",
                                    "frameSize_x = 100",
                                    "frameSize_y = 15",
                                    "up = 72",
                                    "down = 80",
                                    "ok = 13",
                                    "zoom = 61",
                                    "unzoom = 45",
                                    "exit = 27",
                                ]);
                            }
                            file.load();
                            self.file = Some(file);
                        }
                    }
                }
            },
        );
    }
}

pub fn get_all_files_in_dir(target: String) -> Vec<String> {
    let mut result = Vec::<String>::new();

    let mut dir = crate::directory::Directory::default();
    dir.set_relative_path(&target);
    dir.update_file_list();
    for element in dir.get_file_list() {
        if std::path::Path::new(&element).is_dir() {
            result.append(&mut get_all_files_in_dir(element));
        } else {
            result.push(element);
        }
    }

    result
}
