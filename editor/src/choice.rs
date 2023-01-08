use crate::array::Array;

#[derive(Default)]
pub struct Choice {
    pub title: String,
    pub action: Array,
    pub condition: Array,
}
