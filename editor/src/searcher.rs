use eframe::egui::Ui;

use crate::directory::Directory;
use crate::file::TomlFile;

#[derive(Default)]
pub struct Searcher {
    word_buffer: String,
}

impl Searcher {
    pub fn update(&mut self, ctx: &eframe::egui::Context, pages_path: String) {
        let mut directory = Directory::default();
        directory.set_relative_path(&pages_path);
        directory.update_file_list();

        eframe::egui::Window::new("Searcher").show(ctx, |ui|{
            ui.text_edit_singleline(&mut self.word_buffer);

            if !self.word_buffer.is_empty() {
                self.search_in_dir(ui, &pages_path);
            }
        });
    }

    fn search_in_dir(&self, ui: &mut Ui, dir: &str) {
        let mut directory = Directory::default();
        directory.set_relative_path(dir);
        directory.update_file_list();
        for file in directory.get_file_list() {
            if std::path::Path::new(&file).is_dir() {
                return self.search_in_dir(ui, &file);
            } else {
                self.search_in_file(ui, &file);
            }
        }
    }

    fn search_in_file(&self, ui: &mut Ui, file: &str) {
        let mut toml_file = TomlFile::new(file);
        toml_file.load();
        for key in toml_file.get_all_keys_of("content").iter() {
            let content = toml_file.get_mut_item_of("content", &key).as_str().unwrap();
            if content.contains(&self.word_buffer) {
                ui.label(format!("Found at {} in {}", file, key));
            }
        }
    }
}
