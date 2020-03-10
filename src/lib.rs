use std::os::raw::c_char;
use std::ffi::CString;

extern {
    fn compile_snowball(sb_file: *const c_char, out_file: *const c_char);
}

#[derive(Debug)]
pub enum Error {
    InvalidInputName,
    InvalidOutputName,
}
    

pub fn compile(sb_file: &str, out_file: &str) -> Result<(),Error> {
    let sb_file = CString::new(sb_file).map_err(|_| Error::InvalidInputName)?;
    let out_file = CString::new(out_file).map_err(|_| Error::InvalidOutputName)?;
    unsafe {
        compile_snowball(sb_file.as_ptr(), out_file.as_ptr());
    }
    Ok(())
}

