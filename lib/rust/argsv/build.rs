use std::process::Command;
use std::path::Path;
use std::env;

fn main() {
    // Get the OUT_DIR path from Cargo env var
    let out_dir = env::var("OUT_DIR").expect("OUT_DIR not set by Cargo");

    // Build start.cpp -> start.dll + start.lib
    let status_start = Command::new("cl.exe")
        .args(&[
            "/LD",
            "..\\C\\start.cpp",
            "/DGRAMMAR_END_OF_TOKEN_MARKER=\" \"",
            "/DGRAMMAR_END_OF_TOKEN_MARKER_SIZE=1",
            "/DGRAMMAR_START_OF_COMMENT_MARKER=\"(\"",
            "/DGRAMMAR_START_OF_COMMENT_MARKER_SIZE=1",
            "/link",
            &format!("/OUT:{}/start.dll", out_dir),
            &format!("/IMPLIB:{}/start.lib", out_dir),
        ])
        .status()
        .expect("Failed to compile start.cpp");

    assert!(status_start.success(), "Failed to compile start.dll");

    // Build clap.c -> clap.dll + clap.lib, link against start.lib
    let status_clap = Command::new("cl.exe")
        .args(&[
            "/LD",
            "..\\C\\clap.c",
            "/link",
            &format!("/OUT:{}/clap.dll", out_dir),
            &format!("/IMPLIB:{}/clap.lib", out_dir),
            &format!("{}/start.lib", out_dir),
        ])
        .status()
        .expect("Failed to compile clap.c");

    assert!(status_clap.success(), "Failed to compile clap.dll");

    // Tell Rust to link from OUT_DIR
    println!("cargo:rustc-link-search=native={}", out_dir);
    println!("cargo:rustc-link-lib=dylib=clap");
}
