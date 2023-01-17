use crate::directory::Directory;
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

    pub fn create_if_missing_file(&self, path: &str) {
        let path = std::path::Path::new(path);
        if !std::path::Path::new(path).exists() {
            std::fs::File::create(path).unwrap();
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

                if !self.path.is_empty() && std::path::Path::new(&self.path).exists() {
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

                    ui.heading("Config");

                    ui.label("Name");
                    let mut item = file.get_mut_item_of("config", "name");
                    editor::edit_toml_string(ui, item);

                    ui.label("Author");
                    let mut item = file.get_mut_item_of("config", "author");
                    editor::edit_toml_string(ui, item);

                    ui.label("Tags");
                    let mut item = file.get_mut_item_of("config", "tags");
                    editor::edit_toml_string_array(ui, item, None);

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
                        for var in file.get_all_keys_of("data") {
                            let mut item = file.get_mut_item_of("data", &var);
                            ui.label(&var);
                            editor::edit_toml_int(ui, item);
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
                        for var in file.get_all_keys_of("conditions") {
                            ui.label(&var);
                            let mut item = file.get_mut_item_of("conditions", &var);
                            editor::edit_toml_string_array(ui, item, None);
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
                        for var in file.get_all_keys_of("actions") {
                            ui.label(&var);
                            let mut item = file.get_mut_item_of("actions", &var);
                            editor::edit_toml_string_array(ui, item, None);
                        }
                    });

                    eframe::egui::Window::new("Page Editor").show(&ctx, |ui| {
                        self.page_editor.update(ui, &mut file, &mut self.pages_dir);
                    });

                    file.save();
                }
            },
        );
    }
}
