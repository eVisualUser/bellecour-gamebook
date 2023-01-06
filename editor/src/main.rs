#![cfg_attr(not(debug_assertions), windows_subsystem = "windows")] // hide console window on Windows in release

use eframe::egui;

fn main() {
    // Log to stdout (if you run with `RUST_LOG=debug`).
    let options = eframe::NativeOptions {
        initial_window_size: Some(egui::vec2(320.0, 240.0)),
        ..Default::default()
    };
    eframe::run_native(
        "GameBook Editor",
        options,
        Box::new(|_cc| Box::new(App::default())),
    );
}

struct App {
    project_folder: String,
}

impl Default for App {
    fn default() -> Self {
        Self {
            project_folder: "./".to_owned(),
        }
    }
}

impl eframe::App for App {
    fn update(&mut self, ctx: &egui::Context, _frame: &mut eframe::Frame) {
        egui::Window::new("Project").show(ctx, |ui| {
            ui.text_edit_singleline(&mut self.project_folder);
        });
    }
}
