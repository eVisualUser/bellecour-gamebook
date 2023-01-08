#![cfg_attr(not(debug_assertions), windows_subsystem = "windows")] // hide console window on Windows in release
#![allow(unused)]

mod array;
mod choice;
mod page;
mod project;
mod projectdir;
mod variable;

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

#[derive(Default)]
struct App {
    project_dir: Option<projectdir::ProjectDir>,
    current_project: String,
}

impl eframe::App for App {
    fn update(&mut self, ctx: &egui::Context, _frame: &mut eframe::Frame) {
        egui::Window::new("Project Management").show(ctx, |ui| {
            ui.text_edit_singleline(&mut self.current_project);

            if !std::path::Path::new(&self.current_project).exists() {
                if ui.button("Generate Project").clicked() {
                    self.project_dir = Some(projectdir::ProjectDir::default());

                    match &mut self.project_dir {
                        Some(dir) => {
                            dir.path = self.current_project.clone();
                            dir.generate();
                        }
                        None => (),
                    }
                }
            } else if ui.button("Load Project").clicked() {
                match &mut self.project_dir {
                    Some(dir) => {
                        dir.path = self.current_project.clone();
                    }
                    None => (),
                }
            }
        });
    }
}
