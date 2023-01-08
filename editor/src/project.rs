pub struct Project {
    name: String,
    version: String,
    author: String,
}

impl Project {
    pub fn load(&mut self, path: String) {
        let toml = tsu::toml_from_str(&std::fs::read_to_string(path).unwrap());
        self.name = toml.get("config").unwrap().get("name").unwrap().to_string();
        self.version = toml
            .get("config")
            .unwrap()
            .get("version")
            .unwrap()
            .to_string();
    }
}
