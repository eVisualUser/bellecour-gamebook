#![allow(unused)]
#![cfg_attr(not(debug_assertions), windows_subsystem = "windows")]

mod app;
mod debugger;
mod directory;
mod editor;
mod file;
mod page_editor;
mod project;
mod searcher;

fn main() {
    let options = eframe::NativeOptions {
        initial_window_size: Some(eframe::egui::vec2(320.0, 240.0)),
        transparent: true,
        ..Default::default()
    };

    let mut app = app::App::default();
    app.add_editor(Box::new(project::Project::default()));

    eframe::run_native("Client Editor", options, Box::new(|_cc| Box::new(app)));
}
