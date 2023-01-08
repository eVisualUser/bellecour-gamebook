use crate::choice::Choice;
use crate::variable::Variable;

#[derive(Default)]
pub struct Page {
    pub title: String,
    pub local_type: String,
    pub content: Vec<Variable>,
    pub choices: Vec<Choice>,
}
