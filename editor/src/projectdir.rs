#[derive(Default)]
pub struct ProjectDir {
    pub path: String,
}

impl ProjectDir {
    pub fn generate(&self) {
        // Create main dir
        std::fs::create_dir(&self.path).unwrap();
        // Create "actions" dir
        std::fs::create_dir(&format!("{}/actions", self.path)).unwrap();
        // Create "conditions" dir
        std::fs::create_dir(&format!("{}/conditions", self.path)).unwrap();
        // Create "pages" dir
        std::fs::create_dir(&format!("{}/pages", self.path)).unwrap();
        // Create "project.toml" file
        std::fs::File::create(&format!("{}/project.toml", self.path)).unwrap();
    }
}
