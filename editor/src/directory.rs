use eframe::egui::Id;

#[derive(Default)]
pub struct Directory {
    path: String,
    file_list: Vec<String>,
}

impl Directory {
    pub fn get_path(&self) -> String {
        self.path.to_owned()
    }

    pub fn set_relative_path(&mut self, path: &str) {
        self.path = path.to_owned();
    }

    pub fn update_file_list(&mut self) {
        if self.exist() {
            let mut result = Vec::<String>::new();
            for entry in std::fs::read_dir(&self.get_path()).unwrap() {
                result.push(
                    entry
                        .unwrap()
                        .path()
                        .as_path()
                        .to_str()
                        .unwrap()
                        .to_string(),
                );
            }
            self.file_list = result;
        }
    }

    pub fn create_file(&self, filename: &str) {
        std::fs::File::create(&format!("{}/{}", self.path, filename));
    }

    pub fn exist(&self) -> bool {
        std::path::Path::new(&self.path).exists()
    }

    pub fn get_file_list(&self) -> Vec<String> {
        self.file_list.clone()
    }

    pub fn get_dir_list(&self) -> Vec<String> {
        let mut result = Vec::<String>::new();
        if self.exist() {

            for entry in std::fs::read_dir(&self.get_path()).unwrap() {
                result.push(
                        entry
                        .unwrap()
                        .path()
                        .as_path()
                        .to_str()
                        .unwrap()
                        .to_string(),
                );
            }
        }
        result
    }
}
