use std::io::{Read, Write};
use toml_edit::Document;
use toml_edit::Item;

#[derive(Debug, Default, Clone)]
pub struct TomlFile {
    path: String,
    doc: Option<Document>,
}

impl TomlFile {
    pub fn new(path: &str) -> Self {
        Self {
            path: path.to_owned(),
            doc: None,
        }
    }

    pub fn load(&mut self) {
        let mut file = std::fs::File::options()
            .read(true)
            .open(&self.path)
            .unwrap();

        let mut file_content = String::new();
        file.read_to_string(&mut file_content).unwrap();

        self.doc = Some(file_content.parse::<Document>().unwrap());
    }

    pub fn get_all_keys_of(&self, table: &str) -> Vec<String> {
        let mut result = Vec::<String>::new();

        match &self.doc {
            Some(doc) => {
                for item in doc[table].as_table().unwrap().iter() {
                    result.push(item.0.to_owned());
                }
            }
            None => (),
        }

        return result;
    }

    pub fn get_mut_item_of(&mut self, table: &str, key: &str) -> &mut Item {
        match &mut self.doc {
            Some(doc) => {
                return doc[table].get_mut(key).unwrap();
            }
            None => (),
        }

        panic!("Failed to access to {}->{}", table, key);
    }

    pub fn save(&self) {
        match &self.doc {
            Some(doc) => {
                std::fs::write(&self.path, doc.to_string()).unwrap();
            }
            None => {}
        };
    }

    pub fn create(&self) -> bool {
        if !self.exist() {
            std::fs::File::create(&self.path).unwrap();
            return true;
        }
        return false;
    }

    pub fn exist(&self) -> bool {
        std::path::Path::new(&self.path).exists()
    }

    pub fn init(&self, template: Vec<&str>) {
        let mut file = std::fs::File::options()
            .write(true)
            .open(&self.path)
            .unwrap();
        for line in template {
            writeln!(&mut file, "{}", line).unwrap();
        }
    }

    pub fn add_toml<T>(&mut self, table: &str, key: &str, value: T)
    where
        toml_edit::Value: From<T>,
    {
        match &mut self.doc {
            Some(doc) => {
                match doc.get_mut(table).unwrap().as_table_mut() {
                    Some(toml_table) => {
                        toml_table.insert(key, toml_edit::value(value));
                    }
                    None => {}
                };
            }
            None => {}
        };
    }

    pub fn add_toml_array(&mut self, table: &str, key: &str) {
        match &mut self.doc {
            Some(doc) => {
                match doc.get_mut(table).unwrap().as_table_mut() {
                    Some(toml_table) => {
                        let array = "[\"1\"]".parse::<toml_edit::Item>().unwrap();
                        toml_table.insert(key, array);
                    }
                    None => {}
                };
            }
            None => {}
        };
    }

    pub fn remove_toml(&mut self, table: &str, key: &str) {
        match &mut self.doc {
            Some(doc) => {
                doc.get_mut(table)
                    .unwrap()
                    .as_table_mut()
                    .unwrap()
                    .remove(key);
            }
            None => {}
        };
    }
}
