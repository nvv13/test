mod ffi {
    // #[allow(improper_ctypes)]
    extern "stdcall" {
        pub fn MultiByteToWideChar(
            CodePage: u32,
            dwFlags: u32,
            lpMultiByteStr: *const u8,
            cbMultiByte: i32,
            lpWideCharStr: *mut u16,
            cchWideChar: i32,
        ) -> i32;

        pub fn MessageBoxW(
            hWnd: *const (),
            lpText: *const u16,
            lpCaption: *const u16,
            uType: u32,
        ) -> i32;
    }

    pub const CP_UTF8: u32 = 65001;
}

fn message_box(text: &str, caption: &str) {
    unsafe {
        ffi::MessageBoxW(
            std::ptr::null(),
            to_utf16(text).as_ptr(),
            to_utf16(caption).as_ptr(),
            0,
        );
    }
}

fn to_utf16(s: &str) -> Vec<u16> {
    unsafe {
        let req = ffi::MultiByteToWideChar(
            ffi::CP_UTF8,
            0,
            s.as_ptr(),
            s.len() as i32,
            std::ptr::null_mut(),
            0,
        );

        let req = req as usize + 1; // terminating null
        let mut buf = Vec::with_capacity(req);
        buf.set_len(req);

        ffi::MultiByteToWideChar(
            ffi::CP_UTF8,
            0,
            s.as_ptr(),
            s.len() as i32,
            buf.as_mut_ptr(),
            req as i32,
        );

        buf
    }
}

fn main() {
    message_box("Hello!", "Title");
}
