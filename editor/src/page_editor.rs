use crate::directory::Directory;
use crate::file::TomlFile;

#[derive(Default)]
pub struct PageEditor {
    current_file: String,
    text_input_buffer: String,
    text_name_buffer: String,
    choices_input_buffer: String,
    file_open: bool,
}

impl PageEditor {
    pub fn update(
        &mut self,
        ui: &mut eframe::egui::Ui,
        config: &mut TomlFile,
        directory: &mut Directory,
    ) {
        let path = directory.get_path();

        ui.label("Current File");

        ui.text_edit_singleline(&mut self.current_file);

        if !std::path::Path::new(&format!("{}/{}", path, "index.toml")).exists() {
            self.current_file = "index.toml".to_string();
        }

        let mut button_text = "Open";
        if !std::path::Path::new(&format!("{}/{}", path, self.current_file)).exists() {
            button_text = "Create & Open";
            self.file_open = false;
        }

        if self.file_open || ui.button(button_text).clicked() {
            self.file_open = true;
            if ui.button("Close").clicked() {
                self.file_open = false;
            }

            let mut file = TomlFile::new(&format!("{}/{}", path, self.current_file));
            if file.create() {
                file.init(vec![
                    "[config]",
                    "name = \"Great Page\"",
                    "type = \"default\"",
                    "[content]",
                    "[choices]",
                ]);
            }
            file.load();

            ui.heading("Config");
            ui.label("Name");
            let item = file.get_mut_item_of("config", "name");
            crate::editor::edit_toml_string(ui, item);
            ui.label("Type");
            let item = file.get_mut_item_of("config", "type");
            crate::editor::edit_toml_string(ui, item);

            // Text Input

            ui.heading("Text");
            ui.heading("Configure");

            ui.label("Text-Line Name");
            ui.text_edit_singleline(&mut self.text_name_buffer);
            ui.label("Text-Line Content");
            ui.text_edit_singleline(&mut self.text_input_buffer);

            if ui.button("Add").clicked() {
                file.add_toml("content", &self.text_name_buffer, &self.text_input_buffer);
            } else if ui.button("Remove").clicked() {
                file.remove_toml("content", &self.text_name_buffer);
            }

            ui.heading("List");
            for var in file.get_all_keys_of("content") {
                let item = file.get_mut_item_of("content", &var);
                ui.label(&var);
                crate::editor::edit_toml_string(ui, item);
            }

            // Choices

            ui.heading("Choices");

            ui.label("New-Choice Name");
            ui.text_edit_singleline(&mut self.choices_input_buffer);

            if ui.button("Add").clicked() {
                file.add_toml_array("choices", &self.choices_input_buffer);
            } else if ui.button("Remove").clicked() {
                file.remove_toml("choices", &self.choices_input_buffer);
            }

            ui.heading("List");
            for var in file.get_all_keys_of("choices") {
                ui.label(&var);
                let item = file.get_mut_item_of("choices", &var);
                crate::editor::edit_toml_string_array(ui, item, Some(3));
            }

            file.save();
        }
    }
}
