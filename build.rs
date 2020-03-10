
fn main() {
    let file_list = ["compiler/analyser.c",
                     "compiler/driver.c",
                     "compiler/generator.c",
                     "compiler/generator_rust.c",
                     "compiler/space.c",
                     "compiler/tokeniser.c"];
    for f in &file_list {
        println!("cargo:rerun-if-changed={}",f);
    }
    cc::Build::new()
        .files(&file_list)
        .compile("snowballc");
}
