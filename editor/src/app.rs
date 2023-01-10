use crate::editor::Editor;
use eframe::egui;

pub struct App {
    editors: Vec<Box<dyn Editor>>,
    pub path: String,
}

impl Default for App {
    fn default() -> Self {
        Self {
            editors: Vec::new(),
            path: String::new(),
        }
    }
}

impl eframe::App for App {
    fn update(&mut self, ctx: &egui::Context, _frame: &mut eframe::Frame) {
        for i in 0..self.editors.len() {
            let mut editor = self.editors.swap_remove(i);
            editor.update(&ctx, self);
            self.editors.push(editor);
        }
    }
}

impl App {
    pub fn add_editor(&mut self, editor: Box<dyn Editor>) {
        self.editors.push(editor);
    }
}
