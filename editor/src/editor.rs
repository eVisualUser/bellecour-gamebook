use eframe::egui::Context;
use eframe::egui::Ui;
use toml_edit::Item;

pub trait Editor {
    fn update(&mut self, _: &Context, _: &mut crate::app::App);
}

pub fn edit_toml_string(ui: &mut Ui, item: &mut Item) {
    let mut content_buffer = item.as_str().unwrap().to_string().clone();
    ui.text_edit_singleline(&mut content_buffer);
    content_buffer = filter_string(content_buffer, false);
    *item = toml_edit::value(content_buffer);
}

pub fn edit_toml_int(ui: &mut Ui, item: &mut Item) {
    let mut content_buffer = item.as_integer().unwrap().to_string().clone();
    ui.text_edit_singleline(&mut content_buffer);
    if content_buffer.is_empty() {
        content_buffer = String::from("0");
    }
    let value = match content_buffer.parse::<i64>() {
        Ok(number) => number,
        Err(_) => 0_i64,
    };
    *item = toml_edit::value(value);
}

pub fn edit_toml_string_array(ui: &mut Ui, item: &mut Item, limit: Option<usize>) {
    let mut array = item.as_array().unwrap().clone();
    for value in array.iter_mut() {
        let mut content_buffer = value.as_str().unwrap().to_string();
        ui.text_edit_singleline(&mut content_buffer);
        content_buffer = filter_string(content_buffer, true);
        *value = toml_edit::value(content_buffer).as_value().unwrap().clone();
    }

    let can_edit;
    match limit {
        Some(len) => {
            if array.len() < len {
                can_edit = true
            } else {
                can_edit = false;
            }
        }
        None => can_edit = true,
    };

    if can_edit {
        if ui.button("Add").clicked() {
            array.push("");
        } else if !array.is_empty() && ui.button("Remove Last").clicked() {
            array.remove(array.len() - 1);
        }
    }

    *item = toml_edit::value(array);
}

pub fn filter_string(input: String, is_in_array: bool) -> String {
    let mut result = String::new();

    for i in input.chars() {
        match i {
            ',' => {
                if !is_in_array {
                    result.push(',');
                }
            }
            '\n' => {}
            '\"' => {
                result.push_str("''");
            }
            '???' => {
                result.push('\'');
            }
            '\r' => {}
            '??' => {
                result.push('e');
            }
            '??' => {
                result.push('E');
            }
            '??' => {
                result.push('E');
            }
            '??' => {
                result.push('E');
            }
            '??' => {
                result.push('E');
            }
            '??' => {
                result.push('e');
            }
            '??' => {
                result.push('e');
            }
            '??' => {
                result.push('e');
            }
            '??' => {
                result.push('E');
            }
            '??' => {
                result.push('e');
            }
            '??' => {
                result.push('e');
            }
            '??' => {
                result.push('E');
            }
            '??' => {
                result.push('e');
            }
            '??' => {
                result.push('E');
            }
            '??' => {
                result.push('e');
            }
            '??' => {
                result.push('E');
            }
            '??' => {
                result.push('c');
            }
            '??' => {
                result.push('c');
            }
            '??' => {
                result.push('C');
            }
            '??' => {
                result.push('C');
            }
            '??' => {
                result.push('A');
            }
            '??' => {
                result.push('A');
            }
            '??' => {
                result.push('A');
            }
            '??' => {
                result.push('A');
            }
            '???' => result.push_str("..."),
            '??' => result.push('\"'),
            '??' => result.push('\"'),
            '???' => {
                result.push_str("\'\'");
            }
            '???' => {
                result.push_str("\'\'");
            }
            _ => {
                result.push(i);
            }
        }
    }
    return result;
}
